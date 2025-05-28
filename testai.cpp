#include <gtest/gtest.h>
#include "funkcijos.h"
#include "zmogus.h"


class StudentasTest : public ::testing::Test {
protected:
    
    void SetUp() override {
        // Reset static counter before each test
        Studentas::destruktoriuSk = 0;

        
        testStudent.setVardas("Test");
        testStudent.setPavarde("Student");
        testStudent.addND(8);
        testStudent.addND(9);
        testStudent.addND(10);
        testStudent.setEgzaminas(9);
        testStudent.setGalutinis(testStudent.galBalas());
    }

    Studentas testStudent;
};

//Default konstruktorius
TEST_F(StudentasTest, DefaultConstructor)
 {
    Studentas s;
    
    EXPECT_EQ(s.vardas(), "");
    EXPECT_EQ(s.pavarde(), "");
    EXPECT_TRUE(s.nd().empty());
    EXPECT_EQ(s.egzaminas(), 0);
    EXPECT_EQ(s.galutinis(), 0.0);
}

//Kopijavimo konstruktorius
TEST_F(StudentasTest, CopyConstructor) {
    
    Studentas copy(testStudent);
    
    
    EXPECT_EQ(copy.vardas(), testStudent.vardas());
    EXPECT_EQ(copy.pavarde(), testStudent.pavarde());
    EXPECT_EQ(copy.nd().size(), testStudent.nd().size());
    
    
    for (size_t i = 0; i < testStudent.nd().size(); i++) 
    {
        EXPECT_EQ(copy.nd()[i], testStudent.nd()[i]);
    }
    
    EXPECT_EQ(copy.egzaminas(), testStudent.egzaminas());
    EXPECT_EQ(copy.galutinis(), testStudent.galutinis());
    
    
    copy.setVardas("Modified");
    EXPECT_NE(copy.vardas(), testStudent.vardas());
}

//Kopijavimo priskyrimo operatorius
TEST_F(StudentasTest, CopyAssignmentOperator) {
    Studentas s;
    s = testStudent;
    
    EXPECT_EQ(s.vardas(), testStudent.vardas());
    EXPECT_EQ(s.pavarde(), testStudent.pavarde());
    EXPECT_EQ(s.nd().size(), testStudent.nd().size());
    EXPECT_EQ(s.egzaminas(), testStudent.egzaminas());
    EXPECT_EQ(s.galutinis(), testStudent.galutinis());
    
    testStudent = testStudent;
    EXPECT_EQ(testStudent.vardas(), "Test");
}

//Perkelimo konstruktorius
TEST_F(StudentasTest, MoveConstructor) 
{
    
    Studentas tempStudent = testStudent;
    
    
    Studentas movedStudent(std::move(tempStudent));
    
 
    EXPECT_EQ(movedStudent.vardas(), "Test");
    EXPECT_EQ(movedStudent.pavarde(), "Student");
    EXPECT_EQ(movedStudent.nd().size(), 3);
    EXPECT_EQ(movedStudent.egzaminas(), 9);
    

    EXPECT_TRUE(tempStudent.nd().empty());
}

//Perskirimo priskyrimo operatorius
TEST_F(StudentasTest, MoveAssignmentOperator) {
    
    Studentas tempStudent = testStudent;
    
    
    Studentas target;
    target = std::move(tempStudent);
    
   
    EXPECT_EQ(target.vardas(), "Test");
    EXPECT_EQ(target.pavarde(), "Student");
    EXPECT_EQ(target.nd().size(), 3);
    EXPECT_EQ(target.egzaminas(), 9);
    
    
    EXPECT_TRUE(tempStudent.nd().empty());
    
    
    Studentas& refStudent = testStudent;
    testStudent = std::move(refStudent); 
    EXPECT_EQ(testStudent.vardas(), "Test"); 
}

//Destruktorius ir jo skaiciavimas
TEST_F(StudentasTest, Destructor) {
    int initialCount = Studentas::destruktoriuSk;
    
    {
        Studentas s1;
        Studentas s2;
    } 
    
    EXPECT_EQ(Studentas::destruktoriuSk, initialCount + 2);
}

TEST_F(StudentasTest, GradeCalculation) 
{
    // Sukuriam testinį studentą ir nustatom egzamino pažymį
    Studentas testStudent;
    testStudent.setEgzaminas(9);
    testStudent.addND(9);
    testStudent.addND(9);

    // Tikrinam vidurkį
    EXPECT_DOUBLE_EQ(Studentas::skaiciuotiVid(testStudent.nd()), 9.0);

    // Tikrinam medianą (reikia analogiškos funkcijos kaip skaiciuotiVid)
    EXPECT_DOUBLE_EQ(Studentas::skaiciuotiMed(testStudent.nd()), 9.0);

    // Pridedam dar vieną pažymį — dabar nd: {9, 9, 7} → mediana = 9
    testStudent.addND(7);
    EXPECT_DOUBLE_EQ(Studentas::skaiciuotiMed(testStudent.nd()), 9.0);

    // Galutinis pažymys su vidurkiu:
    double expectedVid = Studentas::skaiciuotiVid(testStudent.nd());  // (9+9+7)/3 = 8.33...
    double expectedGalBalas = 0.4 * expectedVid + 0.6 * testStudent.egzaminas();
    EXPECT_NEAR(testStudent.galBalas(), expectedGalBalas, 1e-2);

    // Galutinis pažymys su mediana: 9
    double expectedMed = Studentas::skaiciuotiMed(testStudent.nd());
    double expectedGalBalasMed = 0.4 * expectedMed + 0.6 * testStudent.egzaminas();
    EXPECT_DOUBLE_EQ(testStudent.galBalasMed(), expectedGalBalasMed);

    // Testas su tuščiu studentu
    Studentas emptyStudent;
    EXPECT_THROW(Studentas::skaiciuotiVid(emptyStudent.nd()), std::runtime_error);
    EXPECT_THROW(Studentas::skaiciuotiMed(emptyStudent.nd()), std::runtime_error);

    // Netinkami pažymiai
    EXPECT_THROW(testStudent.addND(0), std::invalid_argument);
    EXPECT_THROW(testStudent.addND(11), std::invalid_argument);
}

//Ivesties ir isvesties operatoriai
TEST_F(StudentasTest, InputOutput) 
{
    //isvesties operatorius
    std::ostringstream oss;
    oss << testStudent;
    std::string output = oss.str();
    
    EXPECT_TRUE(output.find("Test") != std::string::npos);
    EXPECT_TRUE(output.find("Student") != std::string::npos);
    
    //ivestis is string
    std::istringstream iss("John Doe 8 9 10 7");
    Studentas s;
    iss >> s;
    
    EXPECT_EQ(s.vardas(), "John");
    EXPECT_EQ(s.pavarde(), "Doe");
    EXPECT_EQ(s.nd().size(), 3);
    EXPECT_EQ(s.egzaminas(), 7);
}




int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}