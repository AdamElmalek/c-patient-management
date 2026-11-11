#include "date.h"

#include <string.h>


void cpy_str1(char* dest, int destSize, char* src, int srcSize)
{
    int i = 0;
    while (i < destSize - 1 && i < srcSize)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void print_date(Date* date) {
    if (date == NULL)
        printf("\n");
    else
        printf("%d-%02d-%02d %02d:%02d\n", date->Year, date->Month, date->Day, date->Hour, date->Min);
}

Date* get_current_date() {
    Date* current_date = (Date*)malloc(sizeof(Date));
    if (!current_date) return NULL;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    current_date->Year = tm.tm_year + 1900;
    current_date->Month = tm.tm_mon + 1;
    current_date->Day = tm.tm_mday;
    current_date->Hour = tm.tm_hour;
    current_date->Min = tm.tm_min;

    return current_date;
}

Date* read_date(char* dateStr)
{

    if (dateStr == NULL || strlen(dateStr) <= 2) {
        return NULL;
    }

    Date* date = (Date*)malloc(sizeof(Date));
    if (date==NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    

    char buffer[100];  // Ensure buffer is large enough
    //strncpy(buffer, dateStr, sizeof(buffer) - 1);
    cpy_str1(buffer, 100, dateStr, strlen(dateStr));
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

    char* token;
    char* ct = NULL;

    // Parse day
    token = strtok_s(buffer, "/", &ct);
    if (token != NULL) {
        date->Day = atoi(token);
    }
    else {
        printf("Error parsing day.\n");
        return;
    }

    // Parse month
    token = strtok_s(NULL, "/", &ct);
    if (token != NULL) {
        date->Month = atoi(token);
    }
    else {
        printf("Error parsing month.\n");
        return;
    }

    // Parse year
    token = strtok_s(NULL, " ", &ct);
    if (token != NULL) {
        date->Year = atoi(token);
    }
    else {
        printf("Error parsing year.\n");
        return;
    }

    // Parse hour
    token = strtok_s(NULL, ":", &ct);
    if (token != NULL) {
        date->Hour = atoi(token);
    }
    else {
        printf("Error parsing hour.\n");
        return;
    }

    // Parse minute
    token = strtok_s(NULL, ":", &ct);
    if (token != NULL) {
        date->Min = atoi(token);
    }
    else {
        printf("Error parsing minute.\n");
        return;
    }
    
    
    return date;
}

void saveDateToFile(FILE* file, Date* date)
{
    if (date == NULL) {
        fprintf(file, "\n");
        return;
    }
    fprintf(file, "%02d/%02d/%04d %02d:%02d\n", date->Day, date->Month, date->Year, date->Hour, date->Min);
}