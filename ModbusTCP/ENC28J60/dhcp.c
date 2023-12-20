#include "dhcp.h"

extern const uint8_t macaddr[6];
extern uint8_t ip[4];
extern char debug_string[60];
extern uint8_t gateway_ip[4];
extern uint8_t gateway_mac[6];

uint8_t boar_cast[6]=Boardcast;
uint8_t ip_nguon[4]=ip_src;
uint8_t ip_dich[ 4]=ip_dst;

uint8_t getip_ok=0;
uint8_t DHCP_get_ip(uint32_t timeout)
{
	 uint32_t t = HAL_GetTick();
	 while(1)
	 {
		 if( (HAL_GetTick() - t) > timeout)
			 return 0; //time out
		 dhcp_discover();
		 HAL_Delay(1000);
		 if(getip_ok)return 1;
	 }
}
void dhcp_discover(void)
{    
  uint16_t dhcp_discover_length,i;

  DHCP_discover_segment discover;

  discover.op = 1;
  discover.htype=1;
  discover.hlen=6;
  discover.hops=0;
  discover.id = swap32((uint32_t)0x20071999);
  discover.seconds = 0;
  discover.flags=0;

  memcpy(discover.client_ip,ip_nguon,4);
  memcpy(discover.your_id,ip_nguon,4);
  memcpy(discover.server_id,ip_nguon,4);
  memcpy(discover.gateway_id,ip_nguon,4);
  memcpy(discover.client_mac,macaddr,6);
  for(i=0;i<10;i++)discover.client_padding[i]=0x00;
  for(i=0;i<64;i++)discover.server_name[i]=0x00;
  for(i=0;i<128;i++)discover.boot_file_name[i]=0x00;
  discover.magic_cookie=swap32((uint32_t)0x63825363);

  discover.option53= 0x0135;
  discover.option53_dhcp=0x01;
  discover.option61=0x073D;
  discover.option61_hard_type=0x01;
  memcpy(discover.option61_mac,macaddr,6);

  discover.option12 = 0x050C;
  discover.option12_hostname[0]= 'I';
  discover.option12_hostname[1]= 'O';
  discover.option12_hostname[2]= 'T';
  discover.option12_hostname[3]= '4';
  discover.option12_hostname[4]= '7';

  discover.option55=0x0337;
  discover.option55_subnet_mask=1;
  discover.option55_router=3;
  discover.option55_domain_name_sv=6;
  discover.optionEND=0xFF; //end

  dhcp_discover_length=sizeof(discover);
  UDP_send_with_mac(boar_cast,ip_dich,Port_dst,(uint8_t *)macaddr,ip_nguon,Port_src,(uint8_t *)&discover,dhcp_discover_length);
}
void dhcp_getGateway(uint8_t *dhcp_mess,uint8_t *gate_mac,uint8_t *gate_ip)
{
	DHCP_return_segment *DHCP_return = (DHCP_return_segment *)dhcp_mess;
	
	if(getip_ok == 0)
	{
		getip_ok=1;
		memcpy(ip,DHCP_return->offfer_ip,4);
		memcpy(gateway_ip,gate_ip,4);
		memcpy(gateway_mac,gate_mac,6);
		
		sprintf(debug_string,"Your IP:%i.%i.%i.%i\r\n",ip[0],ip[1],ip[2],ip[3]);
		UART_putString(debug_string);
		sprintf(debug_string,"GateWay IP:%i.%i.%i.%i\r\n",gateway_ip[0],gateway_ip[1],gateway_ip[2],gateway_ip[3]);
		UART_putString(debug_string);
		sprintf(debug_string,"GateWay MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n",gateway_mac[0],gateway_mac[1],gateway_mac[2],gateway_mac[3],gateway_mac[4],gateway_mac[5]);
		UART_putString(debug_string);
	  dhcp_discover();
	}
}







//----------------END FILE---------------------------------//
