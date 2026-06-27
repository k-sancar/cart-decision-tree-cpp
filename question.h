#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

inline const std::vector<std::string> SYMPTOM_NAMES = {
    "runny nose", "cough", "strong cough", "sore throat", "fever",
    "high fever", "fatigue", "dizziness", "nausea", "headache",
    "strong muscle pain", "trouble concentrating", "itchy eyes", "dry mouth",
    "chest pain", "shortness of breath", "sweating", "chills", "vomiting", "anxiety"
};


class Question {
public:
    Question();  // default constructor
    Question(int column_index);

    bool match(const std::vector<int>& features) const;
    std::string toString() const;

    int getColumn() const;
    int getValue() const;

private:
    int column_;   // Index of the feature (symptom)
};

#endif
