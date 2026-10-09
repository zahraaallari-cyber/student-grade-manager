Student Grade Manager

A command-line application written in C for managing student exam scores. Developed as a project for Basic Programming 1.

Features
Add and remove student records
Search students by name
Calculate averages and letter grades
View class statistics (average, highest, lowest)
Sort students by average score
Save and load records from a file
Technologies
Language: C
Concepts: functions, arrays, loops, strings, file I/O
Getting Started
Prerequisites

A C compiler such as GCC.

Build and Run
bash
gcc grades.c -o grades
./grades

On Windows PowerShell, run .\grades.exe instead.

Usage

The program displays a menu. Enter the number of an option to use it, and choose 7 to save your data before exiting.

1. Add student
2. Show all students
3. Search by name
4. Class statistics
5. Sort by average
6. Remove student
7. Save to file
0. Exit
Project Structure
.
├── main.c     # Source code
└── README.md    # Documentation