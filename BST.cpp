//============================================================================
// Name        : BST.cpp
// Author      : Matthew A. Keaton
// Version     : 2.0
// Description : Binary Search Tree Implementation for CS Course Management
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Normalize string: trim whitespace and convert to uppercase
string normalize(string str) {
    // Trim any leading whitespace
    size_t start = 0;
    while (start < str.length() && isspace(str[start])) {
        start++;
    }
    
    // Trim any trailing whitespace
    size_t end = str.length();
    while (end > start && isspace(str[end - 1])) {
        end--;
    }
    
    // Extract trimmed substring and convert to uppercase
    string result = str.substr(start, end - start);
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Represents a course with its number, name, and prerequisites
struct Course {
    string courseNumber;
    string courseName;
    vector<string> coursePreReqs;
};

// BST node containing a course and pointers to children
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Constructor with course parameter, delegates to default constructor
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

// Binary search tree for course management
// Organizes courses alphabetically by courseNumber for efficient searching
class BinarySearchTree {
private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void rangeQuery(Node* node, string lowKey, string highKey, vector<Course>& results);

public:
    BinarySearchTree();
    void inOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
    vector<Course> RangeQuery(string lowKey, string highKey);
};

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Public wrapper that initiates traversal from root
void BinarySearchTree::inOrder() {
    inOrder(root);
}

// Recursive in-order traversal: left -> node -> right
// This produces alphabetically sorted output
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }
    
    inOrder(node->left);
    
    cout << node->course.courseNumber << ", " << node->course.courseName;

    // Print prerequisites if any exist
    if (!node->course.coursePreReqs.empty()) {
        cout << "\n Prerequisites: ";
        for (size_t i = 0; i < node->course.coursePreReqs.size(); ++i) {
            cout << node->course.coursePreReqs[i];
            if (i < node->course.coursePreReqs.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else {
        cout << endl;
    }
    
    inOrder(node->right);
}

// Insert a course into the BST, creating root if tree is empty
void BinarySearchTree::Insert(Course course) {
    // Normalize course number before insertion
    course.courseNumber = normalize(course.courseNumber);
    
    // Handle empty tree case
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Recursive helper to insert course maintaining BST property
void BinarySearchTree::addNode(Node* node, Course course) {
    // Navigate left if new course number is smaller
    if (node->course.courseNumber > course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    // Navigate right if new course number is larger or equal
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// Search for a course by courseNumber, returns empty Course if not found
Course BinarySearchTree::Search(string courseNumber) {
    // Normalize input for comparison
    courseNumber = normalize(courseNumber);
    Node* current = root;

    // Iterative search through the tree
    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        
        // Traverse left or right based on comparison
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    
    // Return empty course if not found
    Course course;
    return course;
}

// Helper for range query
// Only traverses subtrees that may contain courses in range
void BinarySearchTree::rangeQuery(Node* node, string lowKey, string highKey, vector<Course>& results) {
    if (node == nullptr) {
        return;
    }
    
    // Search left subtree if there might be values >= lowKey on the left
    if (lowKey < node->course.courseNumber) {
        rangeQuery(node->left, lowKey, highKey, results);
    }
    
    // Add current node if it's within range
    if (node->course.courseNumber >= lowKey && node->course.courseNumber <= highKey) {
        results.push_back(node->course);
    }
    
    // Search right subtree if there might be values <= highKey on the right
    if (highKey > node->course.courseNumber) {
        rangeQuery(node->right, lowKey, highKey, results);
    }
}

// Range query method: returns all courses where lowKey <= courseNumber <= highKey
vector<Course> BinarySearchTree::RangeQuery(string lowKey, string highKey) {
    // Normalize input keys
    lowKey = normalize(lowKey);
    highKey = normalize(highKey);
    
    vector<Course> results;
    rangeQuery(root, lowKey, highKey, results);
    return results;
}

// Load courses from CSV file (format: courseNumber,courseName,prereq1,prereq2,...)
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file: " << csvPath << endl;

    ifstream file(csvPath);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << csvPath << endl;
        return;
    }

    string line;

    // Process each line from the CSV file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Parse CSV line into tokens
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Validate minimum required fields
        if (tokens.size() < 2) {
            cerr << "Invalid row (missing data): " << line << endl;
            continue;
        }

        // Build course object from parsed data
        Course course;
        course.courseNumber = tokens[0];
        course.courseName = tokens[1];

        // Add any prerequisites (columns 3+)
        for (size_t i = 2; i < tokens.size(); i++) {
            course.coursePreReqs.push_back(tokens[i]);
        }

        bst->Insert(course);
    }

    file.close();
}

int main() {
    BinarySearchTree bst;
    int choice = 0;
    string courseNumber;
    string filename;

    // Main menu loop
    while (choice != 9) {
        cout << "Welcome to the course planner. Be sure to load your data structure first (Option 1)." << endl;
        cout << " 1. Load Data Structure." << endl;
        cout << " 2. Print Course List." << endl;
        cout << " 3. Print Course." << endl;
        cout << " 4. Course Range Query." << endl;
        cout << " 9. Exit." << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            // Load courses from CSV file into BST
            cout << "Enter file name: " << endl;
            cin.ignore();
            getline(cin, filename);
            loadCourses(filename, &bst);
            break;

        case 2:
            // Display all courses in alphanumeric order
            cout << "Here is a sample schedule: " << endl;
            bst.inOrder();
            break;

        case 3:
            // Search for and display specific course information
            cout << "What course do you want to know about?" << endl;
            cin.ignore();
            getline(cin, courseNumber);
            {
                Course course = bst.Search(courseNumber);
                
                // Display course details if found
                if (!course.courseNumber.empty()) {
                    cout << course.courseNumber << ", " << course.courseName << endl;
                    
                    // Print comma-separated list of prerequisites
                    if (!course.coursePreReqs.empty()) {
                        cout << "Prerequisites: ";
                        for (size_t i = 0; i < course.coursePreReqs.size(); ++i) {
                            cout << course.coursePreReqs[i];
                            if (i < course.coursePreReqs.size() - 1) {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    }
                }
                else {
                    cout << "Course not found" << endl;
                }
            }
            break;

        case 4:
            // Range query to find all courses between lowKey and highKey
            {
                string lowKey, highKey;
                cout << "Enter lower bound course number: " << endl;
                cin.ignore();
                getline(cin, lowKey);
                cout << "Enter upper bound course number: " << endl;
                getline(cin, highKey);
                
                vector<Course> results = bst.RangeQuery(lowKey, highKey);
                
                if (results.empty()) {
                    cout << "No courses found in range [" << normalize(lowKey) << ", " << normalize(highKey) << "]" << endl;
                }
                else {
                    cout << "Courses in range [" << normalize(lowKey) << ", " << normalize(highKey) << "]:" << endl;
                    for (const Course& course : results) {
                        cout << course.courseNumber << ", " << course.courseName << endl;
                    }
                }
            }
            break;
        }
    }
    
    cout << "Thank you for using the course planner!" << endl;

    return 0;
}



