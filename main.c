/*
 * Infant Incubator Analyzer with DHT11.c
 *
 * Created: 06/03/2018 12:33:49
 * Author : Ahmed Talal Sallam
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"

#define F_CPU 8000000
#define DHT11_PIN 2

uint8_t Receive_data();
uint8_t c=0,I_RH,A_RH,I_Temp,A_Temp,CheckSum;


int main(void)
{
	char data[5];
	init_ports();
	Lcd8_Init();			/* Initialize LCD */
	Lcd8_Cmd(0x0C); 		/* Clear LCD */
	Lcd8_Cmd(0x80);		/* Enter column and row position */
	Lcd8_Write_String("Humidity=");
	Lcd8_Cmd(0xC0);
	Lcd8_Write_String("Temp= ");
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0x94);
	Lcd8_Write_String("Incu Analyzer Proj");
	_delay_ms(500);
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0xD4);
	Lcd8_Write_String("Ist.Uni");
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0xDE);
	Lcd8_Write_String("AHMEDTALAL");
	_delay_ms(1000);
	
	while(1)
	{
		Request();											/* send start pulse */
		Response();											/* receive response */
		I_RH=Receive_data();								/* store first eight bit in I_RH */
		A_RH=Receive_data();								/* store next eight bit in D_RH */
		I_Temp=Receive_data();								/* store next eight bit in I_Temp */
		A_Temp=Receive_data();								/* store next eight bit in D_Temp */
		CheckSum=Receive_data();							/* store next eight bit in CheckSum */
		
		if ((I_RH + A_RH + I_Temp + A_Temp) != CheckSum)
		{
			Lcd8_Cmd(0x80);
			Lcd8_Write_String("Error");
		}
		
		else
		{
			itoa(I_RH,data,10);
			Lcd8_Cmd(0x8b);
			Lcd8_Write_String(data);
			Lcd8_Write_String(".");
			
			itoa(A_RH,data,10);
			Lcd8_Write_String(data);
			Lcd8_Write_String("%");

			itoa(I_Temp,data,10);
			Lcd8_Cmd(0xC6);
			Lcd8_Write_String(data);
			Lcd8_Write_String(".");
			
			itoa(A_Temp,data,10);
			Lcd8_Write_String(data);
			Lcd8_Cmd(0xc9);
			Lcd8_Write_String("C ");
			
			itoa(CheckSum,data,10);
			Lcd8_Write_String(data);
			Lcd8_Write_String(" ");
		}
		
		_delay_ms(500);
	}
}
	
void Request()							/* Microcontroller send start pulse/request */
{
	DDRA |= (1<<DHT11_PIN);
	PORTA &= ~(1<<DHT11_PIN);			/* set to low pin */
	_delay_ms(20);						/* wait for 20ms */
	PORTA |= (1<<DHT11_PIN);			/* set to high pin */
}

void Response()							/* receive response from DHT11 */
{
	DDRA &= ~(1<<DHT11_PIN);
	while(PINA & (1<<DHT11_PIN));
	while((PINA & (1<<DHT11_PIN))==0);
	while(PINA & (1<<DHT11_PIN));
}

uint8_t Receive_data()											/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PINA & (1<<DHT11_PIN)) == 0);				 /* check received bit 0 or 1 */
		_delay_us(30);
		if(PINA & (1<<DHT11_PIN))							/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);									/* then its logic HIGH */
		else												/* otherwise its logic LOW */
		c = (c<<1);
		while(PINA & (1<<DHT11_PIN));
	}
	return c;
}




