#include"STC15W4K32S4.h"
#include <intrins.h>
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7
#define S4_S0 0x04              //P_SW2.2
#define S4TI 0x01
unsigned char tierline[6];
unsigned char keydata[6];
unsigned int a=0;
void rowjudge();
void KeyScan();
sbit tier0=P0^6;
sbit tier1=P0^5; 
sbit tier2=P0^4;
sbit tier3=P0^1;
sbit tier4=P0^0;
sbit tier5=P4^6;		
sbit row0=P4^5;
sbit row1=P2^7;
sbit row2=P2^6;
sbit row3=P2^5;
sbit row4=P2^4;
sbit row5=P2^3;
sbit row6=P2^2;
sbit row7=P2^1;
sbit row8=P2^0;
sbit row9=P4^4;
sbit row10=P4^3;
sbit row11=P4^2;
sbit row12=P4^1;
sbit row13=P3^7;
sbit row14=P3^6;
sbit row15=P3^5;
sbit row16=P3^4;
sbit row17=P3^3;
unsigned char senddata[14]={0x57,0xAB,0,0x02,0x08,0,0,0,0,0,0,0,0,0x0c};
unsigned char code senddatainit[14]={0x57,0xAB,0,0x02,0x08,0,0,0,0,0,0,0,0,0x0c};
//unsigned char code senddatainit[14]={0x57,0xAB,0,0x02,0x08,0,0,0x04,0,0,0,0,0,0x10};
unsigned char code resetdata[4]={0x57,0xAB,0x00,0x11};	
unsigned char code key[109]={
0x00,0x62,0x63,0x58,0xE0/*left ctrl*/,0xE3/*left windows*/,0xE2/*left alt*/,0x2C,0xE6/*right alt*/,0xE7/*right windows*/,0x65/*menu��*/,0xE4/*right ctrl*/,0x50,0x51,0x4F,0x52,0x4C,0x4D,0x4E,
0x59,0x5A,0x5B,0xE1/*left shift*/,0x1D,0x1B,0x06,0x19,0x05,0x11,0x10,0x36,0x37,0x38,0xE5/*right shift*/,0x49,0x4A,0x4B,
0x5C,0x5D,0x5E,0x57,0x39,0x04,0x16,0x07,0x09,0x0A,0x0B,0x0D,0x0E,0x0F,0x33,0x34,0x28,0,
0x5F,0x60,0x61,0x2B,0x14,0x1A,0x08,0x15,0x17,0x1C,0x18,0x0C,0x12,0x13,0x2F,0x30,0x31,0,
0x53,0x54,0x55,0x56,0x35,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2D,0x2E,0x2A,								   
0x29,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0,0};
	void Timer0Init(void)		  //5΢��
{
	AUXR |= 0x80;		
	TMOD &= 0xF0;		
	TMOD |= 0x02;		
	TL0 = 0x91;		
	TH0 = 0x91;		
	TF0 = 0;		
	EA=1;
	ET0=1;
	TR0 = 1;		
}
void UartInit1(void)		//57600bps@22.1184MHz
{
/*	ACC = P_SW1;
	ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
	ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)		 
	P_SW1 = ACC; 				  */
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xF4;		//�趨��ʱ��ֵ
	TH1 = 0xF4;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	ES=1;
	TI=0;
	RI=0; 
}
/*void UartInit2(void)		//19200bps@22.1184MHz
{
	P_SW2 &= ~S4_S0;            //S4_S0=0 (P0.2/RxD4, P0.3/TxD4)
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	S4CON &= 0xBF;		//����4ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xE0;		//�趨��ʱ��ֵ
	T2H = 0xFE;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2			   
	IE2 = 0x10;                 //ʹ�ܴ���4�ж�
}											  */
						
void intInit()
{
	EX0=1;
	IT0=1;

}
void IOInit()
{
	P0M0=0x00;
	P0M1=0x00;
	P1M0=0x00;
	P1M1=0x00;
	P2M0=0x00;
	P2M1=0x00;		  
	P3M0=0x00;
	P3M1=0x00;
	P4M0=0x00;
	P4M1=0x00;		
}
void main()
{
	Timer0Init();
	UartInit1();
//	UartInit2();
	intInit();
	IOInit();
	while(1);
}
void Delay1ms()		//@22.1184MHz
{
	unsigned char i, j;

	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}
void tierdefine(unsigned char i)
{
	tier0=tier1=tier2=tier3=tier4=tier5=i;
}				  
void rowdefine(unsigned char i)
{
	row0=row1=row2=row3=row4=row5=row6=row7=row8=row9=row10=row11=row12=row13=row14=row15=row16=row17=i;
}
void tierinit()
{
	tierline[0]=tierline[1]=tierline[2]=tierline[3]=tierline[4]=tierline[5]=0;
}
void tierjudge()
{
	if(tier0==0)
	tierline[0]=1;
	if(tier1==0)
	tierline[1]=2;
	if(tier2==0)
	tierline[2]=3;
	if(tier3==0)
	tierline[3]=4;
	if(tier4==0)
	tierline[4]=5;
	if(tier5==0)
	tierline[5]=6;
}
void KeyScan()
{
	unsigned char i,j;
	rowdefine(0);				//������͵�ƽ
	tierdefine(1);   			//������ߵ�ƽ
	if(tier0==1&&tier1==1&&tier2==1&&tier3==1&&tier4==1&&tier5==1)
	_nop_();	   					  //���������Ϊ�͵�ƽ������������
	else						//��		 
	{
		Delay1ms();				//����
		Delay1ms();				//����		 
		if(tier0==0||tier1==0||tier2==0||tier3==0||tier4==0||tier5==0)		 
		{						  //�ٴμ��������Ϊ�͵�ƽ�����������������������if�ṹ
			rowdefine(1);
			tierdefine(1);		//������ߵ�ƽ
			Delay1ms();				//����
			Delay1ms();				//����
			Delay1ms();				//����
			Delay1ms();				//����
			Delay1ms();				//����	  
			for(i=0;i<6;i++)
			keydata[i]=0;
			row17=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row17=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(j=0;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+18;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ



			row2=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row2=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+3;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ
			
				  
			row3=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row3=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+4;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ
				  
			row4=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row4=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+5;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ 
			row5=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row5=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+6;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ
			row6=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row6=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+7;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ
			row7=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row7=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+8;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ 
			row8=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row8=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+9;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ  
			row9=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row9=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+10;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			row10=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row10=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+11;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			row11=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row11=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+12;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			row12=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row12=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+13;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼������� 
			tierdefine(1);		//������ߵ�ƽ
			row13=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row13=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+14;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			row14=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row14=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+15;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			row15=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row15=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+16;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ  
			row16=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row16=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+17;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ	  
			
			
		//	for(i=0;i<6;i++)
		//	{
		//		if(keydata[i]==1)
		//		keydata[i]=0;
	//		}
			row0=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row0=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+1;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼������� 
			tierdefine(1);		//������ߵ�ƽ

			row1=0;
			_nop_();		   
			tierjudge();			//���������Ϊ�͵�ƽ�����У����¼����	   
			row1=1;			 //����0����Ϊ�ߵ�ƽ
			for(i=0;i<6;i++)		//ѭ��		
			{
				if(tierline[i]!=0)		 //�жϸ����Ƿ��⵽�͵�ƽ������
				{
					for(;j<6;j++)
					{
						if(keydata[j]==0)
						{
							keydata[j]=(tierline[i]-1)*18+2;
							break;
						}	  //����������еĹ�ϵ������������
					}
				}
			}
			tierinit();					  //���м�¼�������
			tierdefine(1);		//������ߵ�ƽ							
		}
	}



}
void datatokey()
{
	unsigned char i;
	for(i=0;i<6;i++)
	{
		if(keydata[i]==0xE0)
		senddata[5]++;
		else if(keydata[i]==0xE1)
		senddata[5]=senddata[5]+2;
		else if(keydata[i]==0xE2)
		senddata[5]=senddata[5]+4;
		else if(keydata[i]==0xE3)
		senddata[5]=senddata[5]+8;
		else if(keydata[i]==0xE4)
		senddata[5]=senddata[5]+16;
		else if(keydata[i]==0xE5)
		senddata[5]=senddata[5]+32;
		else if(keydata[i]==0xE6)
		senddata[5]=senddata[5]+64;
		else if(keydata[i]==0xE7)
		senddata[5]=senddata[5]+128;
		else
		senddata[i+7]=key[keydata[i]];
		senddata[3]=0x02;
		senddata[4]=0x08;
		senddata[13]=12+senddata[5]+senddata[7]+senddata[8]+senddata[9]+senddata[10]+senddata[11]+senddata[12];
	}	
}
void datasend()
{
	unsigned char i;
		for(i=0;i<14;i++)
		{
			SBUF = senddata[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
			while(!TI);		  //�ȴ������������
			TI=0;			  //���������ɱ�־λ;
		}//keydata���ݴ���	
}

void datainit()
{
	unsigned char i;
	for(i=0;i<14;i++)
	{
		senddata[i]=senddatainit[i];		
	}
	for(i=0;i<6;i++)
	{
		keydata[i]=0;
	}	
}
/*void int0() interrupt 0
{
	unsigned char i;
	EX0=0;
	for(i=0;i<4;i++)	  
	{
		SBUF = resetdata[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
//		S4BUF = resetdata[i];
		while(TI==0);		  //�ȴ������������
		TI=0;			  //���������ɱ�־λ;
//		while(S4CON & S4TI==1);
//		S4CON&=~0x10;
	}
	for(i=0;i<14;i++)
	{
		SBUF = senddatainit[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
//		S4BUF = senddatainit[i];
		while(TI==0);		  //�ȴ������������
		TI=0;			  //���������ɱ�־λ;
//		while(S4CON & S4TI==1);
//		S4CON&=~0x10;
	}
	EX0=1;	
}	   */
void timer0() interrupt 1		  //0.05��һ��
{
	TL0 = 0x91;		
	TH0 = 0x91;		
	a++;	
	if(a==10000)
	{	
		a=0;
		KeyScan();
		datatokey();
		datasend();
		datainit();
	}	
}