
#include "hi_wifi_api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include <stdio.h>

#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "cJSON.h"

#include "zhjt.h"

#define SERVER_PORT 3862


unsigned char recvline[1024];

static int sockfd;

struct sockaddr_in servaddr;

struct sockaddr_in servaddr_ytj;

void printf_hex(char *buf, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		printf("%2x ", buf[i]);
	}
	printf("\r\n");
}

void udp_socket_init(void)
{
    int ret;
    cJSON *recvjson;


    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
 
    //鏈嶅姟鍣? ip port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    printf("udp_thread \r\n");
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    //开启广播功能
    int opt= 1;
    ret = setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)); 

    //设置广播地址
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr("255.255.255.255");

}

extern unsigned int gz_val;

void udp_thread(void *pdata)
{
    int ret;
    cJSON *recvjson;

    pdata = pdata;


    while(1)
    {
        struct sockaddr_in addrClient;
        int sizeClientAddr = sizeof(struct sockaddr_in);

        memset(recvline, sizeof(recvline), 0);
        ret = recvfrom(sockfd, recvline, 1024, 0, (struct sockaddr*)&addrClient,(socklen_t*)&sizeClientAddr);
        
        if(ret>0)
        {
            char *pClientIP =inet_ntoa(addrClient.sin_addr);
 
            //把对方的IP地址打印出来
            printf("%s-%d(%d) says:\n",pClientIP,ntohs(addrClient.sin_port),addrClient.sin_port);

            printf_hex(recvline, ret);

            if(recvline[0] == 0x85)
            {
                printf("recvline[0] == 0x85 \r\n");
                //光照
                if(recvline[1] == EVENT_GZ)
                {
                    gz_val = (recvline[2]<<8) | recvline[3];
                    printf("EVENT_GZ %ld\r\n", gz_val);
                }
            }
		}
    }
}

void send_udp_gb(char *buf, int len)
{
    ssize_t retval = 0;
    retval = sendto(sockfd, buf, len, 0, (struct sockaddr *)&servaddr,sizeof(servaddr));
    if (retval < 0) {
        printf("sendto failed!\r\n");
    }
    printf("send UDP message {%s} %ld done!\r\n", buf, len);
}


void start_udp_thread(void)
{
    osThreadAttr_t attr;

    attr.name = "wifi_config_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 2048;
    attr.priority = 36;

    if (osThreadNew((osThreadFunc_t)udp_thread, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
}

