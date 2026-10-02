#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAILS 1000

typedef struct {
    char sender[100];
    char category[30];
    char date[11];
    char subject[200];
} Email;

typedef struct {
    Email emails[MAX_EMAILS];
    int size;
} MaxHeap;

/* Get the priority value for each sender category */
int get_category_priority(char category[]) {
    if (strcmp(category, "Boss") == 0)
        return 5;
    else if (strcmp(category, "Subordinate") == 0)
        return 4;
    else if (strcmp(category, "Peer") == 0)
        return 3;
    else if (strcmp(category, "ImportantPerson") == 0)
        return 2;
    else
        return 1;
}

/* Convert MM-DD-YYYY into YYYYMMDD so dates can be compared */
int get_date_value(char date[]) {
    int month, day, year;

    sscanf(date, "%d-%d-%d", &month, &day, &year);

    return year * 10000 + month * 100 + day;
}

/* Returns 1 if email1 has higher priority than email2 */
int higher_priority(Email email1, Email email2) {
    int priority1 = get_category_priority(email1.category);
    int priority2 = get_category_priority(email2.category);

    if (priority1 > priority2)
        return 1;

    if (priority1 < priority2)
        return 0;

    /* If categories match, compare dates */
    if (get_date_value(email1.date) > get_date_value(email2.date))
        return 1;

    return 0;
}

/* Swap two emails */
void swap_emails(Email *email1, Email *email2) {
    Email temp = *email1;
    *email1 = *email2;
    *email2 = temp;
}

/* Move an email up the heap */
void heapify_up(MaxHeap *heap, int index) {
    int parent;

    while (index > 0) {
        parent = (index - 1) / 2;

        if (higher_priority(heap->emails[index],
                             heap->emails[parent])) {
            swap_emails(&heap->emails[index],
                        &heap->emails[parent]);

            index = parent;
        } else {
            break;
        }
    }
}

/* Move an email down the heap */
void heapify_down(MaxHeap *heap, int index) {
    int left;
    int right;
    int largest;

    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        if (left < heap->size &&
            higher_priority(heap->emails[left],
                            heap->emails[largest])) {
            largest = left;
        }

        if (right < heap->size &&
            higher_priority(heap->emails[right],
                            heap->emails[largest])) {
            largest = right;
        }

        if (largest != index) {
            swap_emails(&heap->emails[index],
                        &heap->emails[largest]);

            index = largest;
        } else {
            break;
        }
    }
}

/* Add an email to the MaxHeap */
void insert_email(MaxHeap *heap, Email email) {
    if (heap->size >= MAX_EMAILS) {
        printf("Inbox is full.\n");
        return;
    }

    heap->emails[heap->size] = email;
    heap->size++;

    heapify_up(heap, heap->size - 1);
}

/* Remove and return the highest priority email */
Email remove_max(MaxHeap *heap) {
    Email top = heap->emails[0];

    heap->emails[0] = heap->emails[heap->size - 1];
    heap->size--;

    if (heap->size > 0)
        heapify_down(heap, 0);

    return top;
}

/* Display the next highest priority email */
void next_email(MaxHeap *heap) {
    if (heap->size == 0) {
        printf("No emails in inbox.\n");
        return;
    }

    printf("%s %s %s %s\n",
           heap->emails[0].sender,
           heap->emails[0].category,
           heap->emails[0].date,
           heap->emails[0].subject);
}

/* Read and remove the highest priority email */
void read_email(MaxHeap *heap) {
    Email email;

    if (heap->size == 0) {
        printf("No emails in inbox.\n");
        return;
    }

    email = remove_max(heap);

    printf("%s %s %s %s\n",
           email.sender,
           email.category,
           email.date,
           email.subject);
}

/* Print the number of emails currently in the inbox */
void count_emails(MaxHeap *heap) {
    printf("%d\n", heap->size);
}

int main(void) {
    MaxHeap heap;
    char command[20];
    Email email;

    heap.size = 0;

    while (scanf("%s", command) != EOF) {

        if (strcmp(command, "EMAIL") == 0) {

            /*
             * Expected format:
             * EMAIL sender category date subject
             */
            scanf("%s", email.sender);
            scanf("%s", email.category);
            scanf("%s", email.date);

            /*
             * Read the rest of the line as the subject.
             */
            getchar();
            fgets(email.subject, sizeof(email.subject), stdin);

            /* Remove newline from subject */
            email.subject[strcspn(email.subject, "\n")] = '\0';

            insert_email(&heap, email);
        }

        else if (strcmp(command, "NEXT") == 0) {
            next_email(&heap);
        }

        else if (strcmp(command, "READ") == 0) {
            read_email(&heap);
        }

        else if (strcmp(command, "COUNT") == 0) {
            count_emails(&heap);
        }
    }

    return 0;
}