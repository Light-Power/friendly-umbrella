#include "uart.h"
#include <REG52.H>
#include <STDIO.H>

unsigned char recv_flag   = 0;
unsigned char start_timer = 0; // ������ʱ��
unsigned char recv_buf[MAX_REV_NUM];
unsigned char recv_cnt;
unsigned char recv_timer_cnt;
void UartInit(void) // 9600bps@11.0592MHz
{
    PCON &= 0x7F; // �����ʲ�����
    SCON = 0x50;  // 8λ����,�ɱ䲨����
    TMOD &= 0x0F; // ���ö�ʱ��ģʽ
    TMOD |= 0x20; // ���ö�ʱ��ģʽ
    TL1 = 0xFD;   // ���ö�ʱ��ʼֵ
    TH1 = 0xFD;   // ���ö�ʱ����ֵ
    ET1 = 0;      // ��ֹ��ʱ���ж�
    TR1 = 1;      // ��ʱ��1��ʼ��ʱ
    ES  = 1;      // ���������ж�
    //
}

void sendByte(unsigned char dat) // ���ڷ���һ���ֽ�    �������ݲ�����data ����Ϊdata�Ǳ����֡�(����ai��д)
{
    SBUF = dat;
    while (TI == 0);
    TI = 0;
}

void sendString(unsigned char *str) // ���ڷ����ַ���
{
    while (*str) {
        sendByte(*str);
        str++;
    }
}

char putchar(char c) // ���ڷ���һ���ַ�
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
// �����жϷ�����
void uart_ISR() interrupt 4
{
    if (RI == 1) {
        RI = 0;

        start_timer = 1; // 1, ���յ�һ֡���ݵ�ʱ��򿪶�ʱ������ȥ����
        if (recv_cnt < MAX_REV_NUM) {
            recv_buf[recv_cnt] = SBUF; // �������ݵ����ݻ�����
            recv_cnt++;
        } else {
            recv_cnt = MAX_REV_NUM;
        }
        recv_timer_cnt = 0; // ÿ����һ֡���ݰѶ�ʱ�������㡣�൱��ι���źš�
                            // �����ڶ�ʱ�ж���᲻���ۼӡ�
    }
    if (TI == 1) {
        TI = 0;
    }
}

//   �жϷ���

void Timer0_Isr(void) interrupt 1
{

    if (start_timer == 1) {
        recv_timer_cnt++;                   // 1, �ۼӶ�ʱʱ���������
        if (recv_timer_cnt > MAX_REV_NUM) { // 2���ж϶�ʱʱ���Ƿ񳬹����趨�������ֵ��
            // ������˵���ȴ���һ��ʱ���û���µ����ݵ��������ж�һ�����ݽ�����ϡ�
            recv_timer_cnt = 0; // 3�������ʱ���������������ݣ� ���buffer���ŵ����ݴ���֮�󣩡�
            recv_flag      = 1;
            recv_cnt       = 0;
        }
    }

    TL0 = 0x66; // ���ö�ʱ��ʼֵ
    TH0 = 0xFC; // ���ö�ʱ��ʼֵ
    TR0 = 1;
}

// ��ʱ��0��ʼ������
void Timer0_Init(void) // 1����@11.0592MHz
{
    TMOD &= 0xF0; // ���ö�ʱ��ģʽ
    TMOD |= 0x01; // ���ö�ʱ��ģʽ
    TL0 = 0x66;   // ���ö�ʱ��ʼֵ
    TH0 = 0xFC;   // ���ö�ʱ��ʼֵ
    TF0 = 0;      // ���TF0��־
    TR0 = 1;      // ��ʱ��0��ʼ��ʱ
    ET0 = 1;      // ���ж�
}
