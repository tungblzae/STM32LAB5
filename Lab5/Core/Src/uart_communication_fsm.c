// uart_communication_fsm.c

#include "uart_communication_fsm.h"
#include "command_parser_fsm.h"
#include "stm32f1xx_hal.h"

// External declarations for UART and ADC (adjust as per your project)
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

static UartCommunicationState uart_communication_state = UART_IDLE;
static uint32_t adc_value = 0; // Variable to store ADC value
static uint32_t timeout_counter = 0;

// Simple implementation of strlen
size_t custom_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Simple implementation of strcpy
void custom_strcpy(char *dest, const char *src) {
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

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
                if (custom_strlen(get_command_data()) == 3 && get_command_data()[0] == 'R' && get_command_data()[1] == 'S' && get_command_data()[2] == 'T') {
                    uart_communication_state = UART_SENDING_DATA;
                    // Implement code to read ADC data here
                    adc_value = HAL_ADC_GetValue(&hadc1);
                } else if (custom_strlen(get_command_data()) == 2 && get_command_data()[0] == 'O' && get_command_data()[1] == 'K') {
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

// ...

void uart_send_adc_data(void) {
    char uart_tx_buffer[20]; // Adjust the buffer size based on your needs
    custom_strcpy(uart_tx_buffer, "!ADC=");

    // Convert adc_value to ASCII and append to uart_tx_buffer
    uint32_t temp_adc = adc_value;
    char temp_str[10];  // Adjust the buffer size as needed

    // Convert adc_value to ASCII
    int i = 0;
    do {
        temp_str[i++] = temp_adc % 10 + '0';
        temp_adc /= 10;
    } while (temp_adc > 0);

    // Reverse the string
    int j = 0;
    while (i > 0) {
        uart_tx_buffer[custom_strlen(uart_tx_buffer)] = temp_str[--i];
    }

    custom_strcpy(uart_tx_buffer + custom_strlen(uart_tx_buffer), "#");
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, custom_strlen(uart_tx_buffer), HAL_MAX_DELAY);
}

