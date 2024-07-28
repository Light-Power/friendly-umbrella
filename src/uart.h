#ifndef __UART_H__
#define __UART_H__

#define MAX_REV_NUM  20

#define MAX_REV_TIME 5

extern unsigned char start_timer; // ������ʱ��
extern unsigned char recv_buf[MAX_REV_NUM];
extern unsigned char recv_cnt;
extern unsigned char recv_timer_cnt;
extern unsigned char recv_flag;

void UartInit(void);                     // ���ڳ�ʼ��
void sendByte(unsigned char dat);        // ���ڷ���һ���ֽ�
void sendString(unsigned char *str);     // ���ڷ����ַ���
char putchar(char c);                    // ���ڷ���һ���ַ�
void clr_recvbuffer(unsigned char *buf); // ��ս��ջ�����
void Timer0_Init(void);                  // 1���붨ʱ��0��ʼ��
#endif