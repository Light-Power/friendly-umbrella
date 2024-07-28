#include <REG52.H>
#include "delay.h"
#include "uart.h"
#include <STDIO.H>

void main(void)
{
    Timer0_Init(); // 1毫秒@11.0592MHz
    UartInit();    // 串口初始化
    EA = 1;
    printf("Waiting for your start\r\n");
    printf("please send a frame of data\r\n");

    while (1) {
        if (recv_flag == 1) {
            recv_flag   = 0;
            start_timer = 0;          // 关定时计数器
            sendString(recv_buf);     // 处理数据
            clr_recvbuffer(recv_buf); // 清空接收缓冲区
        }
    }
}
