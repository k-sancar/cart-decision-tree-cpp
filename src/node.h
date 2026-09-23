#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "question.h"

// Abstract base class
class Node {
public:
    virtual ~Node() = default;
    virtual std::string classify(const std::vector<int>& features) const = 0;
    virtual bool isLeaf() const = 0;
};

// ---------------------
// LeafNode Declaration
// ---------------------
class LeafNode : public Node {
public:
    explicit LeafNode(const std::vector<std::string>& labels);
    explicit LeafNode(const std::unordered_map<std::string, int>& predictions);

    std::string classify(const std::vector<int>& features) const override;
    bool isLeaf() const override;
    const std::unordered_map<std::string, int>& getPredictions() const;
    float getConfidence() const;


private:
    std::unordered_map<std::string, int> predictions_;
};

// -------------------------
// DecisionNode Declaration
// -------------------------
class DecisionNode : public Node {
public:
    DecisionNode(Question question,
                 std::unique_ptr<Node> true_branch,
                 std::unique_ptr<Node> false_branch);

    std::string classify(const std::vector<int>& features) const override;
    bool isLeaf() const override;

    const Question& getQuestion() const;

    const Node* getTrueBranch() const;   // NEW
    const Node* getFalseBranch() const;

    void setTrueBranch(std::unique_ptr<Node> node);
    void setFalseBranch(std::unique_ptr<Node> node);

private:
    Question question_;
    std::unique_ptr<Node> true_branch_;
    std::unique_ptr<Node> false_branch_;
};

#endif

