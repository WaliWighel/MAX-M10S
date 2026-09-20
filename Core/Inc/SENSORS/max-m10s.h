#ifndef MAX_M10S_H
#define MAX_M10S_H

#include "stdbool.h"

typedef struct {
    bool available;

    bool new_data;

    /* Longitude */
    float lon;

    /* Latitude */
    float lat;

    /* Height above mean sea level in [mm] */
    float hMSL;

    /* NED down velocity in [mm/s] */
    float velD;

    /* Ground Speed  */
    float gSpeed;

    /* Heading of motion */
    float headMot;

} MAX_M10S_Data_t;

typedef enum {
    MAX_M10S_OK = 0x00,
    MAX_M10S_ERROR = 0x01,
    MAX_M10S_I2C_ERROR = 0x02,
    MAX_M10S_INVALID_PACKET = 0x03
}MAX_M10S_Status_t;

typedef struct {
    uint8_t *data;

    uint16_t size;
} ByteBuffer_t;


MAX_M10S_Status_t MAX_M10S_Init (I2C_HandleTypeDef *hi2c);

MAX_M10S_Status_t MAX_M10S_HandleEvent (Event_t event);
MAX_M10S_Status_t MAX_M10S_CopyLatestData (MAX_M10S_Data_t *a);

uint8_t UBX_nav_sat (void);
uint8_t UBX_ReadAvailableData (void);


#endif /* MAX_M10S_H */
