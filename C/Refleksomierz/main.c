//Refleksomierz - gra dla 2 osob
//Autorzy: Konrad Micek, Filip Swiderski

//Piny 0-6 z portu B - wyswietlacz 7-segmentowy
//Piny 7-10 z portu B - piny do wyboru wyswietlacza
//Pin 11 z portu B -  kropka wyswietlacza
//Piny z portu A 5, 6 i 8 - diody
//Piny z portu A od 0, 7, 10 i 11 - do obslugi przyciskow

#include "MKL05Z4.h"
#include <stdlib.h>//do randa
#include "extra.h"
#include "piny.h"

volatile long int wysw[]={0b101110000000, 0b110110000000, 0b111010000000, 0b111100000000};// wybor wyswietlacza
volatile long int znak[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};//tablica ze znakami dla wyswietlacza
volatile long int specjalne[] = {0b1111111, 0b0111111, 0x00, 0x40};//[0]-pusto, [1]-kreska, [2]-8, [3]-0 - sluza do animacji
#define lewy 3//lewy wyswietlacz
#define prawy 0//prawy wyswietlacz
#define dlugi 300//czas
#define krotki 5//czas
#define kropka 0b011111111111
#define przycisk1 7//gracz 1, lewa strona
#define przycisk2 11//gracz 2, prawa strona
int czas=0;//do pomiaru czasu
int gracz1=0, gracz2=0;//wyniki graczy

void jakiczas()//wyswietla czas
{
	czas=czas*2*krotki/10;//konwersja czasu
	int cyfra[4];//do zapisywania pojedynczych cyfr
	for (int a = 0; a <= 3; a++)
	{
		cyfra[a] = czas % 10;
		czas = czas / 10;
	}
	for (int a=0; a<=150; a++)
	{
		wyswietlaj(wysw[lewy], znak[cyfra[3]], krotki);
		wyswietlaj(wysw[2]&kropka, znak[cyfra[2]], krotki);
		wyswietlaj(wysw[1], znak[cyfra[1]], krotki);
		wyswietlaj(wysw[prawy], znak[cyfra[0]], krotki);
	}
}

void zwyciestwo ()//animacja zwyciestwa
{
	int kto=1;//wybiera ekran zwyciezcy
	if (gracz1==5)
		kto=lewy;
	if (gracz2==5)
		kto=prawy;
	for (int a=0; a<=4; a++)
		{
			wyswietlaj(wysw[kto], specjalne[1], dlugi);
			wyswietlaj(wysw[kto], specjalne[2], dlugi);
			wyswietlaj(wysw[kto]&kropka, specjalne[3], dlugi);//&kropka - do obslugi kropki
			wyswietlaj(wysw[kto]&kropka, specjalne[0], dlugi);
		}
		odliczanie();
	gracz1=0;//wyzerowanie wynikow
	gracz2=0;
}

void PORTA_IRQHandler()//funkcja przerwania
{  
	if (PORTA->ISFR & (1UL<<przycisk1))//sprawdzanie pinu 7 portu A
	{
		if (czas>9999)
		{
			czas=0;
		} else gracz1++;
		for (int a=0; a<=3; a++)
		{
			wyswietlaj(wysw[lewy], znak[gracz1], dlugi);
			wyswietlaj(wysw[lewy], specjalne[0], dlugi);
		}
	} else if(PORTA->ISFR & (1UL<<przycisk2))//sprawdzanie pinu 11 portu A
	{
		if (czas>9999)
		{
			czas=0;
		} else gracz2++;
		for (int a=0; a<=3; a++)
		{
			wyswietlaj(wysw[prawy], znak[gracz2], dlugi);
			wyswietlaj(wysw[prawy], specjalne[0], dlugi);
		}
	}
	jakiczas();
	czas=0;//zerowanie czasu
	if ((gracz1==5) || (gracz2==5))
		zwyciestwo();
	wyswietlaj(wysw[prawy], specjalne[0], krotki);
	odliczanie();
	PORTA->ISFR&=(1UL<<przycisk1);//wyczysc przerwanie na pinie 7
	PORTA->ISFR&=(1UL<<przycisk2);//wyczysc przerwanie na pinie 11
}  

int main(void)
{
	init();//funkcja inicjalizujaca

	while(1)//nieskonczona petla wyswietlajaca
	{
		//multipleksowanie
		czas++;
		wyswietlaj(wysw[lewy], znak[gracz1], krotki);
		wyswietlaj(wysw[prawy], znak[gracz2], krotki);
	}
	return(0);
}
