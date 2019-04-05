clc; clear; close all;
%Poszukiwanie minimów funkcji skalarnej dwóch zmiennych metod¹ gradientu prostego.
%Punkt startowy wyczytwany jest myszk¹.
%Enter pokazuje kolejne kroki.
syms x y;
%Dowolna funkcja
funkcja=-(2*cos(x)*cos(x)-sin(3*y))^2;
gradienty=gradient(funkcja);

ezsurf(funkcja, [-3, 3])
hold on
%interpolated shading
shading interp 
%2 wymiary
view(2)
%skok po kolorze
gamma=0.02;

while (1)
[xpunkt ypunkt]=ginput(1);
while(1)
    if size(xpunkt)==0
        break
    end
    podstgrad=double(subs(gradienty, {x, y}, {xpunkt, ypunkt}))
    xpunkt2=xpunkt-gamma*podstgrad(1);
    ypunkt2=ypunkt-gamma*podstgrad(2);
    roznica=[xpunkt2-xpunkt, ypunkt2-ypunkt];
    %kryterium konca
    if ((((roznica(1))^2+(roznica(2))^2)^1/2)<0.0002)
        break
    end
    quiver(xpunkt, ypunkt, roznica(1), roznica(2),'g')
    xpunkt=xpunkt2;
    ypunkt=ypunkt2;
    pause
end
end