function[]=Test(Wagi,piksele,piksele2,DoSciezki)

%TRZEBA WRZUCIC DANE TESTOWE Z ZIPA DO ODPOWIEDNICH FOLDEROW!!!

%Ilosc wyjsc
wyjscia=36;

%Wyzerowanie wartosci
MSE=0;
CE=0;
odrzucone=0;
%Wspolczynnik uczenia
wsp=0.1; 
beta=1;
disp('Pokazuje blad po kazdym sprawdzonym znaku dla wszystkich przetworzonych dotad znakow')

for b=1:wyjscia
    %Okresla litere z jej samplami i podaje sciezke (numer ASCII)
    if b<=10
        ascii=b;
    else
        ascii=b+7;
    end
    ascii=ascii+47;
    Sciezka=fullfile(DoSciezki,int2str(ascii));
    Pliki=dir(Sciezka);

    %Czego sie uczy
    ZadaneWyjscie=zeros(wyjscia,1);
    ZadaneWyjscie(b)=1;
    
    for i=1:(size(Pliki,1)-2)
        NazwaPliku=Pliki(i+2).name;
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
    %Obliczanie bledow
    %MSE - blad sredniokwadratowy
    disp('b - Znak numer: ')
    b
    disp('MSE - Blad sredniokwadratowy: ')
    MSE=0.5*MSE/(1000-odrzucone)/wyjscia
    disp('CE - bledne klasyfikacje (w %)')
    CE=CE/(1000-odrzucone)*100
end
end