#ifndef H_VISIT_H
#define H_VISIT_H


#include <stdio.h>

#include "date.h"
#include "doc.h"

typedef struct Visit {
    Date* tArrival;
    Date* tDismissed;
    float Duration;
    Doc* Doctor;
    char* vSummary;
    struct Visit* next;
} Visit;

void add_visit(Visit** head, Doc* doctor, char* summary);
void print_visits(Visit* head);
void save_visits(Visit* head, FILE* file);

Visit* read_visit(FILE* file, Doc* doc);

void strcpy3(char* dest, int destSize, char* src, int srcSize);

void saveVisitToFile(FILE* f, Visit* visit);

void deleteVisit(Visit* visit);

#endif