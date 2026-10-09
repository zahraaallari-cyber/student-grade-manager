/*
 * Student Grade Manager
 * Basic Programming 1 project
 *
 * Concepts used: functions, arrays (as lists), loops, strings, file I/O
 *
 * Compile: gcc grades.c -o grades
 * Run:     ./grades
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_STUDENTS 50
#define NAME_LEN 50
#define NUM_EXAMS 3
#define DATA_FILE "students.txt"
 
/* The "lists": parallel arrays where index i describes student i */
char  names[MAX_STUDENTS][NAME_LEN];
float scores[MAX_STUDENTS][NUM_EXAMS];
int   count = 0;   /* how many students are stored right now */
 
/* ---------- Input helpers ---------- */
 
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        printf("\nInput ended. Goodbye!\n");
        exit(0);
    }
    buffer[strcspn(buffer, "\n")] = '\0';   /* remove trailing newline */
}
 
int readInt(const char *prompt, int min, int max) {
    char line[100];
    int value;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("  Please enter a whole number between %d and %d.\n", min, max);
    }
}
 
float readScore(const char *prompt) {
    char line[100];
    float value;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (sscanf(line, "%f", &value) == 1 && value >= 0 && value <= 100) {
            return value;
        }
        printf("  Please enter a score between 0 and 100.\n");
    }
}
 
/* ---------- Calculation functions ---------- */
 
float studentAverage(int index) {
    float sum = 0;
    for (int i = 0; i < NUM_EXAMS; i++) {
        sum += scores[index][i];
    }
    return sum / NUM_EXAMS;
}
 
char letterGrade(float average) {
    if (average >= 90) return 'A';
    if (average >= 80) return 'B';
    if (average >= 70) return 'C';
    if (average >= 60) return 'D';
    return 'F';
}
 
/* ---------- Menu actions ---------- */
 
void printHeader(void) {
    printf("\n%-3s %-20s", "#", "Name");
    for (int e = 1; e <= NUM_EXAMS; e++) {
        printf(" Exam%d", e);
    }
    printf("   Avg  Grade\n");
    printf("------------------------------------------------------\n");
}
 
void printStudent(int index) {
    float avg = studentAverage(index);
    printf("%-3d %-20s", index + 1, names[index]);
    for (int e = 0; e < NUM_EXAMS; e++) {
        printf(" %5.1f", scores[index][e]);
    }
    printf(" %6.1f    %c\n", avg, letterGrade(avg));
}
 
void addStudent(void) {
    if (count >= MAX_STUDENTS) {
        printf("The list is full (%d students max).\n", MAX_STUDENTS);
        return;
    }
 
    printf("Student name: ");
    readLine(names[count], NAME_LEN);
    if (strlen(names[count]) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }
 
    for (int e = 0; e < NUM_EXAMS; e++) {
        char prompt[40];
        sprintf(prompt, "  Score for exam %d (0-100): ", e + 1);
        scores[count][e] = readScore(prompt);
    }
 
    count++;
    printf("Student added.\n");
}
 
void showAll(void) {
    if (count == 0) {
        printf("No students yet.\n");
        return;
    }
    printHeader();
    for (int i = 0; i < count; i++) {
        printStudent(i);
    }
}
 
void searchStudent(void) {
    char query[NAME_LEN];
    int found = 0;
 
    printf("Name to search for: ");
    readLine(query, NAME_LEN);
 
    for (int i = 0; i < count; i++) {
        if (strstr(names[i], query) != NULL) {   /* partial match */
            if (!found) printHeader();
            printStudent(i);
            found = 1;
        }
    }
    if (!found) {
        printf("No student matches \"%s\".\n", query);
    }
}
 
void showStatistics(void) {
    if (count == 0) {
        printf("No students yet.\n");
        return;
    }
 
    int best = 0, worst = 0;
    float total = 0;
 
    for (int i = 0; i < count; i++) {
        float avg = studentAverage(i);
        total += avg;
        if (avg > studentAverage(best))  best = i;
        if (avg < studentAverage(worst)) worst = i;
    }
 
    printf("\nStudents:      %d\n", count);
    printf("Class average: %.1f\n", total / count);
    printf("Highest:       %s (%.1f)\n", names[best], studentAverage(best));
    printf("Lowest:        %s (%.1f)\n", names[worst], studentAverage(worst));
}
 
void swapStudents(int a, int b) {
    char tempName[NAME_LEN];
    float tempScore;
 
    strcpy(tempName, names[a]);
    strcpy(names[a], names[b]);
    strcpy(names[b], tempName);
 
    for (int e = 0; e < NUM_EXAMS; e++) {
        tempScore = scores[a][e];
        scores[a][e] = scores[b][e];
        scores[b][e] = tempScore;
    }
}
 
void sortByAverage(void) {
    /* Bubble sort, highest average first */
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (studentAverage(j) < studentAverage(j + 1)) {
                swapStudents(j, j + 1);
            }
        }
    }
    printf("Sorted by average (highest first).\n");
    showAll();
}
 
void removeStudent(void) {
    if (count == 0) {
        printf("No students yet.\n");
        return;
    }
    showAll();
    int number = readInt("Number of student to remove (0 to cancel): ", 0, count);
    if (number == 0) return;
 
    int index = number - 1;
    /* Shift everything after it one step to the left */
    for (int i = index; i < count - 1; i++) {
        strcpy(names[i], names[i + 1]);
        for (int e = 0; e < NUM_EXAMS; e++) {
            scores[i][e] = scores[i + 1][e];
        }
    }
    count--;
    printf("Student removed.\n");
}
 
/* ---------- File storage ---------- */
 
void saveToFile(void) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Could not open %s for writing.\n", DATA_FILE);
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", names[i]);
        for (int e = 0; e < NUM_EXAMS; e++) {
            fprintf(file, "%.1f ", scores[i][e]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Saved %d students to %s.\n", count, DATA_FILE);
}
 
void loadFromFile(void) {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) return;   /* no file yet: start empty */
 
    char line[200];
    count = 0;
    while (count < MAX_STUDENTS && fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        strncpy(names[count], line, NAME_LEN - 1);
        names[count][NAME_LEN - 1] = '\0';
 
        if (fgets(line, sizeof(line), file) == NULL) break;
        if (sscanf(line, "%f %f %f",
                   &scores[count][0], &scores[count][1], &scores[count][2]) != NUM_EXAMS) {
            break;
        }
        count++;
    }
    fclose(file);
    printf("Loaded %d students from %s.\n", count, DATA_FILE);
}
 
/* ---------- Main menu ---------- */
 
void printMenu(void) {
    printf("\n===== Student Grade Manager =====\n");
    printf("1. Add student\n");
    printf("2. Show all students\n");
    printf("3. Search by name\n");
    printf("4. Class statistics\n");
    printf("5. Sort by average\n");
    printf("6. Remove student\n");
    printf("7. Save to file\n");
    printf("0. Exit\n");
}
 
int main(void) {
    int choice;
 
    loadFromFile();
 
    do {
        printMenu();
        choice = readInt("Choose an option: ", 0, 7);
 
        switch (choice) {
            case 1: addStudent();      break;
            case 2: showAll();         break;
            case 3: searchStudent();   break;
            case 4: showStatistics();  break;
            case 5: sortByAverage();   break;
            case 6: removeStudent();   break;
            case 7: saveToFile();      break;
            case 0: printf("Goodbye!\n"); break;
        }
    } while (choice != 0);
 
    return 0;
}
 