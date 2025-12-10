#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int   roll;
    char  name[50];
    float marks;
    struct Student *next;
};

char currentRole[16];
char currentUser[50];
struct Student *head = NULL;

int  loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void staffMenu(void);
void guestMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

void clearInputBuffer(void);

int main(void) {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied. Exiting...\n");
    }
    return 0;
}

int loginSystem(void) {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[16];
    int found = 0;

    printf("===== Login =====\n");
    printf("Username: ");
    if (scanf("%49s", username) != 1) return 0;
    printf("Password: ");
    if (scanf("%49s", password) != 1) return 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %15s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strncpy(currentRole, fileRole, sizeof(currentRole) - 1);
            currentRole[sizeof(currentRole) - 1] = '\0';
            strncpy(currentUser, fileUser, sizeof(currentUser) - 1);
            currentUser[sizeof(currentUser) - 1] = '\0';
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found) {
        printf("\nLogin successful!\n");
        printf("Logged in user : %s\n", currentUser);
        printf("Role           : %s\n", currentRole);
        return 1;
    } else {
        printf("\nInvalid username or password.\n");
        return 0;
    }
}

void mainMenu(void) {
    if (strcmp(currentRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentRole, "USER") == 0) {
        userMenu();
    } else if (strcmp(currentRole, "STAFF") == 0) {
        staffMenu();
    } else {
        guestMenu();
    }
}

void adminMenu(void) {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add student\n");
        printf("2. Display students\n");
        printf("3. Search student\n");
        printf("4. Update student\n");
        printf("5. Delete student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch (choice) {
            case 1: addStudent();       break;
            case 2: displayStudents();  break;
            case 3: searchStudent();    break;
            case 4: updateStudent();    break;
            case 5: deleteStudent();    break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);
}

void userMenu(void) {
    int choice;
    do {
        printf("\n===== USER MENU =====\n");
        printf("1. View profile\n");
        printf("2. View students\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch (choice) {
            case 1:
                printf("Profile: %s (role %s)\n", currentUser, currentRole);
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
}

void staffMenu(void) {
    int choice;
    do {
        printf("\n===== STAFF MENU =====\n");
        printf("1. View students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch (choice) {
            case 1:
                displayStudents();
                break;
            case 2:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 2);
}

void guestMenu(void) {
    int choice;
    do {
        printf("\n===== GUEST MENU =====\n");
        printf("1. View public info\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch (choice) {
            case 1:
                printf("Public info: guest users have no access to student records.\n");
                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 2);
}

void addStudent(void) {
    struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
    struct Student *temp;

    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter roll: ");
    scanf("%d", &newNode->roll);

    printf("Enter name (no spaces): ");
    scanf("%49s", newNode->name);

    printf("Enter marks: ");
    scanf("%f", &newNode->marks);

    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    printf("Student added successfully.\n");
}

void displayStudents(void) {
    struct Student *temp = head;

    if (temp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s\n", "Roll", "Name", "Marks");
    printf("---------------------------------------------\n");

    while (temp != NULL) {
        printf("%-10d %-20s %-10.2f\n", temp->roll, temp->name, temp->marks);
        temp = temp->next;
    }
}

void searchStudent(void) {
    struct Student *temp = head;
    int roll, found = 0;

    if (temp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("Enter roll to search: ");
    scanf("%d", &roll);

    while (temp != NULL) {
        if (temp->roll == roll) {
            printf("Record found:\n");
            printf("Roll : %d\n", temp->roll);
            printf("Name : %s\n", temp->name);
            printf("Marks: %.2f\n", temp->marks);
            found = 1;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("Student with roll %d not found.\n", roll);
    }
}

void updateStudent(void) {
    struct Student *temp = head;
    int roll, found = 0;

    if (temp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("Enter roll to update: ");
    scanf("%d", &roll);

    while (temp != NULL) {
        if (temp->roll == roll) {
            printf("Current: %d %s %.2f\n", temp->roll, temp->name, temp->marks);
            printf("Enter new name (no spaces): ");
            scanf("%49s", temp->name);
            printf("Enter new marks: ");
            scanf("%f", &temp->marks);
            found = 1;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("Student with roll %d not found.\n", roll);
    } else {
        printf("Record updated.\n");
    }
}

void deleteStudent(void) {
    struct Student *temp = head;
    struct Student *prev = NULL;
    int roll, found = 0;

    if (temp == NULL) {
        printf("No student records found.\n");
        return;
    }

    printf("Enter roll to delete: ");
    scanf("%d", &roll);

    while (temp != NULL) {
        if (temp->roll == roll) {
            found = 1;
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found) {
        printf("Student with roll %d not found.\n", roll);
    } else {
        printf("Record deleted.\n");
    }
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}