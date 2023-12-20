#ifndef DHCP_H_
#define DHCP_H_
//--------------------------------------------------
//Include cac thu vien can thiet
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "udp.h"
#include "uart.h"
//-------------------------------------------------
#define Port_src 68
#define Port_dst 67
#define Boardcast {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
#define ip_src {0,0,0,0}
#define ip_dst {0xFF,0xFF,0xFF,0xFF}
//-------------------------------------------------
typedef struct
  {
     uint8_t  op;     //1
     uint8_t  htype;  //1
     uint8_t  hlen;   //6
     uint8_t  hops;   //0
     uint32_t id;
     uint16_t seconds;
     uint16_t flags;
     uint8_t  client_ip[4];
     uint8_t  your_id[4];
     uint8_t  server_id[4];
     uint8_t  gateway_id[4];
     uint8_t  client_mac[6];
     uint8_t  client_padding[10];
     uint8_t  server_name[64];
     uint8_t  boot_file_name[128];
     uint32_t magic_cookie;

     //DHCP OPTION
     uint16_t option53; //0x3501  // Option: (53) DHCP Message Type (Discover)
     uint8_t  option53_dhcp; //0x01 ;//discover

     uint16_t option61; //0x3D07  //Option: (61) Client identifier
     uint8_t  option61_hard_type; // Hardware type: Ethernet (0x01)
     uint8_t  option61_mac[6];    //Client MAC address

     uint16_t option12; //0x0C05   Option: (12) Host Name (IOT47)
     uint8_t  option12_hostname[5];

     uint16_t option55;//0x3705       //Option: (55) Parameter Request List
     uint8_t  option55_subnet_mask;   //Parameter Request List Item: (1) Subnet Mask
     uint8_t  option55_router;        //Parameter Request List Item: (3) Router
     uint8_t  option55_domain_name_sv;//Parameter Request List Item: (6) Domain Name Server
     uint8_t  optionEND;//0xFF
}__attribute__ ((packed)) DHCP_discover_segment;
typedef struct
{
   uint8_t opcode;
   uint8_t dump1[15];
   uint8_t offfer_ip[4];
	 uint8_t *option;
}__attribute__ ((packed)) DHCP_return_segment;
//-----------------------------------------------------------------------------
uint8_t DHCP_get_ip(uint32_t timeout);
void dhcp_discover(void);
void dhcp_getGateway(uint8_t *dhcp_mess,uint8_t *gate_mac,uint8_t *gate_ip);
//-------------------------------------------------

#endif /* ARP_H_ */
