#include "git.h"

// �����ʱ���趨
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;

// ��ȡȫ�ֱ���
const char *topics[] = {S_TOPIC_NAME};
char str[50];

// Ӳ����ʼ��
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    HZ = GB16_NUM();                                // ����
    delay_init();                                   // ��ʱ������ʼ��
    GENERAL_TIM_Init(TIM_4, 0, 1);
    Usart1_Init(115200); 		// ����1��ʼ��Ϊ115200
	
    System_PB34_setIO();
    LED_Init(); //״̬��
		TIM3_PWM_Init(20);  // PWM ���
		Key_GPIO_Config();  // ����
		DS18B20_Init();              // DS18B20
		DS18B20_ReadId(ucDs18b20Id); // ��ȡ DS18B20 �����к�
		RELAY_GPIO_Config(); // �̵��� ������ led
#if OLED // OLED�ļ�����
    OLED_Init();
    OLED_ColorTurn(0);   // 0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0); // 0������ʾ 1 ��Ļ��ת��ʾ
#endif
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // ��ʼ����ֵ
  
#if OLED // OLED�ļ�����
    OLED_Clear();
#endif
		relay1out = 0;
		relay2out = 0;
		relay3out = 0;
		
}
// �����ʼ��
void Net_Init()
{
		u8 i;
#if OLED // OLED�ļ�����
    OLED_Clear();
    // дOLED����
    sprintf(str, "-�������ڳ�ʼ��");
    OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "-���Ե� ...   ");
    OLED_ShowCH(0, 2, (unsigned char *)str);
#endif
	  Usart3_Init(9600); 		// ����3����������
    Usart_SendString(USART3,"AT+BAUD4\r\n");
    delay_ms(1000);
    for ( i=0; i < 2; i++) {
			 Usart_SendString(USART3,"AT+PIN1234\r\n");
        delay_ms(500);
    }
    for (i=0; i < 2; i++) {
				Usart_SendString(USART3,"AT+NAMEBOX\r\n");
        delay_ms(500);
    }
#if OLED              // OLED�ļ�����
    OLED_Clear();
#endif
}

// ����1
void task1(void)
{
	//1�붨ʱ��
 	Automation_Close();
}
// ����2
void task2(void)
{

    Read_Data(&Data_init);   // ���´���������
    Update_oled_massage();   // ����OLED
    Update_device_massage(); // �����豸
		State = ~State;
	
}
// ����3
void task3(void)
{
    // 3��һ��
    if (Data_init.App == 0) {
	
			  Data_init.App = 1;
    }
}
// �����ʼ��
void SoftWare_Init(void)
{
    // ��ʱ����ʼ��
    timer_init(&task1_id, task1, 1000, 1);  // 1S
    timer_init(&task2_id, task2, 200, 1);   // 200ms
    timer_init(&task3_id, task3, 3000, 1);  // 3S

    timer_start(&task1_id);
    timer_start(&task2_id);
    timer_start(&task3_id);
}
// ������
int main(void)
{

    unsigned char *dataPtr = NULL;
    SoftWare_Init(); // �����ʼ��
    Hardware_Init(); // Ӳ����ʼ��
		Beepont = 1;
		delay_ms(100);
		Beepont = 0;
    Net_Init();      // �����ʼ
    TIM_Cmd(TIM4, ENABLE); // ʹ�ܼ�����

    while (1) {
        // �߳�
        timer_loop(); // ��ʱ��ִ��
        // ���ڽ����ж�
        dataPtr = Hc05_GetIPD(0);
      	if (dataPtr != NULL) {
             if (massage_parse_json((char *)dataPtr) == MY_SUCCESSFUL) {
                // ��Ϣ���
                ESP8266_Clear();
            }
        }

#if KEY_OPEN
				// �������
				if(time25ms == MY_TRUE){
						Check_Key_ON_OFF();
						time25ms = MY_FALSE;
				}
#endif
    }
}

