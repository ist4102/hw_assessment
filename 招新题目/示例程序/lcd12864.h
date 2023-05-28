#ifndef _LCD12864BU_H_
#define _LCD12864BU_H_

#define LCDLCDDisp_Off   0x3e
#define Page_Add     0xb8//ҳ��ַ
#define Start_Line     0xC0//�е�ַ
#define LCDCol_Add     0x40//�е�ַ
#define LCDLCDDisp_On    0x3f
//	write_com(LCDLCDDisp_Off);	 //д��ʼ������
//	write_com(Page_Add+0);
//	write_com(Start_Line+0);
//	write_com(LCDCol_Add+0);
//	write_com(LCDLCDDisp_On);

/*****Һ����ʾ���Ķ˿ڶ���*****/
#define data_ora P0 /*Һ����������*/
sbit LCDDi=P2^0 ;     /*����/ָ�� ѡ��*/
sbit LCDRW=P2^1 ;     /*��/д ѡ��*/
sbit LCDEnable=P2^2 ;     /*��/д ʹ��*/
sbit cs1=P2^3 ;    /*Ƭѡ1*/
sbit cs2=P2^4 ;    /*Ƭѡ2*/
sbit RESET=P2^5 ;  /*��λ�ź�*/

uchar menu_1;
uchar flag_clock_en;	//���ӱ�־λ
uchar flag_clock_kai;	//���ӿ���־λ
uchar flag_c_en;
bit flag_100ms;



#include "ds1302.h"


/****************************************************************************
��������:LCD��ʱ����
��ڲ���:t
���ڲ���:
****************************************************************************/
void LCDdelay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++);
	for(j=0;j<10;j++);
}
/****************************************************************************
״̬��飬LCD�Ƿ�æ


*****************************************************************************/
void CheckState()		
{
   unsigned char dat,DATA;//״̬��Ϣ���ж��Ƿ�æ��
   LCDDi=0; // ����\ָ��ѡ��D/I��RS��="L" ����ʾ DB7��DB0 Ϊ��ʾָ������ 
   LCDRW=1; //R/W="H" ��E="H"���ݱ�����DB7��DB0 
   do
   {
      DATA=0x00;
      LCDEnable=1;	//EN�½�Դ
			LCDdelay(2);//��ʱ
			dat=DATA;
      LCDEnable=0;
      dat=0x80 & dat; //������7λΪ0ʱ�ſɲ���(�б�busy�ź�)
    }
    while(!(dat==0x00));
}
/*****************************************************************************
��������:д���LCD����RS(DI)=L,RW=L,EN=H������һ������дһ��
��ڲ���:cmdcode
���ڲ���:
*****************************************************************************/
void write_com(unsigned char cmdcode)
{
    CheckState();//���LCD�Ƿ�æ
	LCDDi=0;
	LCDRW=0;
	P0=cmdcode;		
	LCDdelay(2);
	LCDEnable=1;
	LCDdelay(2);
	LCDEnable=0;
}
/*****************************************************************************
��������:LCD��ʼ������
��ڲ���:
���ڲ���:
*****************************************************************************/
void init_lcd()
{
	LCDdelay(100);	
	cs1=1;//�տ�ʼ�ر�����
	cs2=1;
	LCDdelay(100);
	write_com(LCDLCDDisp_Off);	 //д��ʼ������
	write_com(Page_Add+0);
	write_com(Start_Line+0);
	write_com(LCDCol_Add+0);
	write_com(LCDLCDDisp_On);
}
/*****************************************************************************
��������:д���ݵ�LCD����RS(DI)=H,RW=L,EN=H������һ������дһ��
��ڲ���:LCDDispdata
���ڲ���:
*****************************************************************************/
void write_data(unsigned char LCDDispdata)
{
    CheckState();//���LCD�Ƿ�æ
	LCDDi=1;
	LCDRW=0;
	P0=LCDDispdata;
	LCDdelay(2);
	LCDEnable=1;
	LCDdelay(2);
	LCDEnable=0;
}

/***********************д���ֺ���**********************/
void write_yhanzi(uchar n,uchar x,uchar y,uchar  * p)
{
   uchar i,j,nm,m; 
   m=0xb8+x;

   for(j=0;j<n;j++)
   {
	   
	   if((16*j+y)<64)
	       {
		   		cs1=1;cs2=0;nm=0x40+y+16*j;  
		   }
	   else
	       {
		   		cs1=0;cs2=1;nm=0x40+y+16*(j-4);  
		   }
       write_com(m);
       write_com(nm);
	   for(i=0;i<16;i++)
	    { 
			write_data(*p);p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=16;i<32;i++)
	    { 
			write_data(*p);p++;
	    }
   }
}

void write_yhanzi_fanbai(uchar n,uchar x,uchar y,uchar  * p,uchar fanbai)
{
   uchar i,j,nm,m; 
   m=0xb8+x;

   for(j=0;j<n;j++)
   {
	   
	   if((16*j+y)<64)
	       {
		   		cs1=1;cs2=0;nm=0x40+y+16*j;  
		   }
	   else
	       {
		   		cs1=0;cs2=1;nm=0x40+y+16*(j-4);  
		   }
       write_com(m);
       write_com(nm);
	   for(i=0;i<16;i++)
	    { 
			if(fanbai == 0)
				write_data(*p);
			else 	
				write_data(~(*p));
				p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=16;i<32;i++)
	    { 
			if(fanbai == 0)
				write_data(*p);
			else 	
				write_data(~(*p));
				p++; 
	    }
   }
}

/***********************д���ֺ���**********************/
void write_shuzi(uchar n,uchar x,uchar y,uchar  * p,uchar fanbai)
{
   uchar i,j,nm,m; 
   m=0xb8+x;
   for(j=0;j<n;j++)
   {
	   
	   if((8*j+y)<64)
	       {cs1=1;cs2=0;nm=0x40+y+8*j;  }
	   else
	       {cs1=0;cs2=1;nm=0x40+y+8*(j-8);  }
       write_com(m);
       write_com(nm);
	   for(i=0;i<8;i++)
	    { 
		if(fanbai == 0)
		write_data((*p));
		else 
		write_data(~(*p));
		p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=8;i<16;i++)
	    { 
		if(fanbai == 0)
		write_data((*p));
		else 
		write_data(~(*p));
		p++; 
		}
   }
}

/**********************************************************
*������: write_shu16
*����:��12864����ʾ16��������
*˵��:
*����:��
*����:��
**********************************************************/
void write_shu16(uchar x,uchar y,uchar dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 16],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat % 16],fanbai);
}


void write_shu18b20(uchar x,uchar y,uint dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 100 % 10],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat / 10 % 10],fanbai);
	write_shuzi(1,x,y+24,shuzi[dat % 10],fanbai);
}


void write_shu123(uchar x,uchar y,uchar dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 16],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat % 16],fanbai);
}


/*****************************************************************************
��������:���LCD�ڴ����
��ڲ���:pag,col,hzk
���ڲ���:
*****************************************************************************/
void Clr_Scr()
{
	unsigned char j,k;
	cs1=0; //������������ʾ
	cs2=0;
 	write_com(Page_Add+0);
 	write_com(LCDCol_Add+0);
 	for(k=0;k<8;k++)//����ҳ��0-7����8ҳ
 	{
		write_com(Page_Add+k); //ÿҳÿҳ����д
  		for(j=0;j<64;j++)  //ÿҳ����д32���������ֻ�64��ASCII�ַ�
		{
      		write_com(LCDCol_Add+j);
			write_data(0x00);//��������0-63����64�У�д�����ݣ��е�ַ�Զ���1
		}
	}
}
/*****************************************************************************
��������:����λ����ʾ
��ڲ���:page,column,hzk
���ڲ���:
*****************************************************************************/
void Bmp_Left_Disp(unsigned char page,unsigned char column, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+column);
		for(i=0;i<16;i++) 
		{
			write_data(Bmp[128*j+i]);
			delay_1ms(50);
		}
	}
}

void Bmp_Left_Disp_hanzi(unsigned char page,unsigned char hang, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+hang);
		for(i=0;i<16;i++) 
//			write_data(Bmp[128*j+i]);
			write_data(Bmp[0x00]);
	}
}


void clear_12864()
{	 	
	uchar x,y;
	cs1=1;
	cs2=0;
	for(x=0;x<8;x++)
	{
		write_com(0xb8+x);
		for(y=0;y<64;y++)
		{		 
			write_data(0X00);
		}
	}		
}
/*****************************************************************************
��������:����λ����ʾ
��ڲ���:page,column,hzk
���ڲ���:
*****************************************************************************/
void Bmp_Right_Disp(unsigned char page,unsigned char column, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+column);
		for(i=64;i<128;i++) 
		{
			write_data(Bmp[128*j+i]);
			delay_1ms(50);

		}

	}
}

void qingling()
{
   uchar x,y ;
   cs1=1;
   cs2=1;
   write_com(0x3f);
   write_com(0xc0);
   for(x=0;x<8;x++)
    {  
	   write_com(0xb8+x);
	   write_com(0xc0);
     for(y=0;y<64;y++)
	   {
	      write_data(0x00);
	   }
	   write_com(0xb8+x+1);
	   write_com(0xc0);
     for(y=0;y<64;y++)
	   {
	      write_data(0x00);
	   }
	}
}
#endif