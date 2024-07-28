#include "uart.h"
#include <REG52.H>
#include <STDIO.H>

unsigned char recv_flag   = 0;
unsigned char start_timer = 0; // 启动定时器
unsigned char recv_buf[MAX_REV_NUM];
unsigned char recv_cnt;
unsigned char recv_timer_cnt;
void UartInit(void) // 9600bps@11.0592MHz
{
    PCON &= 0x7F; // 波特率不倍速
    SCON = 0x50;  // 8位数据,可变波特率
    TMOD &= 0x0F; // 设置定时器模式
    TMOD |= 0x20; // 设置定时器模式
    TL1 = 0xFD;   // 设置定时初始值
    TH1 = 0xFD;   // 设置定时重载值
    ET1 = 0;      // 禁止定时器中断
    TR1 = 1;      // 定时器1开始计时
    ES  = 1;      // 开启串口中断
    //
}

void sendByte(unsigned char dat) // 串口发送一个字节    这里数据不能用data ，因为data是保留字。(慎用ai帮写)
{
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}

void sendString(unsigned char *str) // 串口发送字符串
{
    while (*str) {
        sendByte(*str);
        str++;
    }
}

char putchar(char c) // 串口发送一个字符
{
    sendByte(c);
    return c;
}

void clr_recvbuffer(unsigned char *buf)
{
    unsigned char i;
    for (i = 0; i < MAX_REV_NUM; i++) {
        buf[i] = 0;
    }
}
// 串口中断服务函数
void uart_ISR() interrupt 4
{
    if (RI == 1) {
        RI = 0;

        start_timer = 1; // 1, 接收第一帧数据的时候打开定时计数器去计数
        if (recv_cnt < MAX_REV_NUM) {
            recv_buf[recv_cnt] = SBUF; // 接收数据到数据缓冲区
            recv_cnt++;
        } else {
            recv_cnt = MAX_REV_NUM;
        }
        recv_timer_cnt = 0; // 每接收一帧数据把定时计数清零。相当于喂狗信号。
                            // 但是在定时中断里会不断累加。
    }
    if (TI == 1) {
        TI = 0;
    }
}

//   中断服务

void Timer0_Isr(void) interrupt 1
{

    if (start_timer == 1) {
        recv_timer_cnt++;                   // 1, 累加定时时间计数器。
        if (recv_timer_cnt > MAX_REV_NUM) { // 2，判断定时时间是否超过了设定的最大阈值，
            // 超过则说明等待了一段时间后没有新的数据到，我们判断一包数据接收完毕。
            recv_timer_cnt = 0; // 3，清除定时计数器，处理数据， 清除buffer（放到数据处理之后）。
            recv_flag      = 1;
            recv_cnt       = 0;
        }
    }

    TL0 = 0x66; // 设置定时初始值
    TH0 = 0xFC; // 设置定时初始值
    TR0 = 1;
}

// 定时器0初始化函数
void Timer0_Init(void) // 1毫秒@11.0592MHz
{
    TMOD &= 0xF0; // 设置定时器模式
    TMOD |= 0x01; // 设置定时器模式
    TL0 = 0x66;   // 设置定时初始值
    TH0 = 0xFC;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
    ET0 = 1;      // 开中断
}
