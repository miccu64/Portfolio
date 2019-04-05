#include <iostream>
#include <fstream>
using namespace std;
ofstream wynik("wyniki_anagramy.txt");
string sortowanie (string napis)
{
    int dlugosc=napis.length();
    for (int a=0;a<dlugosc;a++)
        for (int b=1;b<dlugosc-a;b++)
        {
            if (napis[b]<napis[b-1])
                swap(napis[b],napis[b-1]);
        }
    return napis;
}
void wczytajDo3 ()
{
    int pomocnicza=0, k=0;
    string tablica[2000];
    ifstream plik;
    plik.open("dane_napisy.txt");
    for (int a=0;a<1000;a++)
    {
        plik>>tablica[a]>>tablica[a+1000];
    }
    for (int b=0;b<2000;b++)
    {
        tablica[b]=sortowanie(tablica[b]);
    }
    for (int c=0;c<2000;c++)
    {
        pomocnicza=0;
        int dlugosc1=tablica[c].length();
        for (int d=c;d<2000;d++)
        {
            int dlugosc2=tablica[d].length();
            if (dlugosc1==dlugosc2)
            {
                int f=0;
                for (int e=0;e<dlugosc1;e++)
                {
                    char litera1=tablica[c][e];
                    char litera2=tablica[d][e];
                    if (litera1!=litera2)
                        break;
                    f=e;
                }
                if (f==(dlugosc1-1))
                    pomocnicza++;
                if (pomocnicza>k)
                    k=pomocnicza;
            }
        }
    }
    wynik<<"3"<<endl<<k;
}
bool anagramDo1 (string nazwaPliku)
{
    string napis1, napis2;
    int ilosc=0, c;
    ifstream plik;
    plik.open(nazwaPliku.c_str());
    if (!plik.good())
        return false;
    for (int a=0;a<1000;a++)
    {
        plik>>napis1>>napis2;
        int dlugosc1=napis1.length();
        int dlugosc2=napis2.length();
        if (dlugosc1==dlugosc2)
        {
            char litera=napis1[0];
            for (int b=0;b<dlugosc1;b++)
            {
                if (litera!=napis1[b]||litera!=napis2[b])
                {
                    break;
                } else c=b;
            }
            if (c==(dlugosc1-1))
                ilosc++;
        }
    }
    wynik<<"1"<<endl<<ilosc<<endl<<endl;
    return true;
}
void anagramDo2 ()
{
    string napis1, napis2;
    ifstream plik;
    plik.open("dane_napisy.txt");
    int ilosc=0;
    for (int a=0;a<1000;a++)
    {
        plik>>napis1>>napis2;
        int dlugosc1=napis1.length();
        int dlugosc2=napis2.length();
        if (dlugosc1==dlugosc2)
        {
            int c=-1;
            napis1=sortowanie(napis1);
            napis2=sortowanie(napis2);
            for (int b=0;b<dlugosc1;b++)
            {
                if (napis1[b]!=napis2[b])
                    break;
                else c=b;
            }
            if (c==dlugosc1-1)
                ilosc++;
        }
    }
    wynik<<"2"<<endl<<ilosc<<endl<<endl;
    return;
}
int main()
{
    if (!anagramDo1("dane_napisy.txt"))
        cout<<"Zle"<<endl;
    else cout<<"Okej"<<endl;
    anagramDo2();
    wczytajDo3();
    wynik.close();
    return 0;
}
