#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "users.txt"
#define TEMP_FILE "temp.txt"
#define NAME_SIZE 50

typedef struct {
    int userId;
    char fullName[NAME_SIZE];
    int yearsOld;
} Record;

int generateId() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        return 1;
    }

    int maxId = 0;
    Record r;
    while (fscanf(fp, "%d,%49[^,],%d\n", &r.userId, r.fullName, &r.yearsOld) == 3) {
        if (r.userId > maxId) {
            maxId = r.userId;
        }
    }
    fclose(fp);
    return maxId + 1;
}

void removeTrailingNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int isDuplicateName(const char *name) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        return 0;
    }
    Record existing;
    int found = 0;
    while (fscanf(fp, "%d,%49[^,],%d\n", &existing.userId, existing.fullName, &existing.yearsOld) == 3) {
        if (strcasecmp(existing.fullName, name) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

void addUser() {
    Record r;
    printf("Enter full name: ");
    fgets(r.fullName, NAME_SIZE, stdin);
    removeTrailingNewline(r.fullName);

    if (isDuplicateName(r.fullName)) {
        printf("A user with this name already exists.\n");
        return;
    }

    printf("Enter age: ");
    if (scanf("%d", &r.yearsOld) != 1 || r.yearsOld < 1 || r.yearsOld > 120) {
        printf("Invalid age. Operation cancelled.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    FILE *fp = fopen(DATA_FILE, "a");
    if (!fp) {
        printf("Could not open data file.\n");
        return;
    }

    r.userId = generateId();
    fprintf(fp, "%d,%s,%d\n", r.userId, r.fullName, r.yearsOld);
    fclose(fp);
    printf("User '%s' added with ID %d.\n", r.fullName, r.userId);
}

void listUsers() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        printf("No records available.\n");
        return;
    }

    Record r;
    int count = 0;
    printf("\n--- User Directory ---\n");
    while (fscanf(fp, "%d,%49[^,],%d\n", &r.userId, r.fullName, &r.yearsOld) == 3) {
        printf("ID: %-4d | Name: %-20s | Age: %d\n", r.userId, r.fullName, r.yearsOld);
        count++;
    }

    if (count == 0) {
        printf("No active users to display.\n");
    }
    printf("----------------------\n");
    fclose(fp);
}

void updateUser() {
    int id, newAge, found = 0;
    printf("Enter user ID to update: ");
    scanf("%d", &id);
    printf("Enter new age: ");
    if (scanf("%d", &newAge) != 1 || newAge < 1 || newAge > 120) {
        printf("Invalid age.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        printf("Data file not found.\n");
        return;
    }
    FILE *temp = fopen(TEMP_FILE, "w");
    if (!temp) {
        printf("Could not create temp file.\n");
        fclose(fp);
        return;
    }

    Record r;
    while (fscanf(fp, "%d,%49[^,],%d\n", &r.userId, r.fullName, &r.yearsOld) == 3) {
        if (r.userId == id) {
            r.yearsOld = newAge;
            found = 1;
        }
        fprintf(temp, "%d,%s,%d\n", r.userId, r.fullName, r.yearsOld);
    }

    fclose(fp);
    fclose(temp);

    remove(DATA_FILE);
    rename(TEMP_FILE, DATA_FILE);

    if (found)
        printf("User ID %d updated.\n", id);
    else
        printf("User not found.\n");
}

void deleteUser() {
    int id, found = 0;
    printf("Enter user ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        printf("Data file not found.\n");
        return;
    }
    FILE *temp = fopen(TEMP_FILE, "w");
    if (!temp) {
        printf("Could not create temp file.\n");
        fclose(fp);
        return;
    }

    Record r;
    while (fscanf(fp, "%d,%49[^,],%d\n", &r.userId, r.fullName, &r.yearsOld) == 3) {
        if (r.userId == id) {
            found = 1;
        } else {
            fprintf(temp, "%d,%s,%d\n", r.userId, r.fullName, r.yearsOld);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(DATA_FILE);
    rename(TEMP_FILE, DATA_FILE);

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
            case 1: addUser(); break;
            case 2: listUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}
