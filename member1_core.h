// MEMBER 1: Core Logic, Token, Admission, Billing & SOS


#ifndef MEMBER1_H
#define MEMBER1_H

#include "member2_auth.h"
#include "member3_data.h"

struct RoomAdmission {
    char patient_id[30];
    int room_type;
    char admission_date[15];
};

struct EmergencyLog {
    char location[100];
    char emergency_type[100];
    char timestamp[50];
};

void bookAppointment() {
    system("cls");
    struct Appointment newAppt, tempAppt;
    FILE *fp;
    char pName[50];
    int previousBookingsCount = 0;

    printf("\n--- BOOK AN APPOINTMENT ---\n");
    printf("Enter Patient Username/ID: ");
    scanf("%s", newAppt.patient_id);

    if(fetchPatientDetails(newAppt.patient_id, pName, NULL, NULL)) {
        printf(">> [AUTO-FETCH SUCCESS] Booking for: %s <<\n", pName);
    } else {
        printf(">> [WARNING] Patient not found! Proceeding with ID only.\n");
    }

    printf("Enter Doctor ID (e.g., DOC01): ");
    scanf("%s", newAppt.doctor_id);
    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%s", newAppt.date);

    fp = fopen("appointments.dat", "rb");
    if (fp != NULL) {
        while (fread(&tempAppt, sizeof(struct Appointment), 1, fp)) {
            if (strcmp(tempAppt.doctor_id, newAppt.doctor_id) == 0 && strcmp(tempAppt.date, newAppt.date) == 0) {
                previousBookingsCount++;
            }
        }
        fclose(fp);
    }

    newAppt.serial_no = previousBookingsCount + 1;
    int totalMins = 0 + (previousBookingsCount * 15);
    int finalHour = 17 + (totalMins / 60);
    int finalMin = totalMins % 60;
    sprintf(newAppt.time_slot, "%02d:%02d", finalHour, finalMin);

    fp = fopen("appointments.dat", "ab");
    if (fp != NULL) {
        fwrite(&newAppt, sizeof(struct Appointment), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] BOOKING CONFIRMED!\n");
        printf("Token: %d | Time: %s\n", newAppt.serial_no, newAppt.time_slot);
        sendNotification(newAppt);
    }
    pauseScreen();
}

void admitPatient() {
    system("cls");
    struct RoomAdmission admission;
    FILE *fp;
    char pName[50];

    printf("\n--- PATIENT ROOM ADMISSION ---\n");
    printf("Enter Patient ID: ");
    scanf("%s", admission.patient_id);

    if(fetchPatientDetails(admission.patient_id, pName, NULL, NULL)) {
        printf(">> [AUTO-FETCH SUCCESS] Admitting Patient: %s <<\n", pName);
    }

    printf("Enter Admission Date (DD-MM-YYYY): ");
    scanf("%s", admission.admission_date);

    printf("\n--- CHOOSE ROOM TYPE ---\n");
    printf("1. Normal Room (500 TK/Day)\n2. AC Room (1500 TK/Day)\n3. Cabin Room (3000 TK/Day)\n");
    printf("Enter Choice (1-3): ");
    scanf("%d", &admission.room_type);

    if(admission.room_type < 1 || admission.room_type > 3) {
        printf("\n[ERROR] Invalid Room.\n"); pauseScreen(); return;
    }

    fp = fopen("admissions.dat", "ab");
    if (fp != NULL) {
        fwrite(&admission, sizeof(struct RoomAdmission), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] Patient admitted successfully!\n");
    }
    pauseScreen();
}

void dischargePatient() {
    system("cls");
    struct RoomAdmission tempAdmit;
    FILE *fp;
    char search_id[30], pName[50];
    int days, found = 0, bill = 0;

    printf("\n--- PATIENT DISCHARGE & BILLING ---\n");
    printf("Enter Patient ID: ");
    scanf("%s", search_id);

    fp = fopen("admissions.dat", "rb");
    if (fp != NULL) {
        while (fread(&tempAdmit, sizeof(struct RoomAdmission), 1, fp)) {
            if (strcmp(tempAdmit.patient_id, search_id) == 0) {
                found = 1;
                fetchPatientDetails(search_id, pName, NULL, NULL);

                printf("Patient: %s | Room Type: ", pName);
                if(tempAdmit.room_type == 1) printf("Normal Room (500 TK/day)\n");
                else if(tempAdmit.room_type == 2) printf("AC Room (1500 TK/day)\n");
                else printf("Cabin Room (3000 TK/day)\n");

                printf("Enter number of days stayed: ");
                scanf("%d", &days);

                if(tempAdmit.room_type == 1) bill = days * 500;
                else if(tempAdmit.room_type == 2) bill = days * 1500;
                else bill = days * 3000;

                printf("\n====================================\n");
                printf("           CLINIC BILLING           \n");
                printf("====================================\n");
                printf("Patient Name: %s\nPatient ID  : %s\nDays Stayed : %d\nTotal Bill  : %d TK\n", pName, tempAdmit.patient_id, days, bill);
                printf("====================================\n");
                printf("[SUCCESS] Patient Discharged!\n");
                break;
            }
        }
        fclose(fp);
    }
    if (found == 0) printf("\n[ERROR] Record not found.\n");
    pauseScreen();
}

void emergency999Service() {
    system("cls");
    struct EmergencyLog sos;
    FILE *fp;

    printf("\n========================================\n");
    printf("  [***] 999 EMERGENCY SOS DISPATCH [***] \n");
    printf("========================================\n");

    printf("Enter Emergency Location: ");
    scanf(" %[^\n]", sos.location);
    printf("Enter Type of Emergency (e.g., Accident, Fire): ");
    scanf(" %[^\n]", sos.emergency_type);

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    if (time_str != NULL && time_str[strlen(time_str) - 1] == '\n') time_str[strlen(time_str) - 1] = '\0';
    strcpy(sos.timestamp, time_str);

    fp = fopen("999_emergency_logs.dat", "ab");
    if (fp != NULL) {
        fwrite(&sos, sizeof(struct EmergencyLog), 1, fp);
        fclose(fp);
        printf("\n[ALERT SENT] Emergency unit dispatched to '%s'!\n[TIME] %s\n", sos.location, sos.timestamp);
    }
    pauseScreen();
}

void showMainMenu() {
    seedMasterData();
    int choice;
    do {
        system("cls");
        printf("\n========================================\n");
        printf("  CLINIC APPOINTMENT SCHEDULING SYSTEM  \n");
        printf("========================================\n");
        printf("1. User Portal (Login / Register)\n");
        printf("2. Book Appointment (Auto-Token)\n");
        printf("3. Admit Patient to Room\n");
        printf("4. Discharge Patient & Billing\n");
        printf("5. Book Emergency Ambulance\n");
        printf("6. Call 999 SOS Dispatch\n");
        printf("7. Issue Birth Certificate\n");
        printf("8. Issue Death Certificate\n");
        printf("9. Submit Feedback / View Ambulance Logs\n");
        printf("10. Exit System\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1) exit(0);

        switch(choice) {
            case 1: handleAuthMenu(); break;
            case 2: bookAppointment(); break;
            case 3: admitPatient(); break;
            case 4: dischargePatient(); break;
            case 5: bookAmbulance(); break;
            case 6: emergency999Service(); break;
            case 7: birthCertificate(); break;
            case 8: deathCertificate(); break;
            case 9:
                printf("\n1. Submit Feedback\n2. View Ambulance Logs\nChoice: ");
                int sub; scanf("%d", &sub);
                if(sub == 1) addFeedback(); else viewAmbulanceRecords();
                break;
            case 10: printf("\nExiting System... Goodbye!\n"); exit(0);
            default: printf("\nInvalid Choice!\n"); pauseScreen();
        }
    } while(choice != 10);
}

#endif
