#include "funkcijos.h"
#include "laikas.h"
#include "zmogus.h"
#include "vector.h"
//---
int main() 
{
    try {
        
        char testChoice;

        std::cout << "Pasirinkite ka norite daryti:\n";
        std::cout << "1 - Vykdyti Studentas klases metodu testa\n";
        std::cout << "2 - Programa su std::Vector\n";
        std::cout << "3 - Programa su Vector\n";
        std::cout << "4 - Spartos analize\n";
        std::cout << "Kitas- programos pabaiga\n";
        std::cout << "Pasirinkimas: ";
        std::cin >> testChoice;

        if (tolower(testChoice) == '1') 
        {
            testuotiStudentoMetodus();
            std::cin.get();
            return 0;
        }
        
        else if (tolower(testChoice) =='2' ||  tolower(testChoice) == '3') 

        {
        std::string aplankas = "test_files";
        int skaicius;
        int pasirinktasDydis;

        std::cout << "Pasirinkite failo dydi testavimui:\n";
        std::cout << "1 - 1000 studentu\n";
        std::cout << "2 - 10000 studentu\n";
        std::cout << "3 - 100000 studentu\n";
        std::cout << "4 - 1000000 studentu\n";
        std::cout << "5 - 10000000 studentu\n";
        std::cout << "Kitas- programos pabaiga\n";
        std::cout << "Pasirinkimas: ";
        std::cin >> pasirinktasDydis;

        switch (pasirinktasDydis) {
            case 1:
                skaicius = 1000;
                break;
            case 2:
                skaicius = 10000;
                break;
            case 3:
                skaicius = 100000;
                break;
            case 4:
                skaicius = 1000000;
                break;
            case 5:
                skaicius = 10000000;
                break;
            default:
                std::cout << "Neteisingas pasirinkimas" << std::endl;
                return 1;
        }
        
        testuotiDuomenuApdorojima(aplankas, skaicius, testChoice);
        std::cin.get();
        return 0;
       }
       
        else if (tolower(testChoice) == '4') 
        {
          std::string aplankas = "test_files";
        int skaicius;
        int pasirinktasDydis;

        std::cout << "Pasirinkite failo dydi testavimui:\n";
        std::cout << "1 - 10000 studentu\n";
        std::cout << "2 - 100000 studentu\n";
        std::cout << "3 - 1000000 studentu\n";
        std::cout << "4 - 10000000 studentu\n";
        std::cout << "5 - 100000000 studentu\n";
        std::cout << "Kitas- programos pabaiga\n";
        std::cout << "Pasirinkimas: ";
        std::cin >> pasirinktasDydis;

        switch (pasirinktasDydis) {
            case 1:
                skaicius = 10000;
                break;
            case 2:
                skaicius = 100000;
                break;
            case 3:
                skaicius = 1000000;
                break;
            case 4:
                skaicius = 10000000;
                break;
            case 5:
                skaicius = 100000000;
                break;
            default:
                std::cout << "Neteisingas pasirinkimas" << std::endl;
                return 1;
        }
        
        srand(time(0));

        Laikas nuskaitymasv1("Failo nuskaitymas v1");
        nuskaitymasv1.pradeti();
        unsigned int sz = skaicius; 
        size_t std_reallocs = 0;
    {
        std::vector<int> v1;
        size_t last_capacity = v1.capacity();
        for (unsigned int i = 1; i <= sz; ++i) {
            v1.push_back(i);
            if (v1.capacity() != last_capacity) {
                std_reallocs++;
                last_capacity = v1.capacity();
            }
        }
    }
    std::cout << "std::vector perskirstymu skaicius: " << std_reallocs << "\n";

        nuskaitymasv1.baigti();

        Laikas nuskaitymasv2("Failo nuskaitymas v2");
        nuskaitymasv2.pradeti();
       Vector<int> v2;
    for (unsigned int i = 1; i <= sz; ++i) {
        v2.push_back(i);
    }
    std::cout << "Vector perskirstymu skaicius: " << v2.reallocations() << "\n";
        nuskaitymasv2.baigti();
        std::cin.get();
        return 0;
        }

        else 
        {
            std::cout <<"Programos pabaiga" << std::endl;
            std::cin.get();
            return 1;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ivyko klaida: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    }
    catch (...) {
        std::cerr << "Nezinoma klaida." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cin.get();
    return 0;
}