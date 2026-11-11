#include <stdio.h>
#include <stdlib.h>
#include "date.h"
#include "doc.h"
#include "visit.h"
#include "patient.h"
#include "ptree.h"
#include "pline.h"
#include <time.h>
#include <string.h>

#include "helperfunctions.h"


void strcpy4(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void load_data(PTree** ptree, PLine** pline, Doc** docs);
void save_data(PTree** ptree, PLine** pline, Doc** docs);
void log_patient_visit(Patient* patient, Doc* docs);
// Function declaration
Doc* assign_doctor_to_patient(Patient* patient, Doc* docs);
void display_menu();

#define MAX_PATIENTS 4 // Maximum number of patients a doctor can handle
Doc* doc_list = NULL; // List of doctors
void discharge_patient(Patient* patient) {
    if (patient->Visits != NULL && patient->Visits->tDismissed  == NULL) {
        // go to the latest visit 
        Visit* visits = patient->Visits;
        while (visits->next != NULL) visits = visits->next;

        visits->tDismissed = get_current_date();


        int duration = 0;
        duration += (visits->tArrival->Day - visits->tDismissed->Day) * 24 * 60;
        duration += (visits->tArrival->Hour - visits->tDismissed->Hour) * 60;
        duration += visits->tArrival->Min - visits->tDismissed->Min;
        

        visits->Duration = duration;

        // reduce the number of patients for the assigned doctor
        visits->Doctor->nPatients--;
    }
    else {
        printf("No ongoing visit to discharge.\n");
    }
}

Doc* find_doctor_by_name(Doc* head, const char* name) {
    Doc* current = head;
    while (current != NULL) {
        if (strcmp(current->Name, name) == 0) {
            return current; // Doctor found
        }
        current = current->next;
    }
    return NULL; // Doctor not found
}

void free_bst(PTree* root) {
    if (root == NULL) return;
    free_bst(root->left);
    free_bst(root->right);
    free(root->patient->Name);
    free(root->patient->Visits); // Assuming Visits is dynamically allocated
    free(root);
}

void free_line(PLine* pline) {
    PInLine* current = pline->front;
    while (current != NULL) {
        PInLine* temp = current;
        current = current->next;
        free(temp->lpatient->Name);
        free(temp->lpatient->Visits); // Assuming Visits is dynamically allocated
        free(temp->lpatient);
        free(temp);
    }
    pline->front = NULL;
    pline->rear = NULL;
}

void free_doctors(Doc* head) {
    Doc* current = head;
    while (current != NULL) {
        Doc* temp = current;
        current = current->next;
        free(temp->Name);
        free(temp);
    }
}

void remove_visit(Patient* patient) {

    if ( patient->nVisits == 1)
    {
        deleteVisit(patient->Visits);
        patient->Visits = NULL;
        patient->nVisits--;
        return;
    }

    Visit* current = patient->Visits;
    Visit* prev = NULL;
    while (current->next!=NULL)
    {
        prev = current;
        current = current->next;
    }

    if (prev == NULL)
    {
        patient->Visits = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    deleteVisit(current);

    free(current);
    patient->nVisits--;
}

void remove_patient(PTree** root,PLine* pline, char* id) {
    
    // removing it from the pline
    PInLine* current = pline->front;
    PInLine* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->lpatient->ID, id) == 0) {
            if (prev == NULL) {
                pline->front = current->next;
            }
            else {
                prev->next = current->next;
            }
            if (pline->rear == current) {
                pline->rear = prev;
            }
            // free(current);
            return;
        }
        prev = current;
        current = current->next;
    }

    /// now removing patient from tree

    removePatientfromTree(root, id);
}

int main() {
    PTree *ptree = NULL;
    PLine *pline = NULL;
    Doc* docs = NULL;

    init_ptree(ptree);
    init_pline(pline);

    
    load_data(&ptree, &pline, &docs);
    //print_doctors(docs);
    //print_patients(ptree);
    //print_line(pline);


    // Display menu and handle user input
    int choice;
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 1: {

            // check if there is a free dotor
            if (!IsFreeDoctorAvailable(docs))
            {
                printf("There is not free doctor available so you can't admit a patient\n");
                continue;
            }

            char name[256], id[11]; // Buffer for name and ID
            unsigned char allergies;


            printf("Enter patient ID: ");
            scanf_s("%s", id,10);
            id[10] = '\0';

            if (strlen(id) != 9)
            {
                printf("Patient ID should be of Length 9..\n");
                continue;
            }

            //printf("id %s", id);

            // Check if patient exists
            Patient* existing_patient = search_patient(ptree, id);
            if (existing_patient) {
                printf("Patient already exists.\n");
                
                log_patient_visit(existing_patient, docs);
                // Add to the queue
                //enqueue_patient(pline, existing_patient);
            }
            else 
            {
                // Input patient name
                while (1) 
                {
                    printf("Enter patient name: ");
                    scanf_s(" %[^\n]%*c", name, 255);
                    if (strlen(name) > 0) break;
                    printf("Invalid patient name. Try again.\n");
                    
                }



                // Input and validate allergies
                unsigned char Allergies = 0;
                char allergyStr[20];
                while (1) {
                    printf("Enter patient allergie (name):");
                    scanf_s("%s", allergyStr, 20);
                    while (getchar() != '\n');
                    Allergies = addAllergy(Allergies, allergyStr);
                    char r;
                    printf("Do you want to add another Allergy?(Y/N)");
                    r = getchar();
                    if (r == 'Y' || r == 'y') continue;
                    else break;
                }
                

                printf("Patient name: %s, ID: %s, Allergies:\t", name, id); // Debugging: print the entered details
                printAllergiesonScreen(Allergies);

                printf("Now Adding a new patient to the tree.\n");
                // Insert new patient
                ptree = insert_patient(ptree, name, id, Allergies, NULL);

                Patient* new_patient = search_patient(ptree, id);

                log_patient_visit(new_patient, docs);

                // Add to the queue
                enqueue_patient(pline, new_patient);
            }
            break;
        }
        case 2: {
            char id[11];
            printf("Enter patient ID: ");
            scanf_s("%s", id,10);
            Patient* patient = search_patient(ptree, id);
            if (patient) {
                printf("Name: %s\n", patient->Name);
                printf("Patient Allergies: ");
                printAllergiesonScreen(patient->Allergies);
                printf("\n");
            }
            else {
                printf("Patient not found.\n");
            }
            break;
        }
        case 3:
            display_patients(ptree);
            break;
        case 4: {
            char id[11];
            printf("Enter patient ID: ");
            scanf_s("%s", id,10);

            Patient* patient = search_patient(ptree, id);
            if (patient) {
                print_visits(patient->Visits);
            }
            else {
                printf("Patient not found.\n");
            }
            break;
        }
        case 5:
            print_line(pline);
            break;
        case 6: {
            // Print the current state of the queue
            print_line(pline);

            // Prompt the user for the ID of the patient they want to advance
            char id[12];
            printf("Enter the ID of the patient to advance to the front: ");
            scanf_s("%11s", id,10);

            // check if the id is valid and exists in the line
            if (strlen(id) == 0)
            {
                printf("Invalid ID.");
                break;
            }
            
            Patient* patient = search_patient_in_q(pline, id);
            if (patient == NULL)
                break;

            advance_patient_in_q(pline, patient);


            break;
        }
        case 7:
            print_doctors(docs);
            break;
        case 8: {
            char doctor_name[256]; // Buffer to hold the doctor's name
            printf("Enter doctor name: ");
            scanf_s(" %255[^\n]", doctor_name,255); // Read the doctor's name, allowing spaces

            // Find the doctor by name
            Doc* doctor = find_doctor_by_name(docs, doctor_name);

            if (doctor) {
                printf("Doctor %s has been visited by the following patients:\n", doctor->Name);

                printPatientsAssignedtoDoctor(ptree, doctor);
                break;

                // Iterate over each patient
                PTree* current_patient = ptree; // Assuming ptree is the root of your BST or list of patients
                while (current_patient != NULL) {
                    // Check all visits of the current patient
                    Visit* visit = current_patient->patient->Visits;
                    while (visit != NULL) {
                        if (visit->Doctor == doctor) { // Check if the doctor is assigned to this visit
                            printf("Patient Name: %s, ID: %s\n", current_patient->patient->Name, current_patient->patient->ID);

                            // Print visit details
                            printf("  Visit Date: ");
                            print_date(visit->tArrival); // Print arrival date
                        }
                        visit = visit->next; // Move to the next visit
                    }
                    current_patient = current_patient->right; // Move to the next patient
                }
            }
            else {
                printf("Doctor not found.\n");
            }

            break;
        }

        case 9: {
            char id[11];
            printf("Enter patient ID to discharge: ");
            scanf_s("%10s", id,10);

            // Find the patient
            Patient* patient = search_patient(ptree, id); // Assuming ptree is the root of your BST or list of patients
            if (patient) {
                // Discharge the patient
                discharge_patient(patient);

                // Remove the patient from the queue
                remove_patient_in_q(pline, patient->ID);

                // Print the updated queue
                printf("Updated queue:\n");
                print_line(pline);
            }
            else {
                printf("Patient not found.\n");
            }

            break;
        }

        case 10: {
            char id[11];
            printf("Enter patient ID: ");
            scanf_s("%s", id,10);
            Patient* patient = search_patient(ptree, id);
            if (patient) {
                printf("Removing visit for patient %s.\n", patient->Name);
                remove_visit(patient);
            }
            else {
                printf("Patient not found.\n");
            }
            break;
        }
        case 11: {
            char id[11];
            printf("Enter patient ID: ");
            scanf_s("%s", id,10);
            Patient* patient = search_patient(ptree, id);
            if (patient) {
                printf("Removing patient %s.\n", patient->Name);
                remove_patient(&ptree, pline, id);
            }
            else {
                printf("Patient not found.\n");
            }
            break;
        }
        case 12: 
        {
            // Clear the binary search tree of patients
            deleteTree(ptree);
            // Clear the patient queue
            freePLine(pline);

            //Clear Doctors
            freeDoctors(docs);

            // Exit the program
            printf("Hospital closed. All data has been deleted.\n");
            exit(0); // Terminate the program
        }

        case 13: {
            save_data(&ptree, &pline, &docs);
            printf("Exiting program.\n");
            exit(0); // Ensure to include <stdlib.h> for exit()
        }
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 13);

    return 0;
}

void log_patient_visit(Patient* patient, Doc* docs) {
    

    // Assign a doctor to the patient (this will also check for availability)
    printf("Assigning Doctor..\n");
    Doc* assigned_doctor = assign_doctor_to_patient(patient, docs);

    if (assigned_doctor == NULL) {
        return; // No available doctor
    }
    // Prompt for visit summary
    char summary[256];
    printf("Enter visit summary for patient ID: %s: ", patient->ID);
    scanf_s(" %255[^\n]", summary,255); // Read up to 255 characters until newline

    // Create a new visit
    add_visit(&patient->Visits, assigned_doctor, summary);


    // Log the visit
    printf("Visit logged for patient ID: %s\n", patient->ID);
    
}
// In doctor.c (or the relevant source file)
Doc* assign_doctor_to_patient(Patient* patient, Doc* docs) {
    Doc* doctor = NULL;
    Doc* current_doc = docs; // Assuming doc_list is a global or accessible list of doctors

    // Find an available doctor
    while (current_doc != NULL) {
        if (current_doc->nPatients < MAX_PATIENTS) {
            doctor = current_doc;
            break;
        }
        current_doc = current_doc->next;
    }

    if (doctor == NULL) {
        printf("No available doctors for patient ID: %s\n", patient->ID);
        return NULL; // Exit if no doctor is available
    }

    // Assign the doctor to the patient
    printf("Assigning doctor %s to patient ID: %s\n", doctor->Name, patient->ID);
    // Update the doctor's patient count or any other relevant information here
    doctor->nPatients++;

    // Log the assignment (if needed)
    printf("Doctor %s assigned to patient ID: %s\n", doctor->Name, patient->ID);
    return doctor;
}

void display_menu() {
    printf("\nHospital Management System\n");
    printf("1. Add Patient\n");
    printf("2. Check Patient's Allergies\n");
    printf("3. Display All Patients\n");
    printf("4. Display All Patient's Admissions\n");
    printf("5. Display Patients in Line\n");
    printf("6. Advance Patient in Line\n");
    printf("7. Display List of Doctors\n");
    printf("8. Display Patients Assigned to a Doctor\n");
    printf("9. Discharge Patient\n");
    printf("10. Remove Visit\n");
    printf("11. Remove Patient\n");
    printf("12. Close Hospital\n");
    printf("13. Exit Program\n");
}

void load_data(PTree** ptree, PLine** pline, Doc** docs) {
    // loading doctors file
    FILE* df = fopen("Doctors.txt", "r");
    if (df) {
        *docs = load_doctors(df);
        printf("Doctors loaded successfully\n");
        fclose(df);
    }
    else
    {
        printf("Problem with loading the Doctors file...\n");
        exit(0);
    }

    // Load patients
    FILE* pf = fopen("Patients.txt", "r");
    if (pf) {
        
        *ptree = load_patients(pf, *docs);
        fclose(pf);
    }
    else {
        perror("Failed to open txt.Patients");
        exit(0);
    }

    // Load line
    FILE* lf = fopen("Line.txt", "r");
    if (lf) {
        *pline = load_patients_line(lf, *ptree);
        fclose(lf);
    }
}
void save_data(PTree** ptree, PLine** pline, Doc** docs) {
    // Save patient data
    FILE* patient_file = fopen("Patients.txt", "w");
    if (patient_file) {
        fprintf(patient_file, "Name; ID; Allergies\n");
        fprintf(patient_file,"========================\n");
        save_patients(*ptree, patient_file);
        fclose(patient_file);
    }
    else {
        perror("Failed to open patients file");
    }
    printf("Patients saved..\n");

    // Save patient queue data
    FILE* line_file = fopen("Line.txt", "w");
    if (line_file) {
        save_line(*pline, line_file);
        fclose(line_file);
        
    }
    else {
        perror("Failed to open line file");
    }

    printf("Queue saved...\n");

    // Save doctor data
    FILE* doctor_file = fopen("Doctors.txt", "w");
    if (doctor_file) {
        save_doctors(*docs, doctor_file);
        fclose(doctor_file);
    }
    else {
        perror("Failed to open doctors file");
    }
    printf("Doctors Saved...\n");
    // Free all allocated memory
    /*deleteTree(*ptree);
    freePLine(*pline);
    freeDoctors(*docs);*/

}