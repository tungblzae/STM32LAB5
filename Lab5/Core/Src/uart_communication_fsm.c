// uart_communication_fsm.c

#include "uart_communication_fsm.h"
#include "command_parser_fsm.h" // Include the command parser module
#include <string.h>
#include <stdio.h> // for sprintf function

// External declarations for UART and ADC (adjust as per your project)
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

static UartCommunicationState uart_communication_state = UART_IDLE;
static uint32_t adc_value = 0; // Variable to store ADC value
static uint32_t timeout_counter = 0;

void uart_communication_fsm_init(void) {
    uart_communication_state = UART_IDLE;
    adc_value = 0;
    timeout_counter = 0;
}

void uart_communication_fsm_update(void) {
    switch (uart_communication_state) {
        case UART_IDLE:
            // Check if a command is complete
            if (is_command_complete()) {
                // Process the command and transition to the appropriate state
                if (strcmp(get_command_data(), "RTS") == 0) {
                    uart_communication_state = UART_SENDING_DATA;
                    // Implement code to read ADC data here
                    adc_value = HAL_ADC_GetValue(&hadc1);
                } else if (strcmp(get_command_data(), "OK") == 0) {
                    uart_communication_state = UART_IDLE;
                    // Implement code for ending communication

                    // Reset command parser for the next command
                    command_parser_fsm_init();
                }
            }
            break;

        case UART_WAITING_FOR_OK:
            timeout_counter++;
            if (timeout_counter >= 3000) {
                // 3-second timeout reached, resend previous packet
                uart_send_adc_data();
                timeout_counter = 0;
            }
            break;

        case UART_SENDING_DATA:
            uart_send_adc_data();
            uart_communication_state = UART_WAITING_FOR_OK; // Transition to the waiting state
            break;

        default:
            // Handle unexpected state
            uart_communication_fsm_init();
            break;
    }
}

void uart_send_adc_data(void) {
    char uart_tx_buffer[20]; // Adjust the buffer size based on your needs
    sprintf(uart_tx_buffer, "!ADC=%lu#", adc_value);
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, strlen(uart_tx_buffer), HAL_MAX_DELAY);
}
