/*
 * uart_communication_fsm.h
 *
 *  Created on: Dec 14, 2023
 *      Author: Admin
 */

#ifndef SRC_UART_COMMUNICATION_FSM_H_
#define SRC_UART_COMMUNICATION_FSM_H_

#include <stdint.h>

// Define possible states for the UART communication FSM
typedef enum {
    UART_IDLE,
    UART_WAITING_FOR_OK,
    UART_SENDING_DATA
} UartCommunicationState;

// Function to initialize the UART communication FSM
void uart_communication_fsm_init(void);

// Function to update the UART communication FSM
void uart_communication_fsm_update(void);

// Function to send ADC data over UART
void uart_send_adc_data(void);

#endif /* SRC_UART_COMMUNICATION_FSM_H_ */
