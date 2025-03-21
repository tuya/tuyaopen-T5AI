#ifndef __DOORBELL_COMM_H__
#define __DOORBELL_COMM_H__

#define UDP_SDP_LOCAL_PORT              (10000)
#define UDP_SDP_REMOTE_PORT             (52110)

#define DOORBELL_CMD_PORT               (7100)

#define DOORBELL_UDP_IMG_PORT           (7180)
#define DOORBELL_UDP_AUD_PORT           (7170)

#define DOORBELL_TCP_IMG_PORT           (7150)
#define DOORBELL_TCP_AUD_PORT           (7140)

#define DOORBELL_NETWORK_MAX_SIZE       (1024)

#define TRANSMISSION_BIG_ENDIAN (BK_FALSE)


#if TRANSMISSION_BIG_ENDIAN == BK_TRUE
#define CHECK_ENDIAN_UINT32(var)    htonl(var)
#define CHECK_ENDIAN_UINT16(var)    htons(var)

#define STREAM_TO_UINT16(u16, p) {u16 = (((uint16_t)(*((p) + 1))) + (((uint16_t)(*((p)))) << 8)); (p) += 2;}
#define STREAM_TO_UINT32(u32, p) {u32 = ((((uint32_t)(*((p) + 3)))) + ((((uint32_t)(*((p) + 2)))) << 8) + ((((uint32_t)(*((p) + 1)))) << 16) + ((((uint32_t)(*((p))))) << 24)); (p) += 4;}


#else
#define CHECK_ENDIAN_UINT32
#define CHECK_ENDIAN_UINT16

#define STREAM_TO_UINT16(u16, p) {u16 = ((uint16_t)(*(p)) + (((uint16_t)(*((p) + 1))) << 8)); (p) += 2;}
#define STREAM_TO_UINT32(u32, p) {u32 = (((uint32_t)(*(p))) + ((((uint32_t)(*((p) + 1)))) << 8) + ((((uint32_t)(*((p) + 2)))) << 16) + ((((uint32_t)(*((p) + 3)))) << 24)); (p) += 4;}


#endif

#define STREAM_TO_UINT8(u8, p) {u8 = (uint8_t)(*(p)); (p) += 1;}


typedef enum
{
	DBEVT_WIFI_STATION_CONNECT,
	DBEVT_WIFI_STATION_CONNECTED,
	DBEVT_WIFI_STATION_DISCONNECTED,

	DBEVT_P2P_CS2_SERVICE_START_REQUEST,
	DBEVT_P2P_CS2_SERVICE_START_RESPONSE,

	DBEVT_LAN_UDP_SERVICE_START_REQUEST,
	DBEVT_LAN_UDP_SERVICE_START_RESPONSE,

	DBEVT_LAN_TCP_SERVICE_START_REQUEST,
	DBEVT_LAN_TCP_SERVICE_START_RESPONSE,


	DBEVT_WIFI_SOFT_AP_TURNING_ON,

	DBEVT_REMOTE_DEVICE_CONNECTED,
	DBEVT_REMOTE_DEVICE_DISCONNECTED,

	DBEVT_START_WIFI_STATION,
	DBEVT_START_TCP_SERVICE,
	DBEVT_START_BOARDING_EVENT,
	DBEVT_BLE_DISABLE,
	DBEVT_SDP,
	DBEVT_EXIT,

	DBEVT_IMAGE_TCP_SERVICE_DISCONNECTED
} dbevt_t;

typedef enum
{
	DOORBELL_SERVICE_NONE = 0,
	DOORBELL_SERVICE_P2P_CS2 = 1,
	DOORBELL_SERVICE_LAN_UDP = 2,
	DOORBELL_SERVICE_LAN_TCP = 3
} doorbell_service_t;


typedef struct
{
	uint32_t event;
	uint32_t param;
} doorbell_msg_t;

typedef enum
{
	DB_TURN_OFF,
	DB_TURN_ON,
} doorbell_state_t;




bk_err_t doorbell_send_msg(doorbell_msg_t *msg);

void doorbell_core_init(void);


typedef struct
{
	int (*init)(void *param);
	void (*deinit)(void);
	int (*camera_state_changed)(doorbell_state_t state);
	int (*audio_state_changed)(doorbell_state_t state);
	const void *camera_transfer_cb;
	const void *audio_transfer_cb;
} doorbell_service_interface_t;



#endif
