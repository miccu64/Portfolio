#ifndef piny_h
#define piny_h

#include "MKL05Z4.h"

void init ()//inicjalizacja we/wy, przerwan, wyswietlacza
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;//zegar dla portu A i B

	//inicjalizacja przyciskow i przerwan
	//PORTA->PCR[0] = PORT_PCR_MUX(1UL) | PORT_PCR_IRQC(10) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;//przerwanie przy zboczu opadajacym
	PORTA->PCR[7] = PORT_PCR_MUX(1UL) | PORT_PCR_IRQC(10) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;//przerwanie przy zboczu opadajacym
	//PORTA->PCR[10] = PORT_PCR_MUX(1UL) | PORT_PCR_IRQC(10) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;//przerwanie przy zboczu opadajacym
	PORTA->PCR[11] = PORT_PCR_MUX(1UL) | PORT_PCR_IRQC(10) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;//przerwanie przy zboczu opadajacym
	
	//inicjalizacja diod
	PORTA->PCR[5] = PORT_PCR_MUX(1UL);
	PORTA->PCR[6] = PORT_PCR_MUX(1UL);
	PORTA->PCR[8] = PORT_PCR_MUX(1UL);
	
	NVIC_ClearPendingIRQ(PORTA_IRQn);//wyczyszczenie w NVIC przerwan na PORTA
	NVIC_EnableIRQ(PORTA_IRQn);//wlaczanie zrodla przerwan NVIC dla portu A
	NVIC_SetPriority(PORTA_IRQn, 3);//ustawianie priorytetu przerwania
	
	//inicjalizacja wyswietlacza
	for (int a=0; a<=11; a++)
		PORTB->PCR[a] = PORT_PCR_MUX(1UL);

	//ustawianie pinow jako we/wy
	PTA->PDDR |= 0b1111001101111110;//wejscia i wyjscia (sa tu ledy na 5, 6, i 8)
	PTB->PDDR |= 0xfff;//wyjscia
}

#endif
