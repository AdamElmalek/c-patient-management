#ifndef H_PLINE_H
#define H_PLINE_H

#include "patient.h"

typedef struct PInLine {
    Patient* lpatient;
    struct PInLine* next;
} PInLine;

typedef struct {
    PInLine* front;
    PInLine* rear;
} PLine;

void init_pline(PLine* pline);
void enqueue_patient(PLine* pline, Patient* patient);
Patient* dequeue_patient(PLine* pline);
void print_line(PLine* pline);
void save_line(PLine* pline, FILE* file);

Patient* search_patient_in_q(PLine* pline,char* id);
void advance_patient_in_q(PLine* pline, Patient* patient);

void remove_patient_in_q(PLine* pline, char* id);

void freePLine(PLine* pline);

#endif