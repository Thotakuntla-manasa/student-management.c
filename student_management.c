/*
 * ============================================================
 *   STUDENT MANAGEMENT SYSTEM
 *   Features: Add, Delete, Update, Search, Display Records
 *   Storage : Binary file (students.dat) for permanence
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ─── Constants ─────────────────────────────────────────── */
#define FILE_NAME   "students.dat"
#define MAX_NAME    60
#define MAX_COURSE  40
#define MAX_EMAIL   60
#define LINE        "─────────────────────────────────────────────────────────────\n"
#define DLINE       "═════════════════════════════════════════════════════════════\n"

/* ─── Structure ──────────────────────────────────────────── */
typedef struct {
    int    roll;
    char   name[MAX_NAME];
    char   course[MAX_COURSE];
    int    year;
    float  cgpa;
    char   email[MAX_EMAIL];
    char   phone[15];
} Student;

/* ─── Prototypes ─────────────────────────────────────────── */
void  clearScreen(void);
void  pauseScreen(void);
void  printHeader(const char *title);
void  printStudentRow(const Student *s);
void  printTableHeader(void);

int   rollExists(int roll);
int   countRecords(void);

void  addStudent(void);
void  displayAll(void);
void  searchStudent(void);
void  updateStudent(void);
void  deleteStudent(void);
void  showStats(void);
void  mainMenu(void);

/* ─── Utility ────────────────────────────────────────────── */
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void) {
    printf("\n  Press ENTER to continue...");
    while (getchar() != '\n');
}

void printHeader(const char *title) {
    printf("\n" DLINE);
    printf("  %-59s\n", title);
    printf(DLINE);
}

void printTableHeader(void) {
    printf("\n  %-6s  %-20s  %-15s  %-4s  %-5s  %-20s  %-12s\n",
           "Roll", "Name", "Course", "Year", "CGPA", "Email", "Phone");
    printf("  " LINE);
}

void printStudentRow(const Student *s) {
    printf("  %-6d  %-20s  %-15s  %-4d  %-5.2f  %-20s  %-12s\n",
           s->roll, s->name, s->course, s->year, s->cgpa, s->email, s->phone);
}

/* ─── File Helpers ───────────────────────────────────────── */
int rollExists(int roll) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;
    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1)
        if (s.roll == roll) { fclose(fp); return 1; }
    fclose(fp);
    return 0;
}

int countRecords(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    int n = (int)(ftell(fp) / sizeof(Student));
    fclose(fp);
    return n;
}

/* ─── 1. Add Student ─────────────────────────────────────── */
void addStudent(void) {
    clearScreen();
    printHeader("  ADD NEW STUDENT");

    Student s;

    /* Roll number */
    while (1) {
        printf("\n  Enter Roll Number : ");
        if (scanf("%d", &s.roll) != 1 || s.roll <= 0) {
            printf("  [!] Invalid roll number. Try again.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (rollExists(s.roll)) {
            printf("  [!] Roll #%d already exists. Try another.\n", s.roll);
        } else break;
    }

    printf("  Enter Name        : ");
    fgets(s.name, MAX_NAME, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("  Enter Course      : ");
    fgets(s.course, MAX_COURSE, stdin);
    s.course[strcspn(s.course, "\n")] = '\0';

    printf("  Enter Year (1-6)  : ");
    scanf("%d", &s.year);
    while (getchar() != '\n');
    if (s.year < 1 || s.year > 6) s.year = 1;

    printf("  Enter CGPA (0-10) : ");
    scanf("%f", &s.cgpa);
    while (getchar() != '\n');
    if (s.cgpa < 0 || s.cgpa > 10) s.cgpa = 0.0f;

    printf("  Enter Email       : ");
    fgets(s.email, MAX_EMAIL, stdin);
    s.email[strcspn(s.email, "\n")] = '\0';

    printf("  Enter Phone       : ");
    fgets(s.phone, 15, stdin);
    s.phone[strcspn(s.phone, "\n")] = '\0';

    /* Write to file */
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) { printf("\n  [!] Error opening file!\n"); pauseScreen(); return; }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\n  [✓] Student #%d (%s) added successfully!\n", s.roll, s.name);
    pauseScreen();
}

/* ─── 2. Display All ─────────────────────────────────────── */
void displayAll(void) {
    clearScreen();
    printHeader("  ALL STUDENT RECORDS");

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("\n  No records found.\n"); pauseScreen(); return; }

    Student s;
    int count = 0;
    printTableHeader();
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printStudentRow(&s);
        count++;
    }
    fclose(fp);

    if (count == 0)
        printf("\n  No records found.\n");
    else
        printf("\n  Total records: %d\n", count);

    pauseScreen();
}

/* ─── 3. Search Student ──────────────────────────────────── */
void searchStudent(void) {
    clearScreen();
    printHeader("  SEARCH STUDENT");

    printf("\n  Search by:\n");
    printf("   [1] Roll Number\n");
    printf("   [2] Name\n");
    printf("  Choice: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n');

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("\n  No records found.\n"); pauseScreen(); return; }

    Student s;
    int found = 0;

    if (choice == 1) {
        int roll;
        printf("  Enter Roll Number: ");
        scanf("%d", &roll);
        while (getchar() != '\n');
        printTableHeader();
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (s.roll == roll) { printStudentRow(&s); found++; }
        }
    } else {
        char keyword[MAX_NAME];
        printf("  Enter Name (partial ok): ");
        fgets(keyword, MAX_NAME, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        /* lowercase for case-insensitive match */
        for (int i = 0; keyword[i]; i++) keyword[i] = tolower(keyword[i]);
        printTableHeader();
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            char lower[MAX_NAME];
            strncpy(lower, s.name, MAX_NAME);
            for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
            if (strstr(lower, keyword)) { printStudentRow(&s); found++; }
        }
    }

    fclose(fp);
    printf("\n  %d record(s) found.\n", found);
    pauseScreen();
}

/* ─── 4. Update Student ──────────────────────────────────── */
void updateStudent(void) {
    clearScreen();
    printHeader("  UPDATE STUDENT RECORD");

    printf("\n  Enter Roll Number to update: ");
    int roll;
    scanf("%d", &roll);
    while (getchar() != '\n');

    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) { printf("\n  No records found.\n"); pauseScreen(); return; }

    Student s;
    int found = 0;
    long pos;

    while ((pos = ftell(fp)), fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
            printf("\n  Current record:\n");
            printTableHeader();
            printStudentRow(&s);
            printf("\n  Enter new details (press ENTER to keep existing):\n\n");

            char buf[MAX_NAME];

            printf("  Name  [%s]: ", s.name);
            fgets(buf, MAX_NAME, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strncpy(s.name, buf, MAX_NAME);

            printf("  Course[%s]: ", s.course);
            fgets(buf, MAX_COURSE, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strncpy(s.course, buf, MAX_COURSE);

            printf("  Year  [%d]: ", s.year);
            fgets(buf, 10, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) s.year = atoi(buf);

            printf("  CGPA  [%.2f]: ", s.cgpa);
            fgets(buf, 10, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) s.cgpa = atof(buf);

            printf("  Email [%s]: ", s.email);
            fgets(buf, MAX_EMAIL, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strncpy(s.email, buf, MAX_EMAIL);

            printf("  Phone [%s]: ", s.phone);
            fgets(buf, 15, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strncpy(s.phone, buf, 15);

            /* Seek back and overwrite */
            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(Student), 1, fp);
            printf("\n  [✓] Record updated successfully!\n");
            break;
        }
    }
    fclose(fp);

    if (!found) printf("\n  [!] Roll #%d not found.\n", roll);
    pauseScreen();
}

/* ─── 5. Delete Student ──────────────────────────────────── */
void deleteStudent(void) {
    clearScreen();
    printHeader("  DELETE STUDENT RECORD");

    printf("\n  Enter Roll Number to delete: ");
    int roll;
    scanf("%d", &roll);
    while (getchar() != '\n');

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("\n  No records found.\n"); pauseScreen(); return; }

    /* Load all records except the target into temp file */
    FILE *tmp = fopen("students_tmp.dat", "wb");
    if (!tmp) { fclose(fp); printf("\n  [!] Temp file error.\n"); pauseScreen(); return; }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
            printf("\n  Found:\n");
            printTableHeader();
            printStudentRow(&s);
            printf("\n  Confirm delete? (y/n): ");
            char c = getchar();
            while (getchar() != '\n');
            if (tolower(c) != 'y') {
                fwrite(&s, sizeof(Student), 1, tmp); /* keep it */
                found = 0; /* treat as cancelled */
            }
        } else {
            fwrite(&s, sizeof(Student), 1, tmp);
        }
    }
    fclose(fp);
    fclose(tmp);

    /* Replace original with temp */
    remove(FILE_NAME);
    rename("students_tmp.dat", FILE_NAME);

    if (found)
        printf("\n  [✓] Roll #%d deleted successfully!\n", roll);
    else if (!found)
        printf("\n  [i] Operation cancelled or roll not found.\n");

    pauseScreen();
}

/* ─── 6. Statistics ──────────────────────────────────────── */
void showStats(void) {
    clearScreen();
    printHeader("  STATISTICS");

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("\n  No records found.\n"); pauseScreen(); return; }

    Student s, top, low;
    int count = 0;
    float sum = 0;
    top.cgpa = -1; low.cgpa = 11;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        sum += s.cgpa;
        if (s.cgpa > top.cgpa) top = s;
        if (s.cgpa < low.cgpa) low = s;
        count++;
    }
    fclose(fp);

    if (count == 0) { printf("\n  No records found.\n"); pauseScreen(); return; }

    printf("\n  Total Students   : %d\n", count);
    printf("  Average CGPA     : %.2f\n", sum / count);
    printf("  Highest CGPA     : %.2f  (%s, Roll #%d)\n", top.cgpa, top.name, top.roll);
    printf("  Lowest  CGPA     : %.2f  (%s, Roll #%d)\n", low.cgpa, low.name, low.roll);
    printf("  Data File        : %s\n", FILE_NAME);

    pauseScreen();
}

/* ─── Main Menu ──────────────────────────────────────────── */
void mainMenu(void) {
    int choice;
    do {
        clearScreen();
        printf("\n" DLINE);
        printf("        STUDENT MANAGEMENT SYSTEM\n");
        printf(DLINE);
        printf("\n   [1]  Add Student\n");
        printf("   [2]  Display All Records\n");
        printf("   [3]  Search Student\n");
        printf("   [4]  Update Student\n");
        printf("   [5]  Delete Student\n");
        printf("   [6]  Statistics\n");
        printf("   [0]  Exit\n\n");
        printf(LINE);
        printf("   Choice: ");

        if (scanf("%d", &choice) != 1) choice = -1;
        while (getchar() != '\n');

        switch (choice) {
            case 1: addStudent();     break;
            case 2: displayAll();     break;
            case 3: searchStudent();  break;
            case 4: updateStudent();  break;
            case 5: deleteStudent();  break;
            case 6: showStats();      break;
            case 0: printf("\n  Goodbye!\n\n"); break;
            default: printf("\n  [!] Invalid choice.\n"); pauseScreen();
        }
    } while (choice != 0);
}

/* ─── Entry Point ────────────────────────────────────────── */
int main(void) {
    mainMenu();
    return 0;
}
