#include <vector>
#include <iostream>
#include <io.h>
#include <ShlObj.h>
#include <ctime>
#include <sys/stat.h>

//Jest to program realizujacy funkcje menedzera aplikacji.
//Pozwala wyszukac zainstalowane aplikacje (pokazuje foldery z Program files 32&64bit
//i traktuje kazdy folder jako program, jesli odnajdzie tam jakikolwiek plik .exe).
//Po wybraniu programu pokazuje date instalacji, rozmiar, pozwala na otworzenie folderu programu oraz odinstalowanie go
//pod warunkiem, ze zostanie odnaleziony plik zawierajacy fraze *nins* i bedacy plikiem .exe.

using namespace std;

string wynikOdinstalowywanie;

struct program
{
    string nazwa;
    int nadfolder;
};

vector <string> folder;

string wyszukajOdinst(char sciezka[])
{
    _finddata_t data;
    intptr_t uchwyt=_findfirst(sciezka, & data);
    if(uchwyt==-1)
        return "bbb";
    _findclose(uchwyt);
    return data.name;
}

void wyszukajFoldery(char sciezka[])
{
    folder.clear();
    _finddata_t data;
    intptr_t uchwyt=_findfirst(sciezka, & data);
    //intptr_t - dla przenosnosci 32/64 bit
    if(uchwyt==-1)
        return;

    folder.push_back(data.name);
    int find=_findnext(uchwyt, & data);
    while(find!=-1)
    {
        folder.push_back(data.name);
        //if (data.attrib==16n),
        find=_findnext(uchwyt, & data);
        //  cout<<data.attrib<<endl<<data.name<<endl;
    }
    _findclose(uchwyt);
    return;
}

bool szukajOdisntalatora(string & tekst, string szukanaFraza)
{
    size_t znalezionaPozycja = tekst.find(szukanaFraza);
    if(znalezionaPozycja==std::string::npos)
        return false;
    else return true;
}

void sciezkaOdinstalowywania(const wstring &folder)
{
    wstring pomocnicze=folder+L"\\*";
    WIN32_FIND_DATAW plik;
    HANDLE uchwyt=FindFirstFileW(pomocnicze.c_str(), &plik);
    if (uchwyt!=INVALID_HANDLE_VALUE)
    {
        vector<wstring>directories;
        do
        {
            if(plik.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if((!lstrcmpW(plik.cFileName, L"."))||(!lstrcmpW(plik.cFileName, L"..")))
                    continue;
            }
            pomocnicze=folder+L"\\"+wstring(plik.cFileName);
            string sciezka(pomocnicze.begin(), pomocnicze.end());

            if (plik.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                directories.push_back(pomocnicze);

            if (szukajOdisntalatora(sciezka,".exe")==true)
                if (szukajOdisntalatora(sciezka,"nins")==true)
                    wynikOdinstalowywanie=sciezka;
        }
        while (FindNextFileW(uchwyt, &plik));
        FindClose(uchwyt);
        for(vector<wstring>::iterator iter=directories.begin(), end=directories.end(); iter!=end; ++iter)
            sciezkaOdinstalowywania(*iter);
    }
    return;
}

void odinstalujProgram (char sumaOdinst[], program aplikacja[])
{
    string pomocnicza1=sumaOdinst;
    wstring katalog;
    katalog.assign(pomocnicza1.begin(), pomocnicza1.end());
    sciezkaOdinstalowywania(katalog);
    cout<<wynikOdinstalowywanie<<endl;
    if (wynikOdinstalowywanie.empty())
        {
            cout<<"Nie mozna odinstalowac programu!"<<endl;
            return;
        }
    const char * konwersja=wynikOdinstalowywanie.c_str();
    ShellExecuteA(0, 0 ,konwersja,0,0,SW_SHOW);
    return;
}

long long int rozmiarFolderu(string sciezka)
{
    WIN32_FIND_DATA data;
    long long int rozmiar=0;
    string sciezkiWewnatrz=sciezka+"\\*.*";
    HANDLE uchwyt=FindFirstFile(sciezkiWewnatrz.c_str(),&data);
    if(uchwyt!=INVALID_HANDLE_VALUE)
    {
        do {
            if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //pomijamy "." and ".."
                //strcmp sprawdza, czy od "." nie zaczyna sie nazwa folderu
                if(strcmp(data.cFileName,".")!=0&&strcmp(data.cFileName,"..")!= 0)
                {
                    //sprawdzamy pliki w podfolderze
                    sciezkiWewnatrz=sciezka+"\\"+data.cFileName;
                    //rekurencja
                    rozmiar+=rozmiarFolderu(sciezkiWewnatrz);
                }
            } else
            {
                LARGE_INTEGER rozmiar2;
                //dla duzych plikow rozmiar to 2 obiekty DWORD - laczymy je wiec w 1 wielkosc
                rozmiar2.LowPart=data.nFileSizeLow;
                rozmiar2.HighPart=data.nFileSizeHigh;
                rozmiar+=rozmiar2.QuadPart;
            }
        } while(FindNextFile(uchwyt,&data)!=0);
        FindClose(uchwyt);
    }
    return rozmiar;
}

void dataInstalacji (char sciezka[])
{
    char czas[MAX_PATH];
    struct stat b;
    if(!stat(sciezka,& b))
    {
        strftime(czas, 100, "%d/%m/%Y %H:%M:%S", localtime(& b.st_ctime));
        cout<<czas<<endl;
    }
    else cout<<"Blad!"<<endl;
    return;
}
void wyborAplikacji (char folderSystemowy1[], char folderSystemowy2[], program aplikacja[], int iloscAplikacji)
{
    wynikOdinstalowywanie.clear();
    int wybranaLiczba;
    while ((wybranaLiczba<1)||(wybranaLiczba>iloscAplikacji))
    {
        cout<<endl<<"Podaj numer programu (numery od 1 do "<<iloscAplikacji<<"): "<<endl;
        cin>>wybranaLiczba;
        if (cin.fail())
        {
            cout<<"To nie jest liczba!"<<endl;
            cin.clear();
            cin.sync();
            continue;
        }
        if ((wybranaLiczba<1)||(wybranaLiczba>iloscAplikacji))
            cout<<"Liczba spoza przedzialu!"<<endl;
    }
    system("cls");

    char suma[MAX_PATH];
    //zerowanie tablic w celu unikniecia losowych bledow
    for (int a=0; a<260; a++)
        suma[a]=0;
    if (aplikacja[wybranaLiczba].nadfolder==1)
        strcat (suma, folderSystemowy1);
    else strcat (suma, folderSystemowy2);
    const char * konwersja=(aplikacja[wybranaLiczba].nazwa).c_str();
    strcat (suma, konwersja);
    long long int rozmiar=0;
    rozmiar=rozmiarFolderu(suma);
    rozmiar=rozmiar/1048576;
    cout<<"Nazwa aplikacji: "<<konwersja<<endl;
    cout<<"Sciezka aplikacji: "<<suma<<endl;
    cout<<"Rozmiar: "<<rozmiar<<" MB"<<endl;
    cout<<"Data instalacji: ";
    dataInstalacji(suma);
    cout<<"1) Otworz folder aplikacji"<<endl;
    cout<<"2) Odinstaluj aplikacje"<<endl;
    cout<<"0) Powrot"<<endl;
    int wybranaLiczba2;
    while (1)
    {
        cout<<"Wybierz czynnosc (wpisz liczbe z zakresu od 0 do 2: ";
        cin>>wybranaLiczba2;
        if (cin.fail())
        {
            cout<<"To nie jest liczba!"<<endl;
            cin.clear();
            cin.sync();
            continue;
        }
        if (wybranaLiczba2<0||wybranaLiczba2>2)
            cout<<"Liczba spoza przedzialu!"<<endl;
        if (wybranaLiczba2==2)
            odinstalujProgram(suma, aplikacja);
        if (wybranaLiczba2==1)
            ShellExecute( 0, "open", suma, NULL, NULL, SW_SHOW );
        if (wybranaLiczba2==0)
        {
            system("cls");
            return;
        }
    }
    return;
}

void wyswietlAplikacje (char sciezkax86[], char sciezkax64[], char poprawneFilesx64[], char filesx86[], program aplikacja[])
{
    //wyswietlanie listy appek
    wyszukajFoldery(sciezkax86);
    int k=folder.size()-1;
    for(int i=1; i<k; i++)
    {
        cout<<i<<") "<<folder[i+1]<<endl;
        aplikacja[i].nazwa=folder[i+1];
        aplikacja[i].nadfolder=1;
    }
    wyszukajFoldery(sciezkax64);
    int m=folder.size()-1;
    for(int j=1; j<m; j++)
    {
        cout<<j+k-1<<") "<<folder[j+1]<<endl;
        aplikacja[j+k-1].nazwa=folder[j+1];
        aplikacja[j+k-1].nadfolder=2;
    }

    int iloscAplikacji=k+m-2;
    wyborAplikacji(filesx86, poprawneFilesx64, aplikacja, iloscAplikacji);
    wyswietlAplikacje(sciezkax86, sciezkax64, poprawneFilesx64, filesx86, aplikacja);
    return;
}

int main()
{
    char filesx64[MAX_PATH];
    char filesx86[MAX_PATH];
    char sciezkax86[MAX_PATH];
    char sciezkax64[MAX_PATH];
    char poprawneFilesx64[MAX_PATH];
    //zerowanie tablic w celu unikniecia losowych bledow
    for (int b=0; b<260; b++)
    {
        filesx64[b]=0;
        filesx86[b]=0;
        sciezkax86[b]=0;
        sciezkax64[b]=0;
        poprawneFilesx64[b]=0;
    }

    //dynamiczne sciezki do Program files
    SHGetFolderPathA(0, CSIDL_PROGRAM_FILES, 0, 0, filesx64);
    SHGetFolderPathA(0, CSIDL_PROGRAM_FILESX86, 0, 0, filesx86);
    program aplikacja [200];

    strcat(sciezkax86,filesx86);
    strcat(sciezkax86,"\\*");

    strncat(sciezkax64,filesx64,16);
    strcat(sciezkax64,"\\*");

    strcat(filesx86,"\\");
    //poprawne - ze wzgledu na przypisywanie przez kod do 64 bit sciezki z 86 bit (dlaczego?)
    strncat(poprawneFilesx64,filesx64,16);
    strcat(poprawneFilesx64,"\\");

    wyswietlAplikacje(sciezkax86, sciezkax64, poprawneFilesx64, filesx86, aplikacja);
    return(0);
}
