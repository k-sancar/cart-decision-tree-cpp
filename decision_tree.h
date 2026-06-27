#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "diagnosis.h"
#include "question.h"
#include "node.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

struct Sample {
    std::vector<int> features;
    std::string label;
};

class DecisionTree {
public:
    DecisionTree();

    void fit(const std::vector<Diagnosis>& data);
    std::string predict(const Diagnosis& sample, float confidence_threshold = 0.6f) const;

    const Node* getRoot() const;
    void setMinDepth(int depth); 
    void setMinLeafSize(int size);


private:
    std::unique_ptr<Node> root_;
    int min_depth_ = 0;
    std::ofstream log_file_;

    std::unique_ptr<Node> build(const std::vector<Sample>& samples, int current_depth = 0);
    int min_leaf_size_ = 5;

    float gini(const std::vector<Sample>& samples) const;
    float infoGain(const std::vector<Sample>& left, const std::vector<Sample>& right) const;
    std::pair<std::vector<Sample>, std::vector<Sample>> partition(const std::vector<Sample>& samples, const Question& question) const;
    std::pair<float, Question> findBestSplit(const std::vector<Sample>& samples) const;
    void openLogFile(const std::string& filename);
    void closeLogFile();
};


#endif
