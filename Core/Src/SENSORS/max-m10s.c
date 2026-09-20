#include "cmsis_gcc.h"
#include "main.h"
#include "i2c.h"

#include "SENSORS/max-m10s.h"
#include "SENSORS/max-m10s_defines.h"
#include "stm32h7rsxx_hal_i2c.h"

#include <math.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>




static I2C_HandleTypeDef *i2c_handle;

static UBX_Packet_t ubx_packet;
static MAX_M10S_Data_t gnss_data;

NC_RAM static uint8_t DMA_rx_buff[(UBX_NAV_PVT_PAYLOAD_SIZE + UBX_FRAME_OVERHEAD_SIZE)];


static MAX_M10S_Status_t UBX_SendCfgValset (uint8_t *keys_and_values, uint16_t size);
static MAX_M10S_Status_t UBX_EnableI2CNavMessages (ByteBuffer_t *config, MAX_M10S_NavMessages_t Messages);
static MAX_M10S_Status_t UBX_ReadMonVer (void);

static void UBX_AppendConfigKeyValue (ByteBuffer_t *cfg, uint32_t key_id, uint64_t val, uint8_t sizeof_val);

static MAX_M10S_Status_t MAX_M10S_SendUbxPacket (uint16_t class_id, uint8_t *payload, uint16_t payload_length);
static MAX_M10S_Status_t MAX_M10S_ReadUbxPacket (uint8_t *payload, uint16_t payload_length);
static MAX_M10S_Status_t MAX_M10S_ReadAvailableByteCount (uint16_t *available);

static void MAX_M10S_Reset (void);
static void MAX_M10S_SetSafeBootPin (uint8_t enable);

static MAX_M10S_Status_t MAX_M10S_CheckHWVersion (void);

static void UBX_AccumulateChecksum (uint8_t *CK_A, uint8_t *CK_B);
static void UBX_UpdatePacketChecksum (void);
static MAX_M10S_Status_t UBX_ValidateChecksum (void);
static MAX_M10S_Status_t UBX_ValidatePayloadLength (uint16_t expected_payload_lenght);
static void UBX_DecodeHeaderAndChecksum (uint8_t *buff, uint16_t sizeofpayload);

static MAX_M10S_Status_t UBX_ProcessReceivedPacket (uint16_t expected_payload_lenght);



static uint8_t append_array (ByteBuffer_t *destination, uint8_t *src_buff, uint16_t src_size) {
    uint16_t new_capacity = destination->size + src_size;

    uint8_t *temp = realloc(destination->data, new_capacity);

    if (!temp) {
        return 0;
    }

    destination->data = temp;
    memcpy(&destination->data[destination->size], src_buff, src_size);

    destination->size += src_size;

    return 1;
}

static void clean_dynamic_buffer (ByteBuffer_t *a) {
    free(a->data);

    a->data = NULL;
    a->size = 0;
}


static MAX_M10S_Status_t MAX_M10S_SendUbxPacket (uint16_t class_id, uint8_t *payload, uint16_t payload_length) {
    uint8_t packet[8 + payload_length];

    ubx_packet.SYNC_CHAR_1 = MAX_M10S_SYNC_CHAR_1;
    ubx_packet.SYNC_CHAR_2 = MAX_M10S_SYNC_CHAR_2;
    ubx_packet.Message_ClassAndID[0] = ((class_id & 0xFF00) >> 8);
    ubx_packet.Message_ClassAndID[1] = (class_id & 0x00FF);
    ubx_packet.Length[0] = (payload_length & 0x00FF);
    ubx_packet.Length[1] = ((payload_length & 0xFF00) >> 8);
    ubx_packet.Payload = payload;
    UBX_UpdatePacketChecksum();

    /* assembly */
    memcpy(packet, &ubx_packet, 6);
    if (payload_length != 0) {
        memcpy(&packet[6], payload, payload_length);
    }
    packet[6 + payload_length] = ubx_packet.Checksum[0];
    packet[7 + payload_length] = ubx_packet.Checksum[1];

    if (HAL_I2C_Master_Transmit(i2c_handle, MAX_M10S_I2C_ADDRESS, packet, sizeof(packet), 100) != HAL_OK) {
        return MAX_M10S_I2C_ERROR;
    }
    return MAX_M10S_OK;
}

static MAX_M10S_Status_t MAX_M10S_ReadUbxPacket (uint8_t *payload, uint16_t payload_length) {
    if (payload_length > (60000 - UBX_FRAME_OVERHEAD_SIZE)) {
        return MAX_M10S_ERROR;
    }
    
    uint8_t packet[UBX_FRAME_OVERHEAD_SIZE + payload_length];

    if (HAL_I2C_Mem_Read(i2c_handle, MAX_M10S_I2C_ADDRESS, 0xFF, I2C_MEMADD_SIZE_8BIT, packet, sizeof(packet), 100) != HAL_OK) {
        return MAX_M10S_I2C_ERROR;
    }

    UBX_DecodeHeaderAndChecksum(packet, payload_length);

    /* Lenght missmatch */
    if (UBX_ValidatePayloadLength(payload_length) != MAX_M10S_OK) {
        return MAX_M10S_ERROR;
    }
    
    memcpy(payload, &packet[6], payload_length);
    ubx_packet.Payload = payload;

    return UBX_ValidateChecksum();
}

static MAX_M10S_Status_t MAX_M10S_ReadAvailableByteCount (uint16_t *available) {
    uint8_t bytes[2];

    if (HAL_I2C_Mem_Read(i2c_handle, MAX_M10S_I2C_ADDRESS, 0xFD, I2C_MEMADD_SIZE_8BIT, bytes, sizeof(bytes), 100) != HAL_OK) {
        return MAX_M10S_I2C_ERROR;
    }

    *available = ((uint16_t)bytes[0] << 8) | bytes[1];
    return MAX_M10S_OK;
}

static MAX_M10S_Status_t MAX_M10S_StartStreamRead_DMA (uint8_t *data, uint16_t sizeofdata) {
    if (HAL_I2C_Mem_Read_DMA(i2c_handle, MAX_M10S_I2C_ADDRESS, 0xFF, I2C_MEMADD_SIZE_8BIT, data, sizeofdata) != HAL_OK) {
        return MAX_M10S_I2C_ERROR;
    }
    return MAX_M10S_OK;
}

// static void MAX_M10S_EnableHighPerformanceMode (void) {
//     static uint8_t high_cpu_clock_otp[] = {
//         0xB5, 0x62, 0x06, 0x41, 0x10, 0x00,
//         0x03, 0x00, 0x04, 0x1F, 0x54, 0x5E,
//         0x79, 0xBF, 0x28, 0xEF, 0x12, 0x05,
//         0xFD, 0xFF, 0xFF, 0xFF, 0x8F, 0x0D,

//         0xB5, 0x62, 0x06, 0x41, 0x1C, 0x00,
//         0x04, 0x01, 0xA4, 0x10, 0xBD, 0x34,
//         0xF9, 0x12, 0x28, 0xEF, 0x12, 0x05,
//         0x05, 0x00, 0xA4, 0x40, 0x00, 0xB0,
//         0x71, 0x0B, 0x0A, 0x00, 0xA4, 0x40,
//         0x00, 0xD8, 0xB8, 0x05, 0xDE, 0xAE
//     };

//     // TODO return? 
//     if (HAL_I2C_Master_Transmit(i2c_handle, MAX_M10S_I2C_ADDRESS, high_cpu_clock_otp, sizeof(high_cpu_clock_otp), 100) != HAL_OK) {
//         return;
//     }
// }

static void MAX_M10S_Reset (void) {
    MAX_M10S_RESET_0;
    HAL_Delay(10);
    MAX_M10S_RESET_1;
    HAL_Delay(1000);
}

static void MAX_M10S_SetSafeBootPin (uint8_t enable) {
    if (enable) {
        MAX_M10S_SB_0;
    } else {
        MAX_M10S_SB_1;
    }
}

MAX_M10S_Status_t MAX_M10S_Init (I2C_HandleTypeDef *hi2c) {
    i2c_handle = hi2c;
    uint16_t output_rate_Hz = 16;

    HAL_NVIC_DisableIRQ(EXTI12_IRQn);

    MAX_M10S_SetSafeBootPin(0);
    MAX_M10S_Reset();


    if (MAX_M10S_CheckHWVersion() != 0) {
        /* Error, no interface comunication */
        return MAX_M10S_ERROR;
    }

    /* Config data */
    ByteBuffer_t config = {0};

    uint8_t cfg_header[4] = {
        0x00,   // Version 0
        0x01,   // Apply to RAM
        0x00,   // Reserved
        0x00    // Reserved
    };
    if (!append_array(&config, cfg_header, sizeof(cfg_header))) {
        return MAX_M10S_ERROR;
    }

    /* GPS L1 C/A */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GPS_ENA,      1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GPS_L1CA_ENA, 1, 1);

    /* Galileo E1 */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GAL_ENA,     1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GAL_E1_ENA,  1, 1);

    /* BeiDou B1C */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_BDS_ENA,     1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_BDS_B1_ENA,  0, 1); /* B1I off */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_BDS_B1C_ENA, 1, 1);

    /* GLONASS off */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GLO_ENA,    0, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_GLO_L1_ENA, 0, 1);

    /* QZSS L1 C/A */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_QZSS_ENA,      1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_QZSS_L1CA_ENA, 1, 1);

    /* SBAS corrections, where available */
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_SBAS_ENA,      1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SIGNAL_SBAS_L1CA_ENA, 1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SBAS_USE_DIFFCORR,    1, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SBAS_USE_RANGING,     0, 1);
    UBX_AppendConfigKeyValue(&config, CFG_SBAS_USE_INTEGRITY,   0, 1);

    // /* Continuous operation */
    UBX_AppendConfigKeyValue(&config, CFG_PM_OPERATEMODE, 0, 1);

    /* Airborne <4g */
    UBX_AppendConfigKeyValue(&config, CFG_NAVSPG_DYNMODEL, 8, 1);

    /* 5 Hz navigation and one NAV-PVT per solution */
    UBX_AppendConfigKeyValue(&config, CFG_RATE_MEAS,              (1000 / output_rate_Hz), 2);
    UBX_AppendConfigKeyValue(&config, CFG_RATE_NAV,                 1, 2);

    /* I2C */
    UBX_AppendConfigKeyValue(&config, CFG_I2C_ENABLED, 0x01, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_I2C_EXTENDEDTIMEOUT, 0x01, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_I2COUTPROT_UBX, 0x01, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_I2COUTPROT_NMEA, 0x00, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_INFMSG_UBX_I2C,         0, 1);
    
    /* Messages */
    if (UBX_EnableI2CNavMessages(&config, UBX_NAV_PVT) != MAX_M10S_OK) {
        /* CRITICAL DESIGN ERROR */
        while (1);
    }

    /* IRQ pin */
    UBX_AppendConfigKeyValue(&config, CFG_TXREADY_ENABLED, 0x01, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_TXREADY_PIN, 0x05, 0x01);
    UBX_AppendConfigKeyValue(&config, CFG_TXREADY_THRESHOLD, 1, 2);


    /* Send config */
    if (UBX_SendCfgValset(config.data, config.size) != MAX_M10S_OK) {
        return MAX_M10S_ERROR;
    }
    /* pull for ack-ack or other message */
    uint16_t bytes = 0;
    while (bytes < 2) {
        HAL_Delay(10);
        MAX_M10S_ReadAvailableByteCount(&bytes);
    }

    uint8_t payload[bytes - UBX_FRAME_OVERHEAD_SIZE];
    MAX_M10S_ReadUbxPacket(payload, sizeof(payload));
    if (!(ubx_packet.Message_ClassAndID[0] == 0x05 && ubx_packet.Message_ClassAndID[1] == 0x01)) {
        /* conf not accepted */
        __HAL_GPIO_EXTI_CLEAR_IT(GPS_EXTI_Pin);
        HAL_NVIC_ClearPendingIRQ(EXTI12_IRQn);
        HAL_NVIC_EnableIRQ(EXTI12_IRQn);

        return MAX_M10S_ERROR;
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPS_EXTI_Pin);
    HAL_NVIC_ClearPendingIRQ(EXTI12_IRQn);
    HAL_NVIC_EnableIRQ(EXTI12_IRQn);
    
    clean_dynamic_buffer(&config);

    return MAX_M10S_OK;
}

static void UBX_AccumulateChecksum (uint8_t *CK_A, uint8_t *CK_B) {
    uint16_t Lenght = ubx_packet.Length[0] | (ubx_packet.Length[1] << 8);


    *CK_A += ubx_packet.Message_ClassAndID[0];
    *CK_B += *CK_A;

    *CK_A += ubx_packet.Message_ClassAndID[1];
    *CK_B += *CK_A;

    *CK_A += ubx_packet.Length[0];
    *CK_B += *CK_A;

    *CK_A += ubx_packet.Length[1];
    *CK_B += *CK_A;

    for (uint16_t i = 0; i < Lenght; i++) {
        *CK_A += ubx_packet.Payload[i];
        *CK_B += *CK_A;
    }
}

static void UBX_UpdatePacketChecksum (void) {
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    
    UBX_AccumulateChecksum(&CK_A, &CK_B);
    
    ubx_packet.Checksum[0] = CK_A;
    ubx_packet.Checksum[1] = CK_B;
}

static MAX_M10S_Status_t UBX_ValidateChecksum (void) {
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;

    UBX_AccumulateChecksum(&CK_A, &CK_B);

    if ((ubx_packet.Checksum[0] != CK_A) || (ubx_packet.Checksum[1] != CK_B)) {
        return MAX_M10S_INVALID_PACKET;
    }

    return MAX_M10S_OK;
}

static MAX_M10S_Status_t UBX_ValidatePayloadLength (uint16_t expected_payload_lenght) {
    uint16_t received_length = (ubx_packet.Length[0]) | (ubx_packet.Length[1] << 8);
    if (expected_payload_lenght != received_length) {
        return MAX_M10S_INVALID_PACKET;
    }
    return MAX_M10S_OK;
}

static void UBX_DecodeHeaderAndChecksum (uint8_t *buff, uint16_t sizeofpayload) {
    ubx_packet.SYNC_CHAR_1 = buff[0];
    ubx_packet.SYNC_CHAR_2 = buff[1];
    ubx_packet.Message_ClassAndID[0] = buff[2];
    ubx_packet.Message_ClassAndID[1] = buff[3];
    ubx_packet.Length[0] = buff[4];
    ubx_packet.Length[1] = buff[5];
    ubx_packet.Checksum[0] = buff[6 + sizeofpayload];
    ubx_packet.Checksum[1] = buff[7 + sizeofpayload];
}

static MAX_M10S_Status_t UBX_EnableI2CNavMessages (ByteBuffer_t *config, MAX_M10S_NavMessages_t Messages) {
    uint16_t min_dma_buff_size = UBX_FRAME_OVERHEAD_SIZE;

    if (Messages & UBX_NAV_AOPSTATUS) {
        // TODO 
        UBX_AppendConfigKeyValue(config, CFG_MSGOUT_UBX_NAV_AOPSTATUS_I2C, 0x01, 0x01);
    }   
    if (Messages & UBX_NAV_CLOCK) {
        // TODO 
        UBX_AppendConfigKeyValue(config, CFG_MSGOUT_UBX_NAV_CLOCK_I2C, 0x01, 0x01);
    }
    if (Messages & UBX_NAV_PVT) {
        min_dma_buff_size += UBX_NAV_PVT_PAYLOAD_SIZE;
        UBX_AppendConfigKeyValue(config, CFG_MSGOUT_UBX_NAV_PVT_I2C, 0x01, 0x01);
    }
    if (Messages & UBX_NAV_SAT) {
        // TODO
        UBX_AppendConfigKeyValue(config, CFG_MSGOUT_UBX_NAV_SAT_I2C, 0x01, 0x01);
    }

    if (sizeof(DMA_rx_buff) < min_dma_buff_size) {
        return MAX_M10S_ERROR;
    }
    return MAX_M10S_OK;
}

/**
* @param keys_and_values is array of keys and values 
* @param size            is sizeof(keys_and_values)
*/
static MAX_M10S_Status_t UBX_SendCfgValset (uint8_t *keys_and_values, uint16_t size) {
    return MAX_M10S_SendUbxPacket(ubx_cfg_valset, keys_and_values, size);
}

static MAX_M10S_Status_t UBX_ReadMonVer (void) {
    static uint8_t rx_payload[190];
    uint16_t bytes = 0;
    MAX_M10S_Status_t status;

    status = MAX_M10S_SendUbxPacket(ubx_mon_ver, NULL, 0);
    if (status != MAX_M10S_OK) {
        return status;
    }

    while (bytes == 0) {
        MAX_M10S_ReadAvailableByteCount(&bytes);
    }\

    status = MAX_M10S_ReadUbxPacket(rx_payload, sizeof(rx_payload));
    if (status != MAX_M10S_OK) {
        return status;
    }

    return status;
}

static MAX_M10S_Status_t UBX_ValidatePacket (uint16_t expected_payload_lenght) {
    MAX_M10S_Status_t status;

    /* sync char */
    if (ubx_packet.SYNC_CHAR_1 != MAX_M10S_SYNC_CHAR_1 || ubx_packet.SYNC_CHAR_2 != MAX_M10S_SYNC_CHAR_2) {
        return MAX_M10S_INVALID_PACKET;
    }

    /* Check lenght match */
    status = UBX_ValidatePayloadLength(expected_payload_lenght);
    if (status != MAX_M10S_OK) {
        return status;
    }

    /* Check crc */
    return UBX_ValidateChecksum();
}

MAX_M10S_Status_t MAX_M10S_HandleEvent (Event_t event) {
    static uint8_t stage = 0;
    static uint8_t available_count_bytes[2];
    static uint16_t payload_size = 0;

    // TODO, no recovery if DMA/I2C errors
    switch (stage) {
        case 0:
            if (event != EVENT_START_CYCLE) {
                return MAX_M10S_ERROR;
            }
            
            if (HAL_I2C_Mem_Read_IT(i2c_handle, MAX_M10S_I2C_ADDRESS, 0xFD, I2C_MEMADD_SIZE_8BIT, available_count_bytes, sizeof(available_count_bytes)) != HAL_OK) {
                return MAX_M10S_I2C_ERROR;
            }
            stage = 1;
            return MAX_M10S_OK;
        
        case 1:
            if (event != EVENT_I2C_RX_DONE) {
                return MAX_M10S_ERROR;
            }
            uint16_t available_bytes = ((uint16_t)available_count_bytes[0] << 8) | available_count_bytes[1];
            if (!(available_bytes > 0)) {
                stage = 0;
                return MAX_M10S_OK;
            }

            ubx_packet.Payload = &DMA_rx_buff[6];
            payload_size = sizeof(DMA_rx_buff) - UBX_FRAME_OVERHEAD_SIZE;

            // TODO if (available_bytes > sizeof(rx_buff))
            if (MAX_M10S_StartStreamRead_DMA(DMA_rx_buff, available_bytes) != MAX_M10S_OK) {
                stage = 0;
                return MAX_M10S_I2C_ERROR;
            }

            stage = 2;
            return MAX_M10S_OK;
    
        case 2:
            if (event != EVENT_I2C_RX_DONE) {
                return MAX_M10S_ERROR;
            }
            stage = 1;

            /* read if any more messages */
            if (HAL_I2C_Mem_Read_IT(i2c_handle, MAX_M10S_I2C_ADDRESS, 0xFD, I2C_MEMADD_SIZE_8BIT, available_count_bytes, sizeof(available_count_bytes)) != HAL_OK) {
                stage = 0;
                return MAX_M10S_I2C_ERROR;
            }

            UBX_DecodeHeaderAndChecksum(DMA_rx_buff, payload_size);
           
            return UBX_ProcessReceivedPacket(payload_size);

        default:
            return MAX_M10S_ERROR;
    }
}

// TODO 
MAX_M10S_Status_t MAX_M10S_CopyLatestData (MAX_M10S_Data_t *a) {
    if (!(gnss_data.available && gnss_data.new_data)) {
        return MAX_M10S_NO_NEW_DATA;
    }
    memcpy(a, &gnss_data, sizeof(gnss_data));

    return MAX_M10S_NEW_DATA;
}

static MAX_M10S_Status_t MAX_M10S_CheckHWVersion (void) {
    /* Reads sv and hv */
    if (UBX_ReadMonVer() != MAX_M10S_OK) {
        return MAX_M10S_ERROR;
    }

    char swVersion[30] = {0};
    char hwVersion[10] = {0};

    memcpy(swVersion, &ubx_packet.Payload[0],  30);
    memcpy(hwVersion, &ubx_packet.Payload[30], 10);

    if (strcmp(hwVersion, MAX_M10S_HW_VERSION) != 0) {
        /* Interface not working */
        return MAX_M10S_ERROR;
    }

    return MAX_M10S_OK;
}

static void UBX_AppendConfigKeyValue (ByteBuffer_t *cfg, uint32_t key_id, uint64_t val, uint8_t sizeof_val) {
    uint8_t temp_buff[sizeof_val + sizeof(key_id)];

    temp_buff[3] = (key_id & 0xFF000000) >> 24;
    temp_buff[2] = (key_id & 0x00FF0000) >> 16;
    temp_buff[1] = (key_id & 0x0000FF00) >> 8;
    temp_buff[0] = (key_id & 0x000000FF);

    for (uint8_t i = 0; i < (sizeof_val); i++) {
        temp_buff[4 + i] = (uint8_t)((val >> (8 * i)));
    }

    append_array(cfg, temp_buff, sizeof(temp_buff));
}

static MAX_M10S_Status_t UBX_InterpretMessage_NavPvt (void) {
    UBX_NavPvt_t nav_pvt = {0};

    nav_pvt.fixType = ubx_packet.Payload[20];
    nav_pvt.flags = ubx_packet.Payload[21];

    /* gnss_data data not acurate */
    if (nav_pvt.fixType != MAX_M10S_3D_FIX) {
        gnss_data.available = 0;
        return MAX_M10S_OK;
    }
    /* gnssFixOK */
    if ((nav_pvt.flags & 0x01) != 1) {
        gnss_data.available = 0;
        return MAX_M10S_OK;
    }
    memcpy(&nav_pvt.lon, &ubx_packet.Payload[24], 4);
    memcpy(&nav_pvt.lat, &ubx_packet.Payload[28], 4);
    memcpy(&nav_pvt.hMSL, &ubx_packet.Payload[36], 4);
    memcpy(&nav_pvt.velD, &ubx_packet.Payload[56], 4);
    memcpy(&nav_pvt.gSpeed, &ubx_packet.Payload[60], 4);
    memcpy(&nav_pvt.headMot, &ubx_packet.Payload[64], 4);

    gnss_data.lon = (float)nav_pvt.lon * 1e-7;
    gnss_data.lat = (float)nav_pvt.lat * 1e-7;
    gnss_data.hMSL = (float)nav_pvt.hMSL;
    gnss_data.gSpeed = (float)nav_pvt.gSpeed;
    gnss_data.headMot = (float)nav_pvt.headMot * 1e-5;
    gnss_data.velD = (float)nav_pvt.velD;

    gnss_data.available = 1;
    gnss_data.new_data  = 1;

    return MAX_M10S_OK;
}

static MAX_M10S_Status_t UBX_ProcessReceivedPacket (uint16_t expected_payload_lenght) {
    MAX_M10S_Status_t status;
    uint16_t message = ubx_packet.Message_ClassAndID[1] | (ubx_packet.Message_ClassAndID[0] << 8);

    status = UBX_ValidatePacket(expected_payload_lenght);
    if (status != MAX_M10S_OK) {
        return status;
    }

    // TODO add other messages 
    switch (message) {
        case ubx_nav_pvt: 
            return UBX_InterpretMessage_NavPvt();

        default:
            return MAX_M10S_ERROR;
    }
}