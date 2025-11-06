#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#include "hi_wifi_api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"

#include "MQTTClient.h"

#include "iot_gpio.h"

#define LED_TEST_GPIO 9 // for hispark_pegasus


static MQTTClient mq_client;

 unsigned char *onenet_mqtt_buf;
 unsigned char *onenet_mqtt_readbuf;
 int buf_size;

Network n;
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;  

//ćśćŻĺč°ĺ˝ć°
void mqtt_callback(MessageData *msg_data)
{
    size_t res_len = 0;
    uint8_t *response_buf = NULL;
    char topicname[45] = { "$crsp/" };

    LOS_ASSERT(msg_data);

    printf("topic %.*s receive a message\r\n", msg_data->topicName->lenstring.len, msg_data->topicName->lenstring.data);

    printf("message is %.*s\r\n", msg_data->message->payloadlen, msg_data->message->payload);

	if(memcmp(msg_data->message->payload, "led on", strlen("led on")) == 0)
	{
		printf("led on\r\n");
		IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
	}else if(memcmp(msg_data->message->payload, "led off", strlen("led off")) == 0){
		printf("led off\r\n");
		IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
	}

}

int mqtt_connect(void)
{
	int rc = 0;
    
	//ĺĺ§ĺGPIO
    IoTGpioInit(LED_TEST_GPIO);

    //čŽžç˝Žä¸şčžĺ?
    IoTGpioSetDir(LED_TEST_GPIO, IOT_GPIO_DIR_OUT);

    //MQTTç˝çťĺĺ§ĺ?
	NetworkInit(&n);
	//čżćĽĺ°MQTTćĺĄĺ?
	NetworkConnect(&n, "54.87.92.106", 1883);

    //çťMQTTĺéĺ¤ĺ°
    buf_size  = 4096+1024;
    onenet_mqtt_buf = (unsigned char *) malloc(buf_size);
    onenet_mqtt_readbuf = (unsigned char *) malloc(buf_size);
    if (!(onenet_mqtt_buf && onenet_mqtt_readbuf))
    {
        printf("No memory for MQTT client buffer!");
        return -2;
    }

    //MQTTĺŽ˘ćˇçŤ?
	MQTTClientInit(&mq_client, &n, 1000, onenet_mqtt_buf, buf_size, onenet_mqtt_readbuf, buf_size);
	
    //ĺŻĺ¨MQTTççşżç¨?
    MQTTStartTask(&mq_client);


    data.keepAliveInterval = 30;
    data.cleansession = 1;
	data.clientID.cstring = "template";
	data.username.cstring = "d1K23h7550";
	data.password.cstring = "version=2018-10-31&res=products%2Fd1K23h7550%2Fdevices%2Ftemplate&et=1828559681&method=md5&sign=Nrs5hrJZwsa1cKvJLLs4ig%3D%3D
";
	data.cleansession = 1;
	
    mq_client.defaultMessageHandler = mqtt_callback;

	//čżćĽćĺĄĺ?
	rc = MQTTConnect(&mq_client, &data);

	//čŽ˘é ä¸ťé˘led
	MQTTSubscribe(&mq_client, "led", 0, mqtt_callback);

	while(1)
	{
		MQTTMessage message;

		message.qos = QOS1;
		message.retained = 0;
		message.payload = (void *)"openharmony";
		message.payloadlen = strlen("openharmony");

		//ĺéćść? ohospub ďźä¸ťé˘?   messageďźĺĺŽ?
		if (MQTTPublish(&mq_client, "ohospub", &message) < 0)
		{
			printf("MQTTPublish faild !\r\n");
		}
		usleep(1000000);
	}

	return 0;
}


void ___mqtt_test(void)
{
    mqtt_connect();
}

