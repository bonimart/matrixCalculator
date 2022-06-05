# Specifikace zadání
Maticová kalkulačka

Vytvořte aplikaci implementující maticovou kalkulačku.

Kalkulačka musí implementovat:

    práci s maticemi různé velikosti
    základní operace: sčítání, odčítání, násobení, transpozice
    sloučení (3x3 sloučeno s 3x3 => matice 6x3) a oříznutí (např. pouze levá polovina matice 6x3 => levá 3x3)
    pro čtvertcové matice navíc výpočet inverze
    efektivní paměťovou reprezentaci (pro řídké matice se nehodí 2D pole, pro husté matice naopak)
    funkce pro výpočet determinantu, určení hodnosti, Gaussova eliminační metoda
    proměnné (uložení matice do proměnné, načítání matice ze vstupu, využití proměnné ve výpočtu)
    uložení matice (v efektivní reprezentaci) do souboru a její načtení ze souboru

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
Polymorfismus je využit při vyhodnocení různých operací nad maticemi.
Dále se uplatňuje při práci s řídkými/hustými maticemi.


# Ukázka použití
```
x = [[2,0],[0,1]];
y = $x+$x;
z = gem($x*$y);
det($z);
rank($z);
inv($z);
build/matice.txt < $z;
x = @build/matice.txt;
sel(join(@build/matice.txt, $z), [[0,0],[2,2]]);
```