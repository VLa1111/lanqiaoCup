#include "reg52.h"
//unsigned int DScode[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
#define Sec_1	120000
#define HalfSec_1	60000

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;
sbit L7 = P0^6;
sbit L8 = P0^7;

sfr P4 = 0xC0;
sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

sbit C4 = P3^4;
sbit C3 = P3^5;
sbit C2 = P4^2;
sbit C1 = P4^4;

sfr AUXR = 0x8e;

unsigned char urdat;
unsigned char yu = 1;
unsigned char stat_k = 0;
unsigned char t_m = 0;
unsigned char t_s = 0;
unsigned char t_005s = 0;

unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
     0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
     0xbf,0x7f};

void delay_us(unsigned int t)
{
	while (t--)
	{
		/* code */
	}
	while (t--)
	{
		/* code */
	}
}

void DelaySMG(unsigned int t)
{
	while (t--)
	{
		/* code */
	}	
}

void DelayK(unsigned int t)
{
	while (t--)
	{
		/* code */
	}
	
}

void SelectHC573(unsigned char channel)
{
	switch (channel)
	{
	case 4:
		/* code */
		P2 = (P2 & 0x1f) | 0x80;
		break;
	case 5:
		P2 = (P2 & 0x1f) | 0xa0;
		break;
	case 6:
		P2 = (P2 & 0x1f) | 0xc0;
		break;
	case 7:
		P2 = (P2 & 0x1f) | 0xe0;
		break;
	}
}
void Init_INT0()
{
	IT0 = 1;	//�ⲿ�ж��½��ش���
	EX0 = 1;	//�����ⲿ�ж�0
	EA  = 1;	//�����ж��ܿ���
}

void Init_Timer0()
{
	TMOD = 0x01;
	TH0  = (65535 - 100) / 256;
	TL0  = (65535 - 100) % 256;

	ET0  = 1;	//������ʱ��0�ж�
	EA   = 1;	//����ȫ���ж�
	//TR0  = 1;	//������ʱ��
}

void SendByte(unsigned char dat);

void InitUart()
{
	TMOD = 0x20;
	TH1  = 0xfd;
	TL1  = 0xfd;
	TR1  = 1;

	SCON = 0x50;
	AUXR = 0x00;

	ES   = 1;
	EA	 = 1;
}

void ServiceUart() interrupt 4
{
	if (RI == 1)
	{
		RI = 0;
		urdat = SBUF;
		SendByte(urdat + 1);
	}
}

void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

unsigned char count = 0;
unsigned char pwm_duty = 0;
void ServerTimer0() interrupt 1
{
	TH0  = (65535 - 100) / 256;
	TL0  = (65535 - 100) % 256;

	// count++;
	// if (count % 10 == 0)
	// {
	// 	L1 = ~L1;
	// }
	// if (count == 100)
	// {
	// 	L8 = ~L8;
	// 	count = 0;
	// }

	// t_005s++;
	// if(t_005s == 20)
	// {
	// 	t_s++;
	// 	t_005s = 0;
	// 	if (t_s == 60)
	// 	{
	// 		t_m++;
	// 		t_s = 0;
	// 	}
	// 	if (t_m == 99)
	// 	{
	// 		t_m = 0;
	// 	}
	// }

	count ++;
	if (count == pwm_duty)
	{
		L1 = 1;
	}
	else if (count == 100)
	{
		L1 = 0;
		count = 0;
	}
}

void LED1_Working()
{
	SelectHC573(4);
	L1 = 0;
	DelayK(60000);
	L1 = 1;
	DelayK(60000);
}

unsigned char stat_int = 0;
void ServerINT0() interrupt 0
{
	stat_int = 1;
} 

void LEDINT()
{
	if (stat_int == 1)
	{
		L8 = 0;
		DelayK(60000);
		DelayK(60000);
		DelayK(60000);
		DelayK(60000);
		DelayK(60000);
		DelayK(60000);
		L8 = 1;
	}
	stat_int = 0;
}

// void OutPutP0(unsigned char channel, unsigned char dat)
// {
// 	InitHC138(channel);
// 	P0 = dat;
// }

void DisplaySMG_Bit(unsigned char value, unsigned pos)
{
	P0 = 0xff;
	SelectHC573(6);
	P0 = 0x01 << pos;
	SelectHC573(7);
	P0 = value;
}

// void ShowSMG_Bit(unsigned char dat, unsigned pos)
// {
// 	InitHC138(6);
// 	P0 = 0x01 << pos;		//�����λ��? 
// 	InitHC138(7);
// 	P0 = dat;				//���������?
// }

void DisplayKeyNum(unsigned char value)
{
	P0 = 0xff;
	SelectHC573(6);
	P0 = 0x01;
	SelectHC573(7);
	P0 = value;
}

// void SMG_Static()
// {
// 	unsigned char i,j;
// 	for (i = 0; i<8; i++)
// 	{
// 		for (j = 0; j < 10; j++)
// 		{
// 			ShowSMG_Bit(SMG_duanma[i],i);
// 			delay_us(HalfSec_1);
// 		}
// 	}
// 	for (j = 0; j < 16; j++)
// 	{
// 		InitHC138(6);
// 		P0 = 0xff;
// 		InitHC138(7);
// 		P0 = SMG_duanma[j];
// 		delay_us(Sec_1);
// 	}
// }

void Display_Dynamic()
{
	DisplaySMG_Bit(SMG_duanma[2],0);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[0],1);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[2],2);
	DelaySMG(500);	
	DisplaySMG_Bit(SMG_duanma[2],3);
	DelaySMG(500);

	DisplaySMG_Bit(SMG_duanma[16],4);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[16],5);
	DelaySMG(500);

	DisplaySMG_Bit(SMG_duanma[yu/10],6);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[yu%10],7);
	DelaySMG(500);
}

void ScanKeys_Alone_1()
{
	if (S7 == 0)
	{
		DelayK(100);
		if (S7 == 0)
		{
			L1 = 0;
			while (S7 == 0);
			L1 = 1;
		}
	}

	if (S6 == 0)
	{
		DelayK(100);
		if (S6 == 0)
		{
			L2 = 0;
			while (S6 == 0);
			L2 = 1;
		}
	}

	if (S5 == 0)
	{
		DelayK(100);
		if (S5 == 0)
		{
			L3 = 0;
			while (S5 == 0);
			L3 = 1;		
		}
		
	}

	if (S4 == 0)
	{
		DelayK(100);
		if (S4 == 0)
		{
			L4 = 0;
			while (S4 == 0);
			L4 = 1;		
		}
	}
}

void ScanKeys_Alone_2()
{
	if (S7 == 0)
	{
		DelayK(100);
		if (S7 == 0)
		{
			if (stat_k == 0)
			{
				L1 = 0;
				stat_k = 1;
			}
			else if (stat_k == 1)
			{
				L1 = 1;
				stat_k = 0;
			}
			while (S7 == 0);	
		}	
	}
	if (S6 == 0)
	{
		DelayK(100);
		if (S6 == 0)
		{
			if (stat_k == 0)
			{
				L2 = 0;
				stat_k = 2;
			}
			else if (stat_k == 2)
			{
				L2 = 1;
				stat_k = 0;
			}
			while (S6 == 0);		
		}
	}
	if (S5 == 0)
	{
		DelayK(100);
		if (S5 == 0)
		{
			if (stat_k == 1)
			{
				L3 = 0;
				while (S5 == 0);
				L3 = 1;
			}
			else if (stat_k == 2)
			{
				L5 = 0;
				while (S5 == 0);
				L5 = 1;
			}
		}
	}
	if (S4 == 0)
	{
		DelayK(100);
		if (S4 == 0)
		{
			if (stat_k == 1)
			{
				L4 = 0;
				while (S4 == 0);
				L4 = 1;
			}
			else if (stat_k == 2)
			{
				L6 = 0;
				while (S4 == 0);
				L6 = 1;			
			}
		}
	}
}

void Delay(unsigned int t)
{
	while (t--)
	{
		/* code */
		Display_Dynamic();
	}
	
}

void LED_Running(void)
{
	unsigned char i;

	for (i = 0; i < 3; i++)
	{
		OutPutP0(4, 0x00);
		//P0 = 0x00;
		delay_us(Sec_1);
		//P0 = 0xff;
		OutPutP0(4,0xff);
		delay_us(Sec_1);
	}

	for (i = 1; i <= 8; i++)
	{
		OutPutP0(4, (0xff << i));
		delay_us(HalfSec_1);
	}

	OutPutP0(5, 0x10);
	delay_us(Sec_1);
	OutPutP0(5, 0x00);

	//InitHC138(4);
	for (i = 1; i <= 8; i++)
	{
		//P0 = ~(0xff << i);
		OutPutP0(4, ~(0xff << i));
		delay_us(HalfSec_1);
	}

	OutPutP0(5, 0x40);
	delay_us(Sec_1);
	OutPutP0(5, 0x00);
}

unsigned char key_num;
void ScanKeysMulti()
{
	R1 = 0;
	R2 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if (C1 == 0)
	{
		while(C1 == 0)
		key_num = 0;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while (C2 == 0)
		key_num = 1;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0)
		key_num = 2;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0)
		key_num = 3;
		DisplayKeyNum(SMG_duanma[key_num]);
	}

	R2 = 0;
	R1 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if (C1 == 0)
	{
		while(C1 == 0)
		key_num = 4;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C2 == 0)
	{
		while(C2 == 0);
		key_num = 5;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C3 == 0)
	{
		while(C3 == 0);
		key_num = 6;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if (C4 == 0)
	{
		while(C4 == 0);
		key_num = 7;
		DisplayKeyNum(SMG_duanma[key_num]);
	}

	R3 = 0;
	R2 = R1 = R4 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 8;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 9;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 10;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 11;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	
	R4 = 0;
	R2 = R3 = R1 = 1;
	C1 = C2 = C3 = C4 =1;
	if(C1 == 0)
	{
		while(C1 == 0);
		key_num = 12;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 == 0);
		key_num = 13;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 == 0);
		key_num = 14;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 == 0);
		key_num = 15;
		DisplayKeyNum(SMG_duanma[key_num]);
	}
}

void InitSystem()
{
	OutPutP0(5,0x00);
	OutPutP0(5, 0x10);
	delay_us(Sec_1);
	OutPutP0(5, 0x00);

	OutPutP0(5, 0x40);
	delay_us(Sec_1);
	OutPutP0(5, 0x00);

	OutPutP0(4, 0xff);
}

void DisplayTime()
{
	DisplaySMG_Bit(SMG_duanma[t_005s%10],7);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[t_005s/10],6);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[16],5);
	DelaySMG(500);

	DisplaySMG_Bit(SMG_duanma[t_s%10],4);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[t_s/10],3);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[16],2);
	DelaySMG(500);

	DisplaySMG_Bit(SMG_duanma[t_m],1);
	DelaySMG(500);
	DisplaySMG_Bit(SMG_duanma[t_m/10],0);
	DelaySMG(500);
}


unsigned char stat = 0;
void ScanKeys()
{
	// if (S4 == 0)
	// {
	// 	DelayK(100);
	// 	if (S4 == 0)
	// 	{
	// 	TR0 = ~TR0;
	// 	while (S4 == 0)
	// 	{
	// 			/* code */
	// 			DisplayTime();
	// 	}
	// 	}
	// }
	// if (S5 == 0)
	// {
	// 	DelayK(100);
	// 	if (S5 == 0)
	// 	{
	// 	t_005s = 0;
	// 	t_s = 0;
	// 	t_m = 0;
	// 	while (S5 == 0)
	// 	{
	// 			/* code */
	// 			DisplayTime();
	// 	}
	// 	}
	// }
	if (S7 == 0)
	{
		DelayK(100);
		if (S7 == 0)
		{
		switch (stat)
		{
		case /* constant-expression */ 0:
				/* code */
				L1 = 0;
				TR0 = 1;
				pwm_duty = 10;
				stat = 1;
				break;
		case 1:
				pwm_duty = 50;
				stat = 2;
				break;
		case 2:
				pwm_duty = 90;
				stat = 3;
				break;

		case 3:
				L1 = 1;
				TR0 = 0;
				stat = 0;
				break;
		}
		while (S7 == 0);
		}
	}
}

void main()
{
	InitSystem();
	// SelectHC573(4);
	// L1 = 1;
	//Init_INT0();
	//Init_Timer0();
	InitUart();
	SendByte(0x5a);
	SendByte(0xa5);
	while(1)
	{	
		//LED_Running();
		//SMG_Static();		
		// Display_Dynamic();
		// yu++;
		// if (yu > 12)
		// {
		// 	yu = 1;
		// }
		// Delay(200);
		// ScanKeys_Alone();
		// ScanKeys_Alone_2();
		// ScanKeysMulti();
		// LED1_Working();
		// LEDINT();
		//DisplayTime();
		//ScanKeys();
		SendByte(0x11);
	}
}