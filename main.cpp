/*
	Practice For Exam: Re-doing Assignment #9
*/

#include <iostream>
#include <assert.h>
#include <stdlib.h>

const int ASSIGN_GRADES_SIZE = 11;
const int ASSIGN_GRADES_COUNTED = 8;
const int QUIZ_GRADES_SIZE = 15;
const int QUIZ_GRADES_COUNTED = 15;
const int LAB_GRADES_COUNTED = 1;
const double EXAM1_PCT = 0.05;
const double EXAM2_PCT = 0.2;
const double FINAL_EXAM_PCT = 0.3;
const double QUIZ_PCT = 0.05;
const double ASSIGN_PCT = 0.3;
const double LAB_PCT = 0.1;
const char DELIMITER = ',';
const unsigned int BUFF_SIZE = 80;
const int MAX_NUM_GRADES = 20;
const int IGNORE_VALUE = 200;

GradeSet1400 readGradeFile(const char* gradeFile);
int extractGradeSet(GradeSet1400* set, const char* line);
double calcTypeGrade(DGrades1400 grades);
unsigned int getNumFields(const char* a_testLine);
unsigned int getFieldAt(char* a_outBuff, size_t outSize, const char* a_textLine, unsigned int idx);
int minIdx(int* gradeArray, int arraySize);
int calculateGrade(GradeSet1400 grades);

struct DGrades1400
{
    int* grades;
    unsigned int totalGrades;
    unsigned int keepNum;
    double percent;
};

struct GradeSet1400
{
    DGrades1400 quizzes;
    DGrades1400 assignments;
    DGrades1400 exam1;
    DGrades1400 exam2;
    DGrades1400 finalExam;
    DGrades1400 labs;
};


int main()
{
    const char* a_filePath = "Grades.txt";
    GradeSet1400 set = readGradeFile(a_filePath);
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

GradeSet1400 readGradeFile(const char* gradeFile)
{
    FILE* fp;
    char line[300];
    GradeSet1400 set;
    memset(line, 0, sizeof(line));
    memset(&set, 0, sizeof(GradeSet1400));

    if (gradeFile == NULL)
    {
        return set;
    }

    fp = fopen(gradeFile, "r");
    if (fp == NULL)
    {
        return set;
    }

    while (fgets(line, 300, fp) != NULL)
    {
        int length = strlen(line);
        line[length - 1] = 0;
        extractGradeSet(&set, line);
    }

}

int extractGradeSet(GradeSet1400* set, const char* line)
{
    char gradeType[255];
    char gradeBuff[255];
    int* grades;
    memset(gradeType, 0, 255);
    memset(gradeBuff, 0, 255);

    if (set == NULL || line == NULL)
    {
        return -1;
    }

    int numFields = getNumFields(line);
    grades = (int*)malloc(numFields * sizeof(int));
   
    for (int i = 0; i < numFields; i++)
    {
        if (i == 0)
        {
            getFieldAt(gradeType, 255, line, i);
        }
        else
        {
            getFieldAt(gradeBuff, 255, line, i); //Removes the \n that is part of the line
            grades[i - 1] = atoi(gradeBuff);
            memset(gradeBuff, 0, 255);
        }
    }

    if (strstr(gradeType, "QUIZ") != NULL)
    {
        set->quizzes.grades = grades;
        set->quizzes.totalGrades = numFields - 1;
        set->quizzes.percent = QUIZ_PCT;
        set->quizzes.keepNum = QUIZ_GRADES_COUNTED;
    }
    else if (strstr(gradeType, "ASSIGNMENT") != NULL)
    {
        set->assignments.grades = grades;
        set->assignments.totalGrades = numFields - 1;
        set->assignments.percent = ASSIGN_PCT;
        set->assignments.keepNum = ASSIGN_GRADES_COUNTED;
    }
    return 0;
}

double calcTypeGrades(DGrades1400 grades)
{
    int dropGrades = 0;
    double sum = 0;
    double total = 0; 

    if (grades.totalGrades > 1)
    {
        while (dropGrades < 2)
        {
            int r_return = minIdx(grades.grades, grades.keepNum);
            grades.grades[r_return] = IGNORE_VALUE;
            dropGrades++;
        }
    }

    for (int i = 0; i < grades.totalGrades; i++)
    {
        if (grades.grades[i] != IGNORE_VALUE)
        {
            sum += grades.grades[i];
            total++;
        }
    }
    double ret = (sum / total) * grades.percent;
    return ret;
}

int calculateGrade(GradeSet1400 grades)
{
    double total = 0;
    double num1 = calcTypeGrade(grades.quizzes);
    double num2 = calcTypeGrade(grades.assignments);
    double num3 = calcTypeGrade(grades.exam1);
    double num4 = calcTypeGrade(grades.exam2);
    double num5 = calcTypeGrade(grades.finalExam);
    double num6 = calcTypeGrade(grades.labs);
    total = num1 + num2 + num3 + num4 + num5 + num6;
    return(int)total;

}


unsigned int getNumFields(const char* a_testLine)
{
    int counter = 0;
    if (a_testLine == NULL)
    {
        return 0;
    }

    const char* p_delimPos = a_testLine;

    while (p_delimPos != NULL)
    {
        p_delimPos = strchr(p_delimPos, DELIMITER);

        if (p_delimPos != NULL)
        {
            p_delimPos++;
            counter++;
        }
    }
    return ++counter;
}

int minIdx(int* gradeArray, int arraySize)
{
    if (arraySize <= 0)
    {
        return -1;
    }
    int minIdx = 0;
    for (int i = 0; i < arraySize; i++)
    {
        if (gradeArray[minIdx] > gradeArray[i])
        {
            minIdx = i;
        }
    }
    return minIdx;
}