#ifndef _KERNEL_SERIAL_PORTS_H
#define _KERNEL_SERIAL_PORTS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool enable_serial_port(void);
uint16_t lowest_available_port(void);
void initialize_port(uint16_t port);
void set_baud_rate(uint16_t port, uint32_t baud_rate);
void send_data(uint16_t port, char * buffer, int length);
void send_char(uint16_t port, char char_to_send);
char read_data(uint16_t port);

#endif