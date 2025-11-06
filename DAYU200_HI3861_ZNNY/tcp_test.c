#include <errno.h> // 包含错误号定义
#include "hi_wifi_api.h" // 包含WiFi API
#include "lwip/ip_addr.h" // 包含IP地址处理
#include "lwip/netifapi.h" // 包含网络接口API
#include "lwip/sockets.h" // 包含socket操作
#include <stdio.h> // 包含标准输入输出

#include <unistd.h> // 包含POSIX操作系统API
#include "ohos_init.h" // 包含操作系统初始化
#include "cmsis_os2.h" // 包含CMSIS-RTOS API


static char request[128] = ""; // 定义请求缓冲区

unsigned short port = 3861; // 定义服务器监听端口

int sockfd ; // TCP socket文件描述符
int connfd = -1; // 连接文件描述符

// TCP服务器测试函数
void TcpServerTest(void *pdata)
{
    ssize_t retval = 0; // 定义返回值变量
    int backlog = 1; // 定义监听队列长度
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // 创建TCP socket
    connfd = -1; // 初始化连接文件描述符

    struct sockaddr_in clientAddr = {0}; // 定义客户端地址结构
    socklen_t clientAddrLen = sizeof(clientAddr); // 客户端地址长度
    struct sockaddr_in serverAddr = {"192.168.84.188"}; // 定义服务器地址结构
    serverAddr.sin_family = AF_INET; // 地址族
    serverAddr.sin_port = htons(port);  // 将端口号转换为网络字节序
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有接口

    retval = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); // 绑定socket到地址
    if (retval < 0) {
        printf("bind failed, %ld!\r\n", retval);
        goto do_cleanup; // 绑定失败，跳转到清理标签
    }
    printf("bind to port %d success!\r\n", port); // 绑定成功，打印信息

    retval = listen(sockfd, backlog); // 开始监听
    if (retval < 0) {
        printf("listen failed!\r\n");
        goto do_cleanup; // 监听失败，跳转到清理标签
    }
    printf("listen with %d backlog success!\r\n", backlog); // 监听成功，打印信息

    while(1)
    {
        // 循环等待客户端连接
        connfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (connfd < 0) {
            printf("accept failed, %d, %d\r\n", connfd, errno);
            goto do_cleanup; // 接受连接失败，跳转到清理标签
        }
        printf("accept success, connfd = %d!\r\n", connfd); // 接受连接成功，打印信息
        printf("client addr info: host = %s, port = %d\r\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); // 打印客户端地址信息
        
        while(1)
        {
            // 循环接收客户端请求
            retval = recv(connfd, request, sizeof(request), 0);
            if (retval < 0) {
                printf("recv request failed, %ld!\r\n", retval);
                goto do_disconnect; // 接收请求失败，跳转到断开连接标签
            }

            // 根据接收到的请求控制开关
            if(memcmp(request, "switch1:on", strlen("switch1:on")) == 0)
            {
                printf("____>>>>>>>switch1:on\r\n");
                jd_switch(1, 1); // 控制switch1打开
            }

            if(memcmp(request, "switch1:off", strlen("switch1:off")) == 0)
            {
                printf("____>>>>>>>switch1:off\r\n");
                jd_switch(1, 0); // 控制switch1关闭
            }

            if(memcmp(request, "switch2:on", strlen("switch2:on")) == 0)
            {
                printf("____>>>>>>>switch2:on\r\n");
                jd_switch(2, 1); // 控制switch2打开
            }

            if(memcmp(request, "switch2:off", strlen("switch2:off")) == 0)
            {
                printf("____>>>>>>>switch2:off\r\n");
                jd_switch(2, 0); // 控制switch2关闭
            }

            printf("recv request{%s} from client done!\r\n", request); // 打印接收到的请求
        }

do_disconnect: // 断开连接标签
        connfd = -1; // 重置连接文件描述符
        printf("do_disconnect \r\n");
        sleep(1); // 等待1秒
        close(connfd); // 关闭连接
        sleep(1); // 调试用的等待

do_cleanup: // 清理标签
        printf("do_cleanup...\r\n");
        close(sockfd); // 关闭socket
    }
}

// 发送TCP响应函数
void send_tcp_gb(char *buf, int len)
{
    ssize_t retval = 0;

    if(connfd == -1)
    {
        // 如果没有连接，直接返回
        return ;
    }

    retval = send(connfd, buf, len, 0); // 发送响应
    if (retval <= 0) {
        printf("send response failed, %ld!\r\n", retval);
    }
    printf("send response{%s} to client done!\r\n", buf); // 打印发送的响应
}

// 启动TCP线程函数
void start_tcp_thread(void)
{
    osThreadAttr_t attr; // 定义线程属性

    attr.name = "TcpServerTest"; // 线程名称
    attr.attr_bits = 0U; // 属性位
    attr.cb_mem = NULL; // 回调内存
    attr.cb_size = 0U; // 回调大小
    attr.stack_mem = NULL; // 栈内存
    attr.stack_size = 2048; // 栈大小
    attr.priority = 36; // 线程优先级

    // 创建线程
    if (osThreadNew((osThreadFunc_t)TcpServerTest, NULL, &attr) == NULL) {
        printf("[TcpServerTest] Falied to create LedTask!\n"); // 创建线程失败，打印错误信息
    }
}