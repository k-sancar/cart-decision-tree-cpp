#include "decision_tree.h"
#include "diagnosis.h"
#include "tree_exporter.h"
#include "tree_importer.h"
#include <iostream>
#include <string>
#include <limits>

void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::vector<int> askUserSymptoms() {
    std::vector<int> answers;
    for (const auto& symptom : SYMPTOM_NAMES) {
        std::string input;
        std::cout << "Do you have " << symptom << "? [y/n]: ";
        std::cin >> input;

        while (input != "y" && input != "n") {
            std::cout << "Please enter 'y' or 'n': ";
            std::cin >> input;
        }

        answers.push_back(input == "y" ? 1 : 0);
    }
    return answers;
}

void printAllCasesAsTable(DiagnosisRepositoryORM& db) {
    auto all = db.getAll();

    if (all.empty()) {
        std::cout << "No cases found in the database.\n";
        return;
    }

    // SYMPTOM_NAMES must match the symptom order in Diagnosis::toVector()
    const std::vector<std::string> SYMPTOM_NAMES = {
        "runny_nose", "cough", "strong_cough", "sore_throat", "fever",
        "high_fever", "fatigue", "diziness", "nausea", "headache",
        "strong_muscle_pain", "trouble_concentrating", "itchy_eyes", "dry_mouth",
        "chest_pain", "shortness_of_breath", "sweating", "chills", "vomiting", "anxiety"
    };

    const int col_width = 5;

    // Print header
    std::cout << "ID  | ";
    for (const auto& name : SYMPTOM_NAMES) {
        std::cout << std::setw(col_width) << name.substr(0, 4) << " ";
    }
    std::cout << "| Diagnosis\n";
    std::cout << std::string(6 + col_width * SYMPTOM_NAMES.size() + 15, '-') << "\n";

    // Print data rows
    for (const auto& d : all) {
        auto values = d.toVector();

        std::cout << std::setw(3) << d.id << " | ";
        for (int val : values) {
            std::cout << std::setw(col_width) << val << " ";
        }
        std::cout << "| " << d.diagnosis << "\n";
    }

    std::cout << "\nTotal entries: " << all.size() << "\n";
}


void menu() {
    DiagnosisRepositoryORM db("test.db");
    DecisionTree tree;
    std::unique_ptr<Node> importedTree;
    float confidence_threshold = 0.75f;
    int min_tree_depth = 6;
    


    while (true) {
        std::cout << "\n==== Decision Tree Diagnostic Menu ====\n";
        std::cout << "[1] Train and test decision tree (with export)\n";
        std::cout << "[2] Import decision tree for diagnosis\n";
        std::cout << "[3] Diagnose user interactively\n";
        std::cout << "[4] Manually add new case to database\n";
        std::cout << "[5] Print all data in the database\n";
        std::cout << "[0] Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                auto [train, test] = db.splitTrainTest(0.2f);
                tree.setMinDepth(min_tree_depth);
                tree.setMinLeafSize(8);
                tree.fit(train);


                int correct = 0;
                for (const auto& d : test) {
                    std::string prediction = tree.predict(d, confidence_threshold);
                    if (prediction == d.diagnosis)
                        ++correct;
                }

                float accuracy = static_cast<float>(correct) / test.size() * 100.0f;
                std::cout << "Training complete. Test accuracy: " << accuracy << "%\n";

                TreeExporter exporter;
                exporter.exportToCSV(tree.getRoot(), "decision_tree.csv");
                std::cout << "Tree exported to decision_tree.csv\n";
                break;
            }

            case 2: {
                std::cout << "Importing tree..." << std::endl;
                TreeImporter importer;
                try {
                    importedTree = importer.importFromCSV("decision_tree.csv");
                    if (!importedTree) {
                        std::cerr << "Error: Tree could not be imported or is null.\n";
                    } else {
                        std::cout << "Tree imported successfully.\n";
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Exception during import: " << e.what() << "\n";
                }
                break;
            }

            case 3: {
                if (!importedTree) {
                    std::cout << "Tree not loaded. Please use option [2] first.\n";
                    break;
                }

                std::cout << "\nPlease answer the following questions:\n";
                Node* current = importedTree.get();

                while (!current->isLeaf()) {
                    auto* decision = dynamic_cast<DecisionNode*>(current);
                    if (!decision) {
                        std::cerr << "Error: Expected DecisionNode but got something else.\n";
                        return;
                    }

                    const Question& q = decision->getQuestion();
                    std::string input;

                    std::cout << q.toString() << " [y/n]: ";
                    std::cin >> input;
                    while (input != "y" && input != "n") {
                        std::cout << "Please enter 'y' or 'n': ";
                        std::cin >> input;
                    }

                    if (input == "y") {
                        current = const_cast<Node*>(decision->getTrueBranch());
                    } else {
                        current = const_cast<Node*>(decision->getFalseBranch());
                    }
                }

                auto* leaf = dynamic_cast<LeafNode*>(current);
                if (leaf) {
                    float confidence = leaf->getConfidence();
                    std::string diagnosis = (confidence >= confidence_threshold) ? leaf->classify({}) : "Uncertain";
                    std::cout << "\nDiagnosis result: " << diagnosis;
                    std::cout << " (confidence: " << confidence * 100 << "%)\n";


                } else {
                    std::cerr << "Error: Final node is not a leaf.\n";
                }
                break;
            }

            case 4: {
                std::cout << "Manually adding a new case.\n";
                std::vector<int> symptoms = askUserSymptoms();
                if (symptoms.size() < 20) {
                    std::cerr << "Error: Not enough symptoms provided.\n";
                    break;
                }
                
                std::string diagnosis;
                std::cout << "Enter diagnosis name: ";
                clearInput();
                std::getline(std::cin, diagnosis);

                Diagnosis newCase{
                    0,
                    symptoms[0], symptoms[1], symptoms[2], symptoms[3], symptoms[4],
                    symptoms[5], symptoms[6], symptoms[7], symptoms[8], symptoms[9],
                    symptoms[10], symptoms[11], symptoms[12], symptoms[13], symptoms[14],
                    symptoms[15], symptoms[16], symptoms[17], symptoms[18], symptoms[19], 
                    diagnosis
                };

                db.add(newCase);
                std::cout << "Case added.\n";
                break;
            }

            case 5: {
                printAllCasesAsTable(db);
                break;
            }

            case 0:
                std::cout << "Goodbye.\n";
                return;

            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }
    }
}

int main() {
    try {
        menu();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
