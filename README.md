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
## Pagrindinės savybės

| Savybė                 | Aprašymas                                                                                   |
|------------------------|--------------------------------------------------------------------------------------------|
| Naršymas               | `begin()`, `end()`, `cbegin()`, `cend()`, `rbegin()`, `rend()`, `crbegin()`, `crend()`    |
| Prieiga prie elementų  | `operator[]`, `at()`, `front()`, `back()`, `data()`                                        |
| Dydžio informacija      | `size()`, `capacity()`, `empty()`                                                          |
| Elementų valdymas      | `push_back()`, `pop_back()`, `insert()`, `erase()`, `clear()`, `resize()`, `reserve()`, `shrink_to_fit()` |
| Perskirstymų skaičius  | `reallocations()` – grąžina, kiek kartų vyko atminties perskirstymas                         |

---

## Konstruktoriai ir destruktorius

| Konstruktorius / Metodas                | Aprašymas                                      |
|---------------------------------------|-----------------------------------------------|
| `Vector()`                            | Sukuria tuščią vektorių                        |
| `Vector(size_type count, const T& value = T())` | Sukuria vektorių su `count` kopijomis `value` |
| `Vector(std::initializer_list<T> init)` | Sukuria vektorių iš inicializavimo sąrašo      |
| `Vector(const Vector& other)`          | Kopijavimo konstruktorius                       |
| `Vector(Vector&& other) noexcept`      | Judančioji konstruktorius                       |
| `~Vector()`                           | Destruktorius, atlaisvina atmintį ir sunaikina elementus |

---

## Naršymo metodai

| Metodas                     | Aprašymas                                  |
|-----------------------------|--------------------------------------------|
| `iterator begin()`           | Grąžina iteratorių į pirmą elementą       |
| `const_iterator begin() const` | Tas pats, bet konstantinis iteratorius      |
| `iterator end()`             | Grąžina iteratorių už paskutinio elemento |
| `const_iterator end() const` | Tas pats, bet konstantinis iteratorius     |
| `reverse_iterator rbegin()` | Grąžina atvirkštinį iteratorių į paskutinį elementą |
| `reverse_iterator rend()`   | Grąžina atvirkštinį iteratorių už pirmojo elemento |

---

## Elementų valdymo metodai

| Metodas                                             | Aprašymas                                                   |
|-----------------------------------------------------|-------------------------------------------------------------|
| `reference at(size_type pos)`                        | Grąžina nuorodą į elementą su ribų tikrinimu                |
| `reference operator[](size_type pos)`               | Grąžina nuorodą į elementą (be ribų tikrinimo)              |
| `void push_back(const T& value)`                     | Prideda kopiją elemento gale                                 |
| `void push_back(T&& value)`                          | Prideda judinamą elementą gale                               |
| `template<class... Args> reference emplace_back(Args&&... args)` | Kuria elementą vietoje gale                                  |
| `void pop_back()`                                    | Pašalina paskutinį elementą                                  |
| `iterator insert(const_iterator pos, const T& value)` | Įterpia kopiją į poziciją                                    |
| `iterator insert(const_iterator pos, T&& value)`    | Įterpia judinamą elementą į poziciją                         |
| `iterator insert(const_iterator pos, size_type count, const T& value)` | Įterpia kelis kopijuotus elementus                           |
| `iterator erase(const_iterator pos)`                 | Pašalina elementą pozicijoje                                 |
| `iterator erase(const_iterator first, const_iterator last)` | Pašalina elementų intervalą                                  |
| `void clear()`                                       | Pašalina visus elementus                                     |
| `void resize(size_type count)`                       | Pakeičia vektoriaus dydį, papildant arba šalinant elementus |
| `void reserve(size_type new_cap)`                     | Rezervuoja atmintį (capacity)                               |
| `void shrink_to_fit()`                               | Sumažina atmintį iki faktinio dydžio                         |

---

## Operatoriai

| Operatorius                    | Aprašymas                               |
|--------------------------------|----------------------------------------|
| `operator=`                   | Priskyrimo operatoriai (kopijavimas, judinimas, initializer_list) |
| `operator==`                  | Lygina du vektorius pagal elementus    |
| `operator!=`                  | Ne lygu (lygina du vektorius)          |

![alt text](https://github.com/Vancyy5/objprog3/blob/v3.0/nuotraukos/Screenshot%202025-05-28%20004408.png)

-----------------------------------------------------------------------------------------------------

Kad patikrinti, ar Vector klasė veikia kaip std::vector, sukūti unit testai "vector_testai.cpp".

---------------------------------------------------------------------------------------------------



