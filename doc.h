#include <stdio.h>
#ifndef H_DOC_H
#define H_DOC_H



typedef struct Doc {
    char* Name;
    char nLicense[8];
    int nPatients;
    struct Doc* next;
} Doc;

Doc* add_doctor(Doc* head, char* name, char* license, int nPatients);
Doc* find_doctor(Doc* head, char* license);
void print_doctors(Doc* head);
void save_doctors(Doc* head, FILE* file);

void strcpy1(char* dest, int destSize, char* src, int srcSize);

int IsFreeDoctorAvailable(Doc* doctors);

void freeDoctors(Doc* doc);

#endif