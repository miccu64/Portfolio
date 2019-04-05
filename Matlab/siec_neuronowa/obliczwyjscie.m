function[Wyjscie]=obliczwyjscie(Wagi, Wejscie)
beta=1;
%Dodatkowe wejscie (dla biasu)
Wejscie=[-1; Wejscie];
%pobudzenie neuronu
Pobudzenie=Wagi'*Wejscie;
%aktywacja neuronu
Wyjscie=1./(1+exp((-beta)*Pobudzenie));
end