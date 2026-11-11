#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doc.h"

void strcpy1(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

Doc* add_doctor(Doc* head, char* name, char* license, int nPatients) {
    Doc* new_doctor = (Doc*)malloc(sizeof(Doc));
    if (new_doctor == NULL)
        return;
    new_doctor->Name = (char*)malloc(sizeof(char) * strlen(name) + 1);
    strcpy1(new_doctor->Name, strlen(name) +1, name, strlen(name));
    //strcpy(new_doctor->nLicense, license);
    strcpy1(new_doctor->nLicense, 8, license, strlen(license));
    new_doctor->nPatients = nPatients;
    new_doctor->next = NULL;
    
    if (head == NULL)
        head = new_doctor;
    else// find the end of the list
    {
        Doc* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = new_doctor;
    }
    return head;
}

Doc* find_doctor(Doc* head, char* license) {
    Doc* current = head;
    while (current != NULL) {
        if (strcmp(current->nLicense, license) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_doctors(Doc* head) {
    Doc* current = head;
    while (current != NULL) {
        printf("Doctor: %s, License: %s, Patients: %d\n", current->Name, current->nLicense, current->nPatients);
        current = current->next;
    }
}

void save_doctors(Doc* head, FILE* file) 
{
    fprintf(file, "Full Name; License Number; Number of Patients\n");
    fprintf(file, "=================================================\n");

    Doc* current = head;
    while (current != NULL) {
        fprintf(file, "%s; %s; %d\n", current->Name, current->nLicense, current->nPatients);
        current = current->next;
    }
}

int IsFreeDoctorAvailable(Doc* doctors)
{
    Doc* current = doctors;
    while (current != NULL)
    {
        if (current->nPatients == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeDoctors(Doc* doc)
{
    Doc* current = doc;
    while (current != NULL) {
        Doc* temp = current;
        current = current->next;
        free(temp->Name);
        free(temp);
    }
}
