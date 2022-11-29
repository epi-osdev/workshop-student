#ifndef __PORT_H__
    #define __PORT_H__

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

#endif 
