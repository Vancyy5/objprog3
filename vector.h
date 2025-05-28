#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <memory>

/**
 * @brief Dinaminis šabloninis konteineris, panašus į std::vector.
 * 
 * @tparam T Elementų tipas.
 */
template <typename T>
class Vector {
public:
    using value_type = T;                   ///< Elemento tipas
    using size_type = std::size_t;         ///< Tipas dydžio žymėjimui
    using difference_type = std::ptrdiff_t;///< Skirtumo tipas tarp iteratorių
    using reference = T&;                  ///< Nuoroda į elementą
    using const_reference = const T&;      ///< Konstanta nuoroda į elementą
    using pointer = T*;                    ///< Rodyklė į elementą
    using const_pointer = const T*;        ///< Konstanta rodyklė į elementą
    using iterator = T*;                   ///< Iteratorius
    using const_iterator = const T*;       ///< Konstanta iteratorius
    using reverse_iterator = std::reverse_iterator<iterator>;           ///< Atvirkštinis iteratorius
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Konstanta atvirkštinis iteratorius

private:
    pointer data_ = nullptr;       ///< Rodyklė į duomenų masyvą
    size_type size_ = 0;           ///< Dabartinis elementų skaičius
    size_type capacity_ = 0;       ///< Dabartinė talpa (alokacijos dydis)
    size_t realloc_count_ = 0;     ///< Kiek kartų buvo perskirstyta atmintis

    /**
     * @brief Atminties perskirstymas naujai talpai.
     * 
     * @param new_capacity Nauja talpa.
     */
    void reallocate(size_type new_capacity);

    /**
     * @brief Naikina visus elementus.
     */
    void destroy_elements();

public:
    /**
     * @brief Numatytasis konstruktorius.
     */
    Vector() = default;

    /**
     * @brief Konstruktorius su elementų skaičiumi ir pradine verte.
     * 
     * @param count Elementų kiekis.
     * @param value Pradinė reikšmė kiekvienam elementui.
     */
    explicit Vector(size_type count, const T& value = T());

    /**
     * @brief Konstruktorius iš initializer_list.
     * 
     * @param init Inicilizacijos sąrašas.
     */
    Vector(std::initializer_list<T> init);

    /**
     * @brief Kopijavimo konstruktorius.
     * 
     * @param other Kitas vektorius.
     */
    Vector(const Vector& other);

    /**
     * @brief Perkėlimo konstruktorius.
     * 
     * @param other Perkeliamas vektorius.
     */
    Vector(Vector&& other) noexcept;

    /**
     * @brief Destruktorius.
     */
    ~Vector();

    // Operatoriai
    Vector& operator=(const Vector& other);               ///< Kopijavimo operatorius
    Vector& operator=(Vector&& other) noexcept;           ///< Perkėlimo operatorius
    Vector& operator=(std::initializer_list<T> ilist);    ///< Priskyrimas iš initializer_list

    // Prieiga prie elementų
    /**
     * @brief Prieiga su ribų tikrinimu.
     * 
     * @param pos Indeksas.
     * @return Nuoroda į elementą.
     */
    reference at(size_type pos);

    /**
     * @brief Prieiga su ribų tikrinimu (konstanta).
     * 
     * @param pos Indeksas.
     * @return Konstanta nuoroda į elementą.
     */
    const_reference at(size_type pos) const;

    reference operator[](size_type pos);             ///< Prieiga be ribų tikrinimo
    const_reference operator[](size_type pos) const; ///< Prieiga be ribų tikrinimo (konstanta)
    reference front();                                ///< Pirmas elementas
    const_reference front() const;                    ///< Pirmas elementas (konstanta)
    reference back();                                 ///< Paskutinis elementas
    const_reference back() const;                     ///< Paskutinis elementas (konstanta)
    pointer data() noexcept;                          ///< Rodyklė į duomenis
    const_pointer data() const noexcept;              ///< Konstanta rodyklė į duomenis

    // Iteratoriai
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // Naudingos funkcijos
    bool empty() const noexcept;             ///< Tikrina ar vektorius tuščias
    size_type size() const noexcept;         ///< Elementų skaičius
    size_type capacity() const noexcept;     ///< Esama talpa
    void reserve(size_type new_cap);         ///< Rezervuoja atmintį
    void shrink_to_fit();                    ///< Sumažina talpą iki dydžio
    void clear() noexcept;                   ///< Ištrina visus elementus

    // Modifikavimas
    iterator insert(const_iterator pos, const T& value); ///< Įterpia vieną elementą
    iterator insert(const_iterator pos, T&& value);       ///< Įterpia vieną elementą (perkėlimas)
    iterator insert(const_iterator pos, size_type count, const T& value); ///< Įterpia kelis
    iterator erase(const_iterator pos);                   ///< Ištrina vieną
    iterator erase(const_iterator first, const_iterator last); ///< Ištrina intervalą
    void push_back(const T& value);                       ///< Prideda gale
    void push_back(T&& value);                            ///< Prideda gale (perkėlimas)

    /**
     * @brief Sukuria objektą gale, panaudojant nurodytus argumentus.
     * 
     * @tparam Args Argumentų tipai.
     * @param args Argumentai, naudojami sukurti objektui.
     * @return Nuoroda į naują objektą.
     */
    template <class... Args>
    reference emplace_back(Args&&... args);

    void pop_back();                           ///< Pašalina paskutinį elementą
    void resize(size_type count);              ///< Keičia dydį (numatyta reikšmė)
    void resize(size_type count, const T& value); ///< Keičia dydį su reikšme
    void swap(Vector& other) noexcept;         ///< Pakeičia du vektorius vietomis

    // Naujos funkcijos
    void assign(size_type count, const T& value); ///< Priskiria reikšmes
    template<class InputIt>
    void assign(InputIt first, InputIt last);     ///< Priskiria iš intervalo
    void assign(std::initializer_list<T> ilist);  ///< Priskiria iš sąrašo

    /**
     * @brief Gauk kiek kartų buvo atlikta atminties perskirstymų.
     * 
     * @return Perskirstymų skaičius.
     */
    size_t reallocations() const;

    // Lyginimo operatoriai
    friend bool operator==(const Vector& lhs, const Vector& rhs) {
        return lhs.size_ == rhs.size_ && std::equal(lhs.data_, lhs.data_ + lhs.size_, rhs.data_);
    }

    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
        return !(lhs == rhs);
    }
};

#endif // VECTOR_H