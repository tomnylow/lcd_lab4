#include <stdint.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include "MDR32F9Qx_config.h"           // Keil::Device:Startup


#define PIN_DB0 PORT_Pin_0
#define PIN_DB1 PORT_Pin_1
#define PIN_DB2 PORT_Pin_1
#define PIN_DB3 PORT_Pin_3
#define PORT_DB0 MDR_PORTC
#define PORT_DB1 MDR_PORTC
#define PORT_DB2 MDR_PORTF
#define PORT_DB3 MDR_PORTF

#define PIN_WR1 PORT_Pin_2
#define PIN_WR2 PORT_Pin_3
#define PIN_A0 PORT_Pin_6
#define PORT_WR1 MDR_PORTF
#define PORT_WR2 MDR_PORTA
#define PORT_A0 MDR_PORTA


#define LOCK_ADDRESS 0x0F

#define UNLOCK_VALUE 0x01
#define DELAY {__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");}
void InitIO()
{

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);

    PORT_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.PORT_MODE = PORT_MODE_DIGITAL;
    GPIO_InitStruct.PORT_SPEED = PORT_SPEED_FAST;
    GPIO_InitStruct.PORT_OE = PORT_OE_OUT;
		GPIO_InitStruct.PORT_FUNC = PORT_FUNC_PORT;
		GPIO_InitStruct.PORT_PULL_UP = PORT_PULL_UP_OFF;
    GPIO_InitStruct.PORT_Pin = PIN_DB0;
    PORT_Init(PORT_DB0, &GPIO_InitStruct);
    GPIO_InitStruct.PORT_Pin = PIN_DB1;
    PORT_Init(PORT_DB1, &GPIO_InitStruct);
    GPIO_InitStruct.PORT_Pin = PIN_DB2;
    PORT_Init(PORT_DB2, &GPIO_InitStruct);
    GPIO_InitStruct.PORT_Pin = PIN_DB3;
    PORT_Init(PORT_DB3, &GPIO_InitStruct);

    GPIO_InitStruct.PORT_Pin = PIN_WR1;
    PORT_Init(PORT_WR1, &GPIO_InitStruct);
    GPIO_InitStruct.PORT_Pin = PIN_WR2;
    PORT_Init(PORT_WR2, &GPIO_InitStruct);
    GPIO_InitStruct.PORT_Pin = PIN_A0;
    PORT_Init(PORT_A0, &GPIO_InitStruct);
}

void SetData(uint8_t data)
{
  
    PORT_WriteBit(PORT_DB0, PIN_DB0, (data & 0x01));
    PORT_WriteBit(PORT_DB1, PIN_DB1, (data & 0x02) >> 1);
    PORT_WriteBit(PORT_DB2, PIN_DB2, (data & 0x04) >> 2);
    PORT_WriteBit(PORT_DB3, PIN_DB3, (data & 0x08) >> 3);
}


void WriteData(uint8_t adress, uint8_t data)
{
    PORT_WriteBit(PORT_WR2, PIN_WR2, 1);
    SetData(adress);
		DELAY
		PORT_WriteBit(PORT_A0, PIN_A0, 0);
		DELAY
    PORT_WriteBit(PORT_WR1, PIN_WR1, 1);
		DELAY	
		PORT_WriteBit(PORT_WR1, PIN_WR1, 0);
		DELAY
		PORT_WriteBit(PORT_A0, PIN_A0, 1);
		DELAY
		
    SetData(data);
		DELAY
    PORT_WriteBit(PORT_WR1, PIN_WR1, 1);
		DELAY	
		PORT_WriteBit(PORT_WR1, PIN_WR1, 0);
		DELAY
	
		SetData(data);
		DELAY
    PORT_WriteBit(PORT_WR1, PIN_WR1, 1);
		DELAY	
		PORT_WriteBit(PORT_WR1, PIN_WR1, 0);
		DELAY
    
    PORT_WriteBit(PORT_WR2, PIN_WR2, 0);

}


void UnlockBus()
{				
    WriteData(LOCK_ADDRESS, UNLOCK_VALUE);
}

int main()
{
    
    InitIO();
	
    UnlockBus();
  
    WriteData(0x00, 0x01);

    WriteData(0x00, 0x00);

    while (1)
    {

    }
}
