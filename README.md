# Specifikace zadání
Cílem semestrální práce je implementace maticové kalkulačky.

Kalkulačka bude splňovat následující:
- práce s maticemi různé velikosti
- základní operace (sčítání, odčítání, násobení, transpozice)
- sloučení a oříznutí matic
- pro čtvercové matice výpočet inverze
- efektivní paměťovou reprezentaci
- výpočet determinantu, určení hodnosti, Gaussova eliminační metoda
- možnost uložení matice do proměnné
- uložení matice do souboru a její načtení ze souboru
- výpočet maticových rozkladů LU, QR a SVD
- pro čtvercové matice výpočet vlastních čísel matice pomocí QR algoritmu

Matice budou podle hustoty vnitřně reprezentovány buď jako 2D pole, nebo jako mapa. Pro Gaussovu eliminační metodu a QR algoritmus bude možné zvolit verzi s komentářem, kde budou vypsány jednotlivé kroky algoritmu.

# Použití polymorfismu
Polymorfismus se v semestrální práci bude využívat v operacích nad maticemi s různou vnitřní reprezentací


# Ukázka použití
```
load x[3][3] matrix.txt //nacteni ze souboru
y[3][3] = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
z = merge x y //[3][3] merge [3][3] -> [6][3]
gem z -v //gaussova eliminace krok po kroku
svd z //vypocet a vypis svd rozkladu
save svd z matrix2.txt //ulozeni vsech tri matic z svd do souboru
```