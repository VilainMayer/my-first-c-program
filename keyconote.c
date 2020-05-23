#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <errno.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

struct saveFileList {
    char fileName[124];
    int idx;
};
struct keycoNote {
    char question[128];
    char answer[128];
};
struct checkScore {
    int attemptCnt;
    int wrongCnt;
};

void printKeyCo(void);
void printFileList(char *pickedSubject);
int fileLoad(struct keycoNote *pNote, char *pickedSubject);
int solveProblem(int pNumberNow, struct keycoNote *pNote, struct checkScore *curScore);
void calculateGrade(float limitTimePerPrb, int problemCnt, float rightAnswer, float checkTime);

int main(int argc, char const *argv[]) {
    struct keycoNote mySubject[300];
    struct checkScore myScore = {0, 0};
    char pickedSubject[124];
    int problemCnt;
    int pNumberNow = 0;
    clock_t timerStart, timerEnd;
    float checkTime;
    int isRight;
    float rightAnswer;
    float limitTimePerPrb = 3.0;

    printKeyCo();
    printFileList(pickedSubject);
    problemCnt = fileLoad(mySubject, pickedSubject);
    timerStart = clock();
    while (pNumberNow != problemCnt) {
        isRight = solveProblem(pNumberNow, mySubject, &myScore);
        if (isRight == 0) {
            pNumberNow++;
        } else {
        }
    }
    timerEnd = clock();
    checkTime = (float)(timerEnd - timerStart) / 1000.0f;
    rightAnswer = 100.0f - (float)myScore.wrongCnt / (float)myScore.attemptCnt * (float)100.0;
    printf("걸린 시간은 %.2f초 입니다.\n", checkTime);
    printf("정답률은 %.2f%% 입니다.\n", rightAnswer);

    calculateGrade(limitTimePerPrb, problemCnt, rightAnswer, checkTime);
    printf("5초 후에 창이 닫힙니다.\n");
    printf("==================================\n");

    Sleep(5000);

    return 0;
}

void printKeyCo(void) {
    puts(" _            ___");
    puts("| |  /|  _   / __|");
    puts("| | / | |_| / /      ___");
    puts("| |/ /   _  | |     /   \\ ");
    puts("|   /   | | | |    | / \\ |");
    puts("|   \\   | | | |    | \\ / |");
    puts("| |\\ \\  |_| \\ \\__   \\___/");
    puts("|_| \\ |      \\___|");
}

void calculateGrade(float limitTimePerPrb, int problemCnt, float rightAnswer, float checkTime) {
    float calcPerformance;
    float pFlag = 1.00f;
    float calGrade;
    char grade[8];
    float gradeDiffer = 3.333333f;

    calcPerformance = (checkTime / (limitTimePerPrb * (float)problemCnt));

    for (size_t i = 0; i < 100; i++) {
        if (calcPerformance <= (1.0f + (float)i * 0.3f)) {
            break;
        }
        pFlag -= 0.01f;
    }

    calGrade = rightAnswer * pFlag;
    grade[0] = 'A';

    for (size_t i = 0; i <= 12; i++) {
        if (i == 12) {
            grade[0] = 'F';
            grade[1] = '\0';
            break;
        }

        if (calGrade >= 100.0f - gradeDiffer * (float)i) {
            if (i % 3 == 0) {
                grade[1] = '+';
            } else if (i % 3 == 1) {
                grade[1] = '0';
            } else if (i % 3 == 2) {
                grade[1] = '-';
            }
            break;
        }
        grade[0] = 'A' + (i / 3);
    }

    printf("성적표: %s (%.1f점)\n", grade, calGrade);
}

int solveProblem(int pNumberNow, struct keycoNote *pNote, struct checkScore *curScore) {
    int isRight;
    char tempStr[128];
    char a[128];
    char b[128];

    printf("%d번 문제: ", pNumberNow + 1);
    printf("%s", pNote[pNumberNow].question);
    printf("%d번 정답: ", pNumberNow + 1);

    // getchar();
    rewind(stdin);
    scanf("%[^\n]", tempStr);
    strcpy(a, pNote[pNumberNow].answer);
    strcpy(b, tempStr);

    ((*curScore).attemptCnt) += 1;

    isRight = strcmp(tempStr, pNote[pNumberNow].answer);

    if (isRight == 0) {
        printf("정답입니다.\n");
        printf("==================================\n");

    } else {
        (*curScore).wrongCnt += 1;
        printf("틀렸습니다.\nHINT: %s\n", pNote[pNumberNow].answer);
        Sleep(750);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("다시 입력해 주세요.\n");
        printf("==================================\n");
    }

    return isRight;
}

void printFileList(char *pickedSubject) {
    struct saveFileList sFL[99];
    struct _finddata_t findSubject;
    intptr_t subjectPath;
    intptr_t isOpened = 1L;
    int tempIDX = 0;
    int pickIDX;

    subjectPath = _findfirst(".\\subject\\*.txt", &findSubject);

    if (subjectPath == -1L) {
        puts("텍스트 파일을 찾을 수 없습니다.");
        isOpened = -1L;
    } else {
        printf("%5s | %s\n", "IDX", "파일이름");
    }

    while (isOpened != -1L) {
        sFL[tempIDX].idx = tempIDX;
        strcpy(sFL[tempIDX].fileName, findSubject.name);
        printf("%5d | %s\n", tempIDX + 1, findSubject.name);
        isOpened = _findnext(subjectPath, &findSubject);
        tempIDX++;
    }

    puts("연습할 과목의 IDX를 입력해 주세요.");
    scanf("%d", &pickIDX);

    strcpy(pickedSubject, sFL[pickIDX - 1].fileName);
    printf("선택한 과목은 %s 입니다.\n", pickedSubject);

    _findclose(subjectPath);
}

int fileLoad(struct keycoNote *pNote, char *pickedSubject) {
    errno_t err;
    FILE *fp;
    char filePath[64];
    int goCopy = 0;
    int lines;
    int arraySize;
    int problemNum = 0;
    int isQuest = 0;
    char tempStr[128];
    int i, j;

    strcpy(filePath, ".\\subject\\");
    strcat(filePath, pickedSubject);

    err = fopen_s(&fp, filePath, "rt");

    if (err == 0) {
        puts("과목 불러오기 완료");
        goCopy = 1;
    } else {
        puts("과목 불러오기 실패");
    }

    if (goCopy == 1) {
        for (lines = 0; !feof(fp); lines++) {
            problemNum = lines / 2;
            isQuest = (lines % 2 == 0) ? 1 : 0;
            if (isQuest == 1) {
                fgets(pNote[problemNum].question, 128, fp);

            } else {
                i = 0;
                j = 0;
                strcpy(tempStr, "");
                fgets(pNote[problemNum].answer, 128, fp);
                for (; i < strlen(pNote[problemNum].answer); i++) {
                    if (pNote[problemNum].answer[i] != '\n') {
                        tempStr[j] = pNote[problemNum].answer[i];
                        j++;
                    }
                }
                tempStr[j] = '\0';
                strcpy(pNote[problemNum].answer, tempStr);
            }
        }
        arraySize = lines / 2;
        printf("총 %d 문제 입니다.\n", arraySize);
        printf("==================================\n");

    } else {
    }

    fclose(fp);

    return problemNum;
}