    #include "diagnosis.h"
    #include <iostream>
    #include <algorithm>
    #include <random>

    using namespace sqlite_orm;

    Diagnosis::Diagnosis() = default;

    Diagnosis::Diagnosis(int id, int runny_nose, int cough, int strong_cough, int sore_throat,
      int fever, int high_fever, int fatigue, int diziness, int nausea,
      int headache, int strong_muscle_pain, int trouble_concentrating,
      int itchy_eyes, int dry_mouth, int chest_pain, int shortness_of_breath, 
      int sweating, int chills, int vomiting, int anxiety,
      std::string diagnosis) :
    id(id), runny_nose(runny_nose), cough(cough), strong_cough(strong_cough),
    sore_throat(sore_throat), fever(fever), high_fever(high_fever),
    fatigue(fatigue), diziness(diziness), nausea(nausea), headache(headache),
    strong_muscle_pain(strong_muscle_pain), trouble_concentrating(trouble_concentrating),
    itchy_eyes(itchy_eyes), dry_mouth(dry_mouth), chest_pain(chest_pain),
    shortness_of_breath(shortness_of_breath), sweating(sweating), chills(chills),
    vomiting(vomiting), anxiety(anxiety), diagnosis(std::move(diagnosis)) {}


    std::vector<int> Diagnosis::toVector() const {
        return {
            runny_nose, cough, strong_cough, sore_throat, fever,
            high_fever, fatigue, diziness, nausea, headache,
            strong_muscle_pain, trouble_concentrating, itchy_eyes, dry_mouth,
            chest_pain, shortness_of_breath, sweating, chills, vomiting, anxiety
        };
    }



    Storage initStorage(const std::string& path) {
    return make_storage(path,
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
    );
}


    DiagnosisRepositoryORM::DiagnosisRepositoryORM(const std::string& dbPath)
        : storage(initStorage(dbPath)) {
        storage.sync_schema();
    }

    std::vector<Diagnosis> DiagnosisRepositoryORM::getAll() {
        return storage.get_all<Diagnosis>();
    }

    void DiagnosisRepositoryORM::add(const Diagnosis& d) {
        storage.insert(d);
    }

    std::vector<Diagnosis> DiagnosisRepositoryORM::getByName(const std::string& name) {
        return storage.get_all<Diagnosis>(where(c(&Diagnosis::diagnosis) == name));
    }

    void DiagnosisRepositoryORM::remove(int id) {
        storage.remove<Diagnosis>(id);
    }

    void DiagnosisRepositoryORM::update(const Diagnosis& d) {
        storage.update(d);
    }

    std::pair<std::vector<Diagnosis>, std::vector<Diagnosis>> DiagnosisRepositoryORM::splitTrainTest(float test_ratio) {
    auto all = getAll();

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all.begin(), all.end(), g);

    size_t test_size = static_cast<size_t>(all.size() * test_ratio);
    std::vector<Diagnosis> test(all.begin(), all.begin() + test_size);
    std::vector<Diagnosis> train(all.begin() + test_size, all.end());

    return {train, test};
    }


