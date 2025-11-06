#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "random.h"

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_i2c.h"
#include "iot_gpio.h"
#include "iot_pwm.h"
#include "iot_errno.h"
#include "hi_io.h"

#include "iot_uart.h" 

#include <hi_pwm.h>
#include <hi_gpio.h>
#include <hi_io.h>
#include <hi_adc.h>
#include "iot_gpio.h"

#include "hi_adc.h"

#include "zhjt.h"
#include "MQTTClient.h"
#include "onenet.h"


#define ONENET_INFO_DEVID "1151356328"
#define ONENET_INFO_AUTH "yangqun"
#define ONENET_INFO_APIKEY "SjQk5LuayemADvWHLQFyKeolAkU="
#define ONENET_INFO_PROID "617100"
#define ONENET_MASTER_APIKEY "6BJA1T2TXpCBw7U8GkN4FCHEif0="


//����GPIO
#define GZ_GPIO         9
#define GZ_ADC          HI_ADC_EQU_MODEL_4

//�̵���GPIO
#define JDQ_GPIO_1           10
#define JDQ_GPIO_2           11

#define AHT20_BAUDRATE 400*1000
#define AHT20_I2C_IDX 0

#define OLED_I2C_BAUDRATE 400*1000

extern int rand(void);

static float ConvertToVoltage(unsigned short data)
{
    return (float)data * 1.8 * 4 / 4096;
}

void onenet_cmd_rsp_cb(uint8_t *recv_data, size_t recv_size, uint8_t **resp_data, size_t *resp_size)
{
    printf("recv data is %.*s\n", recv_size, recv_data);

    *resp_data = NULL;
    *resp_size = 0;
}


unsigned int gz_val;
float temp, humi;

void aht20_init(void)
{
    
    ssize_t retval = 0;
    
    hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
    hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);

    hi_i2c_init(0, 400*1000);

    retval = AHT20_Calibrate();
    printf("AHT20_Calibrate: %d\r\n", retval);

}



void jd_switch(int index, int status)
{
    IoTGpioSetOutputVal(JDQ_GPIO_1, status);
}

void zkmb_test(void)
{
    srand((unsigned)time(NULL));
    double temp = (double)Myrandom(0,100);
    usleep(10);
    double humi = (double)Myrandom(0,100);
    usleep(10);
    int gz_val = (int)Myrandom(500,1000);

    unsigned int len = 0; 
	int retval;
    int ret;
	
    // //IO��ʼ��
    // IoTGpioInit(HI_IO_NAME_GPIO_13);
    // IoTGpioInit(HI_IO_NAME_GPIO_14);

    // hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
    // hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);

    // //I2C��ʼ��    
    // IoTI2cInit(0, OLED_I2C_BAUDRATE);

    // //��������
    // IoTGpioInit(GZ_GPIO);
    // //��ͨIO�ڹ��ܼ���
    // hi_io_set_func(GZ_GPIO, 0);
    // //����ģʽ
    // IoTGpioSetDir(GZ_GPIO, IOT_GPIO_DIR_IN);


    // //�̵���
    IoTGpioInit(JDQ_GPIO_1);
    hi_io_set_func(JDQ_GPIO_1, 0);
    IoTGpioSetDir(JDQ_GPIO_1, IOT_GPIO_DIR_OUT);

    // IoTGpioInit(JDQ_GPIO_2);
    // hi_io_set_func(JDQ_GPIO_2, 0);
    // IoTGpioSetDir(JDQ_GPIO_2, IOT_GPIO_DIR_OUT);


    //OLED��ʼ��
    // ssd1306_Init();

    //��ʪ�ȴ�������ʼ��
	// aht20_init();

    //����TCP�߳�
    start_tcp_thread();

    //OneNET��ʼ��
    device_info_init(ONENET_INFO_DEVID, ONENET_INFO_PROID, ONENET_INFO_AUTH, ONENET_INFO_APIKEY, ONENET_MASTER_APIKEY);
    onenet_mqtt_init();

    onenet_set_cmd_rsp_cb(onenet_cmd_rsp_cb);

	while(1) 
	{
        if(temp >= 5){
        temp += (double)Myrandom(0,4) -2; // -2 ~ 2
        }
        else{
            temp+=4;
        }
		char str[50];
		memset(str, 0, 50);
		snprintf(str, sizeof(str), "temp:%.2f", temp);
		send_tcp_gb(str, strlen(str));

        usleep(10);
        if(humi >=5){
        humi += (double)Myrandom(0,10) -5;}
        else{
            humi+=10;
        }
		memset(str, 0, 50);
		snprintf(str, sizeof(str), "humi:%.2f", humi);
		send_tcp_gb(str, strlen(str));

        usleep(10);
        if(gz_val >= 50){
        gz_val += (int)Myrandom(0,50) -25;}
        else{
            gz_val+=50;
        }
        memset(str, 0, 50);
		snprintf(str, sizeof(str), "illumination:%ld", gz_val);
        send_tcp_gb(str, strlen(str));

        usleep(10);
        
        if (onenet_mqtt_upload_digit("temperature", temp) < 0)
        {
            printf("upload has an error, stop uploading!\n");
            //break;
        }
        else
        {
            printf("buffer : {\"temperature\":%f} \r\n", temp);
        }

        if (onenet_mqtt_upload_digit("humidity", humi) < 0)
        {
            printf("upload has an error, stop uploading!\n");
            //break;
        }
        else
        {
            printf("buffer : {\"humidity\":%f} \r\n", humi);
        }

        if (onenet_mqtt_upload_digit("illumination", gz_val) < 0)
        {
            printf("upload has an error, stop uploading!\n");
            //break;
        }
        else
        {
            printf("buffer : {\"illumination\":%ld} \r\n", gz_val);
        }
        
		sleep(1);
    }
	
}

