/*
* Author: Kaleb Blakemore
* KUID: 3228599
* Date: 09/16/2026
* Lab: Assignment #2 - CEO Email Priority Queue (C Re-submission)
* Description: Implements a custom MaxHeap-based priority queue from scratch 
*              in C to manage and prioritize a CEO's incoming emails based on sender 
*              category hierarchy (Boss -> Subordinate -> Peer -> Important Person 
*              -> Other Person) and recency (newer dates prioritized).
* Inputs: Command stream or input file containing EMAIL, NEXT, READ, and COUNT.
* Output: Terminal printouts displaying unread counts and next email details, 
*              including handling for empty inbox states.
* Collaborators: ChatGPT and Google Gemini (GenAI Analysis baseline)
* Sources: ChatGPT, Google Gemini
* Creation Date: 09/16/2026
* Revision Date: 10/02/2026
* Revisions: Converted codebase from C++ to pure C structs/dynamic arrays, corrected 
*            reverse-chronological date tie-breaking, and added empty-inbox messaging.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure representing an email in the CEO's inbox.
struct Email {
    char category[50];
    char subject[150];
    char date[15];
    int date_value; // Integer representation (YYYYMMDD) for accurate date comparisons
};

// Structure representing a list-based MaxHeap priority queue.
struct MaxHeap {
    struct Email* data;
    int size;
    int capacity;
};

// Converts MM-DD-YYYY date string into a comparable integer YYYYMMDD.
int get_date_value(const char* date_str) {
    int month = 0, day = 0, year = 0;
    if (sscanf(date_str, "%d-%d-%d", &month, &day, &year) == 3) {
        return (year * 10000) + (month * 100) + day;
    }
    return 0;
}

// Converts sender category to a numerical priority weight (Boss highest).
int get_category_priority(const char* category) {
    if (strcmp(category, "Boss") == 0) return 5;
    if (strcmp(category, "Subordinate") == 0) return 4;
    if (strcmp(category, "Peer") == 0) return 3;
    if (strcmp(category, "ImportantPerson") == 0 || strcmp(category, "Important Person") == 0) return 2;
    return 1; // Other Person
}

// Compares two emails. Returns 1 if email 'a' has higher priority than email 'b'.
int compare_emails(struct Email a, struct Email b) {
    int prio_a = get_category_priority(a.category);
    int prio_b = get_category_priority(b.category);

    if (prio_a != prio_b) {
        return prio_a > prio_b;
    }

    // Tie-breaker: Newer date (larger YYYYMMDD value) comes first.
    return a.date_value > b.date_value;
}

// Initialize the MaxHeap.
void init_heap(struct MaxHeap* heap, int initial_capacity) {
    heap->size = 0;
    heap->capacity = initial_capacity;
    heap->data = (struct Email*)malloc(heap->capacity * sizeof(struct Email));
}

// Free allocated memory for the heap.
void free_heap(struct MaxHeap* heap) {
    if (heap->data) {
        free(heap->data);
        heap->data = NULL;
    }
    heap->size = 0;
    heap->capacity = 0;
}

// Helper to swap two emails.
void swap_emails(struct Email* a, struct Email* b) {
    struct Email temp = *a;
    *a = *b;
    *b = temp;
}

// Sift-up operation to maintain MaxHeap property.
void sift_up(struct MaxHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compare_emails(heap->data[index], heap->data[parent])) {
            swap_emails(&heap->data[index], &heap->data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

// Sift-down operation to maintain MaxHeap property.
void sift_down(struct MaxHeap* heap, int index) {
    while (2 * index + 1 < heap->size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < heap->size && compare_emails(heap->data[left], heap->data[largest])) {
            largest = left;
        }
        if (right < heap->size && compare_emails(heap->data[right], heap->data[largest])) {
            largest = right;
        }

        if (largest != index) {
            swap_emails(&heap->data[index], &heap->data[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

// Insert an email into the MaxHeap.
void heap_insert(struct MaxHeap* heap, struct Email email) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        heap->data = (struct Email*)realloc(heap->data, heap->capacity * sizeof(struct Email));
    }
    heap->data[heap->size] = email;
    sift_up(heap, heap->size);
    heap->size++;
}

// Remove the highest priority email from the heap.
void heap_remove_max(struct MaxHeap* heap) {
    if (heap->size <= 0) return;
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    sift_down(heap, 0);
}

// Utility to trim leading/trailing whitespace from parsed fields.
char* trim_whitespace(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int main() {
    struct MaxHeap inbox;
    init_heap(&inbox, 10);

    char line[512];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // Remove newline character if present
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) == 0) continue;

        // Process EMAIL command
        if (strncmp(line, "EMAIL", 5) == 0) {
            char* data = line + 6;
            char* token1 = strtok(data, ",");
            char* token2 = strtok(NULL, ",");
            char* token3 = strtok(NULL, ",");

            if (token1 && token2 && token3) {
                struct Email new_email;
                strcpy(new_email.category, trim_whitespace(token1));
                strcpy(new_email.subject, trim_whitespace(token2));
                strcpy(new_email.date, trim_whitespace(token3));
                new_email.date_value = get_date_value(new_email.date);

                heap_insert(&inbox, new_email);
            }
        }
        // Process NEXT command
        else if (strcmp(line, "NEXT") == 0) {
            if (inbox.size > 0) {
                struct Email top = inbox.data[0];
                printf("Next email:\n");
                printf("Sender: %s\n", top.category);
                printf("Subject: %s\n", top.subject);
                printf("Date: %s\n", top.date);
            } else {
                printf("Inbox is empty. No next email.\n");
            }
        }
        // Process READ command
        else if (strcmp(line, "READ") == 0) {
            if (inbox.size > 0) {
                heap_remove_max(&inbox);
            } else {
                printf("Inbox is empty. Nothing to read.\n");
            }
        }
        // Process COUNT command
        else if (strncmp(line, "COUNT", 5) == 0) {
            printf("There are %d emails to read.\n", inbox.size);
        }
    }

    free_heap(&inbox);
    return 0;
}