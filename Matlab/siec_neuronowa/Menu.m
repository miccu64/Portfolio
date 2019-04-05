clc; clear; close all;

%Wczytanie wag
filename='WagiLiteryICyfry.txt';
delimiterIn=' ';
headerlinesIn=1;
Wagi=importdata(filename,delimiterIn,headerlinesIn);
Wagi=Wagi.data;

%Zmienne
piksele=35;
piksele2=50;
DoSciezki='test_zbioruczacy';

while 1
    disp('Wybierz jedna z podanych opcji: ')
    disp('1) Odczytanie liter i cyfr z podanego obrazu')
    disp('2) Sprawdzanie dzialania w zbiorze uczacym')
    disp('3) Test ze zbioru uczacego')
    disp('4) Test spoza zbioru uczacego')
    disp('9) Nauka')
    disp('0) Wyjdz')
    wybor=input('Wybierz opcje: ');
    switch wybor
        case 9
        %Nauka liter i cyfr
        %Uczy, ale na zbiorze testowym (sluzy jedynie do pokazania dzialania)
        %Wagi sa skopiowane na wszelki wypadek w folderze 'inne'
        UczenieLiteryICyfry(piksele,piksele2,DoSciezki);

        case 1
        %Odczytanie liter i cyfr z dowolnego obrazu
        Sciezka='zdj.bmp';
        Odczytane=OdczytanieZDowolnegoObrazu(Wagi,piksele,piksele2,Sciezka);
        Odczytane=char(Odczytane)

        case 2
        %Sprawdzanie dzialania w zbiorze uczacym na wszystkich znakach
        %Kolejne znaki po wciskaniu entera
        RozpoznawanieLiterICyfr(Wagi,piksele,piksele2,DoSciezki)

        case 3
        %Test ze zbioru uczacego
        DoSciezki2='test_zbioruczacy';
        Test(Wagi,piksele,piksele2,DoSciezki2)
        
        case 4
        %Test spoza zbioru uczacego
        DoSciezki3='test_spozazbioru';
        Test(Wagi,piksele,piksele2,DoSciezki3)

        case 0
        break

        otherwise
        disp('Niewlasciwa cyfra!')
    end
end