#include "tcp.h"
extern const uint8_t macaddr[6];
extern uint8_t ip[4];
extern char debug_string[60];
extern uint8_t eth_buffer[BUFFER_LENGTH];

TCP_client tcp_client1;

uint16_t TCP_checksum(TCP_struct *TCP_Struct_Frame)
{
   uint32_t checksum;
   uint8_t *ptr;
   uint16_t length;
   length = swap16(TCP_Struct_Frame->TotoLength) - 20 + 8 ; //tinh length bat dau tu checksum
   ptr = (uint8_t *)&TCP_Struct_Frame->SourceIP;       //dia chi bat dau tinh checksum

   checksum=6 + length - 8;
   while(length>1) //cong het cac byte16 lai
    {
       checksum += (uint16_t) (((uint32_t)*ptr<<8)|*(ptr+1));
       ptr+=2;
       length-=2;
    };
    if(length) checksum+=((uint32_t)*ptr)<<8; //neu con le 1 byte
    while (checksum>>16) checksum=(uint16_t)checksum+(checksum>>16);
    //nghich dao bit
    checksum=~checksum;
    //hoan vi byte thap byte cao
    return swap16(checksum);
}   
void TCP_make_herder(TCP_struct *TCP_Struct_Frame,uint16_t len,uint8_t type)
{
  uint32_t dat_ack;
  uint16_t port,datalength;
  if(type == FOR_SYN)
  {
     //reply voi SYN|ACK
     //make reply
     memcpy(TCP_Struct_Frame->MAC_dich,TCP_Struct_Frame->MAC_nguon,6);
     memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
     TCP_Struct_Frame->CheckSum=0;
     memcpy(TCP_Struct_Frame->DestIP,TCP_Struct_Frame->SourceIP,4); //hoan vi source, dest
     memcpy(TCP_Struct_Frame->SourceIP,ip,4);                       //ip cua minh
     port = TCP_Struct_Frame->Source_Port;
     TCP_Struct_Frame->Source_Port = TCP_Struct_Frame->Dest_Port;
     TCP_Struct_Frame->Dest_Port = port;
     TCP_Struct_Frame->Acknowledgement = swap32(swap32(TCP_Struct_Frame->Sequence_Number) + 1);
     TCP_Struct_Frame->Sequence_Number = swap32((uint32_t)2071998);
     TCP_Struct_Frame->TCP_Flags = TCP_SYN | TCP_ACK;
     TCP_Struct_Frame->TCP_Checksums=0;
     TCP_Struct_Frame->Urgent_Pointer=0;
     TCP_Struct_Frame->CheckSum = NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IO
     TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);
  }
  else if(type == FOR_FIN || type == FOR_ACK)
  {
     //reply voi ACK
     //make reply
     memcpy(TCP_Struct_Frame->MAC_dich,TCP_Struct_Frame->MAC_nguon,6);
     memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
     TCP_Struct_Frame->CheckSum=0;
     memcpy(TCP_Struct_Frame->DestIP,TCP_Struct_Frame->SourceIP,4); //hoan vi source, dest
     memcpy(TCP_Struct_Frame->SourceIP,ip,4);                       //ip cua minh
     port = TCP_Struct_Frame->Source_Port;
     TCP_Struct_Frame->Source_Port = TCP_Struct_Frame->Dest_Port;
     TCP_Struct_Frame->Dest_Port = port;
     dat_ack =  TCP_Struct_Frame->Acknowledgement;
     TCP_Struct_Frame->Acknowledgement = swap32(swap32(TCP_Struct_Frame->Sequence_Number) + 1);
     TCP_Struct_Frame->Sequence_Number =   dat_ack;
     TCP_Struct_Frame->TCP_Flags = TCP_ACK;
     TCP_Struct_Frame->TCP_Checksums=0;
     TCP_Struct_Frame->Urgent_Pointer=0;
     TCP_Struct_Frame->CheckSum = NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IO
     TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);
  }
  else if(type == FOR_PSH_ACK)
  {
     //tinh do dai cua goi data
     datalength= swap16(TCP_Struct_Frame->TotoLength) -20 - (TCP_Struct_Frame->data_offset >> 2);  // ( >> 4)*4 = >> 2
     //make reply
     len-=datalength; //resize len

     memcpy(TCP_Struct_Frame->MAC_dich,TCP_Struct_Frame->MAC_nguon,6);
     memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
     TCP_Struct_Frame->CheckSum=0;
     TCP_Struct_Frame->TotoLength = swap16(40); //defaul totolength
     TCP_Struct_Frame->data_offset = 0x50;      //defaul data_offset
     memcpy(TCP_Struct_Frame->DestIP,TCP_Struct_Frame->SourceIP,4); //hoan vi source, dest
     memcpy(TCP_Struct_Frame->SourceIP,ip,4);                       //ip cua minh
     port = TCP_Struct_Frame->Source_Port;
     TCP_Struct_Frame->Source_Port = TCP_Struct_Frame->Dest_Port;
     TCP_Struct_Frame->Dest_Port = port;
     dat_ack =  TCP_Struct_Frame->Acknowledgement;
     TCP_Struct_Frame->Acknowledgement = swap32(swap32(TCP_Struct_Frame->Sequence_Number) + datalength);
     TCP_Struct_Frame->Sequence_Number =   dat_ack;
     TCP_Struct_Frame->TCP_Flags = TCP_ACK;
     TCP_Struct_Frame->TCP_Checksums=0;
     TCP_Struct_Frame->Urgent_Pointer=0;
     TCP_Struct_Frame->CheckSum = NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IO
     TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);
  }
  else if(type == FOR_CONNECT)
  {
    memcpy(TCP_Struct_Frame->MAC_dich,tcp_client1.mac_defaul,6);
    memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
    TCP_Struct_Frame->Ethernet_type = 0x0008;

    TCP_Struct_Frame->Header_length = 0x45;         //make IP
    TCP_Struct_Frame->Services=0x0000;
    TCP_Struct_Frame->TotoLength = swap16(52); //do dai cua goi tin IP mac dinh la 0x0034 (52 byte) (66-14)
    TCP_Struct_Frame->Identification=0xBF2B;
    TCP_Struct_Frame->Flag=0x0040;
    TCP_Struct_Frame->TimeToLive=0x80;
    TCP_Struct_Frame->Protocol=0x06; //tcp
    TCP_Struct_Frame->CheckSum=0x0000;
    memcpy(TCP_Struct_Frame->SourceIP,ip,4);

		tcp_client1.client_port = 3456;
    TCP_Struct_Frame->Source_Port = swap16(tcp_client1.client_port);  
		TCP_Struct_Frame->Sequence_Number=swap32(tcp_client1.Sequence_Number);
    TCP_Struct_Frame->Acknowledgement=0;
    TCP_Struct_Frame->data_offset=0x80;
    TCP_Struct_Frame->TCP_Flags = TCP_SYN;
    TCP_Struct_Frame->Window = swap16(8192); // thong bao cho server biet bo dem nhan toi da
    TCP_Struct_Frame->TCP_Checksums=0x0000;
    TCP_Struct_Frame->Urgent_Pointer=0x0000;
    //option
    TCP_Struct_Frame->data[0]=0x02;
    TCP_Struct_Frame->data[1]=0x04;
    TCP_Struct_Frame->data[2]=0x05;
    TCP_Struct_Frame->data[3]=0xb4;
    TCP_Struct_Frame->data[4]=0x01;
    TCP_Struct_Frame->data[5]=0x03;
    TCP_Struct_Frame->data[6]=0x03;
    TCP_Struct_Frame->data[7]=0x08;
    TCP_Struct_Frame->data[8]=0x01;
    TCP_Struct_Frame->data[9]=0x01;
    TCP_Struct_Frame->data[10]=0x04;
    TCP_Struct_Frame->data[11]=0x02;
  }
	else if(type == FOR_DISCONNECT)
  {
		memcpy(TCP_Struct_Frame->MAC_dich,tcp_client1.mac_defaul,6);
    memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
    TCP_Struct_Frame->Ethernet_type = 0x0008;
		
		TCP_Struct_Frame->Header_length = 0x45;         //make IP
    TCP_Struct_Frame->Services=0x0000;
    TCP_Struct_Frame->TotoLength = swap16(40); //do dai cua goi tin IP mac dinh la 0x0028 (40 byte) ( 54 -14)
    TCP_Struct_Frame->Identification=0x4470;
    TCP_Struct_Frame->Flag=0x0040;
    TCP_Struct_Frame->TimeToLive=0x80;
    TCP_Struct_Frame->Protocol=0x06; //tcp
    TCP_Struct_Frame->CheckSum=0x0000;
    memcpy(TCP_Struct_Frame->SourceIP,ip,4);
		
		TCP_Struct_Frame->Source_Port = swap16(tcp_client1.client_port);  
		TCP_Struct_Frame->Dest_Port = swap16(tcp_client1.port);
		TCP_Struct_Frame->Sequence_Number=swap32(tcp_client1.Sequence_Number);
    TCP_Struct_Frame->Acknowledgement=swap32(tcp_client1.Acknowledgement);
    TCP_Struct_Frame->data_offset=0x50;
    TCP_Struct_Frame->TCP_Flags = TCP_FIN|TCP_ACK;
    TCP_Struct_Frame->Window = 0x4400; // thong bao cho server biet bo dem nhan toi da
    TCP_Struct_Frame->TCP_Checksums=0x0000;
    TCP_Struct_Frame->Urgent_Pointer=0x0000;
	}
	else if(type == FOR_SEND)
  {
		memcpy(TCP_Struct_Frame->MAC_dich,tcp_client1.mac_defaul,6);
    memcpy(TCP_Struct_Frame->MAC_nguon,macaddr,6);
    TCP_Struct_Frame->Ethernet_type = 0x0008;
		
		memcpy(TCP_Struct_Frame->SourceIP,ip,4);
		memcpy(TCP_Struct_Frame->DestIP,tcp_client1.ip_defaul,4);
		
		TCP_Struct_Frame->Header_length = 0x45;         //make IP
    TCP_Struct_Frame->Services=0x0000;
		TCP_Struct_Frame->Identification=0x4470;
    TCP_Struct_Frame->Flag=0x0040;
    TCP_Struct_Frame->TimeToLive=0x80;
    TCP_Struct_Frame->Protocol=0x06; //tcp
    TCP_Struct_Frame->CheckSum=0x0000;
		TCP_Struct_Frame->TotoLength = swap16(40);
    memcpy(TCP_Struct_Frame->SourceIP,ip,4);
		
		TCP_Struct_Frame->Source_Port = swap16(tcp_client1.client_port);
    TCP_Struct_Frame->Dest_Port = swap16(tcp_client1.port);		
		TCP_Struct_Frame->Sequence_Number=swap32(tcp_client1.Sequence_Number);
    TCP_Struct_Frame->Acknowledgement=swap32(tcp_client1.Acknowledgement);
		TCP_Struct_Frame->data_offset=0x50;
    TCP_Struct_Frame->Window = 0xCA01; // thong bao cho server biet bo dem nhan toi da
    TCP_Struct_Frame->TCP_Checksums=0x0000;
    TCP_Struct_Frame->Urgent_Pointer=0x0000;
	}
}		
void TCP_read(uint8_t *TCP_Frame,uint16_t len)
{
  uint8_t *data_send=0;
  uint16_t data_send_len=0;
  TCP_struct *TCP_Struct_Frame = (TCP_struct *)TCP_Frame;
  //kiem tra dia chi ip xem co phai no gui cho minh khong
  if( memcmp(TCP_Struct_Frame->DestIP,ip,4) )return; // dung memcmp de so sanh, neu khac thi thoat
	if(TCP_Struct_Frame->TCP_Flags == (TCP_SYN|TCP_ACK))
  {
		 if(tcp_client1.tcp_status == 1 || tcp_client1.tcp_status == 2)
		 {
			  if(swap32(TCP_Struct_Frame->Acknowledgement) == tcp_client1.Sequence_Number+1)
				{
					tcp_client1.Sequence_Number+=1;
					tcp_client1.Acknowledgement = swap32(TCP_Struct_Frame->Sequence_Number) + 1;
					tcp_client1.tcp_status=2; //ket noi thanh cong
					TCP_make_herder(TCP_Struct_Frame,len,FOR_ACK);
		      NET_SendFrame((uint8_t *)TCP_Struct_Frame,len); //gui goi tin tcp ack reply
					UART_putString("Connect OK\r\n");
				}
		 }
	}
  if(TCP_Struct_Frame->TCP_Flags == TCP_SYN)
  {
     TCP_make_herder(TCP_Struct_Frame,len,FOR_SYN);
     NET_SendFrame((uint8_t *)TCP_Struct_Frame,len); //gui goi tin tcp reply
  }
  else if(TCP_Struct_Frame->TCP_Flags == TCP_ACK)
  {
		 if(tcp_client1.tcp_status == 3)
		 {
			 tcp_client1.Sequence_Number = swap32(TCP_Struct_Frame->Acknowledgement);
			 tcp_client1.Acknowledgement = swap32(TCP_Struct_Frame->Sequence_Number);
			 tcp_client1.tcp_status = 2;
		 }
     if(HTTP_get_status()==1 )    //neu dang co nhiem vu gui cac goi tin http
     {
        if(TCP_Struct_Frame->Sequence_Number==HTTP_get_Sequence_Number())  //neu goi tin truoc da gui thanh cong
        {
            TCP_make_herder(TCP_Struct_Frame,len,FOR_PSH_ACK);
            if(HTTP_get_data_num(&data_send,&data_send_len) == 0)       // neu van con goi tin de gui
            {
                TCP_send(TCP_Struct_Frame,len,data_send,data_send_len);
                HTTP_set_Sequence_Number(TCP_Struct_Frame->Acknowledgement); //luu so hieu goi tin da gui lai
            }
            else   //da gui het cac goi tin, dong ket noi
            {
                HTTP_set_status(0);
                //dong ket noi
                TCP_Struct_Frame->TCP_Flags = TCP_FIN|TCP_ACK;
                TCP_Struct_Frame->TCP_Checksums=0;
                TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);

                NET_SendFrame((uint8_t *)TCP_Struct_Frame,len); //gui goi tin ngat ket noi
            }
        }
     }
  }
  else if(TCP_Struct_Frame->TCP_Flags == (TCP_FIN|TCP_ACK))
  {
		 if(swap32(TCP_Struct_Frame->Acknowledgement)  == tcp_client1.Sequence_Number)
		 {
        tcp_client1.tcp_status=0;	
        UART_putString("Server da ngat ket noi !\r\n");			 
     }
		 else if(swap32(TCP_Struct_Frame->Acknowledgement)  == (tcp_client1.Sequence_Number+1))
		 {
        tcp_client1.tcp_status=0;	
        UART_putString("Da ngat ket noi Server!\r\n");			 
     }
		 TCP_make_herder(TCP_Struct_Frame,len,FOR_FIN);
     NET_SendFrame((uint8_t *)TCP_Struct_Frame,len); //gui goi tin tcp reply

     TCP_Struct_Frame->TCP_Flags = TCP_FIN|TCP_ACK;
     TCP_Struct_Frame->TCP_Checksums=0;
     TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);

     NET_SendFrame((uint8_t *)TCP_Struct_Frame,len); //gui goi tin tcp reply
  }
  else if(TCP_Struct_Frame->TCP_Flags == (TCP_PSH|TCP_ACK))
  {
		if(tcp_client1.tcp_status == 2) //neu đã kết nối với server
		{
			  TCP_readData(TCP_Struct_Frame,len);
		}
          if(strstr((const char *)TCP_Struct_Frame->data,"bat1")){;} //your code
     else if(strstr((const char *)TCP_Struct_Frame->data,"tat1")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"bat2")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"tat2")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"bat3")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"tat3")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"bat4")){;}
     else if(strstr((const char *)TCP_Struct_Frame->data,"tat4")){;}
     if (strncmp((char*)TCP_Struct_Frame->data,"GET /", 5) == 0)
     {
        if(HTTP_get_status() != 1)
        {
            HTTP_pack_init(); //bat dau gui 1 goi tin
            TCP_make_herder(TCP_Struct_Frame,len,FOR_PSH_ACK);
            if(HTTP_get_data_num(&data_send,&data_send_len) == 0)
            {
                TCP_send(TCP_Struct_Frame,len,data_send,data_send_len);
                HTTP_set_Sequence_Number(TCP_Struct_Frame->Acknowledgement); //luu so hieu goi tin da gui lai
            }
        }
     }
  }
}

void TCP_send(TCP_struct *TCP_Struct_Frame,uint16_t len,uint8_t *data,uint16_t data_length)
{
    uint16_t i;
    for(i=0;i<data_length;i++)TCP_Struct_Frame->data[i] = data[i];
    len+=data_length;
    TCP_Struct_Frame->TotoLength = swap16(swap16(TCP_Struct_Frame->TotoLength) + data_length); //make totolength
    TCP_Struct_Frame->CheckSum=0;
    TCP_Struct_Frame->TCP_Checksums=0;
    TCP_Struct_Frame->TCP_Flags = TCP_PSH|TCP_ACK;

    TCP_Struct_Frame->CheckSum = NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IO
    TCP_Struct_Frame->TCP_Checksums = TCP_checksum(TCP_Struct_Frame);

    NET_SendFrame((uint8_t *)TCP_Struct_Frame,len);
}
uint8_t TCP_sendSYN(uint8_t *IP_server,uint16_t port,uint32_t timeout)
{
    TCP_struct *TCP_Struct_Frame = (TCP_struct *)eth_buffer;
	  uint32_t count=0;

	  if(IP_server[0] == 192 && IP_server[1] == 168) //local ip
		{
			 while(1)
			 {
				 if(ARP_table_checkIP(IP_server) != -1)
	       {			
 				  ARP_table_get_MAC(IP_server,tcp_client1.mac_defaul);
				  break; // da nhan dc MAC
				 }
				 ARP_send_request(IP_server);
				 HAL_Delay(50);
				 count+=50;
				 if(count >= timeout)
				 {
					 return 0; //gui that bai
				 }
			 }
		}
		tcp_client1.tcp_status = 1;
		tcp_client1.port = port;
		memcpy(tcp_client1.ip_defaul,IP_server,4); //save
		
	  net_SetStatus(0);
    //chinh sua thong tin nguoi nhan
    TCP_make_herder(TCP_Struct_Frame,66,FOR_CONNECT);   //ban tin connect co do dai mac dinh la 66
    memcpy(TCP_Struct_Frame->DestIP,IP_server,4);
    TCP_Struct_Frame->Dest_Port = swap16(port);
		TCP_Struct_Frame->CheckSum =  NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IP
    TCP_Struct_Frame->TCP_Checksums =  TCP_checksum(TCP_Struct_Frame);

    sprintf(debug_string,"Connect to %i.%i.%i.%i:%i ...\r\n",IP_server[0],IP_server[1],IP_server[2],IP_server[3],port);
    UART_putString(debug_string);
    NET_SendFrame((uint8_t *)TCP_Struct_Frame,66);
	  net_SetStatus(1);
		return 1;
}
uint8_t TCP_Connect(uint8_t *IP_server,uint16_t port,uint32_t timeout,int8_t Try_reconnect)
{
	uint32_t t;
	if( tcp_client1.tcp_status==2)
	{
		UART_putString("Da ket noi roi !\r\n");
		return 2;
	}
	tcp_client1.Sequence_Number = rand();      //save Sequence_Number	
	do
	{
	  if(!TCP_sendSYN(IP_server,port,5000)) // gửi bản tin SYN
		{
			UART_putString("Ket noi that bai 1 \r\n");
		  return 0; //gui that bai
		}
		else
		{
			t = HAL_GetTick();
			while(1)
			{
				 if(tcp_client1.tcp_status == 2) 
				 {
					  UART_putString("Ket noi thanh cong\r\n");
					  return 1; 
         }					 
				 if(HAL_GetTick() - t > timeout)
					 break;
			}
		}
	}while(Try_reconnect--);
	UART_putString("Ket noi that bai 2\r\n");
	return 0; //gui that bai
}
uint8_t TCP_Disconect(uint32_t timeout)
{
	uint32_t t;
	if( tcp_client1.tcp_status==2)
	{
		TCP_struct *TCP_Struct_Frame = (TCP_struct *)eth_buffer;
    net_SetStatus(0);
    //chinh sua thong tin nguoi nhan
    memcpy(TCP_Struct_Frame->DestIP,tcp_client1.ip_defaul,4);
    TCP_make_herder(TCP_Struct_Frame,54,FOR_DISCONNECT);   //ban tin disconnect co do dai mac dinh la 54
    TCP_Struct_Frame->CheckSum =  NET_ipchecksum((uint8_t *)&TCP_Struct_Frame->Header_length);  //tinh checksum cho goi IP
    TCP_Struct_Frame->TCP_Checksums =  TCP_checksum(TCP_Struct_Frame);

    NET_SendFrame((uint8_t *)TCP_Struct_Frame,54);
	  net_SetStatus(1);
		t = HAL_GetTick();
		while(1)
		{
			 if( tcp_client1.tcp_status == 0) //ngat ket noi thanh cong
			 {
				  return 1;
			 }
			 if(HAL_GetTick() - t > timeout)
			 {
				  UART_putString("Ngat ket noi that bai\r\n");
				  return 0;
			 }
		}
	}
	return 2; //chua co ket noi de ngat
}
uint8_t TCP_sendData(uint8_t *data,uint16_t data_length,uint32_t timeout)
{
	uint32_t t;
	TCP_struct *TCP_Struct_Frame = (TCP_struct *)eth_buffer;
	
	if(tcp_client1.tcp_status != 2 )
	{
    UART_putString("Khong co ket noi\r\n");
		return 2;
	}
	
	net_SetStatus(0);
	TCP_make_herder(TCP_Struct_Frame,0,FOR_SEND);
	TCP_send(TCP_Struct_Frame,54,data,data_length);
	
	t = HAL_GetTick();
	tcp_client1.tcp_status = 3; //check ack
	net_SetStatus(1);
	while(1)
	{
		if(tcp_client1.tcp_status == 2)
			{
				UART_putString("Gui thanh cong\r\n");
		    return 1;
			}
		if(HAL_GetTick() - t > timeout)
	  {
			tcp_client1.tcp_status = 2;
			UART_putString("Gui du lieu that bai\r\n");
			return 0;
		}
	}
}
void TCP_readData(TCP_struct *TCP_Struct_Frame,uint16_t len)
{
	uint16_t data_len,data_offset,i;
	//kiểm tra số hiệu gói tin
	if(swap32(TCP_Struct_Frame->Sequence_Number) == tcp_client1.Acknowledgement) //nếu chính xác thì nhận, không thì bỏ quả vì có thể nó là gói tin thất lạc thôi
	{	
		data_len= swap16(TCP_Struct_Frame->TotoLength) -20 - (TCP_Struct_Frame->data_offset >> 2);  //tính độ dài chuỗi tin nhắn gửi tới
	  data_offset = (TCP_Struct_Frame->data_offset >> 2) - 20; //tính điểm bắt đầu của data
		
		//in ra màn hình
	  for(i=0;i<data_len;i++)
	    UART_putChar(TCP_Struct_Frame->data[i+data_offset]);
			
		TCP_make_herder(TCP_Struct_Frame,len,FOR_PSH_ACK);
		
		//lúc này client sẽ chủ động + độ dài data mà nó nhận đc vào Sequence_Number	
		tcp_client1.Sequence_Number = swap32(TCP_Struct_Frame->Sequence_Number);
		tcp_client1.Acknowledgement = swap32(TCP_Struct_Frame->Acknowledgement);
		TCP_send(TCP_Struct_Frame,54,(uint8_t *)"",0);//trả lời lại ack
		
	}
}
//--------------------------------------------------
//end file
