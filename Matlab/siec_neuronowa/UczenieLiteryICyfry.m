function[]=UczenieLiteryICyfry(piksele,piksele2,DoSciezki)

disp('Rozpoczynam uczenie...')
%Ilosc wejsc i wyjsc
wejscia=piksele2*piksele;
wyjscia=36;

%Reset wag - losujemy
Wagi=wagipoczatkowe(wejscia,wyjscia);

%Wyzerowanie wartosci
MSE=0;
CE=0;
odrzucone=0;
%Wspolczynnik uczenia
wsp=0.1; 
beta=1;
%Ilosc iteracji
iloscpowtorzen=2000000;

for b=100:iloscpowtorzen
    %Co mod(b,100) losuje nowa litere (dla szybkosci)
    if mod(b,100)==0
        %Okresla litere z jej samplami i podaje sciezke (numer ASCII)
        while 1
            ascii=randi(43);
            if ascii<=10 || ascii>=18
                break
            end
        end   
        
        if ascii>=18
            liczba=ascii-7;
        else
            liczba=ascii;
        end
        ascii=ascii+47;
        Sciezka=fullfile(DoSciezki,int2str(ascii));
        Pliki=dir(Sciezka);
        iloscplikow=size(Pliki,1)-2;

        %Czego sie uczy
        ZadaneWyjscie=zeros(wyjscia,1);
        ZadaneWyjscie(liczba)=1;
        
        %Obliczanie bledow
            if mod(b,1000)==0
                %MSE - blad sredniokwadratowy
                MSE=0.5*MSE/(1000-odrzucone)/wyjscia
                %CE - bledne klasyfikacje (w %)
                CE=CE/(1000-odrzucone)*100
                MSE=0;
                CE=0;
                odrzucone=0;
                b
            end
    end
    
    %Losowanie sampla
    %+2, zeby pominac . i ..
    NazwaPliku=Pliki(randi(iloscplikow)+2).name;
    SciezkaPliku=fullfile(Sciezka,NazwaPliku);
    Wejscie=wczytajobraz(SciezkaPliku,piksele2,piksele);

    %Gdy algorytm dzieli pojedyncza litere na czesci pomijamy ja
    if size(Wejscie,2)~=1
        odrzucone=odrzucone+1;
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
    %Bledy
    MSE=MSE+(Bledy.'*Bledy);
    if (any(abs(Bledy)>0.5))
        CE=CE+1;
    end
end

%Transpozycja, aby poprawnie zapisaly sie dane
Wagi=Wagi';
%Zapisanie wag do pliku
filename='WagiLiteryICyfry.txt';
fileID=fopen(filename,'w');
fprintf(fileID,'%6s\r\n','Znaki (po kolei jak w ASCII)');
fprintf(fileID,'%19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f %19.20f\r\n',Wagi);
fclose(fileID);
end