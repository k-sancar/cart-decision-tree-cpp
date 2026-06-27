#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <string>
#include <vector>
#include "sqlite_orm.h"

using namespace sqlite_orm;

class Diagnosis {
public:
    int id;
    int runny_nose;
    int cough;
    int strong_cough;
    int sore_throat;
    int fever;
    int high_fever;
    int fatigue;
    int diziness;
    int nausea;
    int headache;
    int strong_muscle_pain;
    int trouble_concentrating;
    int itchy_eyes;
    int dry_mouth;  
    int chest_pain;
    int shortness_of_breath;
    int sweating;
    int chills;
    int vomiting;
    int anxiety;
    std::string diagnosis;

    Diagnosis();
    Diagnosis(int id, int runny_nose, int cough, int strong_cough, int sore_throat,
      int fever, int high_fever, int fatigue, int diziness, int nausea,
      int headache, int strong_muscle_pain, int trouble_concentrating,
      int itchy_eyes, int dry_mouth, int chest_pain, int shortness_of_breath, 
      int sweating, int chills, int vomiting, int anxiety,
      std::string diagnosis);
              
    std::vector<int> toVector() const;
};


using Storage = decltype(make_storage("",
    make_table("DIAGNOSIS",
        make_column("id", &Diagnosis::id, primary_key()),
        make_column("runny_nose", &Diagnosis::runny_nose),
        make_column("cough", &Diagnosis::cough),
        make_column("strong_cough", &Diagnosis::strong_cough),
        make_column("sore_throat", &Diagnosis::sore_throat),
        make_column("fever", &Diagnosis::fever),
        make_column("high_fever", &Diagnosis::high_fever),
        make_column("fatigue", &Diagnosis::fatigue),
        make_column("diziness", &Diagnosis::diziness),
        make_column("nausea", &Diagnosis::nausea),
        make_column("headache", &Diagnosis::headache),
        make_column("strong_muscle_pain", &Diagnosis::strong_muscle_pain),
        make_column("trouble_concentrating", &Diagnosis::trouble_concentrating),
        make_column("itchy_eyes", &Diagnosis::itchy_eyes),
        make_column("dry_mouth", &Diagnosis::dry_mouth),
        make_column("chest_pain", &Diagnosis::chest_pain),
        make_column("shortness_of_breath", &Diagnosis::shortness_of_breath),
        make_column("sweating", &Diagnosis::sweating),
        make_column("chills", &Diagnosis::chills),
        make_column("vomiting", &Diagnosis::vomiting),
        make_column("anxiety", &Diagnosis::anxiety),
        make_column("diagnosis", &Diagnosis::diagnosis)
    )
));

Storage initStorage(const std::string& path);


class DiagnosisRepositoryORM {
private:
    Storage storage;

public:
    DiagnosisRepositoryORM(const std::string& dbPath);

    std::vector<Diagnosis> getAll();
    void add(const Diagnosis& d);
    std::vector<Diagnosis> getByName(const std::string& name);
    void remove(int id);
    void update(const Diagnosis& d);
    std::pair<std::vector<Diagnosis>, std::vector<Diagnosis>> splitTrainTest(float test_ratio = 0.2f);
};
#endif