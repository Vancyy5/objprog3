#include "funkcijos.h"
#include "laikas.h"
#include "vector.h"
#include <cstdlib>
#include <ctime>

// Initialize static counter
int Studentas::destruktoriuSk = 0;

//---
Studentas::Studentas(std::istream& is) : Zmogus() 
{
    readStudent(is);
}

// Copy constructor
Studentas::Studentas(const Studentas& other) : 
    Zmogus(other),  
    nd_(other.nd_), 
    egzaminas_(other.egzaminas_),
    galutinis_(other.galutinis_) 
    {}

// Copy assignment operator
Studentas& Studentas::operator=(const Studentas& other) {
    if (this != &other) {
        Zmogus::operator=(other); // Call base class assignment operator
        nd_ = other.nd_;
        egzaminas_ = other.egzaminas_;
        galutinis_ = other.galutinis_;
    }
    return *this;
}

// Move constructor
Studentas::Studentas(Studentas&& other) noexcept : 
    Zmogus(std::move(other)),  
    nd_(std::move(other.nd_)),
    egzaminas_(other.egzaminas_),
    galutinis_(other.galutinis_)
{
    other.egzaminas_ = 0;
    other.galutinis_ = 0;
}

// Move assignment operator
Studentas& Studentas::operator=(Studentas&& other) noexcept {
    if (this != &other) {
        Zmogus::operator=(std::move(other)); 
        nd_ = std::move(other.nd_);
        egzaminas_ = other.egzaminas_;
        galutinis_ = other.galutinis_;
        
        other.egzaminas_ = 0;
        other.galutinis_ = 0;
    }
    return *this;
}

// Destructor
Studentas::~Studentas()
{
    nd_.clear();
    destruktoriuSk++; 
}

// Implementation of pure virtual methods
void Studentas::print(std::ostream& os) const 
{
    os << std::left << std::setw(15) << vardas_ << std::setw(15) << pavarde_;
    
    os << "ND: ";
    for (const auto& nd : nd_) {
        os << nd << " ";
    }
    
    os << "Egz: " << egzaminas_;
    os << " Galutinis: " << std::fixed << std::setprecision(2) << galutinis_;
}

void Studentas::read(std::istream& is) {
    readStudent(is);
}

std::istream& Studentas::readStudent(std::istream& is) {
    is >> vardas_ >> pavarde_;
    
    nd_.clear();
    int pazymys;
    
    while (is >> pazymys) {
        if (pazymys >= 1 && pazymys <= 10) {
            nd_.push_back(pazymys);
        } else {
            throw std::invalid_argument("Klaida: Netinkamas pazymys studentui " + vardas_ + " " + pavarde_);
        }
    }
    
    // Išvalome stream būseną (pašalinta klaidinga klaidos patikra)
    is.clear();
    
    // Paskutinis pažymys yra egzaminas
    if (!nd_.empty()) {
        egzaminas_ = nd_.back();
        nd_.pop_back();
    }
    
    return is;
}

void Studentas::addND(int pazymys) {
    if (pazymys >= 1 && pazymys <= 10) {
        nd_.push_back(pazymys);
    } else {
        throw std::invalid_argument("Klaida: Netinkamas pazymys");
    }
}

void Studentas::clearND() 
{
    nd_.clear();
}

// Comparison functions
bool compareByVardas(const Studentas& a, const Studentas& b) { 
    return a.vardas() < b.vardas(); 
}

bool compareByPavarde(const Studentas& a, const Studentas& b) { 
    return a.pavarde() < b.pavarde(); 
}

bool compareByGalutinis(const Studentas& a, const Studentas& b) { 
    return a.galutinis() > b.galutinis(); 
}


template <typename Container>
void testuotiDuomenuApdorojimaImpl(const std::string& aplankas, int skaicius) 
{
    Container grupe;
    Container kietiakiai;
    Container vargsai;

    std::cout << "Ar galutinio balo skaiciavimui norite naudoti vidurki ar mediana? (v/m): ";
    char ats;
    std::cin >> ats;
    
    while (ats != 'v' && ats != 'V' && ats != 'm' && ats != 'M') {
        std::cout << "Netinkama ivestis. Bandykite dar karta: ";
        std::cin >> ats;
    }
    
    std::cout << "Kaip norite surusiuoti studentus? (v/vardas, p/pavarde, g/galutinis): ";
    char sortingOption;
    std::cin >> sortingOption;
    while (sortingOption != 'v' && sortingOption != 'V' && sortingOption != 'p' && sortingOption != 'P' && sortingOption != 'g' && sortingOption != 'G') {
        std::cout << "Netinkama ivestis. Bandykite dar karta: ";
        std::cin >> sortingOption;
    }
    
    std::string failoPavadinimas = aplankas + "/studentai_" + std::to_string(skaicius) + ".txt";
    
    Laikas nuskaitymas("Failo nuskaitymas");
    nuskaitymas.pradeti();
    skaitytiIsFailo(grupe, failoPavadinimas);
    nuskaitymas.baigti();
    
    bool naudotiVidurki = (std::tolower(ats) == 'v');
    for (auto& studentas : grupe) {
        studentas.setGalutinis(naudotiVidurki ? studentas.galBalas() : studentas.galBalasMed());
    }
    
    Laikas rikiavimas(std::to_string(skaicius) + " studentu failo rusiavimas");
    rikiavimas.pradeti();
    sortStudentai(grupe, sortingOption);
    rikiavimas.baigti();
    
    Laikas skirstymas(std::to_string(skaicius)+" studentu failo skirstymas i du konteinerius");
    skirstymas.pradeti();
    skirstytiStudentus(grupe, kietiakiai, vargsai);
    skirstymas.baigti();
   

    Laikas atspausdinimas(std::to_string(skaicius)+" studentu atspaudinimas");
    atspausdinimas.pradeti();
    isvestiStudentusIFaila(kietiakiai, aplankas + "/kietiakiai.txt", ats);
    isvestiStudentusIFaila(vargsai, aplankas + "/vargsai.txt", ats);
    atspausdinimas.baigti();
    
    kietiakiai.clear();
    vargsai.clear();
    std::cin.get();
}

void testuotiDuomenuApdorojima(const std::string& aplankas, int skaicius, int testChoice)
{
  std::srand(std::time(0));
    if(testChoice == '2')  
        testuotiDuomenuApdorojimaImpl<std::vector<Studentas>>(aplankas, skaicius);
    else if(testChoice == '3') 
        testuotiDuomenuApdorojimaImpl<Vector<Studentas>>(aplankas, skaicius);
}

template void testuotiDuomenuApdorojimaImpl<std::vector<Studentas>>(const std::string& aplankas, int skaicius);
template void testuotiDuomenuApdorojimaImpl<Vector<Studentas>>(const std::string& aplankas, int skaicius);

void testuotiStudentoMetodus() {
    std::cout << "\n===== STUDENTAS KLASES METODU TESTAVIMAS =====\n" << std::endl;
    
    // Reset counter
    Studentas::destruktoriuSk = 0;
    
    // === 1. Default constructor ===
    std::cout << "[TEST 1] Default konstruktorius\n";
    Studentas s1;
    s1.setVardas("Vanesa");
    s1.setPavarde("Balsyte");
    s1.addND(8);
    s1.addND(9);
    s1.addND(10);
    s1.setEgzaminas(9);
    s1.setGalutinis(s1.galBalas());
    std::cout << "Sukurtas studentas s1: " << s1 << std::endl;
    
    // Validacijos tikrinimai
    assert(!s1.vardas().empty());
    assert(!s1.pavarde().empty());
    assert(!s1.nd().empty());
    assert(s1.egzaminas() >= 0 && s1.egzaminas() <= 10);
    assert(s1.galutinis() >= 0 && s1.galutinis() <= 10);
    for (const auto& pazymys : s1.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    std::cout << "Visi s1 atributai validus\n";
    
    // === 2. Copy constructor ===
    std::cout << "\n[TEST 2] Kopijavimo konstruktorius (s2(s1))\n";
    Studentas s2(s1);
    std::cout << "Originalas s1: " << s1 << std::endl;
    std::cout << "Kopija s2: " << s2 << std::endl;
    
    // Validacijos tikrinimai s2
    assert(!s2.vardas().empty());
    assert(!s2.pavarde().empty());
    assert(!s2.nd().empty());
    assert(s2.egzaminas() >= 0 && s2.egzaminas() <= 10);
    assert(s2.galutinis() >= 0 && s2.galutinis() <= 10);
    for (const auto& pazymys : s2.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Lygumas su originalu
    assert(s2.vardas() == s1.vardas());
    assert(s2.pavarde() == s1.pavarde());
    assert(s2.nd() == s1.nd());
    assert(s2.egzaminas() == s1.egzaminas());
    assert(s2.galutinis() == s1.galutinis());
    std::cout << "Visi s2 atributai validus ir lygus s1\n";
    
    // === 3. Copy assignment ===
    std::cout << "\n[TEST 3] Kopijavimo priskyrimo operatorius (s3 = s1)\n";
    Studentas s3;
    s3 = s1;
    std::cout << "Originalas s1: " << s1 << std::endl;
    std::cout << "Priskirta kopija s3: " << s3 << std::endl;
    
    // Validacijos tikrinimai s3
    assert(!s3.vardas().empty());
    assert(!s3.pavarde().empty());
    assert(!s3.nd().empty());
    assert(s3.egzaminas() >= 0 && s3.egzaminas() <= 10);
    assert(s3.galutinis() >= 0 && s3.galutinis() <= 10);
    for (const auto& pazymys : s3.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Lygumas su originalu
    assert(s3.vardas() == s1.vardas());
    assert(s3.pavarde() == s1.pavarde());
    assert(s3.nd() == s1.nd());
    assert(s3.egzaminas() == s1.egzaminas());
    assert(s3.galutinis() == s1.galutinis());
    std::cout << " Visi s3 atributai validus ir lygus s1\n";
    
    // === 4. Move constructor ===
    std::cout << "\n[TEST 4] Perkelimo konstruktorius (s4(move(Studentas(s1))))\n";
    Studentas tempS1(s1);
    std::cout << "Temp studentas pries perkelima: " << tempS1 << std::endl;
    
    // Išsaugoti tempS1 duomenis prieš perkėlimą
    std::string tempVardas = tempS1.vardas();
    std::string tempPavarde = tempS1.pavarde();
    std::vector<int> tempND = tempS1.nd();
    int tempEgzaminas = tempS1.egzaminas();
    double tempGalutinis = tempS1.galutinis();
    
    Studentas s4(std::move(tempS1));
    std::cout << "Perkeltas objektas s4: " << s4 << std::endl;
    std::cout << "Temp studentas po perkelimo: " << tempS1 << std::endl;
    
    // Validacijos tikrinimai s4 (turi turėti originalius duomenis)
    assert(!s4.vardas().empty());
    assert(!s4.pavarde().empty());
    assert(!s4.nd().empty());
    assert(s4.egzaminas() >= 0 && s4.egzaminas() <= 10);
    assert(s4.galutinis() >= 0 && s4.galutinis() <= 10);
    for (const auto& pazymys : s4.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Patikrinti ar duomenys teisingai perkelti
    assert(s4.vardas() == tempVardas);
    assert(s4.pavarde() == tempPavarde);
    assert(s4.nd() == tempND);
    assert(s4.egzaminas() == tempEgzaminas);
    assert(s4.galutinis() == tempGalutinis);
    
    // tempS1 turėtų būti "ištuštintas"
    assert(tempS1.nd().empty());
    std::cout << "s4 turi visus validus atributus, tempS1 istustintas\n";
    
    // === 5. Move assignment ===
    std::cout << "\n[TEST 5] Perkelimo priskyrimo operatorius (s5 = move(s2))\n";
    
    // Išsaugoti s2 duomenis prieš perkėlimą
    std::string s2Vardas = s2.vardas();
    std::string s2Pavarde = s2.pavarde();
    std::vector<int> s2ND = s2.nd();
    int s2Egzaminas = s2.egzaminas();
    double s2Galutinis = s2.galutinis();
    
    Studentas s5;
    s5 = std::move(s2);
    std::cout << "Perkeltas s5: " << s5 << std::endl;
    std::cout << "s2 po perkelimo: " << s2 << std::endl;
    
    // Validacijos tikrinimai s5 (turi turėti s2 duomenis)
    assert(!s5.vardas().empty());
    assert(!s5.pavarde().empty());
    assert(!s5.nd().empty());
    assert(s5.egzaminas() >= 0 && s5.egzaminas() <= 10);
    assert(s5.galutinis() >= 0 && s5.galutinis() <= 10);
    for (const auto& pazymys : s5.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Patikrinti ar duomenys teisingai perkelti
    assert(s5.vardas() == s2Vardas);
    assert(s5.pavarde() == s2Pavarde);
    assert(s5.nd() == s2ND);
    assert(s5.egzaminas() == s2Egzaminas);
    assert(s5.galutinis() == s2Galutinis);
    
    // s2 turėtų būti "ištuštintas"
    assert(s2.nd().empty());
    std::cout << "s5 turi visus validus atributus, s2 istustintas\n";

    // === 6.1 Input operator (istringstream) ===
    std::cout << "\n[TEST 6.1] Ivesties operatorius (>> is istringstream)\n";
    std::istringstream iss("Vardenis Pavardenis 10 10 10 10 10");
    Studentas s6;
    iss >> s6;
    s6.setGalutinis(s6.galBalas());
    std::cout << "Ivestas s6: " << s6 << std::endl;
    
    // Validacijos tikrinimai s6
    assert(!s6.vardas().empty());
    assert(!s6.pavarde().empty());
    assert(!s6.nd().empty());
    assert(s6.egzaminas() >= 0 && s6.egzaminas() <= 10);
    assert(s6.galutinis() >= 0 && s6.galutinis() <= 10);
    for (const auto& pazymys : s6.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Specifiniai duomenys
    assert(s6.vardas() == "Vardenis");
    assert(s6.pavarde() == "Pavardenis");
    assert(s6.egzaminas() == 10);
    std::cout << "Visi s6 atributai validus ir teisingi\n";
    
    // === 6.2 Input from user ===
    std::cout << "\n[TEST 6.2] Ivestis is cin (>>)\n";
    std::cout << "Iveskite studenta (vardas pavarde ND1 ND2... egzaminas) vienoje eiluteje: ";
    Studentas s7;
    std::cin >> s7;
    s7.setGalutinis(s7.galBalas());
    std::cout << "Ivestas s7: " << s7 << std::endl;
    
    // Validacijos tikrinimai s7
    assert(!s7.vardas().empty());
    assert(!s7.pavarde().empty());
    assert(!s7.nd().empty());
    assert(s7.egzaminas() >= 0 && s7.egzaminas() <= 10);
    assert(s7.galutinis() >= 0 && s7.galutinis() <= 10);
    for (const auto& pazymys : s7.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    std::cout << "Visi s7 atributai validus\n";
    
    // === 6.3 Input from file with operator>> ===
    std::cout << "\n[TEST 6.3] Ivestis is failo su operator>>\n";
    std::ofstream failin("failinis.txt");
    failin << "Antanas Antanaitis 9 8 7 6 5" << std::endl;
    failin.close();

    std::ifstream failIn("failinis.txt");
    Studentas s8;
    s8.setVardas("Jonas");
    failIn >> s8;
    failIn.close();
    s8.setGalutinis(s8.galBalas());
    std::cout << "Is failo ivestas s8: " << s8 << std::endl;
    
    // Validacijos tikrinimai s8
    assert(!s8.vardas().empty());
    assert(!s8.pavarde().empty());
    assert(!s8.nd().empty());
    assert(s8.egzaminas() >= 0 && s8.egzaminas() <= 10);
    assert(s8.galutinis() >= 0 && s8.galutinis() <= 10);
    for (const auto& pazymys : s8.nd()) {
        assert(pazymys >= 0 && pazymys <= 10);
    }
    // Specifiniai duomenys
    assert(s8.vardas() == "Antanas");
    assert(s8.pavarde() == "Antanaitis");
    assert(s8.egzaminas() == 5);
    std::cout << " Visi s8 atributai validus ir teisingi\n";
    
    // === 7 Output to stringstream (<<) ===
    std::cout << "\n[TEST 7] Isvedimas i stringstream (<<)\n";
    std::ostringstream oss2;
    oss2 << s8;
    std::cout << "stringstream rezultatas: " << oss2.str() << std::endl;
    
    // Validacijos tikrinimai - patikrinti ar išvestyje yra visi duomenys
    assert(oss2.str().find("Antanas") != std::string::npos);
    assert(oss2.str().find("Antanaitis") != std::string::npos);
    assert(oss2.str().find(std::to_string(s8.galutinis()).substr(0,4)) != std::string::npos);
    std::cout << " Isvestyje yra visi reikalingi duomenys\n";
    
    
    // === 8. Destructor test ===
    std::cout << "\n[TEST 10] Destruktoriaus kvietimo testas\n";
    int priesDestruktoriu = Studentas::destruktoriuSk;
    {
        Studentas laikinas;
        laikinas.setVardas("Laikinas");
        laikinas.setPavarde("Objektas");
        laikinas.addND(5);
        laikinas.setEgzaminas(7);
        laikinas.setGalutinis(laikinas.galBalas());
        
        // Validacijos tikrinimai laikinam objektui
        assert(!laikinas.vardas().empty());
        assert(!laikinas.pavarde().empty());
        assert(!laikinas.nd().empty());
        assert(laikinas.egzaminas() >= 0 && laikinas.egzaminas() <= 10);
        assert(laikinas.galutinis() >= 0 && laikinas.galutinis() <= 10);
        std::cout << "Laikinas objektas validus\n";
    } // Čia turėtų būti iškvietas destruktorius
    
    int poDestruktoriaus = Studentas::destruktoriuSk;
    assert(poDestruktoriaus > priesDestruktoriu);
    std::cout << "Destruktorius sekmingai iskviestas\n";
    std::cout << "Destruktoriaus kvietimu skaicius: " << Studentas::destruktoriuSk << std::endl;
    
    std::cout << "\n===== TESTAVIMAS SEKMINGAI BAIGTAS =====\n" << std::endl;
    std::cout << "Visi testai praejo sekmingai\n";
    std::cout << "Visi objektu atributai buvo patikrinti ir rasti validus\n";
    }

void testuotiZmogausKlase() 
{
    std::cout << "\n===== ZMOGUS KLASES TESTAVIMAS =====\n" << std::endl;
    
    try {
        //std::cout << "[TEST] Bandymas sukurti Zmogus objekta tiesiogiai\n";
        //Atkomentuoti, kad tikrinti
        //Zmogus z; 
    }
    catch (const std::exception& e) {
        std::cerr << "Klaida testuojant Zmogus klase: " << e.what() << std::endl;
    }
}