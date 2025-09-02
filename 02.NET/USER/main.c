#include "git.h"

// 软件定时器设定
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;

// 获取全局变量
const char *topics[] = {S_TOPIC_NAME};
char str[50];

// 硬件初始化
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    HZ = GB16_NUM();                                // 字数
    delay_init();                                   // 延时函数初始化
    GENERAL_TIM_Init(TIM_4, 0, 1);
    Usart1_Init(115200); 		// 串口1初始化为115200
	
    System_PB34_setIO();
    LED_Init(); //状态灯
		TIM3_PWM_Init(20);  // PWM 舵机
		Key_GPIO_Config();  // 按键
		DS18B20_Init();              // DS18B20
		DS18B20_ReadId(ucDs18b20Id); // 读取 DS18B20 的序列号
		RELAY_GPIO_Config(); // 继电器 蜂鸣器 led
#if OLED // OLED文件存在
    OLED_Init();
    OLED_ColorTurn(0);   // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示
#endif
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // 初始化阈值
  
#if OLED // OLED文件存在
    OLED_Clear();
#endif
		relay1out = 0;
		relay2out = 0;
		relay3out = 0;
		
}
// 网络初始化
void Net_Init()
{
		u8 i;
#if OLED // OLED文件存在
    OLED_Clear();
    // 写OLED内容
    sprintf(str, "-蓝牙正在初始化");
    OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "-请稍等 ...   ");
    OLED_ShowCH(0, 2, (unsigned char *)str);
#endif
	  Usart3_Init(9600); 		// 串口3，驱动蓝牙
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
#if OLED              // OLED文件存在
    OLED_Clear();
#endif
}

// 任务1
void task1(void)
{
	//1秒定时器
 	Automation_Close();
}
// 任务2
void task2(void)
{

    Read_Data(&Data_init);   // 更新传感器数据
    Update_oled_massage();   // 更新OLED
    Update_device_massage(); // 更新设备
		State = ~State;
	
}
// 任务3
void task3(void)
{
    // 3发一次
    if (Data_init.App == 0) {
	
			  Data_init.App = 1;
    }
}
// 软件初始化
void SoftWare_Init(void)
{
    // 定时器初始化
    timer_init(&task1_id, task1, 1000, 1);  // 1S
    timer_init(&task2_id, task2, 200, 1);   // 200ms
    timer_init(&task3_id, task3, 3000, 1);  // 3S

    timer_start(&task1_id);
    timer_start(&task2_id);
    timer_start(&task3_id);
}
// 主函数
int main(void)
{

    unsigned char *dataPtr = NULL;
    SoftWare_Init(); // 软件初始化
    Hardware_Init(); // 硬件初始化
		Beepont = 1;
		delay_ms(100);
		Beepont = 0;
    Net_Init();      // 网络初始
    TIM_Cmd(TIM4, ENABLE); // 使能计数器

    while (1) {
        // 线程
        timer_loop(); // 定时器执行
        // 串口接收判断
        dataPtr = Hc05_GetIPD(0);
      	if (dataPtr != NULL) {
             if (massage_parse_json((char *)dataPtr) == MY_SUCCESSFUL) {
                // 消息清空
                ESP8266_Clear();
            }
        }

#if KEY_OPEN
				// 按键监测
				if(time25ms == MY_TRUE){
						Check_Key_ON_OFF();
						time25ms = MY_FALSE;
				}
#endif
    }
}

