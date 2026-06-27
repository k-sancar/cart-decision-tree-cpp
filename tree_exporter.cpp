#include "tree_exporter.h"
#include "node.h"
#include "question.h"

#include <fstream>
#include <map>
#include <iostream>

void TreeExporter::exportToCSV(const Node* root, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return;
    }

    out << "NodeID,IsLeaf,Column,Distribution,TrueBranch,FalseBranch\n";

    int nextId = 1;
    std::map<const Node*, int> nodeMap;
    writeNode(root, out, nextId, nodeMap);

    out.close();
}

int TreeExporter::writeNode(const Node* node, std::ofstream& out, int& nextId,
                            std::map<const Node*, int>& nodeMap) {
    if (!node) return -1;
    if (nodeMap.count(node)) return nodeMap[node];

    int currentId = nextId++;
    nodeMap[node] = currentId;

    if (node->isLeaf()) {
        const LeafNode* leaf = dynamic_cast<const LeafNode*>(node);
        std::string dist;
        for (const auto& [label, count] : leaf->getPredictions()) {
            if (!dist.empty()) dist += ";";
            dist += label + ":" + std::to_string(count);
        }
        out << currentId << ",1,,\"" << dist << "\",,\n";
    } else {
        auto* decision = dynamic_cast<const DecisionNode*>(node);
        const Question& q = decision->getQuestion();

        int trueId  = writeNode(decision->getTrueBranch(),  out, nextId, nodeMap);
        int falseId = writeNode(decision->getFalseBranch(), out, nextId, nodeMap);

        out << currentId << ",0," << q.getColumn() << ",," << trueId << "," << falseId << "\n";
    }

    return currentId;
}
