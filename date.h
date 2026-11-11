#ifndef H_DATE_H
#define H_DATE_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct Date{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Min;
} Date;

void print_date(Date* date);
Date* get_current_date();

Date* read_date(char* dateStr);

void saveDateToFile(FILE* file, Date* date);
#endif
