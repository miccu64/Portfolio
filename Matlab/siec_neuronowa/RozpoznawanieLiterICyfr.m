function[]=RozpoznawanieLiterICyfr(Wagi,piksele,piksele2,DoSciezki)

%Ilosc rozpoznawanych znakow
wyjscia=size(Wagi,2);
while 1
    
    %Okresla litere z jej samplami i podaje sciezke
    liczba=randi(wyjscia);
    if liczba>=11
            ascii=liczba+54;
        else
            ascii=liczba+47;
    end
    Sciezka=fullfile(DoSciezki,int2str(ascii));
    Pliki=dir(Sciezka);

    %Losowanie sampla
    %+2, zeby pominac . i .. - Petla dla wszystkich plikow danego znaku
    NazwaPliku=Pliki(randi(size(Pliki,1)-4)+2).name;
    SciezkaPliku=fullfile(Sciezka,NazwaPliku);
    Wejscie=wczytajobraz(SciezkaPliku,piksele2,piksele);
    
    %Gdy algorytm dzieli pojedyncza litere na czesci pomijamy ja
    iloscczesci=size(Wejscie,2);
    if iloscczesci~=1
        continue
    end
    
    %Pokazywanie wczytanego obrazu
    Obraz=[];
    for i=1:piksele
        Obraz(i,:)=Wejscie((1+piksele2*(i-1)):piksele2*i);
    end
    Obraz=Obraz';
    imshow(Obraz)

    Wyjscie=obliczwyjscie(Wagi,Wejscie);
    
    Odczytane=0;
    %Okreslanie znaku i zamiana na ASCII
    for r=1:size(Wyjscie)
        if Wyjscie(r)>0.5
            if r>=11
                Odczytane=char(r+54)
            else
                Odczytane=char(r+47)
            end
            break
        end
    end
    if Odczytane==0
        disp('Nie rozpoznano')
    end
    pause
end
end