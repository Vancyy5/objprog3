#ifndef FUNKCIJOS_H      
#define FUNKCIJOS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <execution>
#include <numeric>
#include <cassert>
#include "zmogus.h"

/**
 * @class Studentas
 * @brief Student class that inherits from Zmogus (Person) abstract class
 * 
 * This class represents a student with homework grades, exam score and final grade.
 * It inherits from the abstract Zmogus class and implements its pure virtual methods.
 */
class Studentas : public Zmogus 
{
// realizacija
private:
    std::vector<int> nd_;        ///< Vector of homework (namų darbų) scores
    int egzaminas_;              ///< Exam score
    double galutinis_;           ///< Final grade

// interfeisas
public:
    /// Static counter for tracking destructor calls (for testing)
    static int destruktoriuSk;

    /**
     * @brief Default constructor
     * Initializes a student with default values
     */
    Studentas() : Zmogus(), egzaminas_(0), galutinis_(0) { }
    
    /**
     * @brief Constructor that initializes a student from input stream
     * @param is Input stream to read from
     */
    Studentas(std::istream& is);
    
    /**
     * @brief Constructor with name and surname
     * @param vardas Student's first name
     * @param pavarde Student's last name
     */
    Studentas(const std::string& vardas, const std::string& pavarde) 
        : Zmogus(vardas, pavarde), egzaminas_(0), galutinis_(0) { }
    
    // Rule of Five
    /**
     * @brief Copy constructor
     * @param other Another student to copy from
     */
    Studentas(const Studentas& other); 
    
    /**
     * @brief Copy assignment operator
     * @param other Another student to copy from
     * @return Reference to this student after assignment
     */
    Studentas& operator=(const Studentas& other);
    
    /**
     * @brief Move constructor
     * @param other Another student to move from
     */
    Studentas(Studentas&& other) noexcept;
    
    /**
     * @brief Move assignment operator
     * @param other Another student to move from
     * @return Reference to this student after assignment
     */
    Studentas& operator=(Studentas&& other) noexcept;
    
    /**
     * @brief Virtual destructor
     * Overrides the pure virtual destructor from Zmogus
     */
    ~Studentas() override;
    
    // Implementation of pure virtual methods
    /**
     * @brief Prints student information to output stream
     * @param os Output stream to print to
     */
    void print(std::ostream& os) const override;
    
    /**
     * @brief Reads student information from input stream
     * @param is Input stream to read from
     */
    void read(std::istream& is) override;
    
    // Getteriai
    /**
     * @brief Get homework grades
     * @return Vector of homework grades
     */
    inline std::vector<int> nd() const { return nd_; }
    
    /**
     * @brief Get exam grade
     * @return Exam grade
     */
    inline int egzaminas() const { return egzaminas_; }
    
    /**
     * @brief Get final grade
     * @return Final grade
     */
    inline double galutinis() const { return galutinis_; }

    // Setteriai
    /**
     * @brief Set exam grade
     * @param egzaminas Exam grade to set
     */
    inline void setEgzaminas(int egzaminas) { egzaminas_ = egzaminas; }
    
    /**
     * @brief Set final grade
     * @param galutinis Final grade to set
     */
    inline void setGalutinis(double galutinis) { galutinis_ = galutinis; }

    // Metodai
    /**
     * @brief Read student data from input stream
     * @param is Input stream to read from
     * @return Reference to the input stream
     */
    std::istream& readStudent(std::istream& is);
    
    /**
     * @brief Add a homework grade
     * @param pazymys Grade to add
     * @throw std::invalid_argument If grade is not between 1 and 10
     */
    void addND(int pazymys);
    
    /**
     * @brief Clear all homework grades
     */
    void clearND();
    
    /**
     * @brief Calculate final grade (average method)
     * @return Final grade based on homework average and exam
     */
    double galBalas() const {
        if (nd_.empty()) return egzaminas_;
        double ndVidurkis = skaiciuotiVid(nd_);
        return 0.4 * ndVidurkis + 0.6 * egzaminas_;
    }
    
    /**
     * @brief Calculate final grade (median method)
     * @return Final grade based on homework median and exam
     */
    double galBalasMed() const {
        if (nd_.empty()) return egzaminas_;
        double ndMediana = skaiciuotiMed(nd_);
        return 0.4 * ndMediana + 0.6 * egzaminas_;
    }
    
    /**
     * @brief Calculate average of homework grades
     * @param nd Container of grades
     * @return Average value
     */
    template <typename Container>
    static double skaiciuotiVid(const Container& nd) {
        if (nd.empty()) {
            throw std::runtime_error("Namu darbu sarasas negali buti tuscias");
        }
        double suma = 0.0;
        for (const auto& elem : nd) {
            suma += elem;
        }
        return suma / nd.size();
    }

    /**
     * @brief Calculate median of homework grades
     * @param nd Container of grades (will be sorted)
     * @return Median value
     */
    template <typename Container>
    static double skaiciuotiMed(Container nd) {
        if (nd.empty()) {
            throw std::runtime_error("Namu darbu sarasas negali buti tuscias");
        }
        std::sort(nd.begin(), nd.end());
        size_t dydis = nd.size();
        if (dydis % 2 == 0) {
            return (nd[dydis / 2 - 1] + nd[dydis / 2]) / 2.0;
        } else {
            return nd[dydis / 2];
        }
    }
    
    /**
     * @brief Read students from file (static method)
     * @param grupe Container to store students
     * @param failoPavadinimas Name of the file to read from
     */
    template<typename Container>
static void nuskaitymasFile(Container& grupe, const std::string& failoPavadinimas) {
    std::ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        throw std::runtime_error("Nepavyko atidaryti failo: " + failoPavadinimas);
    }
    
    std::string eilute;
    
    // Praleisti pirmą eilutę (header)
    if (std::getline(failas, eilute)) {
        // Pirma eilutė praleista
    }
    
    // Dabar skaityti studentų duomenis
    while (std::getline(failas, eilute)) {
        if (eilute.empty()) continue;
        
        std::istringstream iss(eilute);
        Studentas studentas;
        try {
            studentas.readStudent(iss);
            studentas.setGalutinis(studentas.galBalas());
            grupe.push_back(studentas);
        } catch (const std::exception& e) {
            std::cerr << "Klaida skaitant studenta: " << e.what() << std::endl;
        }
    }
    failas.close();
}
};

// Comparison functions
/**
 * @brief Compare students by first name
 */
bool compareByVardas(const Studentas& a, const Studentas& b);

/**
 * @brief Compare students by last name
 */
bool compareByPavarde(const Studentas& a, const Studentas& b);

/**
 * @brief Compare students by final grade (descending)
 */
bool compareByGalutinis(const Studentas& a, const Studentas& b);

// Template functions for container operations
/**
 * @brief Read students from file into container
 * @param grupe Container to store students
 * @param failoPavadinimas Name of the file
 */
template<typename Container>
void skaitytiIsFailo(Container& grupe, const std::string& failoPavadinimas) {
    Studentas::nuskaitymasFile(grupe, failoPavadinimas);
}

/**
 * @brief Sort students in container
 * @param grupe Container of students
 * @param sortingOption Sorting option ('v' - by name, 'p' - by surname, 'g' - by grade)
 */
template<typename Container>
void sortStudentai(Container& grupe, char sortingOption) {
    switch (tolower(sortingOption)) {
        case 'v':
            std::sort(grupe.begin(), grupe.end(), compareByVardas);
            break;
        case 'p':
            std::sort(grupe.begin(), grupe.end(), compareByPavarde);
            break;
        case 'g':
            std::sort(grupe.begin(), grupe.end(), compareByGalutinis);
            break;
        default:
            std::sort(grupe.begin(), grupe.end(), compareByPavarde);
            break;
    }
}

/**
 * @brief Separate students into two groups based on final grade
 * @param grupe Original container of students
 * @param kietiakiai Container for students with grade >= 5.0
 * @param vargsai Container for students with grade < 5.0
 */
template<typename Container>
void skirstytiStudentus(Container& grupe, Container& kietiakiai, Container& vargsai) {
    // Using std::partition for efficiency
    auto partition_point = std::partition(grupe.begin(), grupe.end(), 
        [](const Studentas& s) { return s.galutinis() >= 5.0; });
    
    // Copy to respective containers
    kietiakiai.assign(grupe.begin(), partition_point);
    vargsai.assign(partition_point, grupe.end());
    
    // Clear original container
    grupe.clear();
}

/**
 * @brief Output students to file
 * @param studentai Container of students
 * @param failoPavadinimas Name of output file
 * @param ats Answer type ('v' - average, 'm' - median)
 */
template<typename Container>
void isvestiStudentusIFaila(const Container& studentai, const std::string& failoPavadinimas, char ats) {
    std::ofstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        throw std::runtime_error("Nepavyko sukurti failo: " + failoPavadinimas);
    }
    
    // Header
    failas << std::left << std::setw(15) << "Vardas" 
           << std::setw(15) << "Pavarde" 
           << std::setw(20) << "Galutinis (" << (tolower(ats) == 'v' ? "Vid." : "Med.") << ")"
           << std::endl;
    failas << std::string(50, '-') << std::endl;
    
    // Student data
    for (const auto& studentas : studentai) {
        failas << std::left << std::setw(15) << studentas.vardas()
               << std::setw(15) << studentas.pavarde()
               << std::fixed << std::setprecision(2) 
               << (tolower(ats) == 'v' ? studentas.galBalas() : studentas.galBalasMed())
               << std::endl;
    }
    
    failas.close();
}

/**
 * @brief Generate random students for testing
 * @param grupe Container to store generated students
 * @param kiekis Number of students to generate
 */
template<typename Container>
void generuotiStudentus(Container& grupe, int kiekis) {
    grupe.clear();
    grupe.reserve(kiekis); // Reserve space if container supports it
    
    for (int i = 1; i <= kiekis; ++i) {
        Studentas studentas("Vardas" + std::to_string(i), "Pavarde" + std::to_string(i));
        
        // Generate 5-10 random homework grades
        int ndKiekis = 5 + rand() % 6;
        for (int j = 0; j < ndKiekis; ++j) {
            studentas.addND(1 + rand() % 10);
        }
        
        // Generate exam grade
        studentas.setEgzaminas(1 + rand() % 10);
        studentas.setGalutinis(studentas.galBalas());
        
        grupe.push_back(studentas);
    }
}

/**
 * @brief Implementation of data processing test
 * @param aplankas Directory for test files
 * @param skaicius Number of students
 */
template<typename Container>
void testuotiDuomenuApdorojimaImpl(const std::string& aplankas, int skaicius);

/**
 * @brief Test data processing performance
 * @param aplankas Directory for files
 * @param skaicius Number of students
 * @param testChoice Test choice (2 for std::vector, 3 for custom Vector)
 */
void testuotiDuomenuApdorojima(const std::string& aplankas, int skaicius, int testChoice);

/**
 * @brief Test Studentas class methods
 */
void testuotiStudentoMetodus();

/**
 * @brief Test Zmogus class
 */
void testuotiZmogausKlase();

#endif // FUNKCIJOS_H