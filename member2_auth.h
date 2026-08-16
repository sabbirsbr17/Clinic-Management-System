// MEMBER 2: Authentication, UI & Help Desk


#ifndef MEMBER2_H
#define MEMBER2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "member3_data.h"

char loggedInUserRole[20] = "";
char loggedInUsername[30] = "";

void generateMD5(const char *password, char *hashed_output) {
    unsigned long hash = 5381;
    int c;
    while ((c = *password++))
        hash = ((hash << 5) + hash) + c;
    sprintf(hashed_output, "%lx", hash);
}

void registerUser() {
    system("cls");
    struct UserRecord newUser;
    FILE *fp;
    int roleChoice;
    char raw_password[50];

    printf("\n--- SYSTEM REGISTRATION ---\n");
    printf("Register as: 1. Patient  2. Doctor\n");
    printf("Choice: ");
    if (scanf("%d", &roleChoice) != 1) return;

    printf("Enter Username/ID (No spaces): ");
    scanf("%s", newUser.username);
    printf("Enter Password: ");
    scanf("%s", raw_password);
    generateMD5(raw_password, newUser.password_hash);

    if(roleChoice == 2) strcpy(newUser.role, "Doctor");
    else strcpy(newUser.role, "Patient");

    printf("Enter Full Name: ");
    scanf(" %[^\n]", newUser.full_name);

    if(roleChoice == 1) {
        printf("Enter Age: ");
        scanf("%d", &newUser.age);
    } else {
        newUser.age = 0;
    }

    printf("Enter Phone Number: ");
    scanf("%s", newUser.phone);

    fp = fopen("users.dat", "ab");
    if (fp != NULL) {
        fwrite(&newUser, sizeof(struct UserRecord), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] Registration Successful! Profile Saved.\n");
    }
    pauseScreen();
}

void doctorDashboard() {
    int choice;
    do {
        system("cls");
        printf("\n\n=== DOCTOR DASHBOARD (%s) ===\n", loggedInUsername);
        printf("1. Write Prescription & Check History\n");
        printf("2. Logout\n");
        printf("===============================\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) return;

        switch(choice) {
            case 1: writePrescription(); break;
            case 2:
                strcpy(loggedInUserRole, "");
                printf("\nLogging out...\n");
                return;
            default: printf("Invalid choice!\n"); pauseScreen();
        }
    } while(choice != 2);
}

int loginSystem() {
    system("cls");
    struct UserRecord tempUser;
    FILE *fp;
    int authenticated = 0;
    char input_username[30], input_raw_password[50], input_hashed_password[33];

    printf("\n--- LOGIN SYSTEM ---\n");
    printf("Username/ID: ");
    scanf("%s", input_username);
    printf("Password: ");
    scanf("%s", input_raw_password);

    generateMD5(input_raw_password, input_hashed_password);

    fp = fopen("users.dat", "rb");
    if (fp != NULL) {
        while (fread(&tempUser, sizeof(struct UserRecord), 1, fp)) {
            if (strcmp(tempUser.username, input_username) == 0 &&
                strcmp(tempUser.password_hash, input_hashed_password) == 0) {
                authenticated = 1;
                strcpy(loggedInUserRole, tempUser.role);
                strcpy(loggedInUsername, tempUser.username);
                break;
            }
        }
        fclose(fp);
    }

    if (authenticated == 1) {
        printf("\n[SUCCESS] Welcome back, %s!\n", tempUser.full_name);
        pauseScreen();
        if (strcmp(loggedInUserRole, "Doctor") == 0) {
            doctorDashboard();
            return 0;
        }
        return 1;
    } else {
        printf("\n[ERROR] Invalid Credentials!\n");
        pauseScreen();
        return 0;
    }
}

void helpDeskMenu() {
    seedMasterData();
    int choice;
    char query[50];

    do {
        system("cls");
        printf("\n\n====== CLINIC HELP DESK ======\n");
        printf("1. Search Doctor by Name\n");
        printf("2. Search Hospital by Name\n");
        printf("3. Search Doctor & Hospital by Specialty\n");
        printf("4. Symptom/Syndrome Analyzer\n");
        printf("5. Back to Main Menu\n");
        printf("==============================\n");
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) return;

        switch (choice) {
            case 1:
                printf("Enter Doctor Name (e.g., Sohrabuzzaman): ");
                scanf(" %[^\n]", query);
                DoctorNode* doc = searchDoctorByName(docRoot, query);
                if (doc) printf("\n[FOUND] Dr. %s | Specialty: %s | Hospital: %s\n", doc->name, doc->specialty, doc->hospital);
                else printf("\n[NOT FOUND]\n");
                pauseScreen();
                break;
            case 2:
                printf("Enter Hospital Name (e.g., Heart / Square): ");
                scanf(" %[^\n]", query);
                HospitalNode* hosp = searchHospitalByName(hospRoot, query);
                if (hosp) printf("\n[FOUND] %s | Location: %s\n", hosp->name, hosp->location);
                else printf("\n[NOT FOUND]\n");
                pauseScreen();
                break;
            case 3:
                printf("Enter Specialty (e.g., Cardiology, Neurology): ");
                scanf(" %[^\n]", query);
                int foundDoc = 0, foundHosp = 0;
                printf("\n--- Doctors under %s ---\n", query);
                searchDoctorBySpecialty(docRoot, query, &foundDoc);
                printf("\n--- Hospitals providing %s ---\n", query);
                searchHospitalBySpecialty(hospRoot, query, &foundHosp);
                pauseScreen();
                break;
            case 4:
                printf("Enter Symptom (e.g., Chest Pain, Headache): ");
                scanf(" %[^\n]", query);
                SyndromeNode* syn = searchSyndromeByName(synRoot, query);
                if (syn) {
                    printf("\n--- ANALYSIS RESULT ---\nSymptom: %s\nSpecialty: %s\nDoctors: %s\nHospitals: %s\n",
                           syn->syndromeName, syn->recommendedSpecialty, syn->bestDoctors, syn->bestHospitals);
                } else printf("\n[NOT FOUND]\n");
                pauseScreen();
                break;
            case 5: return;
            default: printf("\nInvalid!\n"); pauseScreen();
        }
    } while (choice != 5);
}

void handleAuthMenu() {
    int subChoice;
    do {
        system("cls");
        printf("\n\n--- USER PORTAL & HELP DESK ---\n");
        printf("1. Register Account (Create Profile)\n");
        printf("2. Login to Account\n");
        printf("3. Help Desk (Search Doctors/Hospitals)\n");
        printf("4. Back to Main Menu\n");
        printf("-------------------------------\n");
        printf("Enter choice: ");
        if (scanf("%d", &subChoice) != 1) return;

        switch(subChoice) {
            case 1: registerUser(); break;
            case 2: loginSystem(); break;
            case 3: helpDeskMenu(); break;
            case 4: return;
            default: printf("\nInvalid Choice!\n"); pauseScreen();
        }
    } while(subChoice != 4);
}

#endif
