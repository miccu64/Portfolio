nrows=30; %arbitralnie zadany rozmiar literki po wczytaniu, nale¿y sobie
ncols=20; %dobraæ samemu


%%% UWAGA: MA£A LITERA "i" MA JAKO JEDYNA LITERA DWIE CZÊŒCI
%%% MO¯NA ZATEM ODPUŒCIÆ JEJ ROZPOZNAWANIE, ALBO ZROBIÆ PO PROSTU DU¯E
%%% LITERY, ALBO JAKOŒ OBEJŒÆ TEN PROBLEM - JA REKOMENDUJÊ ROBIÆ TYLKO DU¯E
%%% LITERY BEZ POLSKICH ZNAKÓW

b=imread('zdj.bmp'); %wczytanie pliku
b=im2bw(b,0.1); % binaryzacja tzn zamian obrazku na czarno-bia³y bez odcieni
%szaroœci
b=~b; % negacja logiczna binarnego obrazu czyli zamiana zer na jedynki i vice
%wersa
b=bwlabel(b); %poprzyczepianie etykiet obszarom takim jak literka

s=regionprops(b,'all'); %pociêcie na regiony

x=zeros(length(s),nrows*ncols); %pusta macierz do której wsadzimy obrazki
%kolejnych liter jeden za drugim
for i=1:length(s) %pêtla upychaj¹ca obrazki w tej macierzy
    x1=imresize(s(i).Image,[nrows ncols]);
    x(i,:)=x1(:);
end
%imshow(x)
%do sprawdzenia poprawnoœci wczytania mo¿na skorzystaæ z imshow

%w razie pytañ proszê pisaæ na forum