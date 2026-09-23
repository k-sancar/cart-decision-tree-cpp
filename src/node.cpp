#include "node.h"
#include <algorithm>

// ---------- LeafNode Implementation ----------

LeafNode::LeafNode(const std::vector<std::string>& labels) {
    for (const auto& label : labels) {
        predictions_[label]++;
    }
}

LeafNode::LeafNode(const std::unordered_map<std::string, int>& predictions)
    : predictions_(predictions) {}

std::string LeafNode::classify(const std::vector<int>&) const {
    auto it = std::max_element(predictions_.begin(), predictions_.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
    return (it != predictions_.end()) ? it->first : "Unknown";
}

bool LeafNode::isLeaf() const {
    return true;
}

const std::unordered_map<std::string, int>& LeafNode::getPredictions() const {
    return predictions_;
}

float LeafNode::getConfidence() const {
    int total = 0;
    int max_count = 0;
    for (const auto& [label, count] : predictions_) {
        total += count;
        if (count > max_count) max_count = count;
    }
    return total > 0 ? static_cast<float>(max_count) / total : 0.0f;
}

// ---------- DecisionNode Implementation ----------

DecisionNode::DecisionNode(Question question,
                           std::unique_ptr<Node> true_branch,
                           std::unique_ptr<Node> false_branch)
    : question_(std::move(question)),
      true_branch_(std::move(true_branch)),
      false_branch_(std::move(false_branch)) {}

std::string DecisionNode::classify(const std::vector<int>& features) const {
    if (question_.match(features)) {
        return true_branch_->classify(features);
    } else {
        return false_branch_->classify(features);
    }
}

bool DecisionNode::isLeaf() const {
    return false;
}

const Question& DecisionNode::getQuestion() const {
    return question_;
}

const Node* DecisionNode::getTrueBranch() const {
    return true_branch_.get();
}

const Node* DecisionNode::getFalseBranch() const {
    return false_branch_.get();
}

void DecisionNode::setTrueBranch(std::unique_ptr<Node> node) {
    true_branch_ = std::move(node);
}
void DecisionNode::setFalseBranch(std::unique_ptr<Node> node) {
    false_branch_ = std::move(node);
}