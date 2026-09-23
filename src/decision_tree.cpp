#include "decision_tree.h"
#include "node.h"
#include <set>
#include <unordered_map>
#include <iostream>
#include <algorithm>

DecisionTree::DecisionTree() : root_(nullptr) {}

void DecisionTree::fit(const std::vector<Diagnosis>& data) {
    std::vector<Sample> samples;
    samples.reserve(data.size());

    for (const auto& d : data) {
        samples.push_back({d.toVector(), d.diagnosis});
    }

    root_ = build(samples, 0);
}

std::string DecisionTree::predict(const Diagnosis& sample, float confidence_threshold) const {
    if (!root_) return "Unknown";

    const Node* node = root_.get();
    while (!node->isLeaf()) {
        const auto* decision_node = dynamic_cast<const DecisionNode*>(node);
        if (!decision_node) break; // safety fallback
        if (decision_node->getQuestion().match(sample.toVector())) {
            node = decision_node->getTrueBranch();
        } else {
            node = decision_node->getFalseBranch();
        }
    }

    const auto* leaf = dynamic_cast<const LeafNode*>(node);
    if (!leaf) return "Unknown";

    if (leaf->getConfidence() < confidence_threshold) {
        return "Uncertain";
    }

    return leaf->classify(sample.toVector());
}


float DecisionTree::gini(const std::vector<Sample>& samples) const {
    std::unordered_map<std::string, int> label_counts;
    for (const auto& s : samples) label_counts[s.label]++;

    float impurity = 1.0f;
    int total = samples.size();

    for (const auto& [label, count] : label_counts) {
        float p = static_cast<float>(count) / total;
        impurity -= p * p;
    }

    return impurity;
}

float DecisionTree::infoGain(const std::vector<Sample>& left, const std::vector<Sample>& right) const 
{
    float p = static_cast<float>(left.size()) / (left.size() + right.size());
    return 1 - p * gini(left) - (1 - p) * gini(right);
}

std::pair<std::vector<Sample>, std::vector<Sample>> 
DecisionTree::partition(const std::vector<Sample>& samples, const Question& question) const {
    std::vector<Sample> true_branch, false_branch;
    true_branch.reserve(samples.size() / 2);
    false_branch.reserve(samples.size() / 2);

    for (const auto& sample : samples) {
        if (question.match(sample.features)) {
            true_branch.push_back(sample);
        } else {
            false_branch.push_back(sample);
        }
    }

    return {true_branch, false_branch};
}

std::pair<float, Question> DecisionTree::findBestSplit(const std::vector<Sample>& samples) const {
    float best_gain = 0.0f;
    Question best_question;

    int feature_count = samples[0].features.size();

    for (int col = 0; col < feature_count; ++col) {
        std::set<int> unique_vals;
        for (const auto& s : samples) {
            unique_vals.insert(s.features[col]);
        }

        for (int val : unique_vals) {
            Question q(col);
            auto [true_rows, false_rows] = partition(samples, q);

            if (true_rows.empty() || false_rows.empty()) continue;

            float gain = infoGain(true_rows, false_rows);

            if (gain > best_gain) {
                best_gain = gain;
                best_question = q;
            }
        }
    }

    return {best_gain, best_question};
}

void DecisionTree::setMinDepth(int depth) {
    min_depth_ = depth;
}


std::unique_ptr<Node> DecisionTree::build(const std::vector<Sample>& samples, int current_depth) {
    auto [gain, question] = findBestSplit(samples);

    std::vector<std::string> labels;
    for (const auto& s : samples) labels.push_back(s.label);

    bool should_stop = gain == 0 || samples.size() <= min_leaf_size_;

    if (should_stop && current_depth >= min_depth_) {
        return std::make_unique<LeafNode>(labels);
    }

    if (gain > 0) {
        auto [true_rows, false_rows] = partition(samples, question);

        auto true_branch  = build(true_rows,  current_depth + 1);
        auto false_branch = build(false_rows, current_depth + 1);
        return std::make_unique<DecisionNode>(question, std::move(true_branch), std::move(false_branch));
    }

    // No good split, not enough data: still must build leaf
    /////////////////////////////
    return std::make_unique<LeafNode>(labels);
}


void DecisionTree::setMinLeafSize(int size) {
    min_leaf_size_ = size;
}


const Node* DecisionTree::getRoot() const {
    return root_.get();
}

