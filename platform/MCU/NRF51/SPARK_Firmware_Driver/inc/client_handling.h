/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

 /**@file
 *
 * @defgroup XXXX
 * @{
 * @ingroup  YYYY
 *
 * @brief    ZZZZZ.
 */

#ifndef CLIENT_HANDLING_H__
#define CLIENT_HANDLING_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "device_manager.h"
#include "spi_slave_stream.h"
#include "hw_gateway_config.h"

void (*tx_callback)(uint8_t *m_tx_buf, uint16_t size);

/**@brief Funtion for initializing the module.
 */
void client_handling_init(void (*a)(uint8_t *m_tx_buf, uint16_t size));

/**@brief Funtion for returning the current number of clients.
 *
 * @return  The current number of clients.
 */
uint8_t client_handling_count(void);

/**@brief Funtion for creating a new client.
 *
 * @param[in] p_handle    Device Manager Handle. For link related events, this parameter
 *                        identifies the peer.
 *
 * @param[in] conn_handle Identifies link for which client is created.
 * @return NRF_SUCCESS on success, any other on failure.
 */
uint32_t client_handling_create(const dm_handle_t * p_handle, uint16_t conn_handle);

/**@brief Funtion for freeing up a client by setting its state to idle.
 *
 * @param[in] p_handle  Device Manager Handle. For link related events, this parameter
 *                      identifies the peer.
 *
 * @return NRF_SUCCESS on success, any other on failure.
 */
uint32_t client_handling_destroy(const dm_handle_t * p_handle);

/**@brief Funtion for handling client events.
 *
 * @param[in] p_ble_evt  Event to be handled.
 */
void client_handling_ble_evt_handler(ble_evt_t * p_ble_evt);

/**@brief Funtion for sending data to the client
 *
 * @param[in] data  Data to be sent to the client.
 * @param[in] len   Lengthof the data to be sent to the client.
 */
void client_send_data(uint8_t *data, uint16_t len);


/**@brief Funtion for handling device manager events.
 *
 * @param[in] p_handle       Identifies device with which the event is associated.
 * @param[in] p_event        Event to be handled.
 * @param[in] event_result   Event result indicating whether a procedure was successful or not.
 */
ret_code_t client_handling_dm_event_handler(const dm_handle_t    * p_handle,
                                              const dm_event_t     * p_event,
                                              const ret_code_t     event_result);

void disconnect_all_peripherals(void);
void connected_peripherals(uint8_t *values);

void debugHelper(char *str);
void debugHexHelper(unsigned char *data, unsigned short len);

void setSecurityTestflags(unsigned int flag, const dm_handle_t    * p_handle);
void read_batt_level(const dm_handle_t    * p_handle);


#endif // CLIENT_HANDLING_H__

/** @} */
