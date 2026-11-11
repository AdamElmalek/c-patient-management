#include "pline.h"
#include <stdio.h>
#include <stdlib.h>

void init_pline(PLine* pline) {
    pline = NULL;
}

void enqueue_patient(PLine* pline, Patient* patient) 
{
    // first check if the patient doesn't exist in the queue;
    PInLine* new_node = (PInLine*)malloc(sizeof(PInLine));
    if (!new_node)
    {
        printf("Can't allocate memory...\n");
        return;
    }
    new_node->lpatient = patient;
    new_node->next = NULL;
    if (pline->rear == NULL) {
        pline->front = new_node;
        pline->rear = new_node;
    }
    else {
        pline->rear->next = new_node;
        pline->rear = new_node;
    }
}

Patient* dequeue_patient(PLine* pline) {
    if (pline->front == NULL) {
        return NULL;
    }
    PInLine* temp = pline->front;
    pline->front = pline->front->next;
    if (pline->front == NULL) {
        pline->rear = NULL;
    }
    Patient* patient = temp->lpatient;
    free(temp);
    return patient;
}

void print_line(PLine* pline) {
    PInLine* current = pline->front;
    if (current == NULL) {
        printf("line is empty");
        return;
    }
    while (current != NULL) {
        printf("Patient in line: %s->%s\n", current->lpatient->ID, current->lpatient->Name);
        current = current->next;
    }
}

void save_line(PLine* pline, FILE* file) {

    fprintf(file, "Patients' IDs in line\n");
    fprintf(file, "=====================\n");

    PInLine* current = pline->front;
    int i = 1;
    while (current != NULL) {
        fprintf(file, "%d.%s\n", i++, current->lpatient->ID);
        current = current->next;
    }


}

Patient* search_patient_in_q(PLine* pline, char* id)
{
    PInLine* current = pline->front;
    if (current == NULL) {
        return NULL;
    }
    while (current != NULL) {
        if (strcmp(current->lpatient->ID, id) == 0)
            return current->lpatient;
        current = current->next;
    }
    return NULL;
}

void advance_patient_in_q(PLine* pline, Patient* patient)
{
    if (patient == pline->front)
        return;

    // search for the patient in the queue
    PInLine* current = pline->front;
    PInLine* prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->lpatient->ID, patient->ID) == 0)
        {
            if (prev == NULL) // the patient is already at the front
            {
                printf("Patient is already at the front of the line\n");
                return;
            }
            else
            {
                prev->next = current->next;
                if (current->next == NULL)
                {
                    pline->rear = prev;
                }

                current->next = pline->front;
                pline->front = current;
            }
            printf("Patient %s is advanced to the front of the line\n", current->lpatient->Name);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void remove_patient_in_q(PLine* pline, char* id)
{
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
}

void freePLine(PLine* queue)
{
    PInLine* current = queue->front;
    while (current != NULL) {
        PInLine* next = current->next;
        free(current);
        current = next;
    }
    free(queue);

}
