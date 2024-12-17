#include <udpsupp.h>
#include <ansi_c.h>
#include "All.h"			 //This code writed on 2014/09/29 By Siyuan Ma


//unsigned char Response[256]; 


int SendNetData(unsigned int channel,char destAddr[21],unsigned char data[2])
{
	 UDPWrite(channel, 1024, destAddr, data, 2);  
	 return(0);
}

