#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MARKS_LEN 3
#define MAX_NAME_LEN 50

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
void printRollNumbersRecursively(int n);

int main() {
    int n;
    scanf("%d", &n);

    Student *students = (Student *)malloc(n * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d %s %f %f %f", &students[i].Roll, students[i].Name, &students[i].Marks[0], &students[i].Marks[1], &students[i].Marks[2]);

        students[i].Total = calculateTotal(students[i].Marks);
        students[i].Average = calculateAverage(students[i].Total);
        students[i].Grade = calculateGrade(students[i].Average);
    }

    for (int i = 0; i < n; i++) {
        printf("\nRoll: %d\n", students[i].Roll);
        printf("Name: %s\n", students[i].Name);
        printf("Total: %d\n", students[i].Total);
        printf("Average: %.2f\n", students[i].Average);
        printf("Grade: %c\n", students[i].Grade);
        
        if (students[i].Average < 35) {
            continue;
        }
        printStarPattern(students[i].Grade);
    }
    
    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(n);
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
    if (average >= 85) return 'A';
    if (average >= 70) return 'B';
    if (average >= 50) return 'C';
    if (average >= 35) return 'D';
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

void printRollNumbersRecursively(int n) {
    if (n < 1) {
        return;
    }
    printRollNumbersRecursively(n - 1);
    printf("%d ", n);
}
