#include "port.h"

/**
 * @brief 
 * 
 * Cathcing the reponse of the request (port_byte_out)
 * 
 * @param port : the port to read from
 * @return unsigned char : the byte read from the port
 */
unsigned char port_byte_in(unsigned short port)
{
    unsigned char result = 0;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return (result);
}

/**
 * @brief 
 * 
 * Requesting the port with the data
 * 
 * @param port : port number
 * @param data : data to write
 */
void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
