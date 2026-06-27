#ifndef TREE_IMPORTER_H
#define TREE_IMPORTER_H

#include "node.h"
#include <memory>
#include <string>

class TreeImporter {
public:
    static std::unique_ptr<Node> importFromCSV(const std::string& filename);
};

#endif // TREE_IMPORTER_H

