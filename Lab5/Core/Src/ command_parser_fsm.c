/*
 * command_parser_fsm.c
 *
 *  Created on: Dec 14, 2023
 *      Author: Admin
 */

#include "command_parser_fsm.h"
#include <string.h>

static CommandParserState command_state = COMMAND_IDLE;
static uint8_t command_buffer[MAX_BUFFER_SIZE];
static uint8_t command_index = 0;
static uint8_t command_complete_flag = 0;

void command_parser_fsm_init(void) {
    command_state = COMMAND_IDLE;
    command_index = 0;
    command_complete_flag = 0;
}

void command_parser_fsm_update(uint8_t received_char) {
    switch (command_state) {
        case COMMAND_IDLE:
            if (received_char == '!') {
                command_state = COMMAND_RECEIVING;
                command_index = 0;
            }
            break;

        case COMMAND_RECEIVING:
            if (received_char == '#') {
                command_buffer[command_index] = '\0'; // Null-terminate the string
                command_complete_flag = 1;
                command_state = COMMAND_COMPLETE;
            } else {
                command_buffer[command_index++] = received_char;
                if (command_index == MAX_BUFFER_SIZE - 1) {
                    // Buffer overflow, reset state
                    command_state = COMMAND_IDLE;
                }
            }
            break;

        case COMMAND_COMPLETE:
            // Additional processing if needed
            break;

        default:
            // Handle unexpected state
            command_parser_fsm_init();
            break;
    }
}

uint8_t is_command_complete(void) {
    return command_complete_flag;
}

void get_command_data(uint8_t *command_data) {
    // Copy the command data to the provided buffer
    strcpy(command_data, command_buffer);
}
