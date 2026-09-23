#ifndef TREE_EXPORTER_H
#define TREE_EXPORTER_H

#include "node.h"
#include <string>
#include <map>
#include <fstream>

class TreeExporter {
public:
    static void exportToCSV(const Node* root, const std::string& filename);

private:
    static int writeNode(const Node* node,
                         std::ofstream& out,
                         int& nextId,
                         std::map<const Node*, int>& nodeMap);
};

#endif // TREE_EXPORTER_H
