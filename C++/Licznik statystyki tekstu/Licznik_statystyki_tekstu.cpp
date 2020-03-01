////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////LICZNIK STATYSTYKI TEKSTU///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>//potrzebna do odbslugi polskich znakow
#include <string>
#include <iostream>
#include <io.h>
#include <vector>
#include <fstream>
#include <filesystem>//do obslugi sciezki z polskimi znakami podcas otwierania plikow

using std::cout;
using std::wcout;
using std::endl;
using std::cin;
using std::wcin;
using std::vector;
using std::string;
using std::wstring;
using std::wifstream;
using std::ios;

//////////////////////////////
//Klasa zwiazana z folderami//
//////////////////////////////
class Foldery
{
private:
	wstring wyklucz1;//wyklucza sciezke "." podczas wyszukiwania folderow
	wstring wyklucz2;//wyklucza sciezke ".." podczas wyszukiwania folderow
protected:
	void znajdzFoldery(wstring sciezkaString);//funkcja wyszukujaca podfoldery
	vector <wstring> lokacje;//zawiera znalezione foldery do przeszukania
public:
	Foldery();//konstruktor
};

Foldery::Foldery()
	: wyklucz1(L"."),
	wyklucz2(L"..")
{
	lokacje.clear();//wyczyszczenie lokacji
}

void Foldery::znajdzFoldery(wstring sciezkaString)
{
	wstring podfolder;
	int znajdz = {};
	const int dlugosc = 260;
	wchar_t sciezka[dlugosc];//do konwersji wstringa na wchar_t* (260 to dlugosc MAX_PATH)
	for (int a = 0; a < dlugosc; a++)//zerowanie, aby uniknac bledow
		sciezka[a] = {};
	sciezkaString.copy(sciezka, sciezkaString.size() + 1);//zamiana wstringa na wchar_t* (potrzebne do funkcji wyszukujacej)

	_wfinddata_t data;//do wyszukiwania
	intptr_t uchwyt = _wfindfirst(sciezka, &data);//intptr_t - dla przenosnosci 32/64 bit - znajduje pierwszy folder
	if (uchwyt == -1)
		return;

	while (znajdz != -1)
	{
		znajdz = _wfindnext(uchwyt, &data);//do wyszukiwania kolejnych folderow
		if (znajdz == -1)//w celu pominiecia duplikatow przy koncu wywolania wyszukiwania
			break;
		//pomijanie . i .. funkcja wcscmp oraz sprawdzenie, czy jest to podfolder
		if ((data.attrib & _A_SUBDIR) && (wcscmp(data.name, wyklucz1.c_str()) != 0) && (wcscmp(data.name, wyklucz2.c_str()) != 0))
		{
			podfolder = sciezkaString.substr(0, sciezkaString.size() - 1);//obciecie "\\*"
			podfolder += data.name;//dodanie nazwy znalezionego folderu
			lokacje.push_back(podfolder);//zapis sciezki folderu
			podfolder += (L"\\*");//potrzebne do sciezki w celu przeszukania podfolderu
			znajdzFoldery(podfolder);//rekurencja w celu wyszukiwania podfolderow
		}
	}
	_findclose(uchwyt);//zwalnianie zasobow
	return;
}
//////////////////////////////////
//Klasa zwiazana ze statystykami//
//////////////////////////////////
class Statystyki : private Foldery//dziedziczenie z klasy Foldery
{
private:
	void znajdzPliki(wstring sciezka, wstring rozsz);//funkcja szukajaca pliki z zadanym rozszerzeniem
	void otworzPlik(wstring sciezka);//funkcja otwierajaca znalezione pliki i zliczajaca statystyki
public:
	long long int iloscZnakow;
	long long int iloscWyrazow;
	long long int iloscLinii;
	long long int iloscPlikow;
	long long int iloscFolderow;
	Statystyki();//konstruktor
	void start();//funkcja startowa, uruchamiajaca program
	void wyniki();//funkcja wyswietlajaca otrzymane wyniki
	bool zapytaj();//funkcja do kontynuowania lub zakonczenia programu
};

Statystyki::Statystyki()
	: iloscZnakow(0),
	iloscWyrazow(0),
	iloscLinii(0),
	iloscPlikow(0),
	iloscFolderow(0) {}

bool Statystyki::zapytaj()
{
	wcout << endl << endl << L"Czy chcesz utworzyć statystykę dla innego folderu lub innego rozszerzenia?" << endl;
	wcout << "(1 - tak, inny znak - wyjdz z programu) : ";
	int wybor = {};
	wcin >> wybor;
	wcin.ignore();
	if (wybor == 1)
	{
		wcout << L"Czy chcesz wykonać czystą statystykę, czy dodać wyniki do obecnej?" << endl;
		wcout << L"(1 - czysta, 2 - dodaj, inny znak - wyjdź z programu) : ";
		wcin >> wybor;
		wcin.ignore();
		if (wybor == 1)//wyczyszczenie statystyk
		{
			iloscFolderow = 0;
			iloscLinii = 0;
			iloscPlikow = 0;
			iloscWyrazow = 0;
			iloscZnakow = 0;
			lokacje.clear();
			return 1;
		}
		else if (wybor == 2)
		{
			lokacje.clear();//wyczyszczenie lokacji folderow, aby nie robic 2gi raz tej samej statystyki
			return 1;
		}
		else return 0;
	}
	else return 0;
}

void Statystyki::wyniki()
{
	wcout << L"Wyniki dla podanej scieżki i rozszerzenia: " << endl << endl;
	wcout << L"Ilość przeszukanych folderów: " << iloscFolderow << endl;
	wcout << L"Ilość plików z zadanym rozszerzeniem: " << iloscPlikow << endl;
	wcout << L"Ilość linii: " << iloscLinii << endl;
	wcout << L"Ilość wyrazów: " << iloscWyrazow << endl;
	wcout << L"Ilość liter: " << iloscZnakow << endl;
	return;
}

void Statystyki::start()
{
	wstring sciezka;
	wcout << endl << L"Podaj ścieżkę (przykład: C:\\Folder\\folder2) : ";
	wcin.sync();//czasem automatycznie pomija wcin - z ta funkcja dziala bez zarzutu
	wcin >> sciezka;
	lokacje.push_back(sciezka);//dodanie katalogu glownego do lokacji

	wcout << L"Podaj rozszerzenie (przykład: txt): ";
	wstring rozsz = L"\\*.";
	wstring pomoc;
	wcin.sync();
	wcin >> pomoc;
	rozsz += pomoc;

	sciezka += L"\\*";
	znajdzFoldery(sciezka);//wyszukiwanie folderow i podfolderow

	int dlugosc = lokacje.size();
	for (int b = 0; b < dlugosc; b++)
		znajdzPliki(lokacje[b], rozsz);
	iloscFolderow += lokacje.size();//zlicza ilosc folderow
	return;
}

void Statystyki::otworzPlik(wstring sciezka)
{
	wifstream plik;//do otwierania pliku
	wstring linia;//do wczytywania linii
	int iloscLiter = {};
	std::filesystem::path p = sciezka;//zamiana na path z wstring, gdyz .open nie obsluguje wstring w GCC
	plik.open(p, ios::in);//otwarcie pliku
	if (plik.good())//sprawdzanie, czy plik poprawnie otwarto
	{
		while (!plik.eof())//petla do konca pliku
		{
			getline(plik, linia);//wczytywanie linii
			iloscLinii++;
			if (!linia.empty())//sprawdzanie, czy linia nie jest pusta
			{
				iloscLiter = linia.length();
				iloscWyrazow++;//trzeba dodac 1 wyraz w kazdej linii, gdyz np. 5 slow ma 4 przerwy miedzy nimi
				for (int i = 0; i < iloscLiter; i++)
				{
					if ((linia[i] == ' ' && linia[i] != '\0') || (linia[i] == '	' && linia[i] != '\0'))//sprawdza, czy znak to tab lub spacja
					{
						//sprawdzenie kolejnego znaku, czy nie jest tab lub spacja oraz sprawdzanie, zeby nie wyjsc poza wektor
						if ((i < iloscLiter - 1) && (linia[i + 1] != ' ') && (linia[i + 1] != '	') && (i != 0))
						{
							iloscWyrazow++;
						}
					}
				}
				iloscZnakow += iloscLiter;
			}
		}
		iloscPlikow++;
		plik.close();
	}
	return;
}

void Statystyki::znajdzPliki(wstring sciezkaString, wstring rozsz)
{
	wstring plik;
	int znajdz = {};
	const int dlugosc = 260;
	wchar_t sciezka[dlugosc];//do konwersji wstring na wchar_t*
	for (int a = 0; a < dlugosc; a++)//zerowanie, aby uniknac bledow
		sciezka[a] = {};
	wstring sciezkaString2 = sciezkaString;
	sciezkaString2 += (L"\\");
	sciezkaString = sciezkaString + rozsz;
	sciezkaString.copy(sciezka, sciezkaString.size() + 1);//zamiana wstring na wchar_t*
	_wfinddata_t data;
	intptr_t uchwyt = _wfindfirst(sciezka, &data);
	if (uchwyt == -1)
		return;

	while (znajdz != -1)
	{
		plik = sciezkaString2;
		plik += data.name;
		//wcout << plik << endl;//TU MOZNA WYSIETLAC ZNALEZIONE PLIKI
		otworzPlik(plik);//otwieranie pliku
		znajdz = _wfindnext(uchwyt, &data);
		if (znajdz == -1)//w celu pominiecia duplikatow przy koncu wywolania wyszukiwania
			break;
	}
	_findclose(uchwyt);
	return;
}

int main()
{
	setlocale(LC_ALL, "");//umozliwia uzywanie polskich znakow w wstringu
	_setmode(_fileno(stdin), _O_U16TEXT);//umozliwia uzywanie polskich znakow w wstringu
	//pomimo, ze Visual Studio twierdzi, ze ignoruje powyzsza linijke, bez niej nie dzialaja w pelni polskie znaki
	Foldery fold;
	Statystyki stat;

	wcout << endl << L"LICZNIK STATYSTYKI TEKSTU" << endl;
	wcout << L"Zadaniem programu jest przeprowadzenie statystyki plików z określonym rozszerzeniem" << endl;
	wcout << L"z danego folderu i jego podfolderów" << endl;
	while (1)
	{
		stat.start();//funkcja inicjujaca program
		stat.wyniki();//funkcja wyswietlajaca wyniki
		if (stat.zapytaj() == 0)
			break;
	}
	return(0);
}
