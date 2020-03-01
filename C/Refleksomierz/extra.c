#include "extra.h"

void opoznij (uint32_t wart) //opoznienie
{
	uint32_t opoz, x;
	for(x=0; x < wart; x++)
		for(opoz=0; opoz < 7192; opoz++){};
}

void odliczanie()//odlicza czas przed kolejna runda, zapala diody
{
	int dlugosc=0;
	for (int a=5; a<=8; a++)
	{
		if (a==7)
			continue;
		dlugosc =200+rand()%1500;
		PTA->PDOR |= (1UL << a);
		opoznij(dlugosc);
	}
	PTA->PDOR=0x00;//usuniecie diod
}

void wyswietlaj (long int wyswietl, long int symbol, int czas)//funkcja do wyswietlania
{
	PTB->PDOR = wyswietl+symbol;
	opoznij(czas);
}
