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
Collaborators: ChatGPT and [second GenAI], as documented in GenAI analysis PDF
Sources: ChatGPT and [second GenAI]
Creation Date: 09/16/2026
Revision Date: 09/17/2026
Revisions: Corrected priority handling and improved heap implementation.
*/

#include <iostream>     // Includes the standard input/output library.
#include <string>       // Includes the string library for email information.
#include <vector>       // Includes vector to store the list-based heap.
#include <sstream>      // Includes string stream tools for parsing input.

using namespace std;    // Allows standard library names to be used without std::.



// Structure representing one email in the CEO's inbox.
// This structure was generated as a starting point by ChatGPT
// and was reviewed and modified by Kaleb Blakemore.
struct Email {
    string category;    // Stores the sender category.
    string subject;     // Stores the email subject.
    string date;        // Stores the email date in MM-DD-YYYY format.
};



// Converts a sender category into a numerical priority.
// A larger number represents a higher priority so that the
// structure behaves as a MaxHeap.
//
// This function was generated as a starting point by ChatGPT
// and was reviewed and modified by Kaleb Blakemore.
int getCategoryPriority(const string& category) {

    if (category == "Boss")
        return 5;       // Boss has the highest priority.

    if (category == "Subordinate")
        return 4;       // Subordinate has the second-highest priority.

    if (category == "Peer")
        return 3;       // Peer has the third-highest priority.

    if (category == "ImportantPerson")
        return 2;       // ImportantPerson has the fourth-highest priority.

    return 1;           // OtherPerson has the lowest priority.
}



// Compares two emails and returns true when email a has a higher
// priority than email b.
//
// This comparator was generated as a starting point by ChatGPT
// and was reviewed and modified by Kaleb Blakemore.
bool compareEmails(const Email& a, const Email& b) {

    // Get the numerical priority for both sender categories.
    int priorityA = getCategoryPriority(a.category);
    int priorityB = getCategoryPriority(b.category);

    // If the sender categories are different, use category priority.
    if (priorityA != priorityB) {
        return priorityA > priorityB;
    }

    // If the sender categories are the same, compare the dates.
    // Because the required date format is MM-DD-YYYY, the strings
    // can be compared to determine which date is newer.
    return a.date > b.date;
}



// This MaxHeap class was generated as a starting point by ChatGPT
// and was reviewed and modified by Kaleb Blakemore.
//
// The class implements a custom list-based MaxHeap using a vector.
// No pre-existing heap or priority queue module is used.
class MaxHeap {

private:

    // Vector acts as the list-based storage for the MaxHeap.
    vector<Email> heap;


    // Moves an email upward through the heap until the MaxHeap
    // priority property has been restored.
    void heapifyUp(int index) {

        // If the element is already at the root, no more work is needed.
        if (index == 0)
            return;

        // Calculate the index of the current element's parent.
        int parent = (index - 1) / 2;

        // If the current email has a higher priority than its parent,
        // swap the two emails.
        if (compareEmails(heap[index], heap[parent])) {

            swap(heap[index], heap[parent]);

            // Continue moving the email upward if necessary.
            heapifyUp(parent);
        }
    }


    // Moves an email downward through the heap after the highest-priority
    // email has been removed.
    void heapifyDown(int index) {

        // Calculate the indexes of the left and right children.
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        // Start by assuming the current index contains the highest
        // priority email.
        int highest = index;


        // Check whether the left child exists and has a higher priority.
        if (leftChild < static_cast<int>(heap.size()) &&
            compareEmails(heap[leftChild], heap[highest])) {

            highest = leftChild;
        }


        // Check whether the right child exists and has a higher priority.
        if (rightChild < static_cast<int>(heap.size()) &&
            compareEmails(heap[rightChild], heap[highest])) {

            highest = rightChild;
        }


        // If one of the children has a higher priority than the current
        // element, swap them and continue moving downward.
        if (highest != index) {

            swap(heap[index], heap[highest]);

            heapifyDown(highest);
        }
    }


public:

    // Inserts a new email into the MaxHeap.
    // This method was generated as a starting point by ChatGPT
    // and was reviewed and modified by Kaleb Blakemore.
    void insert(const Email& newEmail) {

        // Add the new email to the end of the vector.
        heap.push_back(newEmail);

        // Restore the MaxHeap property by moving the new email upward.
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }


    // Returns the highest-priority email without removing it.
    // This is used by the NEXT command.
    Email peek() const {

        return heap.front();
    }


    // Removes the highest-priority email from the MaxHeap.
    // This method was generated as a starting point by ChatGPT
    // and was reviewed and modified by Kaleb Blakemore.
    void removeMax() {

        // If the heap is already empty, there is nothing to remove.
        if (heap.empty())
            return;

        // Move the last email to the root position.
        heap[0] = heap.back();

        // Remove the duplicate last element.
        heap.pop_back();


        // If emails remain, restore the MaxHeap property.
        if (!heap.empty()) {

            heapifyDown(0);
        }
    }


    // Returns the current number of unread emails.
    int size() const {

        return static_cast<int>(heap.size());
    }


    // Returns true when the MaxHeap contains no emails.
    bool isEmpty() const {

        return heap.empty();
    }
};



// Main function where the program begins execution.
//
// The command-processing portion was generated as a starting point
// by ChatGPT and was reviewed and modified by Kaleb Blakemore.
int main() {

    // Create the MaxHeap that will store the CEO's emails.
    MaxHeap inboxHeap;

    // Stores each input line read from the test file or terminal.
    string line;


    // Continue reading commands until the end of the input.
    while (getline(cin, line)) {

        // Ignore completely empty lines.
        if (line.empty())
            continue;


        // Check whether the current command is an EMAIL command.
        if (line.rfind("EMAIL", 0) == 0) {

            // Remove the "EMAIL " portion from the beginning of the line.
            string data = line.substr(6);


            // Find the first comma between the category and subject.
            size_t firstComma = data.find(',');

            // Find the final comma between the subject and date.
            size_t lastComma = data.rfind(',');


            // Make sure both commas were found and they are different.
            if (firstComma != string::npos &&
                lastComma != string::npos &&
                firstComma != lastComma) {

                // Create an Email object for the new email.
                Email newEmail;


                // Extract the sender category.
                string category = data.substr(0, firstComma);

                // Find the first non-space character.
                size_t start = category.find_first_not_of(" \t");

                // Find the last non-space character.
                size_t end = category.find_last_not_of(" \t");


                // Remove unnecessary spaces from the category.
                if (start != string::npos) {
                    newEmail.category =
                        category.substr(start, end - start + 1);
                }
                else {
                    newEmail.category = category;
                }


                // Extract the subject between the first and last comma.
                string subject =
                    data.substr(firstComma + 1,
                                lastComma - firstComma - 1);


                // Find the first non-space character in the subject.
                start = subject.find_first_not_of(" \t");

                // Find the last non-space character in the subject.
                end = subject.find_last_not_of(" \t");


                // Remove unnecessary spaces from the subject.
                if (start != string::npos) {
                    newEmail.subject =
                        subject.substr(start, end - start + 1);
                }
                else {
                    newEmail.subject = subject;
                }


                // Extract the date after the final comma.
                string date = data.substr(lastComma + 1);


                // Find the first non-space character in the date.
                start = date.find_first_not_of(" \t");

                // Find the last non-space character in the date.
                end = date.find_last_not_of(" \t");


                // Remove unnecessary spaces from the date.
                if (start != string::npos) {
                    newEmail.date =
                        date.substr(start, end - start + 1);
                }
                else {
                    newEmail.date = date;
                }


                // Add the completed email to the MaxHeap.
                inboxHeap.insert(newEmail);
            }
        }


        // Check whether the command is NEXT.
        else if (line == "NEXT") {

            // Only display an email when the heap is not empty.
            if (!inboxHeap.isEmpty()) {

                // Get the highest-priority email without removing it.
                Email topEmail = inboxHeap.peek();


                // Display the required email information.
                cout << "Next email:" << endl;
                cout << "Sender: " << topEmail.category << endl;
                cout << "Subject: " << topEmail.subject << endl;
                cout << "Date: " << topEmail.date << endl;
            }
        }


        // Check whether the command is READ.
        else if (line == "READ") {

            // Only remove an email when the heap is not empty.
            if (!inboxHeap.isEmpty()) {

                // Remove the highest-priority unread email.
                inboxHeap.removeMax();
            }
        }


        // Check whether the command begins with COUNT.
        else if (line.rfind("COUNT", 0) == 0) {

            // Display the current number of unread emails.
            cout << "There are "
                 << inboxHeap.size()
                 << " emails to read."
                 << endl;
        }
    }


    // Return 0 to indicate that the program finished successfully.
    return 0;
}