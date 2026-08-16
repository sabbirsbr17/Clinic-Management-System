// MEMBER 3: Data Structures, Auto-Fetch & Emergency Ambulance


#ifndef MEMBER3_H
#define MEMBER3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
    #define strcasecmp _stricmp
    #define strcasestr strstr
#endif

// --- MASTER USER RECORD ---
struct UserRecord {
    char username[30];
    char password_hash[33];
    char role[15];
    char full_name[50];
    int age;
    char phone[15];
};

struct Appointment {
    char doctor_id[20];
    char date[15];
    char time_slot[15];
    char patient_id[30];
    int serial_no;
};

struct MedicalRecord {
    char patient_id[30];
    char date[15];
    char symptoms[100];
    char tests[100];
    char prescription[200];
    int doctor_fee;
};

struct Feedback {
    char patient_id[30];
    char feedback[200];
};

struct BirthCertificate {
    char baby_name[50];
    char father_name[50];
    char mother_name[50];
    char birth_date[20];
};

struct DeathCertificate {
    char patient_id[30];
    char patient_name[50];
    char death_date[20];
    char cause[100];
};

struct Ambulance {
    char booking_id[15];
    char patient_id[30];
    char pickup_location[100];
    char hospital_type[50];
    char destination_hospital[100];
    char ambulance_type[30];
    float estimated_fare;
    char driver_name[30];
    char driver_phone[15];
    char status[20];
};

// --- BST DATA STRUCTURES ---
typedef struct DoctorNode {
    char name[100];
    char specialty[100];
    char hospital[100];
    struct DoctorNode *left, *right;
} DoctorNode;

typedef struct HospitalNode {
    char name[100];
    char location[100];
    char specialties[200];
    struct HospitalNode *left, *right;
} HospitalNode;

typedef struct SyndromeNode {
    char syndromeName[100];
    char recommendedSpecialty[100];
    char bestDoctors[200];
    char bestHospitals[200];
    struct SyndromeNode *left, *right;
} SyndromeNode;

DoctorNode *docRoot = NULL;
HospitalNode *hospRoot = NULL;
SyndromeNode *synRoot = NULL;

// --- UTILITY FUNCTIONS ---
void pauseScreen() {
    char dummy[10];
    printf("\nType '0' and press Enter to return: ");
    scanf("%s", dummy);
}

// 🌟 MASTER AUTO-FETCH FUNCTION 🌟
int fetchPatientDetails(const char* search_id, char* outName, int* outAge, char* outPhone) {
    FILE *fp = fopen("users.dat", "rb");
    if (!fp) return 0;

    struct UserRecord u;
    while (fread(&u, sizeof(struct UserRecord), 1, fp)) {
        if (strcmp(u.username, search_id) == 0 && strcmp(u.role, "Patient") == 0) {
            if (outName) strcpy(outName, u.full_name);
            if (outAge) *outAge = u.age;
            if (outPhone) strcpy(outPhone, u.phone);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// --- BST HELPER FUNCTIONS ---
DoctorNode* insertDoctor(DoctorNode* root, char* name, char* specialty, char* hospital) {
    if (root == NULL) {
        DoctorNode* newNode = (DoctorNode*)malloc(sizeof(DoctorNode));
        strcpy(newNode->name, name);
        strcpy(newNode->specialty, specialty);
        strcpy(newNode->hospital, hospital);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcasecmp(name, root->name) < 0)
        root->left = insertDoctor(root->left, name, specialty, hospital);
    else if (strcasecmp(name, root->name) > 0)
        root->right = insertDoctor(root->right, name, specialty, hospital);
    return root;
}

HospitalNode* insertHospital(HospitalNode* root, char* name, char* location, char* specialties) {
    if (root == NULL) {
        HospitalNode* newNode = (HospitalNode*)malloc(sizeof(HospitalNode));
        strcpy(newNode->name, name);
        strcpy(newNode->location, location);
        strcpy(newNode->specialties, specialties);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcasecmp(name, root->name) < 0)
        root->left = insertHospital(root->left, name, location, specialties);
    else if (strcasecmp(name, root->name) > 0)
        root->right = insertHospital(root->right, name, location, specialties);
    return root;
}

SyndromeNode* insertSyndrome(SyndromeNode* root, char* syn, char* spec, char* docs, char* hosps) {
    if (root == NULL) {
        SyndromeNode* newNode = (SyndromeNode*)malloc(sizeof(SyndromeNode));
        strcpy(newNode->syndromeName, syn);
        strcpy(newNode->recommendedSpecialty, spec);
        strcpy(newNode->bestDoctors, docs);
        strcpy(newNode->bestHospitals, hosps);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcasecmp(syn, root->syndromeName) < 0)
        root->left = insertSyndrome(root->left, syn, spec, docs, hosps);
    else if (strcasecmp(syn, root->syndromeName) > 0)
        root->right = insertSyndrome(root->right, syn, spec, docs, hosps);
    return root;
}

void searchDoctorBySpecialty(DoctorNode* root, char* targetSpec, int* found) {
    if (root == NULL) return;
    searchDoctorBySpecialty(root->left, targetSpec, found);
    if (strcasecmp(root->specialty, targetSpec) == 0) {
        printf(" -> Dr. %s | Hospital: %s\n", root->name, root->hospital);
        *found = 1;
    }
    searchDoctorBySpecialty(root->right, targetSpec, found);
}

void searchHospitalBySpecialty(HospitalNode* root, char* targetSpec, int* found) {
    if (root == NULL) return;
    searchHospitalBySpecialty(root->left, targetSpec, found);
    if (strcasestr(root->specialties, targetSpec) != NULL) {
        printf(" -> Hospital: %s | Location: %s\n", root->name, root->location);
        *found = 1;
    }
    searchHospitalBySpecialty(root->right, targetSpec, found);
}

DoctorNode* searchDoctorByName(DoctorNode* root, char* name) {
    if (root == NULL) return NULL;
    if (strcasestr(root->name, name) != NULL) return root;
    if (strcasecmp(name, root->name) < 0) return searchDoctorByName(root->left, name);
    return searchDoctorByName(root->right, name);
}

HospitalNode* searchHospitalByName(HospitalNode* root, char* name) {
    if (root == NULL) return NULL;
    if (strcasestr(root->name, name) != NULL) return root;
    if (strcasecmp(name, root->name) < 0) return searchHospitalByName(root->left, name);
    return searchHospitalByName(root->right, name);
}

SyndromeNode* searchSyndromeByName(SyndromeNode* root, char* syn) {
    if (root == NULL) return NULL;
    if (strcasestr(root->syndromeName, syn) != NULL) return root;
    if (strcasecmp(syn, root->syndromeName) < 0) return searchSyndromeByName(root->left, syn);
    return searchSyndromeByName(root->right, syn);
}

void displayHospitalsFromBST(HospitalNode* root, int* count) {
    if (root == NULL) return;
    displayHospitalsFromBST(root->left, count);
    printf("  %d. %-28s | Location: %s\n", (*count)++, root->name, root->location);
    displayHospitalsFromBST(root->right, count);
}

HospitalNode* getHospitalByIndex(HospitalNode* root, int target, int* current) {
    if (root == NULL) return NULL;
    HospitalNode* leftSearch = getHospitalByIndex(root->left, target, current);
    if (leftSearch != NULL) return leftSearch;

    (*current)++;
    if (*current == target) return root;

    return getHospitalByIndex(root->right, target, current);
}

void seedMasterData() {
    if (docRoot != NULL) return;

    docRoot = insertDoctor(docRoot, "Sohrabuzzaman", "Cardiology", "Labaid Cardiac Hospital");
    docRoot = insertDoctor(docRoot, "Rama Biswas", "Neurology", "Square Hospital");
    docRoot = insertDoctor(docRoot, "Amjad Hossain", "Orthopedics", "Labaid Hospital");
    docRoot = insertDoctor(docRoot, "Syeda Ishrat", "Dermatology", "Square Hospital");

    hospRoot = insertHospital(hospRoot, "National Heart Foundation", "Mirpur, Dhaka", "Cardiology, Cardiac Surgery");
    hospRoot = insertHospital(hospRoot, "Ibrahim Cardiac", "Shahbagh, Dhaka", "Cardiology, Vascular Surgery");
    hospRoot = insertHospital(hospRoot, "Labaid Cardiac Hospital", "Dhanmondi, Dhaka", "Cardiology, Orthopedics");
    hospRoot = insertHospital(hospRoot, "Square Hospital", "Panthapath, Dhaka", "Neurology, Cardiology, General");
    hospRoot = insertHospital(hospRoot, "Evercare Hospital", "Bashundhara, Dhaka", "Cardiology, Neurology, Oncology");

    synRoot = insertSyndrome(synRoot, "Chest Pain", "Cardiology", "Dr. Sohrabuzzaman", "National Heart Foundation");
    synRoot = insertSyndrome(synRoot, "Headache", "Neurology", "Dr. Rama Biswas", "Square Hospital");
    synRoot = insertSyndrome(synRoot, "Joint Pain", "Orthopedics", "Dr. Amjad Hossain", "Labaid Hospital");
    synRoot = insertSyndrome(synRoot, "Skin Rash", "Dermatology", "Dr. Syeda Ishrat", "Square Hospital");
}

// --- MEDICAL FUNCTIONS ---
void sendNotification(struct Appointment appt) {
    FILE *logFile = fopen("notifications_log.txt", "a");
    if (logFile == NULL) return;
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    if (timestamp != NULL && timestamp[strlen(timestamp) - 1] == '\n') timestamp[strlen(timestamp) - 1] = '\0';
    fprintf(logFile, "[%s] Patient %s booked Doctor %s on %s. Token: %d, Time: %s\n",
            timestamp, appt.patient_id, appt.doctor_id, appt.date, appt.serial_no, appt.time_slot);
    fclose(logFile);
}

void writePrescription() {
    system("cls");
    struct MedicalRecord record;
    FILE *fp;
    char search_id[30], pName[50];
    int pAge;

    printf("\n--- WRITE PRESCRIPTION ---\n");
    printf("Enter Patient ID: ");
    scanf("%s", search_id);

    if(fetchPatientDetails(search_id, pName, &pAge, NULL)) {
        printf(">> [AUTO-FETCH SUCCESS] Patient Name: %s (Age: %d)\n", pName, pAge);
    } else {
        printf(">> [WARNING] Patient not registered!\n");
    }

    strcpy(record.patient_id, search_id);
    printf("Enter Today's Date (DD-MM-YYYY): ");
    scanf("%s", record.date);

    printf("Enter Symptoms: ");
    scanf(" %[^\n]", record.symptoms);

    printf("Enter Tests Required (or 'None'): ");
    scanf(" %[^\n]", record.tests);

    printf("Enter Medicines: ");
    scanf(" %[^\n]", record.prescription);

    printf("Enter Doctor's Fee: ");
    scanf("%d", &record.doctor_fee);

    fp = fopen("medical_records.dat", "ab");
    if (fp != NULL) {
        fwrite(&record, sizeof(struct MedicalRecord), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] Prescription saved successfully!\n");
    }
    pauseScreen();
}

void birthCertificate() {
    system("cls");
    struct BirthCertificate bc;
    FILE *fp = fopen("birth.dat", "ab");

    printf("\n====== BIRTH CERTIFICATE ======\n");
    printf("Baby Name : ");
    scanf(" %[^\n]", bc.baby_name);
    printf("Father Name : ");
    scanf(" %[^\n]", bc.father_name);
    printf("Mother Name : ");
    scanf(" %[^\n]", bc.mother_name);
    printf("Birth Date (DD-MM-YYYY): ");
    scanf("%s", bc.birth_date);

    if(fp != NULL) {
        fwrite(&bc, sizeof(struct BirthCertificate), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] Birth Certificate Saved Successfully.\n");
    }
    pauseScreen();
}

void deathCertificate() {
    system("cls");
    struct DeathCertificate dc;
    FILE *fp = fopen("death.dat", "ab");
    char pName[50];

    printf("\n====== DEATH CERTIFICATE ======\n");
    printf("Enter Patient ID : ");
    scanf("%s", dc.patient_id);

    if(fetchPatientDetails(dc.patient_id, pName, NULL, NULL)) {
        printf(">> [AUTO-FETCH SUCCESS] Patient Name: %s <<\n", pName);
        strcpy(dc.patient_name, pName);
    } else {
        printf("Enter Patient Name Manually: ");
        scanf(" %[^\n]", dc.patient_name);
    }

    printf("Death Date (DD-MM-YYYY): ");
    scanf("%s", dc.death_date);
    printf("Cause of Death : ");
    scanf(" %[^\n]", dc.cause);

    if(fp != NULL) {
        fwrite(&dc, sizeof(struct DeathCertificate), 1, fp);
        fclose(fp);
        printf("\n[SUCCESS] Death Certificate Issued Successfully.\n");
    }
    pauseScreen();
}

void addFeedback() {
    system("cls");
    struct Feedback fb;
    FILE *fp = fopen("feedback.dat", "ab");
    char pName[50];

    printf("\n========== PATIENT FEEDBACK ==========\n");
    printf("Patient ID : ");
    scanf("%s", fb.patient_id);

    if(fetchPatientDetails(fb.patient_id, pName, NULL, NULL)) {
        printf(">> Welcome back, %s!\n", pName);
    }

    printf("Enter your feedback : ");
    scanf(" %[^\n]", fb.feedback);

    if(fp != NULL) {
        fwrite(&fb, sizeof(struct Feedback), 1, fp);
        fclose(fp);
        printf("\nThank you for your valuable feedback.\n");
    }
    pauseScreen();
}

void bookAmbulance() {
    system("cls");
    seedMasterData();
    struct Ambulance amb;
    FILE *fp = fopen("ambulance.dat", "ab");
    char pName[50], pPhone[15];
    int choice;

    printf("\n=================================================================\n");
    printf("                 EMERGENCY AMBULANCE BOOKING                     \n");
    printf("=================================================================\n");

    snprintf(amb.booking_id, sizeof(amb.booking_id), "AMB-%ld", time(NULL) % 10000);
    printf("Generated Booking ID : %s\n", amb.booking_id);

    printf("Enter Patient ID     : ");
    scanf("%s", amb.patient_id);

    if(fetchPatientDetails(amb.patient_id, pName, NULL, pPhone)) {
        printf(">> [AUTO-FETCH SUCCESS] Name: %s | Phone: %s <<\n", pName, pPhone);
    }

    printf("Pickup Location      : ");
    scanf(" %[^\n]", amb.pickup_location);

    printf("\n-----------------------------------------------------------------\n");
    printf("  [STEP 1/3] SELECT DESTINATION HOSPITAL                         \n");
    printf("-----------------------------------------------------------------\n");
    int hospCount = 1;
    displayHospitalsFromBST(hospRoot, &hospCount);
    printf("  %d. Other / Custom Hospital\n", hospCount);

    printf("\nChoose Hospital (1-%d): ", hospCount);
    scanf("%d", &choice);

    if (choice >= 1 && choice < hospCount) {
        int indexTracker = 0;
        HospitalNode* selectedHosp = getHospitalByIndex(hospRoot, choice, &indexTracker);
        if (selectedHosp != NULL) strcpy(amb.destination_hospital, selectedHosp->name);
    } else {
        printf("Enter Custom Hospital Name: ");
        scanf(" %[^\n]", amb.destination_hospital);
    }

    printf("\n-----------------------------------------------------------------\n");
    printf("  [STEP 2/3] SELECT HOSPITAL CARE / DEPARTMENT TYPE              \n");
    printf("-----------------------------------------------------------------\n");
    printf("  1. General Emergency\n  2. Specialized Cardiac Unit\n  3. Neuro Care Unit\n  4. Cancer Special Care\n  5. Maternity & Pediatrics\n");
    printf("Choose Type (1-5): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: strcpy(amb.hospital_type, "General Emergency"); break;
        case 2: strcpy(amb.hospital_type, "Cardiac Specialized"); break;
        case 3: strcpy(amb.hospital_type, "Neuro Care Unit"); break;
        case 4: strcpy(amb.hospital_type, "Cancer Care Unit"); break;
        case 5: strcpy(amb.hospital_type, "Maternity Care"); break;
        default: strcpy(amb.hospital_type, "General Care"); break;
    }

    printf("\n-----------------------------------------------------------------\n");
    printf("  [STEP 3/3] SELECT AMBULANCE CATEGORY                           \n");
    printf("-----------------------------------------------------------------\n");
    printf("  1. Non-AC Standard Ambulance   (Fare: 1200 TK)\n");
    printf("  2. AC Deluxe Ambulance          (Fare: 2200 TK)\n");
    printf("  3. ICU Support Life Ambulance   (Fare: 5000 TK)\n");
    printf("  4. Freezer / Air Ambulance     (Fare: 8000 TK)\n");
    printf("Choose Category (1-4): ");
    scanf("%d", &choice);

    if (choice == 1) {
        strcpy(amb.ambulance_type, "Non-AC Standard"); amb.estimated_fare = 1200.0;
        strcpy(amb.driver_name, "Kabir Hossain"); strcpy(amb.driver_phone, "01711-000111");
    } else if (choice == 2) {
        strcpy(amb.ambulance_type, "AC Deluxe"); amb.estimated_fare = 2200.0;
        strcpy(amb.driver_name, "Rafiqul Islam"); strcpy(amb.driver_phone, "01822-222333");
    } else if (choice == 3) {
        strcpy(amb.ambulance_type, "ICU Support"); amb.estimated_fare = 5000.0;
        strcpy(amb.driver_name, "Dr. Asif (ICU)"); strcpy(amb.driver_phone, "01933-444555");
    } else {
        strcpy(amb.ambulance_type, "Freezer/Air"); amb.estimated_fare = 8000.0;
        strcpy(amb.driver_name, "Kamrul Hassan"); strcpy(amb.driver_phone, "01644-555666");
    }

    strcpy(amb.status, "On the way");
    if (fp != NULL) {
        fwrite(&amb, sizeof(struct Ambulance), 1, fp);
        fclose(fp);
    }

    system("cls");
    printf("\n=================================================================\n");
    printf("              BOOKING CONFIRMED SUCCESSFULLY!                    \n");
    printf("=================================================================\n");
    printf("Booking ID          : %s\n", amb.booking_id);
    printf("Patient ID          : %s\n", amb.patient_id);
    printf("Destination Hospital: %s\n", amb.destination_hospital);
    printf("Hospital Type       : %s\n", amb.hospital_type);
    printf("Ambulance Category  : %s\n", amb.ambulance_type);
    printf("Fare                : %.2f TK\n", amb.estimated_fare);
    printf("Assigned Driver     : %s (%s)\n", amb.driver_name, amb.driver_phone);
    printf("=================================================================\n");

    pauseScreen();
}

void viewAmbulanceRecords() {
    system("cls");
    struct Ambulance amb;
    FILE *fp = fopen("ambulance.dat", "rb");

    printf("\n=================================================================================================================\n");
    printf("                                      AMBULANCE LIVE TRACKING LOGS                                               \n");
    printf("=================================================================================================================\n");
    printf("%-9s %-8s %-25s %-12s %-10s %-18s\n", "Book ID", "Pat. ID", "Destination", "Amb Type", "Fare(TK)", "Driver Name");
    printf("-----------------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    if (fp != NULL) {
        while (fread(&amb, sizeof(struct Ambulance), 1, fp)) {
            printf("%-9s %-8s %-25.23s %-12s %-10.2f %-18.17s\n",
                   amb.booking_id, amb.patient_id, amb.destination_hospital,
                   amb.ambulance_type, amb.estimated_fare, amb.driver_name);
            found = 1;
        }
        fclose(fp);
    }
    if (!found) printf("No ambulance records found!\n");

    printf("=================================================================================================================\n");
    pauseScreen();
}

#endif
