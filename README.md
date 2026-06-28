# CART Decision Tree Classifier (C++)

## Purpose
This project was developed to deeply understand the core mechanics and mathematics of machine learning algorithms. Instead of using high-level libraries, I implemented a binary CART decision tree classifier from scratch. 

The program operates as a medical diagnosis assistant that analyzes patient symptom data to predict health conditions, based on the dedicated SQL table.

## Key Goals Achieved
* **Algorithm Mastery:** Implemented recursive tree-building and node-splitting logic using Gini Impurity.
* **Data Integration:** Connected the classifier directly to an SQLite database using `sqlite_orm` to manage training data and patient records.
* **Model Persistence:** Created a system to export the trained model to a CSV file and load it back into memory for predictions.