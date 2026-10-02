#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAILS 1000

// Structure to represent an email
typedef struct {
    char category[30];
    char subject[100];
    char date[11]; // Format: MM-DD-YYYY
} Email;

// Structure for the list-based MaxHeap priority queue
typedef struct {
    Email emails[MAX_EMAILS];
    int size;
} MaxHeap;

// Function to assign priority ranks (lower number = higher priority)
int get_priority(const char* category) {
    if (strcmp(category, "Boss") == 0) return 1;
    if (strcmp(category, "Subordinate") == 0) return 2;
    if (strcmp(category, "Peer") == 0) return 3;
    if (strcmp(category, "ImportantPerson") == 0) return 4;
    return 5; // OtherPerson
}

// Function to convert MM-DD-YYYY string to a comparable integer value (YYYYMMDD)
int date_to_int(const char* date_str) {
    int month, day, year;
    sscanf(date_str, "%d-%d-%d", &month, &day, &year);
    return year * 10000 + month * 100 + day;
}

// Comparison function: returns 1 if email a has higher priority than email b
int compare_emails(Email a, Email b) {
    int p_a = get_priority(a.category);
    int p_b = get_priority(b.category);

    // Higher priority category comes first (lower rank number)
    if (p_a != p_b) {
        return p_a < p_b;
    }

    // If categories match, newer dates take precedence
    int date_a = date_to_int(a.date);
    int date_b = date_to_int(b.date);
    return date_a > date_b;
}

// Swap two emails in the heap
void swap(Email* a, Email* b) {
    Email temp = *a;
    *a = *b;
    *b = temp;
}

// Sift-up operation for MaxHeap insertion
void sift_up(MaxHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (compare_emails(heap->emails[idx], heap->emails[parent])) {
            swap(&heap->emails[idx], &heap->emails[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

// Sift-down operation for MaxHeap removal
void sift_down(MaxHeap* heap, int idx) {
    int last = heap->size - 1;
    while (2 * idx + 1 <= last) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int target = left;

        if (right <= last && compare_emails(heap->emails[right], heap->emails[left])) {
            target = right;
        }

        if (compare_emails(heap->emails[target], heap->emails[idx])) {
            swap(&heap->emails[target], &heap->emails[idx]);
            idx = target;
        } else {
            break;
        }
    }
}

// Insert a new email into the MaxHeap
void heap_insert(MaxHeap* heap, Email e) {
    if (heap->size >= MAX_EMAILS) return;
    heap->emails[heap->size] = e;
    sift_up(heap, heap->size);
    heap->size++;
}

// Remove the highest priority email from the MaxHeap
void heap_pop(MaxHeap* heap) {
    if (heap->size <= 0) return;
    heap->emails[0] = heap->emails[heap->size - 1];
    heap->size--;
    sift_down(heap, 0);
}

// Helper function to trim leading and trailing spaces from input strings
void trim_whitespace(char* str) {
    char* end;
    while (*str == ' ' || *str == '\t') str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = 0;
        end--;
    }
}

int main() {
    MaxHeap inbox;
    inbox.size = 0;

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        if (strncmp(line, "EMAIL", 5) == 0) {
            char* token = line + 5;
            char* cat = strtok(token, ",");
            char* subj = strtok(NULL, ",");
            char* date = strtok(NULL, ",");

            if (cat && subj && date) {
                Email e;
                trim_whitespace(cat);
                trim_whitespace(subj);
                trim_whitespace(date);

                strcpy(e.category, cat);
                strcpy(e.subject, subj);
                strcpy(e.date, date);

                heap_insert(&inbox, e);
            }
        } 
        else if (strcmp(line, "NEXT") == 0) {
            if (inbox.size > 0) {
                Email top = inbox.emails[0];
                printf("Next email:\n");
                printf("Sender: %s\n", top.category);
                printf("Subject: %s\n", top.subject);
                printf("Date: %s\n", top.date);
            }
        } 
        else if (strcmp(line, "READ") == 0) {
            if (inbox.size > 0) {
                heap_pop(&inbox);
            }
        } 
        else if (strcmp(line, "COUNT") == 0) {
            printf("There are %d emails to read.\n", inbox.size);
        }
    }

    return 0;
}