function[x]=wczytajobraz(sciezka,nrows,ncols)

b=imread(sciezka); %wczytanie pliku
b=im2bw(b,0.1); %binaryzacja tzn zamian obrazku na czarno-bia³y bez odcieni szaroœci
b=~b; %negacja logiczna binarnego obrazu czyli zamiana zer na jedynki i vice wersa
b=bwlabel(b); %poprzyczepianie etykiet obszarom takim jak literka

s=regionprops(b,'all'); %pociêcie na regiony

x=zeros(length(s),nrows*ncols); %pusta macierz do której wsadzimy obrazki
%kolejnych liter jeden za drugim
for i=1:length(s) %pêtla upychaj¹ca obrazki w tej macierzy
    x1=imresize(s(i).Image,[nrows ncols]);
    x(i,:)=x1(:);
end
x=x';
end