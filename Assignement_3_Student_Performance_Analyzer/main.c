#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MARKS_LEN 3
#define MAX_NAME_LEN 50
#define LINE_BUFFER_SIZE 256

#define GRADE_A_THRESHOLD 85
#define GRADE_B_THRESHOLD 70
#define GRADE_C_THRESHOLD 50
#define GRADE_D_THRESHOLD 35

typedef struct {
    int Roll;
    char Name[MAX_NAME_LEN];
    float Marks[MARKS_LEN];
    int Total;
    float Average;
    char Grade;
} Student;

int calculateTotal(float marks[]);
float calculateAverage(int total);
char calculateGrade(float average);
void printStarPattern(char grade);
void printRollNumbersRecursively(int currentRollNo);
void trimLeadingWhitespace(char *str);

int main() {
    int total_students;
    scanf("%d", &total_students);
    getchar(); 

    Student *students = (Student *)malloc(total_students * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    char line_buffer[LINE_BUFFER_SIZE];
    for (int i = 0; i < total_students; i++) {
        if (fgets(line_buffer, sizeof(line_buffer), stdin) == NULL) {
            break; 
        }

        char *ptr = line_buffer;
        char *last_space;

        last_space = strrchr(ptr, ' ');
        if (last_space) {
            students[i].Marks[2] = atof(last_space + 1);
            *last_space = '\0';
        }

        last_space = strrchr(ptr, ' ');
        if (last_space) {
            students[i].Marks[1] = atof(last_space + 1);
            *last_space = '\0';
        }

        last_space = strrchr(ptr, ' ');
        if (last_space) {
            students[i].Marks[0] = atof(last_space + 1);
            *last_space = '\0';
        }

        sscanf(ptr, "%d %49[^\n]", &students[i].Roll, students[i].Name);

        students[i].Total = calculateTotal(students[i].Marks);
        students[i].Average = calculateAverage(students[i].Total);
        students[i].Grade = calculateGrade(students[i].Average);
    }

    for (int i = 0; i < total_students; i++) {
        printf("\nRoll: %d\n", students[i].Roll);
        printf("Name: %s\n", students[i].Name);
        printf("Total: %d\n", students[i].Total);
        printf("Average: %.2f\n", students[i].Average);
        printf("Grade: %c\n", students[i].Grade);

        if (students[i].Grade == 'F') {
            continue;
        }
        printStarPattern(students[i].Grade);
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(total_students);
    printf("\n");
    free(students);
    return 0;
}

int calculateTotal(float marks[]) {
    float sum = 0;
    for (int i = 0; i < MARKS_LEN; i++){
        sum += marks[i];
    }
    return (int)sum;
}

float calculateAverage(int total) {
    return (float)total / MARKS_LEN;
}

char calculateGrade(float average) {
    if (average >= GRADE_A_THRESHOLD) return 'A';
    if (average >= GRADE_B_THRESHOLD) return 'B';
    if (average >= GRADE_C_THRESHOLD) return 'C';
    if (average >= GRADE_D_THRESHOLD) return 'D';
    return 'F';
}

void printStarPattern(char grade) {
    printf("Performance: ");
    int num_stars = 5 - (grade - 'A'); 
    for (int i = 0; i < num_stars; i++) {
        printf("*");
    }
    printf("\n");
}

void printRollNumbersRecursively(int currentRollNo) {
    if (currentRollNo < 1) {
        return;
    }
    printRollNumbersRecursively(currentRollNo - 1);
    printf("%d ", currentRollNo);
}
