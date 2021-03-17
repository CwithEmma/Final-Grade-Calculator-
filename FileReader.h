#pragma once
#define _CRT_SECURE_NO_WARNINGS 
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
