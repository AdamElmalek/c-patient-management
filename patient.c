#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "patient.h"

void strcpy2(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

unsigned char getAllergy(char* allergy)
{
    int i = -1;
    while (allergy[++i] != '\0')
        allergy[i] = toupper(allergy[i]);

    if (strcmp(allergy, "PENICILLIN") == 0) {
        return PENICILLIN;
    }
    else if (strcmp(allergy, "SULFA") == 0) {
        return SULFA;
    }
    else if (strcmp(allergy, "OPIOIDS") == 0) {
        return OPIOIDS;
    }
    else if (strcmp(allergy, "ANESTHETICS") == 0) {
        return ANESTHETICS;
    }
    else if (strcmp(allergy, "EGGS") == 0) {
        return EGGS;
    }
    else if (strcmp(allergy, "LATEX") == 0) {
        return LATEX;
    }
    else if (strcmp(allergy, "PRESERVATIVES") == 0) {
        return PRESERVATIVES;
    }
    else if (strcmp(allergy, "NONE") == 0) {
        return NONE;
    }
    else {
        printf("Invalid Allergy\n");
        return 0;
    }
}

void print_allergies(unsigned char allergies)
{
    int i = 0, j = 1;
    if (allergies == NONE) {
        printf("NONE");
    }
    else {
        if (allergies & PENICILLIN) { printf("%d.Penicillin", j++); i = 1; }
        if (allergies & SULFA) { printf((i == 1 ? ", %d.Sulfa" : "%d.Sulfa"), j++); i = 1; }
        if (allergies & OPIOIDS) { printf((i == 1 ? ", %d.Opioids" : "%d.Opioids"), j++); i = 1; }
        if (allergies & ANESTHETICS) { printf((i == 1 ? ", %d.Anesthetics" : "%d.Anesthetics"), j++); i = 1; }
        if (allergies & EGGS) { printf((i == 1 ? ", %d.Eggs" : "%d.Eggs"), j++); i = 1; }
        if (allergies & LATEX) { printf((i == 1 ? ", %d.Latex" : "%d.Latex"), j++); i = 1; }
        if (allergies & PRESERVATIVES) { printf((i == 1 ? ", %d.Preservatives" : "%d.Preservatives"), j++); i = 1; }
    }
    printf("\n");
}

unsigned char addAllergy(unsigned char Allergies, char* AllergyStr)
{
    unsigned char allergy = getAllergy(AllergyStr);
    Allergies |= allergy;
    return Allergies;
}

void SaveAllergiesToFile(FILE* file, unsigned int allergies)
{
    int i = 0;
    if (allergies == NONE) {
        fprintf(file, "none");
    }
    else {
        if (allergies & PENICILLIN) { fprintf(file, "Penicillin"); i = 1; }
        if (allergies & SULFA) { fprintf(file, (i == 1 ? ",Sulfa" : "Sulfa")); i = 1; }
        if (allergies & OPIOIDS) { fprintf(file, (i == 1 ? ",Opioids" : "Opioids")); i = 1; }
        if (allergies & ANESTHETICS) { fprintf(file, (i == 1 ? ",Anesthetics" : "Anesthetics")); i = 1; }
        if (allergies & EGGS) { fprintf(file, (i == 1 ? ",Eggs" : "Eggs")); i = 1; }
        if (allergies & LATEX) { fprintf(file, (i == 1 ? ",Latex" : "Latex")); i = 1; }
        if (allergies & PRESERVATIVES) { fprintf(file, (i == 1 ? ",Preservatives" : "Preservatives")); i = 1; }
    }
}


void printAllergiesonScreen(unsigned char allergies)
{
    int i = 0;
    if (allergies == NONE) {
        printf( "none");
    }
    else {
        if (allergies & PENICILLIN) { printf("Penicillin"); i = 1; }
        if (allergies & SULFA) { printf((i == 1 ? ",Sulfa" : "Sulfa")); i = 1; }
        if (allergies & OPIOIDS) { printf( (i == 1 ? ",Opioids" : "Opioids")); i = 1; }
        if (allergies & ANESTHETICS) { printf( (i == 1 ? ",Anesthetics" : "Anesthetics")); i = 1; }
        if (allergies & EGGS) { printf((i == 1 ? ",Eggs" : "Eggs")); i = 1; }
        if (allergies & LATEX) { printf((i == 1 ? ",Latex" : "Latex")); i = 1; }
        if (allergies & PRESERVATIVES) { printf( (i == 1 ? ",Preservatives" : "Preservatives")); i = 1; }
    }
}

void freePatient(Patient* p)
{
    if (p)
    {
        
        free(p->Name);
        Visit* current = p->Visits;
        while (current != NULL)
        {
            Visit* temp = current;
            current = current->next;

            deleteVisit(temp);
        }
    }
}


