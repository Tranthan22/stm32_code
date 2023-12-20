#include "udp.h"

extern const uint8_t macaddr[6];
extern uint8_t ip[4];
extern char debug_string[60];
extern uint8_t eth_buffer[BUFFER_LENGTH];

void UDP_read(uint8_t *UDP_Frame,uint16_t len)
{
  UDP_struct *UDP_Struct_Frame = (UDP_struct *)UDP_Frame;
	
	//kiem tra MAC xem co phai no gui cho minh khong
	if( memcmp(UDP_Struct_Frame->MAC_dich,macaddr,6) )return;
	if(UDP_Struct_Frame->data[0] == 0x02)//dhcp mess
	{
		 dhcp_getGateway(UDP_Struct_Frame->data,UDP_Struct_Frame->MAC_nguon,UDP_Struct_Frame->SourceIP);
		 return;
	}
  //kiem tra dia chi ip xem co phai no gui cho minh khong
  if( memcmp(UDP_Struct_Frame->DestIP,ip,4) )return; // dung memcmp de so sanh, neu khac thi thoat

  //UART_putString((char *)UDP_Struct_Frame->data);
       if(strstr((const char *)UDP_Struct_Frame->data,"LED1=ON")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da bat LED1\r\n",1000);}
  else if(strstr((const char *)UDP_Struct_Frame->data,"LED1=OF")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da tat LED1\r\n",1000);}

  else if(strstr((const char *)UDP_Struct_Frame->data,"LED2=ON")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da bat LED2\r\n",1000);}
  else if(strstr((const char *)UDP_Struct_Frame->data,"LED2=OF")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da tat LED2\r\n",1000);}

  else if(strstr((const char *)UDP_Struct_Frame->data,"LED3=ON")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da bat LED3\r\n",1000);}
  else if(strstr((const char *)UDP_Struct_Frame->data,"LED3=OF")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da tat LED3\r\n",1000);}

  else if(strstr((const char *)UDP_Struct_Frame->data,"LED4=ON")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da bat LED4\r\n",1000);}
  else if(strstr((const char *)UDP_Struct_Frame->data,"LED4=OF")){UDP_send(UDP_Struct_Frame->SourceIP,swap16(UDP_Struct_Frame->UDP_Source_Port),(uint8_t *)"Da tat LED4\r\n",1000);}
}

uint16_t UDP_checksum(UDP_struct *UDP_Struct_Frame)
{
//int dem=0;
   uint32_t checksum;
   uint8_t *ptr;
   uint16_t length;
   UDP_Struct_Frame->UDP_Checksum=0; //reset check sum
   length = swap16(UDP_Struct_Frame->UDP_Length) + 8;
   ptr = (uint8_t *)&UDP_Struct_Frame->SourceIP;

   checksum=17 + length-8;
   while(length>1) //cong het cac byte16 lai
    {
       //sprintf(debug_string,"%02X,%02X,",*ptr,*(ptr+1));
       //UART_putString(debug_string);
       //dem++;if(dem>=16){dem=0;UART_putString("\r\n");}
       checksum += (uint16_t) (((uint32_t)*ptr<<8)|*(ptr+1));
       ptr+=2;
	   length-=2;
    }    //UART_putString("\r\n");
    if(length) checksum+=((uint32_t)*ptr)<<8; //neu con le 1 byte
    while (checksum>>16) checksum=(uint16_t)checksum+(checksum>>16);
    //nghich dao bit
    checksum=~checksum;
    //hoan vi byte thap byte cao
    return swap16(checksum);
}
uint8_t UDP_send(uint8_t *IP_dest,uint16_t port_dest,uint8_t *data,uint32_t timeout)
{
   uint32_t count=0;
   uint16_t length_mess,frame_length;
   UDP_struct UDP_Struct_Frame;
   length_mess = strlen((const char *)data);
   while(1)
   {
     if(ARP_table_checkIP(IP_dest) != -1)break; // da nhan dc MAC
     ARP_send_request(IP_dest);
     HAL_Delay(50);
     count+=50;
     if(count >= timeout)
     {
       UART_putString("Chua gui goi tin udp\r\n");
       return 0; //gui that bai
     }
   }

   //make Ethernet II
   ARP_table_get_MAC(IP_dest,UDP_Struct_Frame.MAC_dich);   //mac cua thang nhan
   memcpy(UDP_Struct_Frame.MAC_nguon,macaddr,6);  //mac cua enc28j60
   UDP_Struct_Frame.Ethernet_type = 0x0008; // Type = 0x800 = IP

   //make IP packet
   UDP_Struct_Frame.Header_length = 0x45;
   UDP_Struct_Frame.Services=0x00;
   UDP_Struct_Frame.TotoLength=swap16(length_mess+8+20);
   UDP_Struct_Frame.Identification=0x2111;
   UDP_Struct_Frame.Flag=0x0000;
   UDP_Struct_Frame.TimeToLive=0x80;
   UDP_Struct_Frame.Protocol=0x11; //UDP
   UDP_Struct_Frame.CheckSum=0x0000;
   memcpy(UDP_Struct_Frame.SourceIP,ip,4); //ip cua enc28j60
   memcpy(UDP_Struct_Frame.DestIP,IP_dest,4); //ip cua thang nhan
   //tinh checksum goi ip
   UDP_Struct_Frame.CheckSum=NET_ipchecksum((uint8_t *)&UDP_Struct_Frame.Header_length);


   //make UDP packet
   UDP_Struct_Frame.UDP_Source_Port = swap16(ENC28J60_UDP_PORT);
   UDP_Struct_Frame.UDP_Dest_Port =  swap16(port_dest);
   UDP_Struct_Frame.UDP_Length = swap16(length_mess + 8);
   UDP_Struct_Frame.UDP_Checksum=0x0000;
   strcpy((char*)UDP_Struct_Frame.data,(const char *)data); //copy data to struct
   //tinh checksum cho udp
   UDP_Struct_Frame.UDP_Checksum = UDP_checksum(&UDP_Struct_Frame);

   frame_length = swap16(UDP_Struct_Frame.UDP_Length) + 34;
   NET_SendFrame((uint8_t *)&UDP_Struct_Frame,frame_length); //gui goi tin udp
   return 1; //da gui
}
void UDP_send_with_mac(uint8_t *mac_dest,uint8_t *IP_dest,uint16_t port_dest,uint8_t *mac_source,uint8_t *IP_source,uint16_t port_source,uint8_t *data,uint16_t length_mess)
{
   uint16_t frame_length;
   UDP_struct *UDP_Struct_Frame = (UDP_struct *)eth_buffer;

   //make Ethernet II
   memcpy(UDP_Struct_Frame->MAC_dich,mac_dest,6);  //mac cua thang nhan
   memcpy(UDP_Struct_Frame->MAC_nguon,mac_source,6);  //mac cua enc28j60
   UDP_Struct_Frame->Ethernet_type = 0x0008; // Type = 0x800 = IP

   //make IP packet
   UDP_Struct_Frame->Header_length = 0x45;
   UDP_Struct_Frame->Services=0x00;
   UDP_Struct_Frame->TotoLength=swap16(length_mess+8+20);
   UDP_Struct_Frame->Identification=0x2111;
   UDP_Struct_Frame->Flag=0x0000;
   UDP_Struct_Frame->TimeToLive=0x80;
   UDP_Struct_Frame->Protocol=0x11; //UDP
   UDP_Struct_Frame->CheckSum=0x0000;
   memcpy(UDP_Struct_Frame->SourceIP,IP_source,4); //ip cua enc28j60
   memcpy(UDP_Struct_Frame->DestIP,IP_dest,4); //ip cua thang nhan
   //tinh checksum goi ip
   UDP_Struct_Frame->CheckSum=NET_ipchecksum((uint8_t *)&UDP_Struct_Frame->Header_length);


   //make UDP packet
   UDP_Struct_Frame->UDP_Source_Port = swap16(port_source);
   UDP_Struct_Frame->UDP_Dest_Port =  swap16(port_dest);
   UDP_Struct_Frame->UDP_Length = swap16(length_mess + 8);
   UDP_Struct_Frame->UDP_Checksum=0x0000;
   memcpy((char*)UDP_Struct_Frame->data,data,length_mess); //copy data to struct
   //tinh checksum cho udp
   UDP_Struct_Frame->UDP_Checksum = UDP_checksum(UDP_Struct_Frame);

   frame_length = swap16(UDP_Struct_Frame->UDP_Length) + 34;
   NET_SendFrame((uint8_t *)UDP_Struct_Frame,frame_length); //gui goi tin udp
}
//--------------------------------------------------
//end file
