 /*
Author: Kaleb Blakemore
KUID: 3228599
Date: 09/16/2026
Lab: Asignment #2 -  CEO Email Priority Queue
Last modified: 09/17/2026
Purpose:  Implements a custom MaxHeap-based priority queue from scratch 
 *              to manage and prioritize a CEO's incoming emails based on sender 
 *              category hierarchy (Boss -> Subordinate -> Peer -> Important Person 
 *              -> Other Person) and recency (newer dates prioritized).
 * Inputs: Command stream or input file containing EMAIL, NEXT, READ, and COUNT.
 * Output: Terminal printouts displaying unread counts and next email details.
*/
#include <iostream>  // Includes standard input/output stream library for terminal printing
#include <string>    // Includes standard string library to handle text for email fields
#include <vector>    // Includes standard vector container to act as our underlying heap list
#include <sstream>   // Includes string stream library to parse command lines easily

using namespace std; // Allows usage of standard library identifiers without prefixing std::

// Structure representing an individual email in the CEO's inbox
struct Email {
    string category; // Sender category (Boss, Subordinate, Peer, Important Person, Other Person)
    string subject;  // Subject line text describing the email content
    string date;     // Date string formatted as MM-DD-YYYY for recency comparison
};

// Function to convert sender category strings into numerical priority ranks (lower number = higher priority)
int getCategoryPriority(const string& category) {
    if (category == "Boss") return 1;              // Boss category holds the highest priority rank (1)
    if (category == "Subordinate") return 2;       // Subordinate category holds second highest priority rank (2)
    if (category == "Peer") return 3;              // Peer category holds third priority rank (3)
    if (category == "Important Person") return 4;  // Important Person category holds fourth priority rank (4)
    return 5;                                      // Any other category defaults to the lowest priority rank (5)
}

// Function to compare two emails and return true if email 'a' has strictly higher priority than email 'b'
bool compareEmails(const Email& a, const Email& b) {
    int pA = getCategoryPriority(a.category);      // Retrieves the numerical priority rank for email a
    int pB = getCategoryPriority(b.category);      // Retrieves the numerical priority rank for email b

    if (pA != pB) {                                // Checks if the two emails belong to different sender categories
        return pA < pB;                            // Returns true if email a has a higher priority rank (lower numerical value)
    }

    // If categories are identical, compare dates (MM-DD-YYYY format allows string comparison for recency)
    // Note: Newer dates should be prioritized, so a "greater" date string means a more recent date.
    return a.date > b.date;                        // Returns true if email a has a newer (greater) date than email b
}

// Class implementing a custom MaxHeap priority queue from scratch using an underlying vector list
class MaxHeap {
private:
    vector<Email> heap; // Vector list storing the email heap elements in sequential memory slots

    // Helper function to bubble an element up to its correct position to maintain heap property
    void heapifyUp(int index) {
        if (index == 0) return;               // Base case: if we have reached the root node, stop recursion
        int parent = (index - 1) / 2;         // Calculates the array index of the current node's parent

        // Checks if the current node has higher priority than its parent using our comparator
        if (compareEmails(heap[index], heap[parent])) {
            swap(heap[index], heap[parent]);  // Swaps the current node with its parent if priority condition is met
            heapifyUp(parent);                // Recursively calls heapifyUp on the parent index to continue bubbling up
        }
    }

    // Helper function to push an element down to its correct position after an extraction
    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;    // Calculates the index of the left child node
        int rightChild = 2 * index + 2;   // Calculates the index of the right child node
        int highest = index;              // Assumes the current index holds the highest priority initially

        // Checks if the left child exists and has higher priority than the current highest
        if (leftChild < heap.size() && compareEmails(heap[leftChild], heap[highest])) {
            highest = leftChild;          // Updates the highest priority index to the left child
        }

        // Checks if the right child exists and has higher priority than the current highest
        if (rightChild < heap.size() && compareEmails(heap[rightChild], heap[highest])) {
            highest = rightChild;         // Updates the highest priority index to the right child
        }

        // If the highest priority index has shifted, swap elements and continue sifting down
        if (highest != index) {
            swap(heap[index], heap[highest]); // Swaps the current node with the highest priority child
            heapifyDown(highest);              // Recursively calls heapifyDown on the new child index
        }
    }

public:
    // Method to insert a new email into the priority queue
    void insert(const Email& newEmail) {
        heap.push_back(newEmail);             // Appends the new email onto the end of the vector list
        heapifyUp(heap.size() - 1);           // Restores the MaxHeap property by bubbling the new element up
    }

    // Method to return the highest priority email without removing it (used for NEXT command)
    Email peek() const {
        return heap.front();                  // Returns the root element of the max heap
    }

    // Method to remove the highest priority email from the heap (used for READ command)
    void removeMax() {
        if (heap.empty()) return;             // Safety check: returns immediately if the heap is empty
        heap[0] = heap.back();                // Moves the last element in the vector up to the root position
        heap.pop_back();                      // Removes the duplicate last element from the vector
        if (!heap.empty()) {
            heapifyDown(0);                   // Restores the MaxHeap property by sifting the root element down
        }
    }

    // Method to check how many unread emails remain in the heap (used for COUNT command)
    int size() const {
        return heap.size();                   // Returns the total number of elements currently stored in the vector
    }

    // Method to check if the heap vector is completely empty
    bool isEmpty() const {
        return heap.empty();                  // Returns true if the vector size is 0, false otherwise
    }
};
// Main function where program execution begins and command parsing occurs
int main() {
    MaxHeap inboxHeap;                       // Instantiates our custom MaxHeap priority queue object
    string line;                             // String variable to store each incoming line of commands

    // Loop to read commands line-by-line from standard input (supports terminal input or file redirection)
    while (getline(cin, line)) {
        if (line.empty()) continue;          // Skips empty lines to avoid parsing errors

        if (line.rfind("EMAIL", 0) == 0) {   // Checks if the line starts with the "EMAIL" command keyword
            string data = line.substr(6);    // Extracts the substring after the "EMAIL " prefix
            
            size_t firstComma = data.find(',');  // Finds the index of the first comma separating category and subject
            size_t lastComma = data.rfind(',');  // Finds the index of the last comma separating subject and date

            if (firstComma != string::npos && lastComma != string::npos && firstComma != lastComma) {
                Email newEmail;              // Declares a temporary Email object to hold parsed data
                
                // Extracts and trims the sender category string
                string cat = data.substr(0, firstComma);
                size_t start = cat.find_first_not_of(" \t");
                size_t end = cat.find_last_not_of(" \t");
                newEmail.category = (start != string::npos) ? cat.substr(start, end - start + 1) : cat;

                // Extracts and trims the email subject line (between first and last comma)
                string subj = data.substr(firstComma + 1, lastComma - firstComma - 1);
                start = subj.find_first_not_of(" \t");
                end = subj.find_last_not_of(" \t");
                newEmail.subject = (start != string::npos) ? subj.substr(start, end - start + 1) : subj;

                // Extracts and trims the date string
                string dt = data.substr(lastComma + 1);
                start = dt.find_first_not_of(" \t");
                end = dt.find_last_not_of(" \t");
                newEmail.date = (start != string::npos) ? dt.substr(start, end - start + 1) : dt;

                inboxHeap.insert(newEmail);  // Inserts the parsed email into our custom MaxHeap priority queue
            }
        } else if (line == "NEXT") {         // Checks if the command is exactly "NEXT"
            if (!inboxHeap.isEmpty()) {      // Verifies that there are emails available in the heap
                Email topEmail = inboxHeap.peek(); // Retrieves the highest priority email without removing it
                cout << "Next email:" << endl; // Prints the required header for the next email
                cout << "Sender: " << topEmail.category << endl; // Prints the sender category
                cout << "Subject: " << topEmail.subject << endl; // Prints the email subject line
                cout << "Date: " << topEmail.date << endl;     // Prints the email date
            }
        } else if (line == "READ") {         // Checks if the command is exactly "READ"
            if (!inboxHeap.isEmpty()) {      // Verifies that there are emails to remove from the heap
                inboxHeap.removeMax();       // Removes the highest priority email from the priority queue
            }
        } else if (line.rfind("COUNT", 0) == 0) { // Checks if the line starts with "COUNT"
            cout << "There are " << inboxHeap.size() << " emails to read." << endl; // Prints the current unread count
        }
    }

    return 0; // Signals to the operating system that program execution ended successfully
} // End of main function
