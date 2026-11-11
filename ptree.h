#ifndef H_PTREE_H
#define H_PTREE_H

#include "patient.h"
#include "visit.h"

typedef struct PTree{
    Patient* patient;
    struct PTree* left;
    struct PTree* right;
} PTree;

void init_ptree(PTree* ptree);
PTree* insert_patient(PTree* ptree, char* name, char* id, unsigned char allergies, Visit* visits);
Patient* search_patient(PTree* ptree, char* id);
void display_patients(PTree* ptree);
void save_ptree(PTree* ptree, FILE* file);
void print_patients(PTree* root);
void save_patients(PTree* root, FILE* file);

void removePatientfromTree(PTree** ptree, char* id);
void deleteTree(PTree* root);

void printPatientsAssignedtoDoctor(PTree* ptree, Doc* dName);

#endif