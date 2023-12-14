/*
 * command_parser_fsm.h
 *
 *  Created on: Dec 14, 2023
 *      Author: Admin
 */

#ifndef SRC_COMMAND_PARSER_FSM_H_
#define SRC_COMMAND_PARSER_FSM_H_

#include <stdint.h>

#define MAX_BUFFER_SIZE 30

// Define possible states for the command parser FSM
typedef enum {
    COMMAND_IDLE,
    COMMAND_RECEIVING,
    COMMAND_COMPLETE
} CommandParserState;

// Function to initialize the command parser FSM
void command_parser_fsm_init(void);

// Function to update the command parser FSM based on the received character
void command_parser_fsm_update(uint8_t received_char);

// Function to check if a command is complete
uint8_t is_command_complete(void);

// Function to get the command data
void get_command_data(uint8_t *command_data);

#endif /* SRC_COMMAND_PARSER_FSM_H_ */
