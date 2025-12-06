#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUDENT_FILE    "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

typedef struct {
    int   roll;
    char  name[50];
    char  dept[30];
    int   year;         // 1–4
    float CGPA;         // 0.0 – 10.0
} Student;

char loggedInUser[30];
char loggedInRole[20];

/*-------------------------------------------------
    UTILITY FUNCTIONS
-------------------------------------------------*/

void flushInput() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void toLowerStr(char *s) {
    while (*s) {
        *s = (char)tolower((unsigned char)*s);
        s++;
    }
}

int equalsIgnoreCase(const char *a, const char *b) {
    char ta[100], tb[100];
    strncpy(ta, a, sizeof(ta) - 1);
    strncpy(tb, b, sizeof(tb) - 1);
    ta[sizeof(ta) - 1] = '\0';
    tb[sizeof(tb) - 1] = '\0';

    toLowerStr(ta);
    toLowerStr(tb);

    return strcmp(ta, tb) == 0;
}

/* Check if roll already present in file */
int rollAlreadyUsed(int roll) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;   // no file = no duplicate

    Student st;
    // roll,name,dept,year,CGPA
    while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                  &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
        if (st.roll == roll) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/*-------------------------------------------------
    GRADE SYSTEM (CGPA 0.0 – 10.0)
-------------------------------------------------*/

const char* getGrade(float CGPA) {
    if (CGPA >= 9.0f)       return "O";   // Outstanding
    else if (CGPA >= 8.0f)  return "A+";
    else if (CGPA >= 7.0f)  return "A";
    else if (CGPA >= 6.0f)  return "B";
    else if (CGPA >= 5.0f)  return "C";
    else if (CGPA >= 4.0f)  return "D";
    else                    return "F";  // Fail
}

/*-------------------------------------------------
    LOGIN RELATED
-------------------------------------------------*/

int verifyCredentials(const char *user, const char *pass, char *roleOut) {
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("\n[ERROR] Could not open %s\n", CREDENTIAL_FILE);
        return 0;
    }

    char fileUser[30], filePass[30], fileRole[20];
    int found = 0;

    while (fscanf(fp, "%29s %29s %19s", fileUser, filePass, fileRole) == 3) {
        if (equalsIgnoreCase(user, fileUser) && strcmp(pass, filePass) == 0) {
            strcpy(roleOut, fileRole);
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

int performLogin() {
    char username[30], password[30];
    int triesLeft = 3;

    while (triesLeft > 0) {
        printf("\n********** SRM LOGIN AUTHENTICATION **********\n");
        printf("Username : ");
        scanf("%29s", username);
        printf("Password : ");
        scanf("%29s", password);

        if (verifyCredentials(username, password, loggedInRole)) {
            strcpy(loggedInUser, username);
            return 1;
        }

        triesLeft--;
        printf("\nInvalid username or password. Tries left: %d\n", triesLeft);
    }

    printf("\nToo many failed attempts. Exiting program.\n");
    return 0;
}

/*-------------------------------------------------
    DEPARTMENT INPUT (VALIDATED)
-------------------------------------------------*/

void readValidDepartment(char *dept, size_t size) {
    while (1) {
        printf("Enter Department [CSE/ECE/EEE/MECH/CIVIL]: ");
        fgets(dept, (int)size, stdin);
        dept[strcspn(dept, "\n")] = '\0';

        // convert to uppercase for consistency
        for (int i = 0; dept[i]; i++)
            dept[i] = (char)toupper((unsigned char)dept[i]);

        if (strcmp(dept, "CSE") == 0 ||
            strcmp(dept, "ECE") == 0 ||
            strcmp(dept, "EEE") == 0 ||
            strcmp(dept, "MECH") == 0 ||
            strcmp(dept, "CIVIL") == 0) {
            break;
        }

        printf("Invalid department! Please enter one of: CSE, ECE, EEE, MECH, CIVIL.\n");
    }
}

/*-------------------------------------------------
    STUDENT OPERATIONS
-------------------------------------------------*/

void addStudentRecord() {
    Student st;
    FILE *fp;

    printf("\n--- Add New SRM Student Record ---\n");

    printf("Enter Roll No           : ");
    while (scanf("%d", &st.roll) != 1 || st.roll <= 0) {
        printf("Invalid input. Enter a positive integer: ");
        flushInput();
    }
    flushInput();

    if (rollAlreadyUsed(st.roll)) {
        printf("\n[ERROR] Roll number already exists. Try another.\n");
        return;
    }

    printf("Enter Name              : ");
    fgets(st.name, sizeof(st.name), stdin);
    st.name[strcspn(st.name, "\n")] = '\0';

    readValidDepartment(st.dept, sizeof(st.dept));

    printf("Enter Year (1-4)        : ");
    while (scanf("%d", &st.year) != 1 || st.year < 1 || st.year > 4) {
        printf("Invalid year. Enter 1–4: ");
        flushInput();
    }

    printf("Enter CGPA (0.0 - 10.0) : ");
    while (scanf("%f", &st.CGPA) != 1 || st.CGPA < 0.0f || st.CGPA > 10.0f) {
        printf("Invalid CGPA. Enter a value between 0.0 and 10.0: ");
        flushInput();
    }

    fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("\n[ERROR] Could not open %s for writing.\n", STUDENT_FILE);
        return;
    }

    // Store as: roll,name,dept,year,CGPA
    fprintf(fp, "%d,%s,%s,%d,%.2f\n",
            st.roll, st.name, st.dept, st.year, st.CGPA);
    fclose(fp);

    printf("\nStudent added successfully. Grade: %s\n", getGrade(st.CGPA));
}

void showAllStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("\nNo student data found.\n");
        return;
    }

    Student st;
    printf("\n=========== SRM STUDENT RECORDS ===========\n");
    printf("%-6s %-20s %-8s %-6s %-8s %-6s\n",
           "Roll", "Name", "Dept", "Year", "CGPA", "Grade");
    printf("--------------------------------------------------------\n");

    while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                  &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
        printf("%-6d %-20s %-8s %-6d %-8.2f %-6s\n",
               st.roll, st.name, st.dept, st.year, st.CGPA, getGrade(st.CGPA));
    }

    fclose(fp);
}

void searchStudentRecord() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("\nNo student data to search.\n");
        return;
    }

    int choice;
    int rollToSearch;
    char nameToSearch[50];
    int found = 0;
    Student st;

    printf("\n--- Search SRM Student Record ---\n");
    printf("1. By Roll\n");
    printf("2. By Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    flushInput();

    if (choice == 1) {
        printf("Enter Roll No: ");
        while (scanf("%d", &rollToSearch) != 1) {
            printf("Invalid input. Enter roll again: ");
            flushInput();
        }
        flushInput();

        while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                      &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
            if (st.roll == rollToSearch) {
                printf("\nRecord found:\n");
                printf("Roll : %d\nName : %s\nDept : %s\nYear : %d\nCGPA: %.2f\nGrade: %s\n",
                       st.roll, st.name, st.dept, st.year, st.CGPA, getGrade(st.CGPA));
                found = 1;
                break;
            }
        }
    } else if (choice == 2) {
        printf("Enter Name : ");
        fgets(nameToSearch, sizeof(nameToSearch), stdin);
        nameToSearch[strcspn(nameToSearch, "\n")] = '\0';

        while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                      &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
            if (equalsIgnoreCase(st.name, nameToSearch)) {
                printf("\nRecord found:\n");
                printf("Roll : %d\nName : %s\nDept : %s\nYear : %d\nCGPA: %.2f\nGrade: %s\n",
                       st.roll, st.name, st.dept, st.year, st.CGPA, getGrade(st.CGPA));
                found = 1;
                break;
            }
        }
    } else {
        printf("\nInvalid choice.\n");
    }

    if (!found)
        printf("\nNo matching record found.\n");

    fclose(fp);
}

void modifyStudentRecord() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("\n[ERROR] Could not open files for updating.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    int rollToEdit;
    Student st;
    int found = 0;

    printf("\n--- Update SRM Student ---\n");
    printf("Enter Roll No to update: ");
    while (scanf("%d", &rollToEdit) != 1) {
        printf("Invalid input. Enter roll again: ");
        flushInput();
    }
    flushInput();

    while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                  &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
        if (st.roll == rollToEdit) {
            found = 1;

            printf("Current Name : %s\n", st.name);
            printf("Current Dept : %s\n", st.dept);
            printf("Current Year : %d\n", st.year);
            printf("Current CGPA: %.2f (Grade: %s)\n", st.CGPA, getGrade(st.CGPA));

            printf("Enter New Name  : ");
            fgets(st.name, sizeof(st.name), stdin);
            st.name[strcspn(st.name, "\n")] = '\0';

            readValidDepartment(st.dept, sizeof(st.dept));

            printf("Enter New Year (1-4): ");
            while (scanf("%d", &st.year) != 1 || st.year < 1 || st.year > 4) {
                printf("Invalid year. Enter 1–4: ");
                flushInput();
            }

            printf("Enter New CGPA (0.0 - 10.0): ");
            while (scanf("%f", &st.CGPA) != 1 || st.CGPA < 0.0f || st.CGPA > 10.0f) {
                printf("Invalid CGPA. Enter a value between 0.0 and 10.0: ");
                flushInput();
            }
            flushInput();

            printf("Updated Grade will be: %s\n", getGrade(st.CGPA));
        }
        fprintf(temp, "%d,%s,%s,%d,%.2f\n",
                st.roll, st.name, st.dept, st.year, st.CGPA);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nStudent updated successfully.\n");
    else
        printf("\nNo student found with that roll number.\n");
}

void removeStudentRecord() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("\n[ERROR] Could not open files for deleting.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    int rollToDelete;
    Student st;
    int found = 0;

    printf("\n--- Delete SRM Student ---\n");
    printf("Enter Roll No to delete: ");
    while (scanf("%d", &rollToDelete) != 1) {
        printf("Invalid input. Enter roll again: ");
        flushInput();
    }
    flushInput();

    while (fscanf(fp, "%d,%49[^,],%29[^,],%d,%f",
                  &st.roll, st.name, st.dept, &st.year, &st.CGPA) == 5) {
        if (st.roll == rollToDelete) {
            found = 1;
            continue;   // skip writing = delete
        }
        fprintf(temp, "%d,%s,%s,%d,%.2f\n",
                st.roll, st.name, st.dept, st.year, st.CGPA);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nRecord deleted successfully.\n");
    else
        printf("\nRoll number not found.\n");
}

/*-------------------------------------------------
    MENUS
-------------------------------------------------*/

void showAdminMenu() {
    int choice;

    do {
        printf("\n===== SRM ADMIN CONTROL PANEL =====\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Edit Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: addStudentRecord();    break;
            case 2: showAllStudents();     break;
            case 3: searchStudentRecord(); break;
            case 4: modifyStudentRecord(); break;
            case 5: removeStudentRecord(); break;
            case 6: printf("\nLogging out...\n"); break;
            default: printf("\nInvalid option.\n");
        }
    } while (choice != 6);
}

void showStaffMenu() {
    int choice;

    do {
        printf("\n===== SRM STAFF ACCESS MENU =====\n");
        printf("1. View All Students\n");
        printf("2. Search Student\n");
        printf("3. Edit Student\n");
        printf("4. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: showAllStudents();     break;
            case 2: searchStudentRecord(); break;
            case 3: modifyStudentRecord(); break;
            case 4: printf("\nLogging out...\n"); break;
            default: printf("\nInvalid option.\n");
        }
    } while (choice != 4);
}

void showGuestMenu() {
    int choice;

    do {
        printf("\n===== SRM GUEST VIEW MENU =====\n");
        printf("1. View All Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: showAllStudents();     break;
            case 2: searchStudentRecord(); break;
            case 3: printf("\nLogging out...\n"); break;
            default: printf("\nInvalid option.\n");
        }
    } while (choice != 3);
}

void routeByRole() {
    if (strcmp(loggedInRole, "admin") == 0)
        showAdminMenu();
    else if (strcmp(loggedInRole, "staff") == 0)
        showStaffMenu();
    else if (strcmp(loggedInRole, "guest") == 0)
        showGuestMenu();
    else
        printf("\n[ERROR] Unknown role: %s\n", loggedInRole);
}

/*-------------------------------------------------
    MAIN
-------------------------------------------------*/

int main() {
    printf("\n===============================================\n");
    printf("   SRM UNIVERSITY - STUDENT RECORD SYSTEM\n");
    printf("===============================================\n");

    if (!performLogin())
        return 0;

    printf("\nLogin successful! Welcome %s (%s)\n",
           loggedInUser, loggedInRole);

    routeByRole();

    printf("\nThank you for using SRM Record Management System.\n");
    return 0;
}
