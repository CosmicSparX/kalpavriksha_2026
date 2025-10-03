#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "users.bin"
#define NAME_SIZE 50

typedef struct {
    int userId;
    char fullName[NAME_SIZE];
    int yearsOld;
} Record;

int generateId(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    int totalRecords = fileSize / sizeof(Record);
    return totalRecords + 1;
}

void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

void addUser() {
    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp) {
        fp = fopen(DATA_FILE, "wb+");
        if (!fp) {
            printf("Could not open data file.\n");
            return;
        }
    }

    Record r;
    r.userId = generateId(fp);

    printf("Enter full name: ");
    fgets(r.fullName, NAME_SIZE, stdin);
    trimNewline(r.fullName);

    printf("Enter age: ");
    if (scanf("%d", &r.yearsOld) != 1 || r.yearsOld < 1 || r.yearsOld > 120) {
        printf("Invalid age. Operation cancelled.\n");
        fclose(fp);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    rewind(fp);
    Record existing;
    while (fread(&existing, sizeof(Record), 1, fp)) {
        if (strcasecmp(existing.fullName, r.fullName) == 0) {
            printf("A user with this name already exists.\n");
            fclose(fp);
            return;
        }
    }

    fseek(fp, 0, SEEK_END);
    fwrite(&r, sizeof(Record), 1, fp);
    fclose(fp);
    printf("User '%s' added with ID %d.\n", r.fullName, r.userId);
}

void listUsers() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("No records available.\n");
        return;
    }

    Record r;
    int count = 0;

    printf("\n--- User Directory ---\n");
    while (fread(&r, sizeof(Record), 1, fp)) {
        printf("ID: %d | Name: %s | Age: %d\n", 
                r.userId, r.fullName, r.yearsOld);
        count++;
    }

    if (count == 0) {
        printf("No active users to display.\n");
    }
    printf("----------------------\n");
    fclose(fp);
}

void updateUser() {
    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp) {
        printf("Data file not found.\n");
        return;
    }

    int id, newAge;
    printf("Enter user ID to update: ");
    scanf("%d", &id);
    printf("Enter new age: ");
    if (scanf("%d", &newAge) != 1 || newAge < 1 || newAge > 120) {
        printf("Invalid age.\n");
        fclose(fp);
        while (getchar() != '\n'); 
        return;
    }
    while (getchar() != '\n'); 

    Record r;
    int found = 0;
    rewind(fp);
    while (fread(&r, sizeof(Record), 1, fp)) {
        if (r.userId == id) {
            r.yearsOld = newAge;
            fseek(fp, -sizeof(Record), SEEK_CUR);
            fwrite(&r, sizeof(Record), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (found)
        printf("User ID %d updated.\n", id);
    else
        printf("User not found or inactive.\n");
}

void deleteUser() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("Data file not found.\n");
        return;
    }

    int id;
    printf("Enter user ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Could not create temp file.\n");
        fclose(fp);
        return;
    }

    Record r;
    int found = 0;
    while (fread(&r, sizeof(Record), 1, fp)) {
        if (r.userId == id) {
            found = 1;
        } else {
            fwrite(&r, sizeof(Record), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(DATA_FILE);
    rename("temp.bin", DATA_FILE);

    if (found)
        printf("User ID %d has been permanently deleted.\n", id);
    else
        printf("User ID %d not found.\n", id);
}


int main() {
    int choice;
    do {
        printf("\n===== User Management =====\n");
        printf("1. Add User\n");
        printf("2. List Users\n");
        printf("3. Update User Age\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1: 
                addUser(); 
                break;
            case 2: 
                listUsers(); 
                break;
            case 3: 
                updateUser(); 
                break;
            case 4: 
                deleteUser(); 
                break;
            case 5: 
                printf("Exiting...\n"); 
                break;
            default: 
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}
