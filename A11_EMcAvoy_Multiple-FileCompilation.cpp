/*
    A11_Multiple-File Compilation: Emma McAvoy
    Grade Calculator reading in a comma delimited file of numbers
    where each row is a set of values with the first field being the grade type
    Ouput: 71
*/

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include "FileReader.h"
#define _CRT_SECURE_NO_WARNINGS

int main()
{
    const char* a_filePath = "Grades.txt";
    GradeSet1400  set = readGradeFile(a_filePath);
    int total = calculateGrade(set);
    printf("\nThe student final mark for BIT1400 is: %i%% \n", total);

    free(set.assignments.grades);
    free(set.quizzes.grades);
    free(set.exam1.grades);
    free(set.exam2.grades);
    free(set.finalExam.grades);
    free(set.labs.grades);

    set.assignments.grades = NULL;
    set.quizzes.grades = NULL;
    set.exam1.grades = NULL;
    set.exam2.grades = NULL;
    set.finalExam.grades = NULL;
    set.labs.grades = NULL;
    return 0;
}