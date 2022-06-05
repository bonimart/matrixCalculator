# Zadání z Progtestu
Cílem semestrální práce je implementace maticové kalkulačky.

Kalkulačka musí implementovat:

    práci s maticemi různé velikosti
    základní operace: sčítání, odčítání, násobení, transpozice
    sloučení (3x3 sloučeno s 3x3 => matice 6x3) a oříznutí (např. pouze levá polovina matice 6x3 => levá 3x3)
    pro čtvertcové matice navíc výpočet inverze
    efektivní paměťovou reprezentaci (pro řídké matice se nehodí 2D pole, pro husté matice naopak)
    funkce pro výpočet determinantu, určení hodnosti, Gaussova eliminační metoda
    proměnné (uložení matice do proměnné, načítání matice ze vstupu, využití proměnné ve výpočtu)
    uložení matice (v efektivní reprezentaci) do souboru a její načtení ze souboru

# Specifikace zadání
Matice bude možné zadat ve tvaru `[[a,b,...],[...],...]` nebo `[{i, j}:val,...]`.
Příkazy budou oddělovány znakem `;`.

Kalkulačka bude implementovat následující operace:
- `help` - vypíše návod použití
- `vars` - vypíše všechny uložené proměnné
- `[x] = [exp]` - přiřadí do proměnné s názvem 'x' hodnotu výrazu 'exp'
- `[f] < [exp]` - vypíše vyhodnocený výraz do souboru s názvem 'f'
- `$[x]` - vrátí aktuální hodnotu proměnné 'x'
- `@[f]` - importuje výraz ze souboru
- `tr([exp])` - transponuje hodnotu výrazu
- `print([exp])` - vypíše aktuální hodnotu výrazu
- `join([exp1], [exp2])` - vytvoří novou matici tak, že přidá sloupce druhé matice ke sloupcům první matice
- `sel([exp1], [exp2])` - vybere část první matice podle indexů v druhé matici (musí být 2x2)
- `[exp1] + [exp2]` - sečte matice po složkách
- `[exp1] - [exp2]` - odečte matice po složkách
- `[exp1] * [exp2]` - provede maticové násobení
- `det([exp1])` - vypočítá determinant matice
- `gem([exp1])` - vrátí "zgemovanou" vstupní matici
- `rank([exp1])` - vypočítá hodnost matice
- `inv([exp1])` - vypočítá inverzi regulární matice

Kalkulačka bude navíc implementovat rekurzivní parser pro zpracování komplikovanějších příkazů v rámci jednoho řádku.


# Použití polymorfismu
Polymorfismus se využívá u operací - třída `Operation` má abstraktní metody `evaluate` a `validate`. Polymorfní volání je ve tříde `Parser`, kde při parsování vstupu rovnou dochází k vyhodnocení operací.
Dále se polymorfismus uplatňuje při práci s hustými a řídkými maticemi. Ve tříde `Matrix` je abstraktní třída `Data`, ze které dědí `DataDense` a `DataSparse`. Tyto podtřídy implementují abstraktní metody `at` a `set`.


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
sel(join(@build/matice.txt, $z), [[0,0],[1,1]]);

```