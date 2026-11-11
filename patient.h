#ifndef H_PATIENT_H
#define H_PATIENT_H


#include "visit.h"

#define NONE 0x00
#define PENICILLIN 0x01
#define SULFA 0x02
#define OPIOIDS 0x04
#define ANESTHETICS 0x08
#define EGGS 0x10
#define LATEX 0x20
#define PRESERVATIVES 0x40

typedef struct Patient {
    char* Name;
    char ID[11];
    unsigned char Allergies;
    Visit* Visits;
    int nVisits;
} Patient;




void strcpy2(char* dest, int destSize, char* src, int srcSize);

unsigned char getAllergy(char* allergy);
unsigned char addAllergy(unsigned char Allergies, char* AllergyStr);

void SaveAllergiesToFile(FILE* file, unsigned int Allergies);
void printAllergiesonScreen(unsigned char Allergies);

void freePatient(Patient* p);

#endif