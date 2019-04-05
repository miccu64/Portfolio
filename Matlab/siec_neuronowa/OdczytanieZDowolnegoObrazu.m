function[Odczytane]=OdczytanieZDowolnegoObrazu(Wagi,piksele,piksele2,Sciezka)

%Wczytaj obraz do macierzy
Wejscie=wczytajobraz(Sciezka,piksele2,piksele);

%Wyswietlanie obrazu
imshow(Sciezka)

%Ilosc liter na zdjeciu
iloscliter=size(Wejscie,2);
for numer=1:iloscliter
    Wyjscie=obliczwyjscie(Wagi,Wejscie(:,numer));
    Odczytane(numer)=0;
    for r=1:size(Wyjscie)
        if Wyjscie(r)>=0.5
            if r>=11
                Odczytane(numer)=r+54;
            else
                Odczytane(numer)=r+47;
            end
            break
        end
    end
end
end