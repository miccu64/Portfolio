clc; clear; close all;

%Ilosc rozpoznawanych znakow
wyjscia=2;

%Reset wag - losujemy
Wagi=wagipoczatkowe(1750,wyjscia);

MSE=0;
CE=0;
odrzucone=0;
%Wspolczynnik uczenia
wsp=0.1; 
beta=1;
%Ilosc iteracji
iloscpowtorzen=100000;
%Folder
DoSciezki='do_nauki';

for b=100:iloscpowtorzen
    %Co mod(b,100) losuje nowa litere (dla szybkosci)
    if mod(b,100)==0
        %Okresla litere z jej samplami i podaje sciezke
        liczba=randi(wyjscia);
        Sciezka=fullfile(DoSciezki,int2str(liczba));
        Pliki=dir(Sciezka);

        %Czego sie uczy
        ZadaneWyjscie=zeros(wyjscia,1);
        ZadaneWyjscie(liczba)=1;
            if mod(b,1000)==0
                MSE=0.5*MSE/(1000-odrzucone)/wyjscia
                CE=CE/(1000-odrzucone)*100
                MSE=0;
                CE=0;
                odrzucone=0;
            end
    end
    
    %Losowanie sampla
    %+2, zeby pominac . i .. - Petla dla wszystkich plikow danego znaku
    NazwaPliku=Pliki(randi(1014)+2).name;
    SciezkaPliku=fullfile(Sciezka,NazwaPliku);
    Wejscie=wczytajobraz(SciezkaPliku);

    %Gdy algorytm dzieli pojedyncza litere na czesci pomijamy ja
    if size(Wejscie,2)~=1
        odrzucone=odrzucone+1
        b
        continue
    end
    
    Wyjscie=obliczwyjscie(Wagi,Wejscie);
    Bledy=ZadaneWyjscie-Wyjscie;
    Bledy2=beta*Bledy.*Wyjscie.*(1-Wyjscie);
    Wejscie=[-1 ; Wejscie];
    %Poprawki wag
    Poprawki=wsp*Wejscie*Bledy2';
    %Dodanie wag
    Wagi=Wagi+Poprawki;
    
    MSE=MSE+(Bledy.'*Bledy);
    if (any(abs(Bledy)>0.5))
        CE=CE+1;
    end
end

%Transpozycja, aby poprawnie zapisaly sie dane
Wagi=Wagi';

%Zapisanie wag do pliku
filename='Wagi.txt';
fileID=fopen(filename,'w');
fprintf(fileID,'%6s\r\n','Litera');
fprintf(fileID,'%19.20f %19.20f\r\n',Wagi);
fclose(fileID);