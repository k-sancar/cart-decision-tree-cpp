#include "question.h"
#include <sstream>

Question::Question() : column_(0) {}

Question::Question(int column_index)
    : column_(column_index) {}

bool Question::match(const std::vector<int>& features) const {
    return features[column_] == 1;
}

std::string Question::toString() const {
    std::ostringstream oss;
    if (column_ >= 0 && column_ < static_cast<int>(SYMPTOM_NAMES.size()))
        oss << "Do you have " << SYMPTOM_NAMES[column_] << "?";
    else
        oss << "Unknown symptom column";
    return oss.str();
}

int Question::getColumn() const {
    return column_;
}

