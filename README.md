# objprog3

APRAŠYMAS:
---
Ši programa analizuoja studentų duomenų failus su skirtingais įrašų dydžiai, juos surūšiuoja, vėliau juos išskirsto pagal jų galutinį balą į dvi grupes: kietekų(galutinis balas >=5.0 ) ir vargšų(galutinis balas < 5.0). Šioje versijoje įgyvendinta Vector šabloninę klasę, kuri funkcionalumu siekia atkartoti std::vector.
-------------------------------------------------------------------------------------------------------
TESTAVIMO SISTEMOS PARAMETRAI:
---
CPU: Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz, 1190 Mhz, 4 Core(s), 8 Logical Processors

RAM: 8GB DDR4-2666 

DISK: SKHynix_HFM512GDHTNI-87A0B (476,9 GB)

OS: Microsoft Windows 11 Home

GPU: Intel(R) UHD Graphics (1920x1080x32b)

G++ versija 14.2.0

------------------------------------------------------------------------------------------------------
# V3.0
---
Vector klasės funkcionalumas
---
# Vector – Vektoriaus šablono klasė

## Pagrindinės savybės

| Savybė | Aprašymas |
| --- | --- |
| Naršymas | `begin()`, `end()`, `cbegin()`, `cend()`, `rbegin()`, `rend()`, `crbegin()`, `crend()` |
| Prieiga prie elementų | `operator[]`, `at()`, `front()`, `back()`, `data()` |
| Dydžio informacija | `size()`, `capacity()`, `empty()` |
| Elementų valdymas | `push_back()`, `pop_back()`, `insert()`, `erase()`, `clear()`, `resize()`, `reserve()`, `shrink_to_fit()` |

---

## Konstruktoriai ir destruktorius

| Konstruktorius / Metodas | Aprašymas |
| --- | --- |
| `Vector()` | Sukuria tuščią vektorių |
| `Vector(size_type count, const T& value = T())` | Sukuria vektorių su `count` kopijomis `value` |
| `Vector(std::initializer_list<T> init)` | Sukuria vektorių iš inicializacijos sąrašo |
| `Vector(const Vector& other)` | Kopijavimo konstruktorius |
| `Vector(Vector&& other) noexcept` | Judinimo konstruktorius |
| `~Vector()` | Destruktorius, atlaisvina atmintį |

---

## Naršymo metodai

| Metodas | Aprašymas |
| --- | --- |
| `iterator begin()` | Grąžina iteratorių į pirmą elementą |
| `const_iterator begin() const` | Kaip `begin()`, bet konstantinis |
| `iterator end()` | Grąžina iteratorių už paskutinio elemento |
| `const_iterator end() const` | Kaip `end()`, bet konstantinis |
| `reverse_iterator rbegin()` | Atvirkštinis iteratorius nuo galo |
| `reverse_iterator rend()` | Atvirkštinis iteratorius iki pradžios |

---

## Elementų valdymo metodai

| Metodas | Aprašymas |
| --- | --- |
| `reference at(size_type pos)` | Prieiga su ribų tikrinimu |
| `reference operator[](size_type pos)` | Prieiga be ribų tikrinimo |
| `void push_back(const T& value)` | Prideda elementą gale (kopija) |
| `void push_back(T&& value)` | Prideda elementą gale (judinamas) |
| `template<class... Args> reference emplace_back(Args&&... args)` | Sukuria elementą vietoje gale |
| `void pop_back()` | Pašalina paskutinį elementą |
| `iterator insert(const_iterator pos, const T& value)` | Įterpia kopiją |
| `iterator insert(const_iterator pos, T&& value)` | Įterpia judinamą reikšmę |
| `iterator insert(const_iterator pos, size_type count, const T& value)` | Įterpia kelis |
| `iterator erase(const_iterator pos)` | Pašalina vieną elementą |
| `iterator erase(const_iterator first, const_iterator last)` | Pašalina intervalą |
| `void clear()` | Pašalina visus elementus |
| `void resize(size_type count)` | Pakeičia dydį į `count` |
| `void reserve(size_type new_cap)` | Rezervuoja atmintį |
| `void shrink_to_fit()` | Sumažina talpą iki faktinio dydžio |

---

## Operatoriai

| Operatorius | Aprašymas |
| --- | --- |
| `operator=` | Priskyrimas (kopijavimas, judinimas, initializer_list) |
| `operator==` | Lygina ar du vektoriai yra lygūs |
| `operator!=` | Tikrina ar vektoriai skirtingi |

---

## Papildomos funkcijos

| Funkcija | Aprašymas |
| --- | --- |
| `void assign(size_type count, const T& value)` | Priskiria `count` kopijų `value` |
| `template<class InputIt> void assign(InputIt first, InputIt last)` | Priskiria iš intervalo |
| `void assign(std::initializer_list<T> ilist)` | Priskiria iš sąrašo |
| `void swap(Vector& other) noexcept` | Apsikeičia su kitu vektoriumi |
| `size_t reallocations() const` | Grąžina kiek kartų buvo perkelta atmintis |


-----------------------------------------------------------------------------------------------------

Kad patikrinti, ar Vector klasė veikia kaip std::vector, sukūti unit testai "vector_testai.cpp".

![alt text](https://github.com/Vancyy5/objprog3/blob/v3.0/nuotraukos/Screenshot%202025-05-28%20004408.png)
---------------------------------------------------------------------------------------------------

# Efektyvumo analizė

Atlikti efektyvumo testai lyginant std::vector ir nuosavą Vector, tuščius vektorius užpildant: 10000, 100000, 1000000, 10000000 ir 100000000 int elementų naudojant push_back() funkciją.


| Elementų skaičius| std::vector (s) | custom vector (s) |
| --- | --- | --- |
| 10000 | 0.000688 | 0.0007489 |
| 100000 | 0.0031742 | 0.0026978 |
| 1000000 | 0.0110823 | 0.0124785 |
| 10000000 | 0.052412  | 0.038796  |
| 100000000 | 0.431652  | 0.395542 |

Išvados:
Vector yra spartesnis.
Tai labiau pasireiškia, kai didėja elementų skaičius.
------------------------------------------------------------------------------

# Atminties persiskirstymai 

Atliekant testą su (100000000) int tipo elementų:

| Konteineris| Perskirstymų skaičius | 
| --- | --- | 
| std::vector | 28 | 
| custom vector (s) | 28 | 

Išvados:
Abi struktūros elgiasi labai panašiai.
std::vector gali naudoti šiek tiek kitokį augimo algoritmą, todėl kartais perskirstymų daugiau.

Šis testas parodo, kad Vector klasė yra efektyviai įgyvendinta ir savo elgsena artima standartinei std::vector klasei.

------------------------------------------------------------------------
![alt text](https://github.com/Vancyy5/objprog3/blob/v3.0/nuotraukos/Screenshot%202025-05-28%20073805.png)

------------------------------------------------------------------------

# Spartos analizė

Skaitymo, rūšiavimo, skaidymo į grupes ir spausdinimo operacijų laiko matavimas: Išmatuoti laikai kiekvienam žingsniui naudojant tiek std::vector, tiek MyVector konteinerį.

Testavimo metu buvo naudojami šie duomenų dydžiai:

100 000 studentų įrašų
1 000 000 studentų įrašų
10 000 000 studentų įrašų

Vidurkiai:

### 100 000 studentų

| Testuojamas konteineris | Failo nuskaitymas (s) | Studentų rūšiavimas (s) | Studentų skaidymas į grupes (s) | Studentų spausdinimas (s) |
|-------------------------|------------------------:|---------------------------:|----------------------------------:|-----------------------------:|
| `std::vector`           | 0.194867                 | 0.0200955                      | 0.0214709                             | 0.583871                       |
| `Vector`              |  0.195314                  | 0.0218867                     | 0.0227389                             | 0.585393                       |

---

### 1 000 000 studentų

| Testuojamas konteineris | Failo nuskaitymas (s) | Studentų rūšiavimas (s) | Studentų skaidymas į grupes (s) | Studentų spausdinimas (s) |
|-------------------------|------------------------:|---------------------------:|----------------------------------:|-----------------------------:|
| `std::vector`           | 2.55053                 | 0.279619                    | 0.332568                            | 6.0867                    |
| `Vector`              |  2.48155                  | 0.271694                    | 0.312339                           | 6.256                    |

---

### 10 000 000 studentų

| Testuojamas konteineris | Failo nuskaitymas (s) | Studentų rūšiavimas (s) | Studentų skaidymas į grupes (s) | Studentų spausdinimas (s) |
|-------------------------|------------------------:|---------------------------:|----------------------------------:|-----------------------------:|
| `std::vector`           | 43.0905               | 4.31249                   | 6.96529                          |  88.344                    |
| Vector`              | 29.2696               | 3.60891                   | 5.07625                          | 78.0648                    |

------------------------------------------------------------------------------
Išvados
---
Spartos skirtumai: Palyginus laikus su std::vector ir MyVector konteineriais, skirtumai buvo minimalių dydžių. std::vector užtrunka šiek tiek daugiau laiko, tačiau tai gali būti dėl optimizacijų, kurias teikia C++ standartinė biblioteka. MyVector turi šiek tiek mažesnį laiką rūšiavimui ir spausdinimui, tačiau kitose operacijose skirtumai nėra dideli.

Atminties naudojimas: Tiek std::vector, tiek MyVector naudoja panašias atminties valdymo technikas, todėl atminties sąnaudos yra vienodas.

Veikimo stabilumas: Abi realizacijos gerai veikia su dideliais duomenų kiekiais, nes didėjant duomenų kiekiui (iki 10 milijonų studentų) laikai auga, tačiau neproporcingai didėja, kas rodo stabilų veikimą.