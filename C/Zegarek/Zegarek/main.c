//4 piny z B - sterowanie, 6 pinow z D + 2 piny z B - wyswietlacz
//Dp0-g,Dp1-f,Dp2-e,Dp3-d,Dp4-c,Dp5-b,Bp0-a,Bp1-dp - tak podlaczyc wyswietlacz

#define F_CPU 16000000L//do obslugi opoznien
#include <avr/io.h>
#include <util/delay.h>//do obslugi opoznien
#include <avr/interrupt.h>//do obslugi przerwan

volatile unsigned char godzina1,godzina2,minuta1,minuta2,sekunda1,sekunda2,setna1,setna2;//do petli
volatile unsigned char portchistory=0b001111;//stan poczatkowy portc
volatile unsigned char doDg1,doDg2,doDm1,doDm2;//pomocnicze do D
volatile unsigned char doBg1,doBg2,doBm1,doBm2;//pomocnicze do B
volatile unsigned char wyswg1=0b111000,wyswg2=0b110100,wyswm1=0b101100,wyswm2=0b011100;
//wyswg1-1 wyswietlacz godzin, wyswg2-2 wyswietlacz godzin, wyswm1-1 wyswietlacz minut, wyswm2-2 wyswietlacz minut

//case'y do wybrania wyswietlanego znaku na wyswietlaczu
unsigned char wyborLiczby(unsigned char cyfra)
{
	unsigned char wynik;
	switch(cyfra)
	{
		case 0:
		wynik=0x7e;
		break;
		case 1:
		wynik=0x30;
		break;
		case 2:
		wynik=0x6d;
		break;
		case 3:
		wynik=0x79;
		break;
		case 4:
		wynik=0x33;
		break;
		case 5:
		wynik=0x5b;
		break;
		case 6:
		wynik=0x5f;
		break;
		case 7:
		wynik=0x70;
		break;
		case 8:
		wynik=0x7f;
		break;
		case 9:
		wynik=0x7b;
		break;
		default:
		wynik=0x7e;//domyslnie 0
		break;
	}
	return wynik;
}

//do dzielenia bitow miedzy B i D
unsigned int wyswietlacz(unsigned char nrwysw, unsigned char cyfra)
{
	unsigned char wynik,pomoc,doD,doB;//doD - do PORTD, doB - do PORTB
	unsigned int polaczone;//polaczone - laczymy PORTD i PORTB, zeby funkcja mogla zwrocic 2 wartosci na raz
	wynik=wyborLiczby(cyfra);//wybiera liczbe
	doB=nrwysw;
	doD=(wynik<<2);//trzeba przesunac, bo to 0 i 1 sa tx i rx
	//negacje musza wystapic by swiecilo
	doD=~doD;
	pomoc=~wynik;
	pomoc=(pomoc>>6);//do portb chcemy wyslac tylko 2 bity
	doB=doB | pomoc;
	polaczone=(doB<<8);
	polaczone=polaczone+doD;
	return polaczone;
}

//dzieli z funkcji polaczone wartosci na port D i B
void wyslanieNaPort(unsigned char nrWysw)
{
	unsigned int polaczone;
	switch(nrWysw)
	{
		case 1:
		polaczone=wyswietlacz(wyswg1,godzina1);
		doDg1=polaczone & 0xff;
		doBg1=(polaczone>>8) & 0xff;
		break;
		case 2:
		polaczone=wyswietlacz(wyswg2,godzina2);
		doDg2=polaczone & 0xff;
		doBg2=(polaczone>>8) & 0xff;
		break;
		case 3:
		polaczone=wyswietlacz(wyswm1,minuta1);
		doDm1=polaczone & 0xff;
		doBm1=(polaczone>>8) & 0xff;
		break;
		case 4:
		polaczone=wyswietlacz(wyswm2,minuta2);
		doDm2=polaczone & 0xff;
		doBm2=(polaczone>>8) & 0xff;
		break;
		case 5:
		polaczone=wyswietlacz(wyswg1,sekunda1);
		doDg1=polaczone & 0xff;
		doBg1=(polaczone>>8) & 0xff;
		break;
		case 6:
		polaczone=wyswietlacz(wyswg2,sekunda2);
		doDg2=polaczone & 0xff;
		doBg2=(polaczone>>8) & 0xff;
		break;
		case 7:
		polaczone=wyswietlacz(wyswm1,setna1);
		doDm1=polaczone & 0xff;
		doBm1=(polaczone>>8) & 0xff;
		break;
		case 8:
		polaczone=wyswietlacz(wyswm2,setna2);
		doDm2=polaczone & 0xff;
		doBm2=(polaczone>>8) & 0xff;
		break;
	}
	return;
}

//wyswietlaj - wysyla 6 bitow do D i 2 do B
void wyswietlaj()
{
	PORTB=doBg1;
	PORTD=doDg1;
	_delay_ms(2);
	PORTB=doBg2;
	PORTD=doDg2;
	if (setna1>=5)
	PORTB&=0b111101;//migajaca kropka
	_delay_ms(3);
	PORTB=doBm1;
	PORTD=doDm1;
	_delay_ms(2);
	PORTB=doBm2;
	PORTD=doDm2;
	_delay_ms(3);
	return;
}

//to co wyswietlaj, ale bez migajacej kropki i z lekkim opoznieniem aby za szybko nie zmienialo liczb
void wyswietlajZmiana()
{
	for (unsigned char i=1;i<=10;i++)
	{
		PORTB=doBg1;
		PORTD=doDg1;
		_delay_ms(2);
		PORTB=doBg2;
		PORTD=doDg2;
		PORTB&=0b111101;//kropka
		_delay_ms(3);
		PORTB=doBm1;
		PORTD=doDm1;
		_delay_ms(2);
		PORTB=doBm2;
		PORTD=doDm2;
		_delay_ms(3);
	}
	return;
}

//glowna funkcja
int main(void)
{
	unsigned char wybor=0;//domyslnie pokazuje godziny i minuty
	DDRB=0xff;//wyjscie
	DDRD=0xff;//wyjscie
	DDRC=0b110000;//wejscie (przyciski)
	PORTC=0b001111;//pull-upy
	PCICR|=(1<<PCIE1);// set PCIE0 to enable PCMSK0 scan
	PCMSK1|=(1<<PCINT11);// set PCINT0 to trigger an interrupt on state change
	sei();//wlaczenie przerwan
	while(1)
	{//petle do liczenia czasu
		for (godzina1=0;godzina1<3;godzina1++)
		{
			for (godzina2=0;godzina2<10;godzina2++)
			{
				if (godzina1==2 && godzina2==4)//zeby nie bylo godziny 24, lecz 0
				{
					godzina1=0;
					godzina2=0;
					wyslanieNaPort(1);
					wyslanieNaPort(2);
				}
				
				for (minuta1=0;minuta1<6;minuta1++)
				{
					for (minuta2=0;minuta2<10;minuta2++)
					{
						for (sekunda1=0;sekunda1<=5;sekunda1++)
						{
							for (sekunda2=0;sekunda2<=9;sekunda2++)
							{
								for (setna1=0;setna1<=9;setna1++)
								{
									for (setna2=0;setna2<=9;setna2++)
									{
										if (((PINC & (1<<PINC2))==0) || (wybor==1))
										{
											wyslanieNaPort(5);
											wyslanieNaPort(6);
											wyslanieNaPort(7);
											wyslanieNaPort(8);
											wybor=1;
											PORTC|=0b010000;//zapalenie diody
										}
										if (((PINC & (1<<PINC1))==0) || (wybor==0))
										{
											wyslanieNaPort(1);
											wyslanieNaPort(2);
											wyslanieNaPort(3);
											wyslanieNaPort(4);
											wybor=0;
											PORTC&=0b101111;//zgaszenie diody
										}
										wyswietlaj();
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return(0);
}

//przerwanie do ustawiania godziny
ISR (PCINT1_vect)//PCINT1 jest dla C
{
	//do obslugi przerwan na konkretnym pinie
	unsigned char changedbits;
	changedbits=PINC ^ portchistory;
	portchistory=PINC;

	if ((changedbits & (1<<PINC0))==1)
	{
		PORTC|=0b010000;//zapalenie diody
		wyslanieNaPort(1);//zmiana na godziny i minuty w przypadku obserwowania sekund
		wyslanieNaPort(2);
		wyslanieNaPort(3);
		wyslanieNaPort(4);
		while(1)
		{
			wyswietlajZmiana();//wyswietla na ekranie
			if ((PINC & (1<<PINC1))==0)//zmienianie godziny
			{
				if (godzina2!=9)
				godzina2++;
				else
				{
					godzina2=0;
					godzina1++;
				}
			}
			if ((PINC & (1<<PINC2))==0)//zmienianie minut
			{
				if (minuta2!=9)
				minuta2++;
				else
				{
					minuta2=0;
					minuta1++;
				}
				if ((minuta1==6) && (minuta2==0))
				{
					minuta1=0;
					minuta2=0;
				}
			}
			if (((PINC & (1<<PINC2))==0) || ((PINC & (1<<PINC1))==0))//zmiana wartosci na ekranie
			{
				if ((godzina1==2) && (godzina2==4))
				{
					godzina1=0;
					godzina2=0;
				}
				wyslanieNaPort(1);
				wyslanieNaPort(2);
				wyslanieNaPort(3);
				wyslanieNaPort(4);
			}
			if ((PINC & (1<<PINC3))==0)//wyjscie z ustawiania godziny
			{
				PORTC&=0b101111;//zgaszenie diody
				break;
			}
		}
	}
}