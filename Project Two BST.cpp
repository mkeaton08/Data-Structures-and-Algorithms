//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Matthew A Keaton
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project Two - Binary Search Tree Implementation for Course Management
//============================================================================

#include <iostream>   // For console input/output operations
#include <string>     // For string data type and operations
#include <fstream>    // For file reading operations
#include <vector>     // For dynamic array to store prerequisites
#include <sstream>    // For parsing CSV lines
 

using namespace std;

/**
 * Course Structure
 * Represents a single course with its identifying information and prerequisites
 * Used as the data payload stored in each BST node
 */
struct Course {

    string courseNumber;              // Unique course identifier (e.g., "CSCI100")
    string courseName;                // Full name of the course
    vector<string> coursePreReqs;     // Dynamic list of prerequisite course numbers

};

/**
 * Node Structure
 * Internal structure representing a single node in the binary search tree
 * Each node contains a Course object and pointers to left and right children
 */
struct Node {
    Course course;     // The course data stored in this node
    Node* left;        // Pointer to left child (courses with smaller courseNumber)
    Node* right;       // Pointer to right child (courses with larger courseNumber)


    /**
     * Default Constructor
     * Initializes a node with null pointers for left and right children
     */
    Node() {
        left = nullptr;
        right = nullptr;
    }

    /**
     * Parameterized Constructor
     * Creates a node with a specific course and initializes child pointers to null
     * @param aCourse The course object to store in this node
     */
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }

};

/**
 * BinarySearchTree Class
 * Implements a binary search tree data structure for efficient course storage and retrieval
 * Courses are ordered alphabetically by courseNumber for O(log n) search performance
 */
class BinarySearchTree {

private:

    Node* root;     // Pointer to the root node of the tree (nullptr if tree is empty)

    // Private helper method for recursive node insertion
    void addNode(Node* node, Course course);
    
    // Private helper method for recursive in-order traversal
    void inOrder(Node* node);
    

public:
    /**
     * Constructor - Initializes an empty binary search tree
     */
    BinarySearchTree();
    
    /**
     * Public interface for in-order traversal
     * Prints all courses in alphabetical order by courseNumber
     */
    void inOrder();
    
    /**
     * Insert a course into the binary search tree
     * @param course The course object to insert
     */
    void Insert(Course course);
    
    /**
     * Search for a course by its course number
     * @param courseNumber The unique identifier of the course to find
     * @return Course object if found, empty Course object if not found
     */
    Course Search(string courseNumber);
};

/**
 * BinarySearchTree Constructor
 * Initializes an empty tree by setting root pointer to null
 * Time Complexity: O(1)
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}


/**
 * Public In-Order Traversal Method
 * Entry point for in-order traversal, starts recursion from root node
 * Calls the private recursive inOrder method
 * Time Complexity: O(n) where n is the number of nodes
 */
void BinarySearchTree::inOrder() {
    inOrder(root);
}

/**
 * Private Recursive In-Order Traversal Method
 * Traverses the tree in sorted order: left subtree -> current node -> right subtree
 * This produces alphabetically sorted output by courseNumber
 * @param node Current node being processed in the traversal
 * Time Complexity: O(n) - visits each node exactly once
 */
void BinarySearchTree::inOrder(Node* node) {
    // Base case: if node is null, return (end of branch reached)
    if (node == nullptr) {
        return;
    }
    
    // Step 1: Recursively traverse the left subtree (smaller course numbers)
    inOrder(node->left);
    
    // Step 2: Process current node - print course number and name
    cout << node->course.courseNumber << ", " << node->course.courseName;

    // Step 3: Display prerequisites if the course has any
    if (!node->course.coursePreReqs.empty()) {
        cout << "\n Prerequisites: ";
        // Loop through all prerequisites and print them comma-separated
        for (size_t i = 0; i < node->course.coursePreReqs.size(); ++i) {
            cout << node->course.coursePreReqs[i];
            // Add comma separator between prerequisites (but not after the last one)
            if (i < node->course.coursePreReqs.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else {
        // No prerequisites, just add newline
        cout << endl;
    }
    
    // Step 4: Recursively traverse the right subtree (larger course numbers)
    inOrder(node->right);
}

/**
 * Insert Method
 * Public method to insert a new course into the binary search tree
 * Handles the special case of inserting into an empty tree
 * @param course The course object to insert into the tree
 * Time Complexity: O(log n) average case, O(n) worst case (unbalanced tree)
 */
void BinarySearchTree::Insert(Course course) {
    // Special case: if tree is empty, create root node
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        // Tree is not empty, use recursive helper method to find insertion point
        addNode(root, course);
    }
}

/**
 * Add Node Helper Method
 * Private recursive method that finds the correct position and inserts a new course
 * Maintains BST property: left child < parent < right child (based on courseNumber)
 * @param node Current node being examined in the traversal
 * @param course The course object to insert
 * Time Complexity: O(log n) average case, O(n) worst case
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    // Compare current node's course number with the course to insert
    // If current node's courseNumber is greater, new course belongs in left subtree
    if (node->course.courseNumber > course.courseNumber) {

        // Check if left child exists
        if (node->left == nullptr) {
            // Left child is empty, insert new course here
            node->left = new Node(course);
        }
        else {
            // Left child exists, recursively continue down left subtree
            addNode(node->left, course);
        }
    }
    else {
        // Current node's courseNumber is less than or equal to new course
        // New course belongs in right subtree

        // Check if right child exists
        if (node->right == nullptr) {
            // Right child is empty, insert new course here
            node->right = new Node(course);
        }
        else {
            // Right child exists, recursively continue down right subtree
            addNode(node->right, course);
        }
    }

}

/**
 * Search Method
 * Searches the binary search tree for a course with the specified course number
 * Uses iterative approach (more memory efficient than recursion)
 * @param courseNumber The unique identifier of the course to find
 * @return Course object if found, empty Course object if not found
 * Time Complexity: O(log n) average case, O(n) worst case
 */
Course BinarySearchTree::Search(string courseNumber) {
    // Start search at the root of the tree
    Node* current = root;

    // Traverse tree until we find the course or reach a null pointer (course not found)
    while (current != nullptr) {
        
        // Check if current node contains the course we're looking for
        if (current->course.courseNumber == courseNumber) {
            // Match found! Return the course object
            return current->course;
        }
        
        // Course number we're searching for is smaller than current node
        // Move to left subtree (smaller values)
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        // Course number we're searching for is larger than current node
        // Move to right subtree (larger values)
        else {
            current = current->right;
        }
    }
    
    // If we exit the loop, course was not found in the tree
    // Return an empty Course object to indicate "not found"
    Course course;
    return course;
}

/**
 * Load Courses Function
 * Reads course data from a CSV file and populates the binary search tree
 * CSV Format: courseNumber,courseName,prerequisite1,prerequisite2,...
 * @param csvPath The file path to the CSV file containing course data
 * @param bst Pointer to the BinarySearchTree to populate with courses
 * Time Complexity: O(n log n) where n is number of courses (n insertions * log n per insertion)
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file: " << csvPath << endl;

    // Open the CSV file for reading
    ifstream file(csvPath);
    
    // Validate that file opened successfully
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << csvPath << endl;
        return;
    }

    string line;

    // Read the file line by line until end of file
    while (getline(file, line)) {
        // Create a string stream to parse the current line
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Parse the line by splitting on commas
        // Each comma-separated value becomes a token
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Validate row has minimum required fields (courseNumber and courseName)
        if (tokens.size() < 2) {
            cerr << "Invalid row (missing data): " << line << endl;
            continue;  // Skip this row and move to next line
        }

        // Create a Course object and populate it with parsed data
        Course course;
        course.courseNumber = tokens[0];  // First column: course number (e.g., "CSCI100")
        course.courseName = tokens[1];    // Second column: course name

        // Process any additional columns as prerequisites
        // Starting from index 2 (third column) to end of tokens
        for (size_t i = 2; i < tokens.size(); i++) {
            course.coursePreReqs.push_back(tokens[i]);
        }

        // Insert the fully populated course into the BST
        bst->Insert(course);
    }

    // Close the file to free system resources
    file.close();
}

/**
 * Main Function
 * Entry point for the Course Planner application
 * Provides a menu-driven interface for loading courses, displaying course lists,
 * and searching for specific course information
 * @return 0 on successful program completion
 */
int main() {

    // Create an empty binary search tree to store courses
    BinarySearchTree bst;
    
    // Variables for user input and menu navigation
    int choice = 0;              // Stores user's menu selection
    string courseNumber;         // Stores course number for search queries
    string filename;             // Stores CSV filename for loading courses

    // Main program loop - continues until user selects option 9 (Exit)
    while (choice != 9) {

        // Display the main menu options to the user
        cout << "Welcome to the course planner." << endl;
        cout << " 1. Load Data Structure." << endl;
        cout << " 2. Print Course List." << endl;
        cout << " 3. Print Course." << endl;
        cout << " 9. Exit." << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;

        // Process user's menu selection
        switch (choice) {

        // OPTION 1: Load courses from a CSV file into the BST
        case 1:
            cout << "Enter file name: " << endl;
            cin.ignore();  // Clear newline character left in input buffer
            getline(cin, filename);  // Read full filename (allows spaces)
            loadCourses(filename, &bst);  // Parse CSV and populate BST
            break;

        // OPTION 2: Display all courses in alphabetical order
        case 2:
            cout << "Here is a sample schedule: " << endl;
            bst.inOrder();  // Perform in-order traversal to print sorted course list
            break;

        // OPTION 3: Search for and display a specific course's details
        case 3:
            cout << "What course do you want to know about?" << endl;
            cin.ignore();  // Clear newline character left in input buffer
            getline(cin, courseNumber);  // Read course number from user
            {
                // Search BST for the requested course
                Course course = bst.Search(courseNumber);
                
                // Check if course was found (non-empty courseNumber indicates success)
                if (!course.courseNumber.empty()) {
                    // Display course number and name
                    cout << course.courseNumber << ", " << course.courseName << endl;
                    
                    // Display prerequisites if the course has any
                    if (!course.coursePreReqs.empty()) {
                        cout << "Prerequisites: ";
                        // Loop through and print all prerequisites
                        for (size_t i = 0; i < course.coursePreReqs.size(); ++i) {
                            cout << course.coursePreReqs[i];
                            // Add comma separator between prerequisites (not after last one)
                            if (i < course.coursePreReqs.size() - 1) {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    }
                }
                else {
                    // Course not found in BST
                    cout << "Course not found" << endl;
                }
            }
            break;

        }
    }
    
    // Display exit message when user chooses to quit
    cout << "Thank you for using the course planner!" << endl;

    return 0;  // Indicate successful program termination
}



