#ifndef MAX_M10S_DEFINES_H
#define MAX_M10S_DEFINES_H

#include "main.h"
#include "stdbool.h"    

#define MAX_M10S_SYNC_CHAR_1        (0xB5U)
#define MAX_M10S_SYNC_CHAR_2        (0x62U)

#define MAX_M10S_I2C_ADDRESS        (0x42U << 1)

#define MAX_M10S_HW_VERSION         ("000A0000")


#define MAX_M10S_RESET_0 HAL_GPIO_WritePin(GPS_RESET_GPIO_Port, GPS_RESET_Pin, GPIO_PIN_RESET)
#define MAX_M10S_RESET_1 HAL_GPIO_WritePin(GPS_RESET_GPIO_Port, GPS_RESET_Pin, GPIO_PIN_SET)

#define MAX_M10S_SB_0 HAL_GPIO_WritePin(GPS_SAFEBOOT_GPIO_Port, GPS_SAFEBOOT_Pin, GPIO_PIN_RESET)
#define MAX_M10S_SB_1 HAL_GPIO_WritePin(GPS_SAFEBOOT_GPIO_Port, GPS_SAFEBOOT_Pin, GPIO_PIN_SET)



typedef struct {
    uint8_t Type;

    uint8_t Name;

    uint8_t Scale;

    uint8_t Unit;
} MAX_M10S_Payload_t;

typedef struct {
    uint8_t SYNC_CHAR_1;
    uint8_t SYNC_CHAR_2;

    uint8_t Message_ClassAndID[2];

    /* payload only */
    uint8_t Length[2];

    uint8_t *Payload;

    uint8_t Checksum[2];

} UBX_Packet_t;

typedef struct {
    uint32_t iTOW;

    uint16_t year;

    uint8_t month;

    uint8_t day;

    uint8_t hour;

    uint8_t min;

    uint8_t sec;

    uint8_t valid;

    uint32_t tAcc;

    int32_t nano;

    uint8_t fixType;

    uint8_t flags;

    uint8_t numSV;
    /* Longitude in [1e-7 deg] */
    int32_t lon;

    /* Latitude in [1e-7 deg] */
    int32_t lat;

    /* Height above mean sea level in [mm] */
    int32_t hMSL;

    /* NED north velocity in [mm/s] */
    int32_t velN;

    /* NED east velocity in [mm/s] */
    int32_t velE;

    /* NED down velocity in [mm/s] */
    int32_t velD;

    /* Ground Speed (2-D) in [mm/s] */
    int32_t gSpeed;

    /* Heading of motion (2-D) in [1e-5 deg ] */
    int32_t headMot;

} UBX_NavPvt_t;

typedef struct {
    uint8_t gnssId;
    uint8_t svId;
    uint8_t cno;
    int8_t elev;
    int16_t azim;
    int16_t prRes;

    uint8_t qualityInd;
    bool svUsed;
    uint8_t health;
    bool diffCorr;
    bool smoothed;
    uint8_t orbitSource;
    bool ephAvail;
    bool almAvail;
    bool anoAvail;
    bool aopAvail;
    bool sbasCorrUsed;
    bool rtcmCorrUsed;
    bool slasCorrUsed;
    bool spartnCorrUsed;
    bool prCorrUsed;
    bool crCorrUsed;
    bool doCorrUsed;
    bool clasCorrUsed;
} UBX_NavSat_t;

typedef enum {
    MAX_M10S_NO_FIX = 0x00,
    MAX_M10S_DEAD_RECKONING = 0x01,
    MAX_M10S_2D_FIX = 0x02,
    MAX_M10S_3D_FIX = 0x03,
    MAX_M10S_GNSS_DEAD_RECKONING_FIX = 0x04,
    MAX_M10S_TIME_FIX = 0x05
} MAX_M10S_FixType_t;

typedef enum {
    UBX_NAV_AOPSTATUS = (1U),
    UBX_NAV_CLOCK = (1U << 1U),
    UBX_NAV_PVT = (1U << 2U),
    UBX_NAV_SAT = (1U << 3U),
    // TODO add 
} MAX_M10S_NavMessages_t;

#define UBX_HEADER_SIZE          (6U)
#define UBX_CHECKSUM_SIZE        (2U)
#define UBX_FRAME_OVERHEAD_SIZE  (UBX_HEADER_SIZE + UBX_CHECKSUM_SIZE)
#define UBX_NAV_PVT_PAYLOAD_SIZE (92U)



#define CFG_ANA_ORBMAXERR                              (0x30230002U)   // type U2 (2B value) -- Maximum acceptable (modeled) orbit error in m. Range is from 5 to 1000.
#define CFG_ANA_USE_ANA                                (0x10230001U)  // type L (1B value) -- Use AssistNow Autonomous
 
/* ===================== CFG-BDS ===================== */
 
#define CFG_BDS_USE_PRN_1_TO_5                         (0x10340014U)  // type L (1B value) -- Use BeiDou geostationary satellites (PRN 1-5)
#define CFG_BDS_D1D2_NAVDATA                           (0x20340009U)  // type E1 (1B value) -- Enable only the given BDS D1/D2 navigation data streams
 
/* ===================== CFG-HW ===================== */
 
#define CFG_HW_DCDC_DIS                                (0x10A30018U)  // type L (1B value) -- DCDC converter disabled
#define CFG_HW_SINGLE_CLK                              (0x10A30019U)  // type L (1B value) -- Single-clock system
#define CFG_HW_OSC_TYPE                                (0x20A30025U)  // type E1 (1B value) -- Oscillator type
#define CFG_HW_CLK_OFFSET                              (0x40A30028U)  // type I4 (4B value) -- Clock offset
#define CFG_HW_CLK_OFFSET_VALID                        (0x10A30029U)  // type L (1B value) -- Clock offset valid
#define CFG_HW_CLK_PRECISION                           (0x40A3002AU)  // type U4 (4B value) -- Precision of the clock offset
#define CFG_HW_CLK_MAX_CALIB_DEV                       (0x40A3002BU)  // type U4 (4B value) -- Maximum calibration deviation
#define CFG_HW_CLK_MAX_CALIB_DEV_VALID                 (0x10A3002CU)  // type L (1B value) -- Max calibration deviation valid
#define CFG_HW_CLK_IS_TCXO                             (0x10A30047U)  // type L (1B value) -- Oscillator type indicator
#define CFG_HW_ANT_CFG_VOLTCTRL                        (0x10A3002EU)  // type L (1B value) -- Active antenna voltage control flag
#define CFG_HW_ANT_CFG_SHORTDET                        (0x10A3002FU)  // type L (1B value) -- Short antenna detection flag
#define CFG_HW_ANT_CFG_SHORTDET_POL                    (0x10A30030U)  // type L (1B value) -- Short antenna detection polarity
#define CFG_HW_ANT_CFG_OPENDET                         (0x10A30031U)  // type L (1B value) -- Open antenna detection flag
#define CFG_HW_ANT_CFG_OPENDET_POL                     (0x10A30032U)  // type L (1B value) -- Open antenna detection polarity
#define CFG_HW_ANT_CFG_PWRDOWN                         (0x10A30033U)  // type L (1B value) -- Power down antenna flag
#define CFG_HW_ANT_CFG_PWRDOWN_POL                     (0x10A30034U)  // type L (1B value) -- Power down antenna logic polarity
#define CFG_HW_ANT_CFG_RECOVER                         (0x10A30035U)  // type L (1B value) -- Automatic recovery from short state flag
#define CFG_HW_ANT_SUP_SWITCH_PIN                      (0x20A30036U)  // type U1 (1B value) -- ANT1 PIO number
#define CFG_HW_ANT_SUP_SHORT_PIN                       (0x20A30037U)  // type U1 (1B value) -- ANT0 PIO number
#define CFG_HW_ANT_SUP_OPEN_PIN                        (0x20A30038U)  // type U1 (1B value) -- ANT2 PIO number
#define CFG_HW_ANT_ON_SHORT_US                         (0x30A3003CU)  // type U2 (2B value) -- ANT on->short timeout[us]
#define CFG_HW_ANT_SUP_ENGINE                          (0x20A30054U)  // type E1 (1B value) -- Antenna supervisor engine selection
#define CFG_HW_ANT_SUP_SHORT_THR                       (0x20A30055U)  // type U1 (1B value) -- Antenna supervisor MADC engine short detection threshold
#define CFG_HW_ANT_SUP_OPEN_THR                        (0x20A30056U)  // type U1 (1B value) -- Antenna supervisor MADC engine open detection threshold
#define CFG_HW_SENS_WOM_MODE                           (0x20A30063U)  // type E1 (1B value) -- Select Wake-On-Motion mode
#define CFG_HW_SENS_WOM_THLD                           (0x20A30064U)  // type U1 (1B value) -- Wake-On-Motion threshold
#define CFG_HW_RF_LNA_MODE                             (0x20A30057U)  // type E1 (1B value) -- Mode for internal LNA (NEO-F10)
#define CFG_HW_RF1_LNA_MODE_LOWGAIN                    (0x10A3006AU)  // type E1 (1B value) -- Low Gain Mode for internal LNA RF1
#define CFG_HW_RF2_LNA_MODE_LOWGAIN                    (0x10A3006BU)  // type E1 (1B value) -- Low Gain Mode for internal LNA RF2
#define CFG_HW_RF3_LNA_MODE_LOWGAIN                    (0x10A3006CU)  // type E1 (1B value) -- Low Gain Mode for internal LNA RF3
 
/* ===================== CFG-I2C ===================== */
 
#define CFG_I2C_ADDRESS                                (0x20510001U)  // type U1 (1B value) -- I2C slave address of the receiver (7 bits)
#define CFG_I2C_EXTENDEDTIMEOUT                        (0x10510002U)  // type L (1B value) -- Flag to disable timeouting the interface after 1.5 s
#define CFG_I2C_ENABLED                                (0x10510003U)  // type L (1B value) -- Flag to indicate if the I2C interface should be enabled
#define CFG_I2C_PULL_UPS_DISABLED                      (0x1051000BU)  // type L (1B value) -- Flag to indicate if the internal Pull-Ups (SDA andSCL lines) should be disabled
 
/* ===================== CFG-I2CINPROT ===================== */
 
#define CFG_I2CINPROT_UBX                              (0x10710001U)  // type L (1B value) -- Flag to indicate if UBX should be an input protocol on I2C
#define CFG_I2CINPROT_NMEA                             (0x10710002U)  // type L (1B value) -- Flag to indicate if NMEA should be an input protocol on I2C
#define CFG_I2CINPROT_RTCM3X                           (0x10710004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an input protocol on I2C
#define CFG_I2CINPROT_SPARTN                           (0x10710005U)  // type L (1B value) -- Flag to indicate if SPARTN should be an input protocol on I2C
 
/* ===================== CFG-I2COUTPROT ===================== */
 
#define CFG_I2COUTPROT_UBX                             (0x10720001U)  // type L (1B value) -- Flag to indicate if UBX should be an output protocol on I2C
#define CFG_I2COUTPROT_NMEA                            (0x10720002U)  // type L (1B value) -- Flag to indicate if NMEA should be an output protocol on I2C
#define CFG_I2COUTPROT_RTCM3X                          (0x10720004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an output protocol on I2C
 
/* ===================== CFG-INFMSG ===================== */
 
#define CFG_INFMSG_UBX_I2C                             (0x20920001U)  // type X1 (1B value) -- Information message enable flags for the UBX protocol on the I2C interface
#define CFG_INFMSG_UBX_UART1                           (0x20920002U)  // type X1 (1B value) -- Information message enable flags for the UBX protocol on the UART1 interface
#define CFG_INFMSG_UBX_UART2                           (0x20920003U)  // type X1 (1B value) -- Information message enable flags for the UBX protocol on the UART2 interface
#define CFG_INFMSG_UBX_USB                             (0x20920004U)  // type X1 (1B value) -- Information message enable flags for the UBX protocol on the USB interface
#define CFG_INFMSG_UBX_SPI                             (0x20920005U)  // type X1 (1B value) -- Information message enable flags for the UBX protocol on the SPI interface
#define CFG_INFMSG_NMEA_I2C                            (0x20920006U)  // type X1 (1B value) -- Information message enable flags for the NMEA protocol on the I2C interface
#define CFG_INFMSG_NMEA_UART1                          (0x20920007U)  // type X1 (1B value) -- Information message enable flags for the NMEA protocol on the UART1 interface
#define CFG_INFMSG_NMEA_UART2                          (0x20920008U)  // type X1 (1B value) -- Information message enable flags for the NMEA protocol on the UART2 interface
#define CFG_INFMSG_NMEA_USB                            (0x20920009U)  // type X1 (1B value) -- Information message enable flags for the NMEA protocol on the USB interface
#define CFG_INFMSG_NMEA_SPI                            (0x2092000AU)  // type X1 (1B value) -- Information message enable flags for the NMEA protocol on the SPI interface
 
/* ===================== CFG-ITFM ===================== */
 
#define CFG_ITFM_BBTHRESHOLD                           (0x20410001U)  // type U1 (1B value) -- Broadband jamming detection threshold
#define CFG_ITFM_CWTHRESHOLD                           (0x20410002U)  // type U1 (1B value) -- CW jamming detection threshold
#define CFG_ITFM_ENABLE                                (0x1041000DU)  // type L (1B value) -- Enable interference detection
#define CFG_ITFM_ANTSETTING                            (0x20410010U)  // type E1 (1B value) -- Antenna setting
#define CFG_ITFM_ENABLE_AUX                            (0x10410013U)  // type L (1B value) -- Scan auxiliary bands
 
/* ===================== CFG-MOT ===================== */
 
#define CFG_MOT_GNSSSPEED_THRS                         (0x20250038U)  // type U1 (1B value) -- GNSS speed threshold below which platform is considered as stationary (a.k.a. static hold threshold)
#define CFG_MOT_GNSSDIST_THRS                          (0x3025003BU)  // type U2 (2B value) -- Distance above which GNSS-based stationary motion is exit (a.k.a. static hold distance threshold)
#define CFG_MOT_IMU_FILT_WINDOW                        (0x30250016U)  // type U2 (2B value) -- Averaging window for IMU measurements in noisy setups.
 
/* ===================== CFG-MSGOUT ===================== */
 
#define CFG_MSGOUT_NMEA_ID_DTM_I2C                     (0x209100A6U)  // type U1 (1B value) -- Output rate of the NMEA-GX-DTM message on port I2C
#define CFG_MSGOUT_NMEA_ID_DTM_SPI                     (0x209100AAU)  // type U1 (1B value) -- Output rate of the NMEA-GX-DTM message on port SPI
#define CFG_MSGOUT_NMEA_ID_DTM_UART1                   (0x209100A7U)  // type U1 (1B value) -- Output rate of the NMEA-GX-DTM message on port UART1
#define CFG_MSGOUT_NMEA_ID_GBS_I2C                     (0x209100DDU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GBS message on port I2C
#define CFG_MSGOUT_NMEA_ID_GBS_SPI                     (0x209100E1U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GBS message on port SPI
#define CFG_MSGOUT_NMEA_ID_GBS_UART1                   (0x209100DEU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GBS message on port UART1
#define CFG_MSGOUT_NMEA_ID_GGA_I2C                     (0x209100BAU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GGA message on port I2C
#define CFG_MSGOUT_NMEA_ID_GGA_SPI                     (0x209100BEU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GGA message on port SPI
#define CFG_MSGOUT_NMEA_ID_GGA_UART1                   (0x209100BBU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GGA message on port UART1
#define CFG_MSGOUT_NMEA_ID_GLL_I2C                     (0x209100C9U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GLL message on port I2C
#define CFG_MSGOUT_NMEA_ID_GLL_SPI                     (0x209100CDU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GLL message on port SPI
#define CFG_MSGOUT_NMEA_ID_GLL_UART1                   (0x209100CAU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GLL message on port UART1
#define CFG_MSGOUT_NMEA_ID_GNS_I2C                     (0x209100B5U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GNS message on port I2C
#define CFG_MSGOUT_NMEA_ID_GNS_SPI                     (0x209100B9U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GNS message on port SPI
#define CFG_MSGOUT_NMEA_ID_GNS_UART1                   (0x209100B6U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GNS message on port UART1
#define CFG_MSGOUT_NMEA_ID_GRS_I2C                     (0x209100CEU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GRS message on port I2C
#define CFG_MSGOUT_NMEA_ID_GRS_SPI                     (0x209100D2U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GRS message on port SPI
#define CFG_MSGOUT_NMEA_ID_GRS_UART1                   (0x209100CFU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GRS message on port UART1
#define CFG_MSGOUT_NMEA_ID_GSA_I2C                     (0x209100BFU)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSA message on port I2C
#define CFG_MSGOUT_NMEA_ID_GSA_SPI                     (0x209100C3U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSA message on port SPI
#define CFG_MSGOUT_NMEA_ID_GSA_UART1                   (0x209100C0U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSA message on port UART1
#define CFG_MSGOUT_NMEA_ID_GST_I2C                     (0x209100D3U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GST message on port I2C
#define CFG_MSGOUT_NMEA_ID_GST_SPI                     (0x209100D7U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GST message on port SPI
#define CFG_MSGOUT_NMEA_ID_GST_UART1                   (0x209100D4U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GST message on port UART1
#define CFG_MSGOUT_NMEA_ID_GSV_I2C                     (0x209100C4U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSV message on port I2C
#define CFG_MSGOUT_NMEA_ID_GSV_SPI                     (0x209100C8U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSV message on port SPI
#define CFG_MSGOUT_NMEA_ID_GSV_UART1                   (0x209100C5U)  // type U1 (1B value) -- Output rate of the NMEA-GX-GSV message on port UART1
#define CFG_MSGOUT_NMEA_ID_RLM_I2C                     (0x20910400U)  // type U1 (1B value) -- Output rate of the NMEA-GX-RLM message on port I2C
#define CFG_MSGOUT_NMEA_ID_RLM_SPI                     (0x20910404U)  // type U1 (1B value) -- Output rate of the NMEA-GX-RLM message on port SPI
#define CFG_MSGOUT_NMEA_ID_RLM_UART1                   (0x20910401U)  // type U1 (1B value) -- Output rate of the NMEA-GX-RLM message on port UART1
#define CFG_MSGOUT_NMEA_ID_RMC_I2C                     (0x209100ABU)  // type U1 (1B value) -- Output rate of the NMEA-GX-RMC message on port I2C
#define CFG_MSGOUT_NMEA_ID_RMC_SPI                     (0x209100AFU)  // type U1 (1B value) -- Output rate of the NMEA-GX-RMC message on port SPI
#define CFG_MSGOUT_NMEA_ID_RMC_UART1                   (0x209100ACU)  // type U1 (1B value) -- Output rate of the NMEA-GX-RMC message on port UART1
#define CFG_MSGOUT_NMEA_ID_VLW_I2C                     (0x209100E7U)  // type U1 (1B value) -- Output rate of the NMEA-GX-VLW message on port I2C
#define CFG_MSGOUT_NMEA_ID_VLW_SPI                     (0x209100EBU)  // type U1 (1B value) -- Output rate of the NMEA-GX-VLW message on port SPI
#define CFG_MSGOUT_NMEA_ID_VLW_UART1                   (0x209100E8U)  // type U1 (1B value) -- Output rate of the NMEA-GX-VLW message on port UART1
#define CFG_MSGOUT_NMEA_ID_VTG_I2C                     (0x209100B0U)  // type U1 (1B value) -- Output rate of the NMEA-GX-VTG message on port I2C
#define CFG_MSGOUT_NMEA_ID_VTG_SPI                     (0x209100B4U)  // type U1 (1B value) -- Output rate of the NMEA-GX-VTG message on port SPI
#define CFG_MSGOUT_NMEA_ID_VTG_UART1                   (0x209100B1U)  // type U1 (1B value) -- Output rate of the NMEA-GX-VTG message on port UART1
#define CFG_MSGOUT_NMEA_ID_ZDA_I2C                     (0x209100D8U)  // type U1 (1B value) -- Output rate of the NMEA-GX-ZDA message on port I2C
#define CFG_MSGOUT_NMEA_ID_ZDA_SPI                     (0x209100DCU)  // type U1 (1B value) -- Output rate of the NMEA-GX-ZDA message on port SPI
#define CFG_MSGOUT_NMEA_ID_ZDA_UART1                   (0x209100D9U)  // type U1 (1B value) -- Output rate of the NMEA-GX-ZDA message on port UART1
#define CFG_MSGOUT_PUBX_ID_POLYP_I2C                   (0x209100ECU)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX00 message on port I2C
#define CFG_MSGOUT_PUBX_ID_POLYP_SPI                   (0x209100F0U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX00 message on port SPI
#define CFG_MSGOUT_PUBX_ID_POLYP_UART1                 (0x209100EDU)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX00 message on port UART1
#define CFG_MSGOUT_PUBX_ID_POLYS_I2C                   (0x209100F1U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX03 message on port I2C
#define CFG_MSGOUT_PUBX_ID_POLYS_SPI                   (0x209100F5U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX03 message on port SPI
#define CFG_MSGOUT_PUBX_ID_POLYS_UART1                 (0x209100F2U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX03 message on port UART1
#define CFG_MSGOUT_PUBX_ID_POLYT_I2C                   (0x209100F6U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX04 message on port I2C
#define CFG_MSGOUT_PUBX_ID_POLYT_SPI                   (0x209100FAU)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX04 message on port SPI
#define CFG_MSGOUT_PUBX_ID_POLYT_UART1                 (0x209100F7U)  // type U1 (1B value) -- Output rate of the NMEA-GX-PUBX04 message on port UART1
#define CFG_MSGOUT_UBX_MON_COMMS_I2C                   (0x2091034FU)  // type U1 (1B value) -- Output rate of the UBX-MON-COMMS message on port I2C
#define CFG_MSGOUT_UBX_MON_COMMS_SPI                   (0x20910353U)  // type U1 (1B value) -- Output rate of the UBX-MON-COMMS message on port SPI
#define CFG_MSGOUT_UBX_MON_COMMS_UART1                 (0x20910350U)  // type U1 (1B value) -- Output rate of the UBX-MON-COMMS message on port UART1
#define CFG_MSGOUT_UBX_MON_HW3_I2C                     (0x20910354U)  // type U1 (1B value) -- Output rate of the UBX-MON-HW3 message on port I2C
#define CFG_MSGOUT_UBX_MON_HW3_SPI                     (0x20910358U)  // type U1 (1B value) -- Output rate of the UBX-MON-HW3 message on port SPI
#define CFG_MSGOUT_UBX_MON_HW3_UART1                   (0x20910355U)  // type U1 (1B value) -- Output rate of the UBX-MON-HW3 message on port UART1
#define CFG_MSGOUT_UBX_MON_RF_I2C                      (0x20910359U)  // type U1 (1B value) -- Output rate of the UBX-MON-RF message on port I2C
#define CFG_MSGOUT_UBX_MON_RF_SPI                      (0x2091035DU)  // type U1 (1B value) -- Output rate of the UBX-MON-RF message on port SPI
#define CFG_MSGOUT_UBX_MON_RF_UART1                    (0x2091035AU)  // type U1 (1B value) -- Output rate of the UBX-MON-RF message on port UART1
#define CFG_MSGOUT_UBX_MON_RXR_I2C                     (0x20910187U)  // type U1 (1B value) -- Output rate of the UBX-MON-RXR message on port I2C
#define CFG_MSGOUT_UBX_MON_RXR_SPI                     (0x2091018BU)  // type U1 (1B value) -- Output rate of the UBX-MON-RXR message on port SPI
#define CFG_MSGOUT_UBX_MON_RXR_UART1                   (0x20910188U)  // type U1 (1B value) -- Output rate of the UBX-MON-RXR message on port UART1
#define CFG_MSGOUT_UBX_MON_SPAN_I2C                    (0x2091038BU)  // type U1 (1B value) -- Output rate of the UBX-MON-SPAN message on port I2C
#define CFG_MSGOUT_UBX_MON_SPAN_SPI                    (0x2091038FU)  // type U1 (1B value) -- Output rate of the UBX-MON-SPAN message on port SPI
#define CFG_MSGOUT_UBX_MON_SPAN_UART1                  (0x2091038CU)  // type U1 (1B value) -- Output rate of the UBX-MON-SPAN message on port UART1
#define CFG_MSGOUT_UBX_NAV_CLOCK_I2C                   (0x20910065U)  // type U1 (1B value) -- Output rate of the UBX-NAV-CLOCK message on port I2C
#define CFG_MSGOUT_UBX_NAV_CLOCK_SPI                   (0x20910069U)  // type U1 (1B value) -- Output rate of the UBX-NAV-CLOCK message on port SPI
#define CFG_MSGOUT_UBX_NAV_CLOCK_UART1                 (0x20910066U)  // type U1 (1B value) -- Output rate of the UBX-NAV-CLOCK message on port UART1
#define CFG_MSGOUT_UBX_NAV_DOP_I2C                     (0x20910038U)  // type U1 (1B value) -- Output rate of the UBX-NAV-DOP message on port I2C
#define CFG_MSGOUT_UBX_NAV_DOP_SPI                     (0x2091003CU)  // type U1 (1B value) -- Output rate of the UBX-NAV-DOP message on port SPI
#define CFG_MSGOUT_UBX_NAV_DOP_UART1                   (0x20910039U)  // type U1 (1B value) -- Output rate of the UBX-NAV-DOP message on port UART1
#define CFG_MSGOUT_UBX_NAV_EOE_I2C                     (0x2091015FU)  // type U1 (1B value) -- Output rate of the UBX-NAV-EOE message on port I2C
#define CFG_MSGOUT_UBX_NAV_EOE_SPI                     (0x20910163U)  // type U1 (1B value) -- Output rate of the UBX-NAV-EOE message on port SPI
#define CFG_MSGOUT_UBX_NAV_EOE_UART1                   (0x20910160U)  // type U1 (1B value) -- Output rate of the UBX-NAV-EOE message on port UART1
#define CFG_MSGOUT_UBX_NAV_ODO_I2C                     (0x2091007EU)  // type U1 (1B value) -- Output rate of the UBX-NAV-ODO message on port I2C
#define CFG_MSGOUT_UBX_NAV_ODO_SPI                     (0x20910082U)  // type U1 (1B value) -- Output rate of the UBX-NAV-ODO message on port SPI
#define CFG_MSGOUT_UBX_NAV_ODO_UART1                   (0x2091007FU)  // type U1 (1B value) -- Output rate of the UBX-NAV-ODO message on port UART1
#define CFG_MSGOUT_UBX_NAV_ORB_I2C                     (0x20910010U)  // type U1 (1B value) -- Output rate of the UBX-NAV-ORB message on port I2C
#define CFG_MSGOUT_UBX_NAV_ORB_SPI                     (0x20910014U)  // type U1 (1B value) -- Output rate of the UBX-NAV-ORB message on port SPI
#define CFG_MSGOUT_UBX_NAV_ORB_UART1                   (0x20910011U)  // type U1 (1B value) -- Output rate of the UBX-NAV-ORB message on port UART1
#define CFG_MSGOUT_UBX_NAV_PL_I2C                      (0x20910415U)  // type U1 (1B value) -- Output rate of the UBX-NAV-PL message on port I2C
#define CFG_MSGOUT_UBX_NAV_PL_SPI                      (0x20910419U)  // type U1 (1B value) -- Output rate of the UBX-NAV-PL message on port SPI
#define CFG_MSGOUT_UBX_NAV_PL_UART1                    (0x20910416U)  // type U1 (1B value) -- Output rate of the UBX-NAV-PL message on port UART1
#define CFG_MSGOUT_UBX_NAV_POSECEF_I2C                 (0x20910024U)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSECEF message on port I2C
#define CFG_MSGOUT_UBX_NAV_POSECEF_SPI                 (0x20910028U)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSECEF message on port SPI
#define CFG_MSGOUT_UBX_NAV_POSECEF_UART1               (0x20910025U)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSECEF message on port UART1
#define CFG_MSGOUT_UBX_NAV_POSLLH_I2C                  (0x20910029U)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSLLH message on port I2C
#define CFG_MSGOUT_UBX_NAV_POSLLH_SPI                  (0x2091002DU)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSLLH message on port SPI
#define CFG_MSGOUT_UBX_NAV_POSLLH_UART1                (0x2091002AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-POSLLH message on port UART1
#define CFG_MSGOUT_UBX_NAV_PVT_I2C                     (0x20910006U)  // type U1 (1B value) -- Output rate of the UBX-NAV-PVT message on port I2C
#define CFG_MSGOUT_UBX_NAV_PVT_SPI                     (0x2091000AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-PVT message on port SPI
#define CFG_MSGOUT_UBX_NAV_PVT_UART1                   (0x20910007U)  // type U1 (1B value) -- Output rate of the UBX-NAV-PVT message on port UART1
#define CFG_MSGOUT_UBX_NAV_SAT_I2C                     (0x20910015U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SAT message on port I2C
#define CFG_MSGOUT_UBX_NAV_SAT_SPI                     (0x20910019U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SAT message on port SPI
#define CFG_MSGOUT_UBX_NAV_SAT_UART1                   (0x20910016U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SAT message on port UART1


#define CFG_MSGOUT_UBX_NAV_SBAS_I2C                   (0x2091006AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-SBAS message on port I2C
#define CFG_MSGOUT_UBX_NAV_SBAS_SPI                   (0x2091006EU)  // type U1 (1B value) -- Output rate of the UBX-NAV-SBAS message on port SPI
#define CFG_MSGOUT_UBX_NAV_SBAS_UART1                 (0x2091006BU)  // type U1 (1B value) -- Output rate of the UBX-NAV-SBAS message on port UART1
#define CFG_MSGOUT_UBX_NAV_SIG_I2C                    (0x20910345U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SIG message on port I2C
#define CFG_MSGOUT_UBX_NAV_SIG_SPI                    (0x20910349U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SIG message on port SPI
#define CFG_MSGOUT_UBX_NAV_SIG_UART1                  (0x20910346U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SIG message on port UART1
#define CFG_MSGOUT_UBX_NAV_SLAS_I2C                   (0x20910336U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SLAS message on port I2C
#define CFG_MSGOUT_UBX_NAV_SLAS_SPI                   (0x2091033AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-SLAS message on port SPI
#define CFG_MSGOUT_UBX_NAV_SLAS_UART1                 (0x20910337U)  // type U1 (1B value) -- Output rate of the UBX-NAV-SLAS message on port UART1
#define CFG_MSGOUT_UBX_NAV_STATUS_I2C                 (0x2091001AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-STATUS message on port I2C
#define CFG_MSGOUT_UBX_NAV_STATUS_SPI                 (0x2091001EU)  // type U1 (1B value) -- Output rate of the UBX-NAV-STATUS message on port SPI
#define CFG_MSGOUT_UBX_NAV_STATUS_UART1               (0x2091001BU)  // type U1 (1B value) -- Output rate of the UBX-NAV-STATUS message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEBDS_I2C                (0x20910051U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEBDS message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEBDS_SPI                (0x20910055U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEBDS message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEBDS_UART1              (0x20910052U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEBDS message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEGAL_I2C                (0x20910056U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGAL message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEGAL_SPI                (0x2091005AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGAL message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEGAL_UART1              (0x20910057U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGAL message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEGLO_I2C                (0x2091004CU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGLO message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEGLO_SPI                (0x20910050U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGLO message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEGLO_UART1              (0x2091004DU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGLO message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEGPS_I2C                (0x20910047U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGPS message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEGPS_SPI                (0x2091004BU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGPS message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEGPS_UART1              (0x20910048U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEGPS message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMELS_I2C                 (0x20910060U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMELS message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMELS_SPI                 (0x20910064U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMELS message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMELS_UART1               (0x20910061U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMELS message on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEQZSS_I2C               (0x20910386U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEQZSSmessage on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEQZSS_SPI               (0x2091038AU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEQZSSmessage on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEQZSS_UART1             (0x20910387U)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEQZSSmessage on port UART1
#define CFG_MSGOUT_UBX_NAV_TIMEUTC_I2C                (0x2091005BU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEUTC message on port I2C
#define CFG_MSGOUT_UBX_NAV_TIMEUTC_SPI                (0x2091005FU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEUTC message on port SPI
#define CFG_MSGOUT_UBX_NAV_TIMEUTC_UART1              (0x2091005CU)  // type U1 (1B value) -- Output rate of the UBX-NAV-TIMEUTC message on port UART1
#define CFG_MSGOUT_UBX_NAV_VELECEF_I2C                (0x2091003DU)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELECEF message on port I2C
#define CFG_MSGOUT_UBX_NAV_VELECEF_SPI                (0x20910041U)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELECEF message on port SPI
#define CFG_MSGOUT_UBX_NAV_VELECEF_UART1              (0x2091003EU)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELECEF message on port UART1
#define CFG_MSGOUT_UBX_NAV_VELNED_I2C                 (0x20910042U)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELNED message on port I2C
#define CFG_MSGOUT_UBX_NAV_VELNED_SPI                 (0x20910046U)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELNED message on port SPI
#define CFG_MSGOUT_UBX_NAV_VELNED_UART1               (0x20910043U)  // type U1 (1B value) -- Output rate of the UBX-NAV-VELNED message on port UART1
#define CFG_MSGOUT_UBX_RXM_MEASX_I2C                  (0x20910204U)  // type U1 (1B value) -- Output rate of the UBX-RXM-MEASX message on port I2C
#define CFG_MSGOUT_UBX_RXM_MEASX_SPI                  (0x20910208U)  // type U1 (1B value) -- Output rate of the UBX-RXM-MEASX message on port SPI
#define CFG_MSGOUT_UBX_RXM_MEASX_UART1                (0x20910205U)  // type U1 (1B value) -- Output rate of the UBX-RXM-MEASX message on port UART1
#define CFG_MSGOUT_UBX_RXM_RLM_I2C                    (0x2091025EU)  // type U1 (1B value) -- Output rate of the UBX-RXM-RLM message on port I2C
#define CFG_MSGOUT_UBX_RXM_RLM_SPI                    (0x20910262U)  // type U1 (1B value) -- Output rate of the UBX-RXM-RLM message on port SPI
#define CFG_MSGOUT_UBX_RXM_RLM_UART1                  (0x2091025FU)  // type U1 (1B value) -- Output rate of the UBX-RXM-RLM message on port UART1
#define CFG_MSGOUT_UBX_RXM_SFRBX_I2C                  (0x20910231U)  // type U1 (1B value) -- Output rate of the UBX-RXM-SFRBX message on port I2C
#define CFG_MSGOUT_UBX_RXM_SFRBX_SPI                  (0x20910235U)  // type U1 (1B value) -- Output rate of the UBX-RXM-SFRBX message on port SPI
#define CFG_MSGOUT_UBX_RXM_SFRBX_UART1                (0x20910232U)  // type U1 (1B value) -- Output rate of the UBX-RXM-SFRBX message on port UART1
#define CFG_MSGOUT_UBX_TIM_TM2_I2C                    (0x20910178U)  // type U1 (1B value) -- Output rate of the UBX-TIM-TM2 message on port I2C
#define CFG_MSGOUT_UBX_TIM_TM2_SPI                    (0x2091017CU)  // type U1 (1B value) -- Output rate of the UBX-TIM-TM2 message on port SPI
#define CFG_MSGOUT_UBX_TIM_TM2_UART1                  (0x20910179U)  // type U1 (1B value) -- Output rate of the UBX-TIM-TM2 message on port UART1
#define CFG_MSGOUT_UBX_TIM_TP_I2C                     (0x2091017DU)  // type U1 (1B value) -- Output rate of the UBX-TIM-TP message on port I2C
#define CFG_MSGOUT_UBX_TIM_TP_SPI                     (0x20910181U)  // type U1 (1B value) -- Output rate of the UBX-TIM-TP message on port SPI
#define CFG_MSGOUT_UBX_TIM_TP_UART1                   (0x2091017EU)  // type U1 (1B value) -- Output rate of the UBX-TIM-TP message on port UART1
#define CFG_MSGOUT_UBX_TIM_VRFY_I2C                   (0x20910092U)  // type U1 (1B value) -- Output rate of the UBX-TIM-VRFY message on port I2C
#define CFG_MSGOUT_UBX_TIM_VRFY_SPI                   (0x20910096U)  // type U1 (1B value) -- Output rate of the UBX-TIM-VRFY message on port SPI
#define CFG_MSGOUT_UBX_TIM_VRFY_UART1                 (0x20910093U)  // type U1 (1B value) -- Output rate of the UBX-TIM-VRFY message on port UART1
#define CFG_MSGOUT_UBX_NAV_COV_I2C                    (0x20910083U)  // type U1 (1B value) -- Output rate of the UBX-NAV-COV message on port I2C
#define CFG_MSGOUT_UBX_NAV_COV_UART1                  (0x20910084U)  // type U1 (1B value) -- Output rate of the UBX-NAV-COV message on port UART1
#define CFG_MSGOUT_UBX_NAV_COV_SPI                    (0x20910087U)  // type U1 (1B value) -- Output rate of the UBX-NAV-COV message on port SPI
#define CFG_MSGOUT_UBX_NAV_AOPSTATUS_I2C              (0x20910079U)  // type U1 (1B value)
#define CFG_MSGOUT_UBX_NAV_AOPSTATUS_SPI              (0x2091007DU)  // type U1 (1B value)
#define CFG_MSGOUT_UBX_NAV_AOPSTATUS_UART1            (0x2091007AU)  // type U1 (1B value)
 
/* ===================== CFG-NAVSPG ===================== */
 
#define CFG_NAVSPG_FIXMODE                            (0x20110011U)  // type E1 (1B value) -- Position fix mode
#define CFG_NAVSPG_INIFIX3D                           (0x10110013U)  // type L (1B value) -- Initial fix must be a 3D fix
#define CFG_NAVSPG_WKNROLLOVER                        (0x30110017U)  // type U2 (2B value) -- GPS week rollover number
#define CFG_NAVSPG_USE_PPP                            (0x10110019U)  // type L (1B value) -- Use precise point positioning (PPP)
#define CFG_NAVSPG_UTCSTANDARD                        (0x2011001CU)  // type E1 (1B value) -- UTC standard to be used
#define CFG_NAVSPG_DYNMODEL                           (0x20110021U)  // type E1 (1B value) -- Dynamic platform model
#define CFG_NAVSPG_ACKAIDING                          (0x10110025U)  // type L (1B value) -- Acknowledge assistance input messages
#define CFG_NAVSPG_USE_USRDAT                         (0x10110061U)  // type L (1B value) -- Use user geodetic datum parameters
#define CFG_NAVSPG_USRDAT_MAJA                        (0x50110062U)  // type R8 (8B value) -- Geodetic datum semi-major axis
#define CFG_NAVSPG_USRDAT_FLAT                        (0x50110063U)  // type R8 (8B value) -- Geodetic datum 1.0 flattening
#define CFG_NAVSPG_USRDAT_DX                          (0x40110064U)  // type R4 (4B value) -- Geodetic datum X axis shift at the origin
#define CFG_NAVSPG_USRDAT_DY                          (0x40110065U)  // type R4 (4B value) -- Geodetic datum Y axis shift at the origin
#define CFG_NAVSPG_USRDAT_DZ                          (0x40110066U)  // type R4 (4B value) -- Geodetic datum Z axis shift at the origin
#define CFG_NAVSPG_USRDAT_ROTX                        (0x40110067U)  // type R4 (4B value) -- arcsec Geodetic datum rotation about the X axis
#define CFG_NAVSPG_USRDAT_ROTY                        (0x40110068U)  // type R4 (4B value) -- arcsec Geodetic datum rotation about the Y axis
#define CFG_NAVSPG_USRDAT_ROTZ                        (0x40110069U)  // type R4 (4B value) -- arcsec Geodetic datum rotation about the Z axis
#define CFG_NAVSPG_USRDAT_SCALE                       (0x4011006AU)  // type R4 (4B value) -- ppm Geodetic datum scale factor
#define CFG_NAVSPG_INFIL_MINSVS                       (0x201100A1U)  // type U1 (1B value) -- Minimum number of satellites for navigation
#define CFG_NAVSPG_INFIL_MAXSVS                       (0x201100A2U)  // type U1 (1B value) -- Maximum number of satellites for navigation
#define CFG_NAVSPG_INFIL_MINCNO                       (0x201100A3U)  // type U1 (1B value) -- Minimum satellite signal level for navigation
#define CFG_NAVSPG_INFIL_MINELEV                      (0x201100A4U)  // type I1 (1B value) -- Minimum elevation for a GNSS satellite to be used in navigation
#define CFG_NAVSPG_INFIL_NCNOTHRS                     (0x201100AAU)  // type U1 (1B value) -- Number of satellites required to have C/N0 above const uint32_t UBLOX_CFG_NAVSPG-INFIL_CNOTHRS for a fix to be attempted
#define CFG_NAVSPG_INFIL_CNOTHRS                      (0x201100ABU)  // type U1 (1B value) -- C/N0 threshold for deciding whether to attempt a fix
#define CFG_NAVSPG_OUTFIL_PDOP                        (0x301100B1U)  // type U2 (2B value) -- Output filter position DOP mask (threshold)
#define CFG_NAVSPG_OUTFIL_TDOP                        (0x301100B2U)  // type U2 (2B value) -- Output filter time DOP mask (threshold)
#define CFG_NAVSPG_OUTFIL_PACC                        (0x301100B3U)  // type U2 (2B value) -- Output filter position accuracy mask (threshold)
#define CFG_NAVSPG_OUTFIL_TACC                        (0x301100B4U)  // type U2 (2B value) -- Output filter time accuracy mask (threshold)
#define CFG_NAVSPG_OUTFIL_FACC                        (0x301100B5U)  // type U2 (2B value) -- Output filter frequency accuracy mask (threshold)
#define CFG_NAVSPG_CONSTR_ALT                         (0x401100C1U)  // type I4 (4B value) -- Fixed altitude (mean sea level) for 2D fix mode
#define CFG_NAVSPG_CONSTR_ALTVAR                      (0x401100C2U)  // type U4 (4B value) -- Fixed altitude variance for 2D mode
#define CFG_NAVSPG_CONSTR_DGNSSTO                     (0x201100C4U)  // type U1 (1B value) -- DGNSS timeout
#define CFG_NAVSPG_CONSTR_DGNSSTO_SCALE               (0x201100C5U)  // type U1 (1B value) -- DGNSS timeout value scale for CFG-NAVSPGCONSTR_DGNSSTO
#define CFG_NAVSPG_SIGATTCOMP                         (0x201100D6U)  // type E1 (1B value) -- Permanently attenuated signal compensation mode
#define CFG_NAVSPG_PL_ENA                             (0x101100D7U)  // type L (1B value) -- Enable Protection level. If enabled, protection level computing will be on.
#define CFG_NAVSPG_ONLY_AUTHDATA                      (0x101100DDU)  // type L (1B value) -- Enable using only signals with authenticated navigation data
#define CFG_NAVSPG_MAX_TIMETRUSTED_ACC                (0x301100DEU)  // type U2 (2B value) -- Maximum trusted time accuracy
 
/* ===================== CFG-NMEA ===================== */
 
#define CFG_NMEA_PROTVER                              (0x20930001U)  // type E1 (1B value) -- NMEA protocol version
#define CFG_NMEA_MAXSVS                               (0x20930002U)  // type E1 (1B value) -- Maximum number of SVs to report per Talker ID
#define CFG_NMEA_COMPAT                               (0x10930003U)  // type L (1B value) -- Enable compatibility mode
#define CFG_NMEA_CONSIDER                             (0x10930004U)  // type L (1B value) -- Enable considering mode
#define CFG_NMEA_LIMIT82                              (0x10930005U)  // type L (1B value) -- Enable strict limit to 82 characters maximum NMEA message length
#define CFG_NMEA_HIGHPREC                             (0x10930006U)  // type L (1B value) -- Enable high precision mode
#define CFG_NMEA_SVNUMBERING                          (0x20930007U)  // type E1 (1B value) -- Display configuration for SVs that do not have value defined in NMEA
#define CFG_NMEA_FILT_GPS                             (0x10930011U)  // type L (1B value) -- Disable reporting of GPS satellites
#define CFG_NMEA_FILT_SBAS                            (0x10930012U)  // type L (1B value) -- Disable reporting of SBAS satellites
#define CFG_NMEA_FILT_GAL                             (0x10930013U)  // type L (1B value) -- Disable reporting of Galileo satellites
#define CFG_NMEA_FILT_QZSS                            (0x10930015U)  // type L (1B value) -- Disable reporting of QZSS satellites
#define CFG_NMEA_FILT_GLO                             (0x10930016U)  // type L (1B value) -- Disable reporting of GLONASS satellites
#define CFG_NMEA_FILT_BDS                             (0x10930017U)  // type L (1B value) -- Disable reporting of BeiDou satellites
#define CFG_NMEA_FILT_NAVIC                           (0x10930018U)  // type L (1B value) -- Disable reporting of NavIC satellites
#define CFG_NMEA_OUT_INVFIX                           (0x10930021U)  // type L (1B value) -- Enable position output for failed or invalid fixes
#define CFG_NMEA_OUT_MSKFIX                           (0x10930022U)  // type L (1B value) -- Enable position output for invalid fixes
#define CFG_NMEA_OUT_INVTIME                          (0x10930023U)  // type L (1B value) -- Enable time output for invalid times
#define CFG_NMEA_OUT_INVDATE                          (0x10930024U)  // type L (1B value) -- Enable date output for invalid dates
#define CFG_NMEA_OUT_ONLYGPS                          (0x10930025U)  // type L (1B value) -- Restrict output to GPS satellites only
#define CFG_NMEA_OUT_FROZENCOG                        (0x10930026U)  // type L (1B value) -- Enable course over ground output even if it is frozen
#define CFG_NMEA_MAINTALKERID                         (0x20930031U)  // type E1 (1B value) -- Main Talker ID
#define CFG_NMEA_GSVTALKERID                          (0x20930032U)  // type E1 (1B value) -- Talker ID for GSV NMEA messages
#define CFG_NMEA_BDSTALKERID                          (0x30930033U)  // type U2 (2B value) -- BeiDou Talker ID
 
/* ===================== CFG-ODO ===================== */
 
#define CFG_ODO_USE_ODO                               (0x10220001U)  // type L (1B value) -- Use odometer
#define CFG_ODO_USE_COG                               (0x10220002U)  // type L (1B value) -- Use low-speed course over ground filter
#define CFG_ODO_OUTLPVEL                              (0x10220003U)  // type L (1B value) -- Output low-pass filtered velocity
#define CFG_ODO_OUTLPCOG                              (0x10220004U)  // type L (1B value) -- Output low-pass filtered course over ground (heading)
#define CFG_ODO_PROFILE                               (0x20220005U)  // type E1 (1B value) -- Odometer profile configuration
#define CFG_ODO_COGMAXSPEED                           (0x20220021U)  // type U1 (1B value) -- Upper speed limit for low-speed course over ground filter
#define CFG_ODO_COGMAXPOSACC                          (0x20220022U)  // type U1 (1B value) -- Maximum acceptable position accuracy for computing low-speed filtered course over ground
#define CFG_ODO_VELLPGAIN                             (0x20220031U)  // type U1 (1B value) -- Velocity low-pass filter level
#define CFG_ODO_COGLPGAIN                             (0x20220032U)  // type U1 (1B value) -- Course over ground low-pass filter level (at speed < 8 m/s)
 
/* ===================== CFG-PM ===================== */
 
#define CFG_PM_OPERATEMODE                            (0x20D00001U)  // type E1 (1B value) -- General mode of operation
#define CFG_PM_POSUPDATEPERIOD                        (0x40D00002U)  // type U4 (4B value) -- Position update period for PSMOO - s
#define CFG_PM_ACQPERIOD                              (0x40D00003U)  // type U4 (4B value) -- Acquisition period used if the receiver previously failed to achieve a position fix - s
#define CFG_PM_GRIDOFFSET                             (0x40D00004U)  // type U4 (4B value) -- Position update period grid offset relative to GPS start of week - s
#define CFG_PM_ONTIME                                 (0x30D00005U)  // type U2 (2B value) -- Time to stay in Tracking state - s
#define CFG_PM_MINACQTIME                             (0x20D00006U)  // type U1 (1B value) -- Minimum time to spend in Acquisition state - s
#define CFG_PM_MAXACQTIME                             (0x20D00007U)  // type U1 (1B value) -- Maximum time to spend in Acquisition state - s
#define CFG_PM_DONOTENTEROFF                          (0x10D00008U)  // type L (1B value) -- Behavior of receiver in case it cannot achieve a position fix during a position update period.
#define CFG_PM_WAITTIMEFIX                            (0x10D00009U)  // type L (1B value) -- Wait for time fix
#define CFG_PM_UPDATEEPH                              (0x10D0000AU)  // type L (1B value) -- Update ephemeris regularly.
#define CFG_PM_EXTINTSEL                              (0x20D0000BU)  // type E1 (1B value) -- EXTINT pin select
#define CFG_PM_EXTINTWAKE                             (0x10D0000CU)  // type L (1B value) -- EXTINT pin control (Wake). Enable to keep receiver awake as long as selected EXTINT pin is "high".
#define CFG_PM_EXTINTBACKUP                           (0x10D0000DU)  // type L (1B value) -- EXTINT pin control (Backup). Enable to force receiver into BACKUP mode when selected EXTINT pin is "low".
#define CFG_PM_EXTINTINACTIVE                         (0x10D0000EU)  // type L (1B value) -- EXTINT pin control (Inactive). Enable to force backup in case EXTINT Pin is inactive for time longer than CFG-PM-EXTINTINACTIVITY.
#define CFG_PM_EXTINTINACTIVITY                       (0x40D0000FU)  // type U4 (4B value) -- Inactivity time out on EXTINT pin if enabled
#define CFG_PM_LIMITPEAKCURR                          (0x10D00010U)  // type L (1B value) -- Limit peak current
 
/* ===================== CFG-QZSS ===================== */
 
#define CFG_QZSS_USE_SLAS_DGNSS                       (0x10370005U)  // type L (1B value) -- Apply QZSS SLAS DGNSS corrections
#define CFG_QZSS_USE_SLAS_TESTMODE                    (0x10370006U)  // type L (1B value) -- Use QZSS SLAS data when it is in test mode (SLAS msg 0)
#define CFG_QZSS_USE_SLAS_RAIM_UNCORR                 (0x10370007U)  // type L (1B value) -- Raim out measurements that are not corrected by QZSS SLAS, if at least 5 measurements are corrected
#define CFG_QZSS_SLAS_MAX_BASELINE                    (0x30370008U)  // type U2 (2B value) -- Maximum baseline distance to closest Ground Monitoring Station: km
 
/* ===================== CFG-RATE ===================== */
 
#define CFG_RATE_MEAS                                 (0x30210001U)  // type U2 (2B value) -- Nominal time between GNSS measurements
#define CFG_RATE_NAV                                  (0x30210002U)  // type U2 (2B value) -- Ratio of number of measurements to number of navigation solutions
#define CFG_RATE_TIMEREF                              (0x20210003U)  // type E1 (1B value) -- Time system to which measurements are aligned
#define CFG_RATE_NAV_PRIO                             (0x20210004U)  // type U1 (1B value) -- Output rate of priority navigation mode messages
 
/* ===================== CFG-RINV ===================== */
 
#define CFG_RINV_DUMP                                 (0x10C70001U)  // type L (1B value) -- Dump data at startup
#define CFG_RINV_BINARY                               (0x10C70002U)  // type L (1B value) -- Data is binary
#define CFG_RINV_DATA_SIZE                            (0x20C70003U)  // type U1 (1B value) -- Size of data
#define CFG_RINV_CHUNK0                               (0x50C70004U)  // type X8 (8B value) -- Data bytes 1-8 (LSB)
#define CFG_RINV_CHUNK1                               (0x50C70005U)  // type X8 (8B value) -- Data bytes 9-16
#define CFG_RINV_CHUNK2                               (0x50C70006U)  // type X8 (8B value) -- Data bytes 17-24
#define CFG_RINV_CHUNK3                               (0x50C70007U)  // type X8 (8B value) -- Data bytes 25-30 (MSB)
 
/* ===================== CFG-SBAS ===================== */
 
#define CFG_SBAS_USE_TESTMODE                         (0x10360002U)  // type L (1B value) -- Use SBAS data when it is in test mode (SBAS msg 0)
#define CFG_SBAS_USE_RANGING                          (0x10360003U)  // type L (1B value) -- Use SBAS GEOs as a ranging source (for navigation)
#define CFG_SBAS_USE_DIFFCORR                         (0x10360004U)  // type L (1B value) -- Use SBAS diﬀerential corrections
#define CFG_SBAS_USE_INTEGRITY                        (0x10360005U)  // type L (1B value) -- Use SBAS integrity information
#define CFG_SBAS_ACCEPT_NOT_IN_PRNMASK                (0x30360008U)  // type X2 (2B value) -- Accept corrections from SBAS SV
#define CFG_SBAS_USE_IONOONLY                         (0x10360007U)  // type L (1B value) -- Use SBAS ionosphere correction only
#define CFG_SBAS_PRNSCANMASK                          (0x50360006U)  // type X8 (8B value) -- SBAS PRN search configuration
 
/* ===================== CFG-SEC ===================== */
 
#define CFG_SEC_CFG_LOCK                              (0x10F60009U)  // type L (1B value) -- Configuration lockdown
#define CFG_SEC_CFG_LOCK_UNLOCKGRP1                   (0x30F6000AU)  // type U2 (2B value) -- Configuration lockdown exempted group 1
#define CFG_SEC_CFG_LOCK_UNLOCKGRP2                   (0x30F6000BU)  // type U2 (2B value) -- Configuration lockdown exempted group 2
#define CFG_SEC_SPOOFDET_SIM_SIG_DIS                  (0x10F6005DU)  // type L (1B value) -- Disabling the simulated signal spoofing detection
#define CFG_SEC_JAMDET_SENSITIVITY_HI                 (0x10F60051U)  // type L (1B value) -- go for a more sensitive jamming detection
 
/* ===================== CFG-SIGNAL ===================== */
 
#define CFG_SIGNAL_GPS_ENA                            (0x1031001FU)  // type L (1B value) -- GPS enable
#define CFG_SIGNAL_GPS_L1CA_ENA                       (0x10310001U)  // type L (1B value) -- GPS L1C/A
#define CFG_SIGNAL_GPS_L5_ENA                         (0x10310004U)  // type L (1B value) -- GPS L5
#define CFG_SIGNAL_GPS_L2C_ENA                        (0x10310003U)  // type L (1B value) -- GPS L2C (only on u-blox F9 platform products)
#define CFG_SIGNAL_SBAS_ENA                           (0x10310020U)  // type L (1B value) -- SBAS enable
#define CFG_SIGNAL_SBAS_L1CA_ENA                      (0x10310005U)  // type L (1B value) -- SBAS L1C/A
#define CFG_SIGNAL_GAL_ENA                            (0x10310021U)  // type L (1B value) -- Galileo enable
#define CFG_SIGNAL_GAL_E1_ENA                         (0x10310007U)  // type L (1B value) -- Galileo E1
#define CFG_SIGNAL_GAL_E5A_ENA                        (0x10310009U)  // type L (1B value) -- Galileo E5a
#define CFG_SIGNAL_GAL_E5B_ENA                        (0x1031000AU)  // type L (1B value) -- Galileo E5b (only on u-blox F9 platform products)
#define CFG_SIGNAL_GAL_E6_ENA                         (0x1031000BU)  // type L (1B value) -- Galileo E6 (Default is true)
#define CFG_SIGNAL_BDS_ENA                            (0x10310022U)  // type L (1B value) -- BeiDou Enable
#define CFG_SIGNAL_BDS_B1_ENA                         (0x1031000DU)  // type L (1B value) -- BeiDou B1I
#define CFG_SIGNAL_BDS_B1C_ENA                        (0x1031000FU)  // type L (1B value) -- BeiDou B1C
#define CFG_SIGNAL_BDS_B2A_ENA                        (0x10310028U)  // type L (1B value) -- BeiDou B2a
#define CFG_SIGNAL_BDS_B2_ENA                         (0x1031000EU)  // type L (1B value) -- BeiDou B2I (only on u-blox F9 platform products)
#define CFG_SIGNAL_BDS_B3_ENA                         (0x10310010U)  // type L (1B value) -- BeiDou B3I (Default is true)
#define CFG_SIGNAL_QZSS_ENA                           (0x10310024U)  // type L (1B value) -- QZSS enable
#define CFG_SIGNAL_QZSS_L1CA_ENA                      (0x10310012U)  // type L (1B value) -- QZSS L1C/A
#define CFG_SIGNAL_QZSS_L1CB_ENA                      (0x10310039U)  // type L (1B value) -- QZSS L1C/B
#define CFG_SIGNAL_QZSS_L5_ENA                        (0x10310017U)  // type L (1B value) -- QZSS L5
#define CFG_SIGNAL_QZSS_L1S_ENA                       (0x10310014U)  // type L (1B value) -- QZSS L1S
#define CFG_SIGNAL_QZSS_L2C_ENA                       (0x10310015U)  // type L (1B value) -- QZSS L2C (only on u-blox F9 platform products)
#define CFG_SIGNAL_GLO_ENA                            (0x10310025U)  // type L (1B value) -- GLONASS enable
#define CFG_SIGNAL_GLO_L1_ENA                         (0x10310018U)  // type L (1B value) -- GLONASS L1
#define CFG_SIGNAL_GLO_L2_ENA                         (0x1031001AU)  // type L (1B value) -- GLONASS L2 (only on u-blox F9 platform products)
#define CFG_SIGNAL_NAVIC_ENA                          (0x10310026U)  // type L (1B value) -- NavIC
#define CFG_SIGNAL_NAVIC_L5_ENA                       (0x1031001DU)  // type L (1B value) -- NavIC L5
#define CFG_SIGNAL_PLAN                               (0x2031003AU)  // type E1 (1B value) -- Active signal plan
 
/* ===================== CFG-SPI ===================== */
 
#define CFG_SPI_MAXFF                                 (0x20640001U)  // type U1 (1B value) -- Number of bytes containing 0xFF to receive before switching oﬀ reception. Range: 0 (mechanism oﬀ) - 63
#define CFG_SPI_CPOLARITY                             (0x10640002U)  // type L (1B value) -- Clock polarity select: 0: Active Hight Clock, SCLK idles low, 1: Active Low Clock, SCLK idles high
#define CFG_SPI_CPHASE                                (0x10640003U)  // type L (1B value) -- Clock phase select: 0: Data captured on first edge of SCLK, 1: Data captured on second edge of SCLK
#define CFG_SPI_EXTENDEDTIMEOUT                       (0x10640005U)  // type L (1B value) -- Flag to disable timeouting the interface after 1.5s
#define CFG_SPI_ENABLED                               (0x10640006U)  // type L (1B value) -- Flag to indicate if the SPI interface should be enabled
 
/* ===================== CFG-SPIINPROT ===================== */
 
#define CFG_SPIINPROT_UBX                             (0x10790001U)  // type L (1B value) -- Flag to indicate if UBX should be an input protocol on SPI
#define CFG_SPIINPROT_NMEA                            (0x10790002U)  // type L (1B value) -- Flag to indicate if NMEA should be an input protocol on SPI
#define CFG_SPIINPROT_RTCM3X                          (0x10790004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an input protocol on SPI
#define CFG_SPIINPROT_SPARTN                          (0x10790005U)  // type L (1B value) -- Flag to indicate if SPARTN should be an input protocol on SPI
 
/* ===================== CFG-SPIOUTPROT ===================== */
 
#define CFG_SPIOUTPROT_UBX                            (0x107A0001U)  // type L (1B value) -- Flag to indicate if UBX should be an output protocol on SPI
#define CFG_SPIOUTPROT_NMEA                           (0x107A0002U)  // type L (1B value) -- Flag to indicate if NMEA should be an output protocol on SPI
#define CFG_SPIOUTPROT_RTCM3X                         (0x107A0004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an output protocol on SPI
 
/* ===================== CFG-TP ===================== */
 
#define CFG_TP_PULSE_DEF                              (0x20050023U)  // type E1 (1B value) -- Determines whether the time pulse is interpreted as frequency or period
#define CFG_TP_PULSE_LENGTH_DEF                       (0x20050030U)  // type E1 (1B value) -- Determines whether the time pulse length is interpreted as length[us] or pulse ratio[%]
#define CFG_TP_ANT_CABLEDELAY                         (0x30050001U)  // type I2 (2B value) -- Antenna cable delay
#define CFG_TP_PERIOD_TP1                             (0x40050002U)  // type U4 (4B value) -- Time pulse period (TP1)
#define CFG_TP_PERIOD_LOCK_TP1                        (0x40050003U)  // type U4 (4B value) -- Time pulse period when locked to GNSS time (TP1)
#define CFG_TP_FREQ_TP1                               (0x40050024U)  // type U4 (4B value) -- Time pulse frequency (TP1)
#define CFG_TP_FREQ_LOCK_TP1                          (0x40050025U)  // type U4 (4B value) -- Time pulse frequency when locked to GNSS time (TP1)
#define CFG_TP_LEN_TP1                                (0x40050004U)  // type U4 (4B value) -- Time pulse length (TP1)
#define CFG_TP_LEN_LOCK_TP1                           (0x40050005U)  // type U4 (4B value) -- Time pulse length when locked to GNSS time (TP1)
#define CFG_TP_DUTY_TP1                               (0x5005002AU)  // type R8 (8B value) -- Time pulse duty cycle (TP1)
#define CFG_TP_DUTY_LOCK_TP1                          (0x5005002BU)  // type R8 (8B value) -- Time pulse duty cycle when locked to GNSS time (TP1)
#define CFG_TP_USER_DELAY_TP1                         (0x40050006U)  // type I4 (4B value) -- User-configurable time pulse delay (TP1)
#define CFG_TP_TP1_ENA                                (0x10050007U)  // type L (1B value) -- Enable the first timepulse
#define CFG_TP_SYNC_GNSS_TP1                          (0x10050008U)  // type L (1B value) -- Sync time pulse to GNSS time or local clock (TP1)
#define CFG_TP_USE_LOCKED_TP1                         (0x10050009U)  // type L (1B value) -- Use locked parameters when possible (TP1)
#define CFG_TP_ALIGN_TO_TOW_TP1                       (0x1005000AU)  // type L (1B value) -- Align time pulse to top of second (TP1)
#define CFG_TP_POL_TP1                                (0x1005000BU)  // type L (1B value) -- Set time pulse polarity (TP1)
#define CFG_TP_TIMEGRID_TP1                           (0x2005000CU)  // type E1 (1B value) -- Time grid to use (TP1)
#define CFG_TP_PERIOD_TP2                             (0x4005000DU)  // type U4 (4B value) -- Time pulse period (TP2)
#define CFG_TP_PERIOD_LOCK_TP2                        (0x4005000EU)  // type U4 (4B value) -- Time pulse period when locked to GNSS time
#define CFG_TP_FREQ_TP2                               (0x40050026U)  // type U4 (4B value) -- Time pulse frequency (TP2)
#define CFG_TP_FREQ_LOCK_TP2                          (0x40050027U)  // type U4 (4B value) -- Time pulse frequency when locked to GNSS time
#define CFG_TP_LEN_TP2                                (0x4005000FU)  // type U4 (4B value) -- Time pulse length (TP2)
#define CFG_TP_LEN_LOCK_TP2                           (0x40050010U)  // type U4 (4B value) -- Time pulse length when locked to GNSS time
#define CFG_TP_DUTY_TP2                               (0x5005002CU)  // type R8 (8B value) -- Time pulse duty cycle (TP2)
#define CFG_TP_DUTY_LOCK_TP2                          (0x5005002DU)  // type R8 (8B value) -- Time pulse duty cycle when locked to GNSS time
#define CFG_TP_USER_DELAY_TP2                         (0x40050011U)  // type I4 (4B value) -- User-configurable time pulse delay (TP2)
#define CFG_TP_TP2_ENA                                (0x10050012U)  // type L (1B value) -- Enable the second timepulse
#define CFG_TP_SYNC_GNSS_TP2                          (0x10050013U)  // type L (1B value) -- Sync time pulse to GNSS time or local clock
#define CFG_TP_USE_LOCKED_TP2                         (0x10050014U)  // type L (1B value) -- Use locked parameters when possible (TP2)
#define CFG_TP_ALIGN_TO_TOW_TP2                       (0x10050015U)  // type L (1B value) -- Align time pulse to top of second (TP2)
#define CFG_TP_POL_TP2                                (0x16050016U)  // type L (1B value) -- Set time pulse polarity (TP2)
#define CFG_TP_TIMEGRID_TP2                           (0x20050017U)  // type E1 (1B value) -- Time grid to use (TP2)
#define CFG_TP_DRSTR_TP1                              (0x20050035U)  // type E1 (1B value) -- Set drive strength of TP1
#define CFG_TP_DRSTR_TP2                              (0x20050036U)  // type E1 (1B value) -- Set drive strength of TP2
 
/* ===================== CFG-TXREADY ===================== */
 
#define CFG_TXREADY_ENABLED                           (0x10A20001U)  // type L (1B value) -- Flag to indicate if TX ready pin mechanism should be enabled
#define CFG_TXREADY_POLARITY                          (0x10A20002U)  // type L (1B value) -- The polarity of the TX ready pin: false:high- active, true:low-active
#define CFG_TXREADY_PIN                               (0x20A20003U)  // type U1 (1B value) -- Pin number to use for the TX ready functionality
#define CFG_TXREADY_THRESHOLD                         (0x30A20004U)  // type U2 (2B value) -- Amount of data that should be ready on the interface before triggering the TX ready pin
#define CFG_TXREADY_INTERFACE                         (0x20A20005U)  // type E1 (1B value) -- Interface where the TX ready feature should be linked to
 
/* ===================== CFG-UART1 ===================== */
 
#define CFG_UART1_BAUDRATE                            (0x40520001U)  // type U4 (4B value) -- The baud rate that should be configured on the UART1
#define CFG_UART1_STOPBITS                            (0x20520002U)  // type E1 (1B value) -- Number of stopbits that should be used on UART1
#define CFG_UART1_DATABITS                            (0x20520003U)  // type E1 (1B value) -- Number of databits that should be used on UART1
#define CFG_UART1_PARITY                              (0x20520004U)  // type E1 (1B value) -- Parity mode that should be used on UART1
#define CFG_UART1_ENABLED                             (0x10520005U)  // type L (1B value) -- Flag to indicate if the UART1 should be enabled
#define CFG_UART1_REMAP                               (0x10520006U)  // type L (1B value) -- UART1 Remapping
 
/* ===================== CFG-UART1INPROT ===================== */
 
#define CFG_UART1INPROT_UBX                           (0x10730001U)  // type L (1B value) -- Flag to indicate if UBX should be an input protocol on UART1
#define CFG_UART1INPROT_NMEA                          (0x10730002U)  // type L (1B value) -- Flag to indicate if NMEA should be an input protocol on UART1
#define CFG_UART1INPROT_RTCM2X                        (0x10730003U)  // type L (1B value) -- Flag to indicate if RTCM2X should be an input protocol on UART1
#define CFG_UART1INPROT_RTCM3X                        (0x10730004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an input protocol on UART1
#define CFG_UART1INPROT_SPARTN                        (0x10730005U)  // type L (1B value) -- Flag to indicate if SPARTN should be an input protocol on UART1
 
/* ===================== CFG-UART1OUTPROT ===================== */
 
#define CFG_UART1OUTPROT_UBX                          (0x10740001U)  // type L (1B value) -- Flag to indicate if UBX should be an output protocol on UART1
#define CFG_UART1OUTPROT_NMEA                         (0x10740002U)  // type L (1B value) -- Flag to indicate if NMEA should be an output protocol on UART1
#define CFG_UART1OUTPROT_RTCM3X                       (0x10740004U)  // type L (1B value) -- Flag to indicate if RTCM3X should be an output protocol on UART1


/* ===================== UBX-ACK (0x05) ===================== */
#define ubx_ack_ack (0x0501U)
#define ubx_ack_nak (0x0500U)
/* ===================== UBX-CFG (0x06) ===================== */
#define ubx_cfg_cfg (0x0609U)
#define ubx_cfg_rst (0x0604U)
#define ubx_cfg_valdel (0x068CU)
#define ubx_cfg_valget (0x068BU)
#define ubx_cfg_valset (0x068AU)
/* ===================== UBX-INF (0x04) ===================== */
#define ubx_inf_debug (0x0404U)
#define ubx_inf_error (0x0400U)
#define ubx_inf_notice (0x0402U)
#define ubx_inf_test (0x0403U)
#define ubx_inf_warning (0x0401U)
/* ===================== UBX-LOG (0x21) ===================== */
#define ubx_log_batch (0x2111U)
#define ubx_log_retrievebatch (0x2110U)
/* ===================== UBX-MGA (0x13) ===================== */
#define ubx_mga_ack (0x1360U)
#define ubx_mga_ano (0x1320U)
#define ubx_mga_bds (0x1303U)
#define ubx_mga_dbd (0x1380U)
#define ubx_mga_flash (0x1321U)
#define ubx_mga_gal (0x1302U)
#define ubx_mga_glo (0x1306U)
#define ubx_mga_gps (0x1300U)
#define ubx_mga_ini (0x1340U)
#define ubx_mga_qzss (0x1305U) 
/* ===================== UBX-MON (0x0a) ===================== */ 
#define ubx_mon_batch (0x0A32U)
#define ubx_mon_comms (0x0A36U)
#define ubx_mon_gnss (0x0A28U)
#define ubx_mon_hw3 (0x0A37U)
#define ubx_mon_patch (0x0A27U)
#define ubx_mon_rf (0x0A38U)
#define ubx_mon_rxr (0x0A21U)
#define ubx_mon_span (0x0A31U)
#define ubx_mon_ver (0x0A04U)
/* ===================== UBX-NAV (0x01) ===================== */
#define ubx_nav_aopstatus (0x0160U)
#define ubx_nav_clock (0x0122U)
#define ubx_nav_cov (0x0136U)
#define ubx_nav_dop (0x0104U)
#define ubx_nav_eoe (0x0161U)
#define ubx_nav_odo (0x0109U)
#define ubx_nav_orb (0x0134U)
#define ubx_nav_pl (0x0162U)
#define ubx_nav_posecef (0x0101U)
#define ubx_nav_posllh (0x0102U)
#define ubx_nav_pvt (0x0107U)
#define ubx_nav_resetodo (0x0110U)
#define ubx_nav_sat (0x0135U)
#define ubx_nav_sbas (0x0132U)
#define ubx_nav_sig (0x0143U)
#define ubx_nav_slas (0x0142U)
#define ubx_nav_status (0x0103U)
#define ubx_nav_timebds (0x0124U)
#define ubx_nav_timegal (0x0125U)
#define ubx_nav_timeglo (0x0123U)
#define ubx_nav_timegps (0x0120U)
#define ubx_nav_timels (0x0126U)
#define ubx_nav_timeqzss (0x0127U)
#define ubx_nav_timeutc (0x0121U)
#define ubx_nav_velecef (0x0111U)
#define ubx_nav_velned (0x0112U)
/* ===================== UBX-RXM (0x02) ===================== */
#define ubx_rxm_meas20 (0x0284U)
#define ubx_rxm_meas50 (0x0286U)
#define ubx_rxm_measc12 (0x0282U)
#define ubx_rxm_measd12 (0x0280U)
#define ubx_rxm_measx (0x0214U)
#define ubx_rxm_pmreq (0x0241U)
#define ubx_rxm_rlm (0x0259U)
#define ubx_rxm_sfrbx (0x0213U)
/* ===================== UBX-SEC (0x27) ===================== */
#define ubx_sec_uniqid (0x2703U)
/* ===================== UBX-TIM (0x0d) ===================== */
 
#define ubx_tim_tm2 (0x0D03U)
#define ubx_tim_tp (0x0D01U)
#define ubx_tim_vrfy (0x0D06U)
/* ===================== UBX-UPD (0x09) ===================== */
#define ubx_upd_sos (0x0914U)


#endif /* MAX_M10S_DEFINES_H */