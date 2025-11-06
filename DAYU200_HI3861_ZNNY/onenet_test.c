#include <stdio.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "onenet.h"

//1011738446
#define ONENET_INFO_DEVID "1151356328"

//20221031aa
#define ONENET_INFO_AUTH "yangqun"

//JDww3LcZCh3Ujy3R2etGnWkBNdE=
#define ONENET_INFO_APIKEY "SjQk5LuayemADvWHLQFyKeolAkU="

//345377
#define ONENET_INFO_PROID "617100"

//gwaK2wJT5wgnSbJYz67CVRGvwkI=
#define ONENET_MASTER_APIKEY "6BJA1T2TXpCBw7U8GkN4FCHEif0="

extern int rand(void);


void onenet_cmd_rsp_cb(uint8_t *recv_data, size_t recv_size, uint8_t **resp_data, size_t *resp_size)
{
    printf("recv data is %.*s\n", recv_size, recv_data);

    *resp_data = NULL;
    *resp_size = 0;
}

int onenet_test(void)
{
	
    device_info_init(ONENET_INFO_DEVID, ONENET_INFO_PROID, ONENET_INFO_AUTH, ONENET_INFO_APIKEY, ONENET_MASTER_APIKEY);
    onenet_mqtt_init();

    onenet_set_cmd_rsp_cb(onenet_cmd_rsp_cb);

	while (1)
    {
		int value = 0;
		
        value = rand() % 100;

        if (onenet_mqtt_upload_digit("temperature", value) < 0)
        {
            printf("upload has an error, stop uploading");
            //break;
        }
        else
        {
            printf("buffer : {\"temperature\":%d} \r\n", value);
        }
        sleep(1);
    }
	return 0;
}