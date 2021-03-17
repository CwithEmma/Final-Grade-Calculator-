/*
    A9_GradeCalc.cpp : Emma McAvoy
    Grade Calculator reading in a comma delimited file of numbers
    where each row is a set of values with the first field being the grade type
    Ouput: 71
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

struct DGrades1400
{
    int* grades;
    unsigned int numOfGrades;
    unsigned int numKeep;
    double pct;
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

int extractGradeSet(GradeSet1400* set, const char* line);
double calcTypeGrade(DGrades1400 grades);
GradeSet1400 readGradeFile(const char* gradeFile);
unsigned int getNumFields(const char* a_testLine);
unsigned int getFieldAt(char* a_outBuff, size_t outSize, const char* a_textLine, unsigned int idx);
int minIdx(int* gradeArray, int arraySize);
int calculateGrade(GradeSet1400 grades);

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

/// <summary>
/// takes a file name and reads the file line by line extracting the grades
/// associated with each grade type and creates a grade set structure
/// </summary>
/// <param name="gradeFile"> pointer to the file called in the main function </param>
/// <returns> the information into the struct </returns>
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
        int len = strlen(line);
        line[len - 1] = 0; //removing the "\n" character that is feed in by the file 
        extractGradeSet(&set, line);
    }
    return set;
}

/// <summary>
/// Determines the number of fields in the line
/// </summary>
/// <param name="a_testLine">a pointer to a line in the file</param>
/// <returns> the number of fields in the line in the file</returns>
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
/// <summary>
/// Getting the values at each index from the passed in line
/// </summary>
/// <param name="a_outBuff"> The value at the index </param>
/// <param name="outSize"> The size of the output buffer </param>
/// <param name="a_textLine"> pointer to a line being passed in </param>
/// <param name="idx"> the index of the field youre extracting </param>
/// <returns></returns>
unsigned int getFieldAt(char* a_outBuff, size_t outSize, const char* a_textLine, unsigned int idx)
{
    int totalFields = 0;
    int ret = 0;
    if (a_textLine == NULL || a_outBuff == NULL || strlen(a_textLine) == 0)
    {
        return 0;
    }
    // Reset the comma pos
    const char* p_delimPos = a_textLine;
    for (unsigned int i = 0; i < idx; i++)
    {
        p_delimPos = strchr(p_delimPos, DELIMITER);

        // If the index is not valid (not enough delimiters)
        if (p_delimPos == NULL)
        {
            return 0;
        }
        else
        {
            p_delimPos += 1;
        }
    }

    while (*p_delimPos != DELIMITER && *p_delimPos != NULL)
    {
        *a_outBuff++ = *p_delimPos++;
        ret++;
    }
    return ret;
}


/// <summary>
/// called by function calTypeGrade to find the lowest value
/// and set it to IGNORE_VALUE
/// </summary>
/// <param name="gradeArray"> points to the array of grades </param>
/// <param name="arraySize"> the lenght of the array </param>
/// <returns> the lowest value in the array </returns>
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

/// <summary>
/// populating the struct based on the [0] line read from the file
/// on their "gradeType"
/// <functions called>
/// getFieldAt, getNumFields
/// </functions called>
/// </summary>
/// <param name="set"> pointer the struct GradeSet1400 </param>
/// <param name="line"> pointer to the extracted line from the file</param>
/// <returns> the file information into the struct </returns>
int extractGradeSet(GradeSet1400* set, const char* line)
{
    char gradeType[255];
    char gradeBuf[255];
    int* grades;
    memset(gradeType, 0, 255);
    memset(gradeBuf, 0, 255);

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
            getFieldAt(gradeBuf, 255, line, i); //Removes the \n that is part of the line
            grades[i - 1] = atoi(gradeBuf);
            memset(gradeBuf, 0, 255);
        }
    }

    if (strstr(gradeType, "QUIZ") != NULL)
    {
        set->quizzes.grades = grades;
        set->quizzes.numOfGrades = numFields - 1; 
        set->quizzes.pct = QUIZ_PCT;
        set->quizzes.numKeep = QUIZ_GRADES_COUNTED;
    }
    else if (strstr(gradeType, "ASSIGNMENT") != NULL)
    {
        set->assignments.grades = grades;
        set->assignments.numOfGrades = numFields - 1;
        set->assignments.pct = ASSIGN_PCT;
        set->assignments.numKeep = ASSIGN_GRADES_COUNTED;
    }
    else if (strstr(gradeType, "EXAM1") != NULL)
    {
        set->exam1.grades = grades;
        set->exam1.numOfGrades = numFields - 1;
        set->exam1.pct = EXAM1_PCT;
    }
    else if (strstr(gradeType, "EXAM2") != NULL)
    {
        set->exam2.grades = grades;
        set->exam2.numOfGrades = numFields - 1;
        set->exam2.pct = EXAM2_PCT;
    }
    else if (strstr(gradeType, "FINALEXAM") != NULL)
    {
        set->finalExam.grades = grades;
        set->finalExam.numOfGrades = numFields - 1;
        set->finalExam.pct = FINAL_EXAM_PCT;
    }
    else if (strstr(gradeType, "LAB") != NULL)
    {
        set->labs.grades = grades;
        set->labs.numOfGrades = numFields - 1;
        set->labs.pct = LAB_PCT;
        set->labs.numKeep = LAB_GRADES_COUNTED;
    }
    return 0;
}

/// <summary>
/// Drops the two lowest marks if the number of grades is greater than 1 & then
/// calculates the total and times them by the precent they are worth
/// </summary>
/// <param name="grades">passing in the struct by value</param>
/// <returns>The student grade for that section of the struct copied in</returns>
double calcTypeGrade(DGrades1400 grades)
{
    int dropGrades = 0;
    double sum = 0;
    double total = 0;

    if (grades.numOfGrades > 1)
    {
        while (dropGrades < 2)
        {
            int ret = minIdx(grades.grades, grades.numOfGrades);
            grades.grades[ret] = IGNORE_VALUE;
            dropGrades++;
        }
    }

    for (int i = 0; i < grades.numOfGrades; i++)
    {
        if (grades.grades[i] != IGNORE_VALUE)
        {
            sum += grades.grades[i];
            total++;
        }
    }
    double ret = (sum / total) * grades.pct;
    return ret;
}

/// <summary>
/// This calls the function calcTypeGrade
/// </summary>
/// <param name="grades"> passing in the struct by value </param>
/// <returns> The final grade of the student </returns>
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
    return (int)total;
}
