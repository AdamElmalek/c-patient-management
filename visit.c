#include "visit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strcpy3(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


void add_visit(Visit** head, Doc* doctor, char* summary) {
    printf("inside Add visit\n");
    Visit* new_visit = (Visit*)malloc(sizeof(Visit));
    if (!new_visit) return;
    new_visit->tArrival = get_current_date();
    printf("Current Date: ");
    print_date(new_visit->tArrival);
    new_visit->tDismissed = NULL;
    new_visit->Duration = -1;
    new_visit->Doctor = doctor;
    //new_visit->vSummary = strdup(summary);
    new_visit->vSummary = (char*)malloc(sizeof(char*) * strlen(summary) + 1);
    strcpy3(new_visit->vSummary, strlen(summary) + 1, summary, strlen(summary));
    new_visit->next = NULL;

    if (*head == NULL) *head = new_visit;
    else
    {
        Visit* current = *head;
        while (current->next != NULL) current = current->next;

        current->next = new_visit;
    }
}

void print_visits(Visit* head) {
    Visit* current = head;
    while (current != NULL) {
        printf("Visit:\n");
        printf("Arrival: ");
        print_date(current->tArrival);
        printf("Dismissed: ");
        print_date(current->tDismissed);
        printf("Duration: %.2f hours\n", current->Duration);
        printf("Doctor: %s\n", current->Doctor->Name);
        printf("Summary: %s\n", (current->vSummary==NULL ? "": current->vSummary));
        current = current->next;
    }
}

void save_visits(Visit* head, FILE* file) {
    Visit* current = head;
    while (current != NULL) {
        if (current->tArrival == NULL)
            fprintf(file, "\n");
        fprintf(file, "%d-%02d-%02d %02d:%02d ", current->tArrival->Year, current->tArrival->Month, current->tArrival->Day,
            current->tArrival->Hour, current->tArrival->Min);
        fprintf(file, "%d-%02d-%02d %02d:%02d ", current->tDismissed->Year, current->tDismissed->Month, current->tDismissed->Day,
            current->tDismissed->Hour, current->tDismissed->Min);
        fprintf(file, "%.2f %s %s\n", current->Duration, current->Doctor->nLicense, current->vSummary);
        current = current->next;
    }
}

Visit* read_visit(FILE* file, Doc* doc)
{
    Visit* visit = (Visit*)malloc(sizeof(Visit));
    visit->next = NULL;

    char data[256];

    // Parse arrival date
    fgets(data, sizeof(data), file);
    char* arrivalStr = strchr(data, ':') + 1;
    visit->tArrival = read_date(arrivalStr);

    // Parse dismissed date
    fgets(data, sizeof(data), file);
    char* dismissedStr = strchr(data, ':') + 1;
    visit->tDismissed = read_date(dismissedStr);

    // Parse duration
    fgets(data, sizeof(data), file);
    // remove the newline character
    data[strcspn(data, "\n")] = 0;
    data[strcspn(data, "\r")] = 0;
    char* durationStr = strchr(data, ':') + 1;

    // check if duration is empty
    if (strlen(durationStr) == 0) {
        visit->Duration = -1;
    }
    else {
        int hours, minutes;
        sscanf_s(durationStr, "%d:%d", &hours, &minutes);
        visit->Duration = hours * 60.0 + minutes;
    }

    // Parse doctor
    fgets(data, sizeof(data), file);
    // remove the newline and return characters
    data[strcspn(data, "\n")] = '\0';
    data[strcspn(data, "\r")] = '\0';

    char* doctorStr = strchr(data, ':') + 1;

    
    Doc* current = doc;
    while (current != NULL) {
        if (strcmp(current->Name, doctorStr) == 0) {
            break;
        }
        current = current->next;
    }

    if (current == NULL) {
        fprintf(stderr, "Doctor %s not found\n", doctorStr);
        exit(EXIT_FAILURE);
    }

    visit->Doctor = current;

    // create memory for summary 
    fgets(data, sizeof(data), file);
    // remove the newline
    data[strcspn(data, "\n")] = '\0';
    data[strcspn(data, "\r")] = '\0';
    char* summaryStr = strchr(data, ':') + 1;


    if (strlen(summaryStr) != 0)
    {
        visit->vSummary = (char*)malloc(sizeof(char) * strlen(summaryStr) + 1);
        strcpy3(visit->vSummary, strlen(summaryStr) + 1, summaryStr, strlen(summaryStr));
    }
    else
    {
        visit->vSummary = NULL;
    }

    return visit;
}



void saveVisitToFile(FILE* file, Visit* visit)
{

    fprintf(file, "\nArrival:"); saveDateToFile(file, visit->tArrival);
    fprintf(file, "Dismissed:"); saveDateToFile(file, visit->tDismissed);
    if (visit->Duration != 0)
        fprintf(file, "Duration:%d:%02d\n", (int)visit->Duration / 60, (int)visit->Duration % 60);
    else
        fprintf(file, "Duration:\n");
    fprintf(file, "Doctor:%s\n", (visit->Doctor->Name == NULL ? "" : visit->Doctor->Name));
    fprintf(file, "Summary:%s\n", (visit->vSummary == NULL ? "" : visit->vSummary));

}

void deleteVisit(Visit* visit)
{
    if (visit)
    {
        free(visit->tArrival);
        free(visit->tDismissed);
        free(visit->vSummary);
    }
}
