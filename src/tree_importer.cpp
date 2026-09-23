#include "tree_importer.h"
#include "node.h"
#include "question.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <stdexcept>

static std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    auto start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

std::unique_ptr<Node> TreeImporter::importFromCSV(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    std::getline(in, line); // skip header

    std::map<int, std::unique_ptr<Node>> nodeMap;
    std::map<int, std::pair<int, int>> pending;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string tok;

        std::getline(ss, tok, ','); int id = std::stoi(trim(tok));
        std::getline(ss, tok, ','); bool isLeaf = (trim(tok) == "1");
        std::getline(ss, tok, ','); int col = tok.empty() ? -1 : std::stoi(trim(tok));
        std::getline(ss, tok, ','); std::string dist = trim(tok);
        std::getline(ss, tok, ','); int tId = tok.empty() ? -1 : std::stoi(trim(tok));
        std::getline(ss, tok, ','); int fId = tok.empty() ? -1 : std::stoi(trim(tok));

        if (isLeaf) {
            std::unordered_map<std::string, int> preds;
            std::stringstream dss(dist);
            std::string entry;
            while (std::getline(dss, entry, ';')) {
                size_t pos = entry.find(':');
                if (pos == std::string::npos) continue;
                std::string label = entry.substr(0, pos);
                int count = std::stoi(entry.substr(pos + 1));
                preds[label] = count;
            }
            nodeMap[id] = std::make_unique<LeafNode>(preds);
        } else {
            if (col < 0 || col >= static_cast<int>(SYMPTOM_NAMES.size()))
                throw std::runtime_error("Invalid column for decision node ID " + std::to_string(id));
            Question q(col);
            nodeMap[id] = std::make_unique<DecisionNode>(q, nullptr, nullptr);
            pending[id] = {tId, fId};
        }
    }

    std::map<int, Node*> raw;
    for (auto& [i, up] : nodeMap)
        raw[i] = up.get();

    for (auto& [id, children] : pending) {
        auto* dn = dynamic_cast<DecisionNode*>(raw[id]);
        if (!dn) throw std::runtime_error("Expected DecisionNode at ID " + std::to_string(id));

        int tId = children.first, fId = children.second;
        if (nodeMap.count(tId) == 0 || nodeMap.count(fId) == 0)
            throw std::runtime_error("Missing branch for node " + std::to_string(id));

        dn->setTrueBranch(std::move(nodeMap[tId]));
        dn->setFalseBranch(std::move(nodeMap[fId]));
    }

    if (!nodeMap.count(1))
        throw std::runtime_error("Root node ID=1 missing");
    return std::move(nodeMap[1]);
}
