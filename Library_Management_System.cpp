#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void add_book();
void display_book();
void search_book();
void issue_book();
void return_book();

struct books {
    char title[50];
    char author[50];
    int book_id;
    int quantity;
    int year;
};

struct issue {
    int user_id;
    char name[50];
    int issue_book_id;
    char issue_book_title[50];
    char issue_date[20]; // Format: YYYY-MM-DD
};

int main() {
    int choice;
    do {
        printf("\n====== LIBRARY MANAGEMENT SYSTEM ======\n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                add_book();
                break;
            case 2:
                display_book();
                break;
            case 3:
                search_book();
                break;
            case 4:
                issue_book();
                break;
            case 5:
                return_book();
                break;
            case 6:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while(choice != 6);
    return 0;
}

void add_book() {
    FILE *fp = fopen("Book_Record.txt", "ab");
    struct books b;
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book Title: ");
    scanf(" %[^\n]", b.title);
    printf("Enter Book ID: ");
    scanf("%d", &b.book_id);
    printf("Enter Author Name: ");
    scanf(" %[^\n]", b.author);
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);
    printf("Enter Year: ");
    scanf("%d", &b.year);
    fwrite(&b, sizeof(struct books), 1, fp);
    fclose(fp);
    printf("Book added successfully.\n");
}

void display_book() {
    FILE *fp = fopen("Book_Record.txt", "rb");
    struct books b;
    int found = 0;
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\n--- Book List ---\n");
    while(fread(&b, sizeof(struct books), 1, fp)) {
        printf("\nTitle: %s\nID: %d\nAuthor: %s\nQuantity: %d\nYear: %d\n",
               b.title, b.book_id, b.author, b.quantity, b.year);
        found = 1;
    }

    if(!found) {
        printf("No books found.\n");
    }
    fclose(fp);
}

void search_book() {
    FILE *fp = fopen("Book_Record.txt", "rb");
    struct books b;
    int ch, found = 0;
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter Book ID to search: ");
    scanf("%d", &ch);
    while(fread(&b, sizeof(struct books), 1, fp)) {
        if(ch == b.book_id) {
            printf("Book Found:\n");
            printf("Title: %s\nID: %d\nAuthor: %s\nQuantity: %d\nYear: %d\n",
                   b.title, b.book_id, b.author, b.quantity, b.year);
            found = 1;
            break;
        }
    }
    if(!found) {
        printf("Book not found.\n");
    }

    fclose(fp);
}

void issue_book() {
    FILE *fp = fopen("Book_Record.txt", "rb+");
    FILE *fi = fopen("Book_Issue.txt", "ab");
    struct books b;
    struct issue i;
    int b_id, found = 0;

    if(fp == NULL || fi == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &b_id);
    while(fread(&b, sizeof(struct books), 1, fp)) {
        if(b_id == b.book_id) {
            found = 1;
            if(b.quantity > 0) {
                printf("Book Found.\n");
                printf("Enter User Name: ");
                scanf(" %[^\n]", i.name);
                printf("Enter User ID: ");
                scanf("%d", &i.user_id);
                printf("Enter Issue Date (YYYY-MM-DD): ");
                scanf(" %[^\n]", i.issue_date);
                i.issue_book_id = b.book_id;
                strcpy(i.issue_book_title, b.title);
                fwrite(&i, sizeof(struct issue), 1, fi);
                b.quantity -= 1;
                fseek(fp, -sizeof(struct books), SEEK_CUR);
                fwrite(&b, sizeof(struct books), 1, fp);
                printf("Book (ID: %d) issued to %s (User ID: %d).\n", b.book_id, i.name, i.user_id);
            } else {
                printf("Book is currently out of stock.\n");
            }
            break;
        }
    }

    if(!found) {
        printf("Book with ID %d not found.\n", b_id);
    }

    fclose(fp);
    fclose(fi);
}

void return_book() {
    int book_id, user_id, found = 0;
    struct issue i;
    printf("Enter Book ID to return: ");
    scanf("%d", &book_id);
    printf("Enter Your User ID: ");
    scanf("%d", &user_id);
    FILE *fi = fopen("Book_Issue.txt", "rb");
    FILE *temp = fopen("Temp_Issue.txt", "wb");
    if(fi == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while(fread(&i, sizeof(struct issue), 1, fi)) {
        if(i.issue_book_id == book_id && i.user_id == user_id) {
            found = 1;
            printf("Returning Book: %s issued by %s (User ID: %d)\n", i.issue_book_title, i.name, i.user_id);
        } else {
            fwrite(&i, sizeof(struct issue), 1, temp);
        }
    }

    fclose(fi);
    fclose(temp);
    if(!found) {
        printf("No matching issue record found.\n");
        remove("Temp_Issue.txt");
        return;
    }
    remove("Book_Issue.txt");
    rename("Temp_Issue.txt", "Book_Issue.txt");
    FILE *fp = fopen("Book_Record.txt", "rb+");
    struct books b;
    if(fp == NULL) {
        printf("Error opening book file.\n");
        return;
    }

    while(fread(&b, sizeof(struct books), 1, fp)) {
        if(b.book_id == book_id) {
            b.quantity += 1;
            fseek(fp, -sizeof(struct books), SEEK_CUR);
            fwrite(&b, sizeof(struct books), 1, fp);
            printf("Book quantity updated. New quantity: %d\n", b.quantity);
            break;
        }
    }

    fclose(fp);
}
