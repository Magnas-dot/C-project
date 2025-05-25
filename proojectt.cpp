#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addpatient();
void viewpatient();
void updatepatient();
void removepatient();
void searchpatient();

struct patient {
    int id;
    char name[50];
    char gender[10];
    int age;
    char disease[50];
};

int main() {
    int choice;
    do {
        printf("\n+----------------------------------------+ ");
        printf("\n|           Patient Record System        |");
        printf("\n+----------------------------------------+ ");
        printf("\n|  Enter 1 to add patient details:       |");
        printf("\n|  Enter 2 to view patient details:      |");
        printf("\n|  Enter 3 to search patient details:    |");
        printf("\n|  Enter 4 to remove patient details:    |");
        printf("\n|  Enter 5 to update patient details:    |");
        printf("\n|  Enter 6 to exit:                      |");
        printf("\n+----------------------------------------+ ");
        printf("\nPlease enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addpatient();
                break;
            case 2:
                viewpatient();
                break;
            case 3:
                searchpatient();
                break;
            case 4:
                removepatient();
                break;
            case 5:
                updatepatient();
                break;
            case 6:
                printf("\nExiting program...\n");
                break;
            default:
                printf("\nInvalid choice, please enter a valid number.\n");
        }
    } while (choice != 6);

    return 0;
}

void addpatient() {
    FILE *fp = fopen("patients.txt", "ab");
    struct patient p;
    if (fp == NULL) {
        printf("\nERROR opening file...\n");
        exit(1);
    }

    printf("\nEnter patient id: ");
    scanf("%d", &p.id);
    printf("Enter patient name: ");
    scanf(" %[^\n]", p.name);
    printf("Enter patient gender: ");
    scanf("%s", p.gender);
    printf("Enter patient age: ");
    scanf("%d", &p.age);
    printf("Enter patient disease: ");
    scanf(" %[^\n]", p.disease);

    fwrite(&p, sizeof(struct patient), 1, fp);
    fclose(fp);

    printf("\nPatient data added successfully.\n");
}

void viewpatient() {
    FILE *fp = fopen("patients.txt", "rb");
    struct patient p;
    int found = 0;
    if (fp == NULL) {
        printf("\nERROR opening file...\n");
        exit(1);
    }

    printf("\n-----Patients Info-----\n");
    printf("\n+--------------------------------------------------------------------------+ \n");
    printf("| %-5s | %-20s | %-10s | %-5s | %-20s |\n", "ID", "Name", "Gender", "Age", "Disease");
    printf("+--------------------------------------------------------------------------+ \n");

    while (fread(&p, sizeof(struct patient), 1, fp)) {
        printf("| %-5d | %-20s | %-10s | %-5d | %-20s |\n", p.id, p.name, p.gender, p.age, p.disease);
        found = 1;
    }

    if (found == 0) {
        printf("\nNo patient records found.\n");
    }
    printf("+--------------------------------------------------------------------------+ \n");
    fclose(fp);
}

void updatepatient() {
    FILE *fp = fopen("patients.txt", "rb+");
    struct patient p;
    int id, found = 0;
    if (fp == NULL) {
        printf("\nERROR opening file...\n");
        exit(1);
    }

    printf("\nEnter patient id to update: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(struct patient), 1, fp)) {
        if (id == p.id) {
            found = 1;
            printf("\nEnter new patient id: ");
            scanf("%d", &p.id);
            printf("Enter patient name: ");
            scanf(" %[^\n]", p.name);
            printf("Enter new patient gender: ");
            scanf("%s", p.gender);
            printf("Enter new patient age: ");
            scanf("%d", &p.age);
            printf("Enter new patient disease: ");
            scanf(" %[^\n]", p.disease);

            fseek(fp, -sizeof(struct patient), SEEK_CUR);
            fwrite(&p, sizeof(struct patient), 1, fp);
            printf("\nPatient data updated successfully.\n");
            break;
        }
    }

    if (found == 0) {
        printf("\nPatient not found.\n");
    }

    fclose(fp);
}

void searchpatient() {
    FILE *fp = fopen("patients.txt", "rb");
    struct patient p;
    int id, found = 0;
    if (fp == NULL) {
        printf("\nERROR opening file...\n");
        exit(1);
    }

    printf("\nEnter patient id to search for: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(struct patient), 1, fp)) {
        if (id == p.id) {
            found = 1;
            printf("\nRecord found!\n");
             printf("\n+--------------------------------------------------------------------------+ \n");
            printf("| %-5s | %-20s | %-10s | %-5s | %-20s |\n", "ID", "Name", "Gender", "Age", "Disease");
             printf("+--------------------------------------------------------------------------+ \n");
            printf("| %-5d | %-20s | %-10s | %-5d | %-20s |\n", p.id, p.name, p.gender, p.age, p.disease);
                printf("+--------------------------------------------------------------------------+\n");
            break;
        }
    }

    if (found == 0) {
        printf("\nPatient not found.\n");
    }

    fclose(fp);
}

void removepatient() {
    FILE *fp = fopen("patients.txt", "rb");
    struct patient p;
    int found = 0, id;
    if (fp == NULL) {
        printf("\nERROR opening file...\n");
        exit(1);
    }

    FILE *temp = fopen("temp.dat", "wb");

    printf("\nEnter patient id to remove: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(struct patient), 1, fp)) {
        if (id == p.id) {
            printf("\nPatient data removed successfully.\n");
            found = 1;
        } else {
            fwrite(&p, sizeof(struct patient), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("patients.txt");
    rename("temp.dat", "patients.txt");

    if (found == 0) {
        printf("\nPatient not found.\n");
    }
}
