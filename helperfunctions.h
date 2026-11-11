#ifndef H_HF_H
#define H_HF_H

#include "doc.h"
#include "ptree.h"
#include "visit.h"
#include "pline.h"

void cpy_str(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void remove_digits(char* str)
{
    int i = 0, j = 0;
    // Skip initial numeric part
    while (str[i] != '.') {
        i++;
    }
    i++;
    // Copy the rest of the string to output
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }

    // Null-terminate the output string
    str[j] = '\0';
}

// function for loading the doctors
Doc* load_doctors(FILE* file)
{
    printf("Loading doctors...\t");
    
    Doc* head = NULL;
    char line[256];
    char name[256];
    char license[8];
    int nPatients = 0;

    // Skip the top two lines
    fgets(line, sizeof(line), file); fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // remove the redundant characters
        line[strcspn(line, "\n")] = '\0'; line[strcspn(line, "\r")] = '\0'; 

        char* context = NULL;
        char* token = strtok_s(line, ";", &context);
        if (token != NULL) {
            cpy_str(name, 256, token, strlen(token));
            name[strlen(token)] = 0;
        }

        token = strtok_s(NULL, ";", &context);
        
        if (token != NULL) {
            int c = 0,j=0;
            while (c < 7 && c < strlen(token))
            {
                if (token[j] == ' ')
                {
                    j++;
                    continue;
                }
                license[c] = token[j];
                c++; j++;
            }
            license[c] = '\0';
        }

        token = strtok_s(NULL, ";", &context);
        if (token != NULL) {
            nPatients = atoi(token + 1);
        }


        head = add_doctor(head, name, license, nPatients);

    }
    fclose(file);



    return head;
}


// Function for loading the patients
PTree* load_patients(FILE* file, Doc* doc_head)
{
    PTree* root = NULL;
    printf("Loading patients from file...\t");

    char line[256];
    char id[10];
    char name[256];
    char allergiesStr[256];

    
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);


    while (fgets(line, sizeof(line), file) != NULL)
    {
        // remove the redundant characters
        line[strcspn(line, "\r")] = 0; line[strcspn(line, "\n")] = 0; 

        // empty line
        if (strlen(line) == 0 || line[0] == '\n' || line[0] == '\0') { 
            break;
        }

        char* context = NULL;

        char* token = strtok_s(line, ";", &context);
        if (token != NULL) {
            cpy_str(name, 256, token, strlen(token));
            remove_digits(name);
        }

        token = strtok_s(NULL, ";", &context);
        if (token != NULL) {
            cpy_str(id, 10, token, strlen(token));
        }

        unsigned char allergies = 0;

        token = strtok_s(NULL, ";", &context);
        if (token != NULL) {
            cpy_str(allergiesStr, 256, token, strlen(token));

            char* context = NULL;
            // split the allergies string into individual allergies
            char* allergy = strtok_s(allergiesStr, ",", &context);
            while (allergy != NULL) {
                allergies = addAllergy(allergies, allergy);
                allergy = strtok_s(NULL, ",", &context);
            }
        }

        fgets(line, sizeof(line), file); // Skip the empty line

        int counter = 0;
        Visit* visits_root = NULL;
        while (counter < 15 && line[0] != '=')
        {
            Visit * visit = read_visit(file, doc_head);
            if (visits_root == NULL) visits_root = visit;
            else
            {
                Visit* current = visits_root;
                while (current->next != NULL) current = current->next;
                current->next = visit;
            }

            counter++;

            fgets(line, sizeof(line), file); // Skip the empty line after each visit or the line with '='
        } // end of while loop

        
        root = insert_patient(root, name, id, allergies, visits_root);
        
        
    }
    printf("Patients file reading finished\n");

    return root;
}


PLine* load_patients_line(FILE* file, PTree* ptree)
{
    PLine* queue = (PLine*)malloc(sizeof(PLine));
    queue->front = queue->rear = NULL;

    printf("Loading the patients the queue==> ");
    

    char line[256];

    // Skip the header lines
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0'; line[strcspn(line, "\r")] = '\0'; 

        // remove the first number and the dot
        remove_digits(line);


        if (line != NULL && strlen(line) > 0) {
            Patient* patient = search_patient(ptree, line);
            if (patient == NULL) {
                fprintf(stderr, "Patient with ID %s not found\n", line);
                exit(EXIT_FAILURE);
            }
            enqueue_patient(queue, patient);
        }
    }
    printf("Finished.\n");
    return queue;
}

#endif