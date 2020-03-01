# Sieć neuronowa
Program z interfejsem tekstowym umożliwiający rozpoznawanie pisma przy użyciu sztucznej sieci neuronowej.
Korzysta z fragmentu kodu umożliwiającego łatwe i szybkie wyodrębnianie symboli z plików .bmp i zamienianie ich na binarną reprezentacje w macierzy w Matlabie.

## Szczegóły
Sieć ma jedną warstwę i tyle wyjściach ile rozpoznawanych wzorców. 
Podawałem dużo wzorców, aby sieć nie nauczyła się na pamięć. 
Program zawiera zestawy testowe i pokazuje procent błędów w funkcji liczby iteracji procesu uczenia (zestawy testowe - dwa typy:
jeden ze znakami ze zbioru uczącego, drugi ze znakami spoza niego).
Wykorzystałem uczenie z nauczycielem.

Dane testowe z zipa należy powrzucać do odwpowiednich folderów.
