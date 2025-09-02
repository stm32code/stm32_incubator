// �����豸
#include "usart3.h"

// Э���ļ�
#include "net.h"
#include "mqttkit.h"
#include "delay.h"

// Ӳ������
#include "usart.h"
#include "led.h"

//  C��
#include <string.h>
#include <stdio.h>

extern unsigned char esp8266_buf[256];

U8 Connect_Net;


unsigned char MQTT_FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");
	//��Ч����
	memset(text, 0, sizeof(text));
	sprintf(text, "\"flage\":\"%d\",", Data_init.Flage); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"hot\":\"%d\",", (u8)relay3in); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"cold\":\"%d\",", (u8)relay2in); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"led\":\"%d\",",  (u8)relay1in); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"tmep\":\"%.1f\",",  Data_init.temperatuer); 
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"tmep_max\":\"%d\",", threshold_value_init.temp_max_value); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"tmep_min\":\"%d\"", threshold_value_init.temp_min_value); 
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}

//==========================================================
//	�������ƣ�	Send
//==========================================================
void Send(U8 Cmd)
{
	char buf[254];
	short body_len = 0;
	memset(buf, 0, sizeof(buf));
	switch (Cmd)
	{
	case 1:
		body_len = MQTT_FillBuf(buf); // ������
		break;


	default:
		break;
	}

	if (body_len)
	{
		SendString((unsigned char *)buf,body_len);
	}
}

