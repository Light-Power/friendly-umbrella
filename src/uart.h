#ifndef __UART_H__
#define __UART_H__

#define MAX_REV_NUM  20

#define MAX_REV_TIME 5

extern unsigned char start_timer; // 启动定时器
extern unsigned char recv_buf[MAX_REV_NUM];
extern unsigned char recv_cnt;
extern unsigned char recv_timer_cnt;
extern unsigned char recv_flag;

void UartInit(void);                     // 串口初始化
void sendByte(unsigned char dat);        // 串口发送一个字节
void sendString(unsigned char *str);     // 串口发送字符串
char putchar(char c);                    // 串口发送一个字符
void clr_recvbuffer(unsigned char *buf); // 清空接收缓冲区
void Timer0_Init(void);                  // 1毫秒定时器0初始化
#endif