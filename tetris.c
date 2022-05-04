#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#include "tetris.h"

void delay(int milliseconds) {
        time_t end;

        end = clock() + milliseconds*(CLOCKS_PER_SEC/1000);
        while(clock() < end) {}
}

void clearConsole() {
        system("cls");
}

void calculateRelativePosition(float absolute_x, float absolute_y, float base_x, float base_y, float *relative_x, float *relative_y) {
        *relative_x = absolute_x - base_x;
        *relative_y = absolute_y - base_y;
}

void calculateAbsolutePosition(float relative_x, float relative_y, float base_x, float base_y, float *absolute_x, float *absolute_y) {
        *absolute_x = base_x + relative_x;
        *absolute_y = base_y + relative_y;
}

void calculateCenteredFromLeftUpper3x3(float left_upper_x, float left_upper_y, float *centered_x, float *centered_y) {
        *centered_x = left_upper_x - 1;
        *centered_y = (left_upper_y - 1) * (-1);
}

void calculateLeftUpperFromCentered3x3(float centered_x, float centered_y, float *left_upper_x, float *left_upper_y) {
        *left_upper_x = centered_x + 1;
        *left_upper_y = (-1) * centered_y + 1;
}

void calculateCenteredFromLeftUpper4x4(float left_upper_x, float left_upper_y, float *centered_x, float *centered_y) {
        *centered_x = left_upper_x - 1.5;
        *centered_y = (left_upper_y - 1.5) * (-1);
}

void calculateLeftUpperFromCentered4x4(float centered_x, float centered_y, float *left_upper_x, float *left_upper_y) {
        *left_upper_x = centered_x + 1.5;
        *left_upper_y = (-1) * centered_y + 1.5;
}

void calculateRotationClockwise90(float x, float y, float *rotated_x, float *rotated_y) {
        *rotated_x = y;
        *rotated_y = (-1) * x;
}

int **declarePreview() {
        int **preview;
        int i;

        preview = (int **) malloc(MAX_BOUND_ALL_TILES_Y * sizeof(int *));
        if (preview == NULL) {
                /*error*/
        }
        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                *(preview + i) = (int *) malloc(MAX_BOUND_ALL_TILES_X * sizeof(int));
                if (*(preview + i) == NULL) {
                        /*error*/
                }
        }
        return preview;
}

void deletePreview(int **preview) {
        int i;

        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                free(*(preview + i));
        }
        free(preview);
}

void initializePreview(int **preview) {
        int i, j;

        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = 0; j < MAX_BOUND_ALL_TILES_X; j++) {
                        *(*(preview + i) + j) = 0;
                }
        }
}

int **declareField() {
        int **field;
        int i;

        field = (int **) malloc(FIELD_HEIGHT * sizeof(int *));
        if (field == NULL) {
                /*error*/
        }
        for (i = 0; i < FIELD_HEIGHT; i++) {
                *(field + i) = (int *) malloc(FIELD_WIDTH * sizeof(int));
                if (*(field + i) == NULL) {
                        /*error*/
                }
        }
        return field;
}

void deleteField(int **field) {
        int i;

        for (i = 0; i < FIELD_HEIGHT; i++) {
                free(*(field + i));
        }
        free(field);
}

void initializeField(int **field) {
        int i, j;

        for (i = 0; i < FIELD_HEIGHT; i++) {
                for (j = 0; j < FIELD_WIDTH; j++) {
                        *(*(field + i) + j) = 0;
                }
        }
}

int checkCoordinatesOnField(int x, int y) {
        if (x < 0 || x > FIELD_WIDTH - 1 || y < 0 || y > FIELD_HEIGHT - 1) {
                return FALSE;
        } else {
                return TRUE;
        }
}

void convertTime(double time, int *hours, int *minutes, int *seconds) {
        *hours = (int) floor(time / 3600);
        *minutes = (int) floor((time - (*hours * 3600)) / 60);
        *seconds = (int) floor((time - (*hours * 3600) - (*minutes * 60)));
}

void printGame(int **static_field, int **dynamic_field, int**preview, double time, int line_cnt, int tetris_cnt, int score, int level) {
        char *printString;
        int i, j, k, hours, minutes, seconds;

        printString = (char *) malloc(5 * sizeof(char));
        clearConsole();
        convertTime(time, &hours, &minutes, &seconds);
        for (i = 2; i < FIELD_HEIGHT; i++) {
                if (i == 2) {
                        printf("Tetrises scored: %i\t\t<! ", tetris_cnt);
                } else if (i == 4) {
                        printf("Lines cleared: %i\t\t<! ", line_cnt);
                } else if (i == 6) {
                        printf("Level: %i\t\t\t<! ", level);
                } else if (i == 8) {
                        printf("Score: %i\t\t\t<! ", score);
                } else if (i == 10) {
                        printf("Time played: %ih:%im:%is\t\t<! ", hours, minutes, seconds);
                } else if (i == 12) {
                        printf("Next tetromino:\t\t\t<! ");
                } else if (i >= 14 && i <= 17) {
                        printf("\t<! ");
                        for (k = 0; k < MAX_BOUND_ALL_TILES_X; k++) {
                                if (*(*(preview + i - PREVIEW_HIGHT) + k) == 0) {
                                        printf(".  ");
                                } else {
                                        printf("[] ");
                                }
                        }
                        printf("!>\t<! ");
                } else if (i == 18) {
                        printf("\t<! ");
                        for (k = 0; k < MAX_BOUND_ALL_TILES_X; k++) {
                                printf("*  ");
                        }
                        printf("!>\t<! ");
                } else if (i == 19) {
                        printf("\t<! ");
                        for (k = 0; k < MAX_BOUND_ALL_TILES_X; k++) {
                                printf("\\/ ");
                        }
                        printf("!>\t<! ");
                } else if (i == 21) {
                        printf("Highscore:\t\t\t<! ");
                } else {
                        printf("\t\t\t\t<! ");
                }
                for (j = 0; j < FIELD_WIDTH; j++) {
                        if (*(*(static_field + i) + j) == 1 || *(*(dynamic_field + i) + j) == 1) {
                                printString = "[] ";
                        } else {
                                printString = ".  ";
                        }
                        printf("%s", printString);
                }
                printf("!>\n");
        }
        printf("\t\t\t\t<! ");
        for (i = 0; i < FIELD_WIDTH; i++) {
                printf("*  ");
        }
        printf("!>\n\t\t\t\t   ");
        for (i = 0; i < FIELD_WIDTH; i++) {
                printf("\\/ ");
        }
        printf("\n");
        free(printString);
}

void addActiveTileToStaticField(int **static_field, int **dynamic_field, int x_pos, int y_pos) {
        int i, j;

        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                *(*(static_field + i) + j) = 1;
                        }
                }
        }
}

enum TETROMINOS getNextTile() {
        static int count, currentSet[NUMBER_OF_TETROMINIOS] = {1, 1, 1, 1, 1, 1, 1};
        int i, tile, a;

        if (count == 0) {
                count = NUMBER_OF_TETROMINIOS;
                for (i = 0; i < NUMBER_OF_TETROMINIOS; i++) {
                        currentSet[i] = 1;
                }
        }
        tile = rand() % count;
        a = 0;
        for (i = 0; i < NUMBER_OF_TETROMINIOS; i++) {
                if (a == tile && currentSet[i] == 1) {
                        currentSet[i] = 0;
                        count--;
                        return i + 1;
                } else if (currentSet[i] == 1) {
                        a++;
                }
        }
        return 0;
}

int checkGameOver(int **static_field) {
        int i;

        for (i = 0; i < FIELD_WIDTH; i++) {
                if (*(*static_field + i) == 1) {
                        return TRUE;
                }
        }
        return FALSE;
}

int checkLineFull(int **static_field, int line) {
        int i;

        if (line < 0 || line >= FIELD_HEIGHT) {
                return FALSE;
        }

        for (i = 0; i < FIELD_WIDTH; i++) {
                if (*(*(static_field + line) + i) == 0) {
                        return FALSE;
                }
        }
        return TRUE;
}

void setPreview(int **preview, enum TETROMINOS tile) {
        initializePreview(preview);
        switch(tile) {
                case I:
                        *(*(preview + I_UP[0]) + I_UP[1]) = 1;
                        *(*(preview + I_UP[2]) + I_UP[3]) = 1;
                        *(*(preview + I_UP[4]) + I_UP[5]) = 1;
                        *(*(preview + I_UP[6]) + I_UP[7]) = 1;
                        break;
                case O:
                        *(*(preview + O_UP[0]) + O_UP[1]) = 1;
                        *(*(preview + O_UP[2]) + O_UP[3]) = 1;
                        *(*(preview + O_UP[4]) + O_UP[5]) = 1;
                        *(*(preview + O_UP[6]) + O_UP[7]) = 1;
                        break;
                case L:
                        *(*(preview + L_UP[0]) + L_UP[1]) = 1;
                        *(*(preview + L_UP[2]) + L_UP[3]) = 1;
                        *(*(preview + L_UP[4]) + L_UP[5]) = 1;
                        *(*(preview + L_UP[6]) + L_UP[7]) = 1;
                        break;
                case J:
                        *(*(preview + J_UP[0]) + J_UP[1]) = 1;
                        *(*(preview + J_UP[2]) + J_UP[3]) = 1;
                        *(*(preview + J_UP[4]) + J_UP[5]) = 1;
                        *(*(preview + J_UP[6]) + J_UP[7]) = 1;
                        break;
                case T:
                        *(*(preview + T_UP[0]) + T_UP[1]) = 1;
                        *(*(preview + T_UP[2]) + T_UP[3]) = 1;
                        *(*(preview + T_UP[4]) + T_UP[5]) = 1;
                        *(*(preview + T_UP[6]) + T_UP[7]) = 1;
                        break;
                case Z:
                        *(*(preview + Z_UP[0]) + Z_UP[1]) = 1;
                        *(*(preview + Z_UP[2]) + Z_UP[3]) = 1;
                        *(*(preview + Z_UP[4]) + Z_UP[5]) = 1;
                        *(*(preview + Z_UP[6]) + Z_UP[7]) = 1;
                        break;
                case S:
                        *(*(preview + S_UP[0]) + S_UP[1]) = 1;
                        *(*(preview + S_UP[2]) + S_UP[3]) = 1;
                        *(*(preview + S_UP[4]) + S_UP[5]) = 1;
                        *(*(preview + S_UP[6]) + S_UP[7]) = 1;
                        break;
        }
}

int checkSpaceSpawn(int **static_field, enum TETROMINOS next_tile) {
        switch(next_tile) {
                case I:
                        if (*(*(static_field + I_UP[0]) + I_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + I_UP[2]) + I_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + I_UP[4]) + I_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + I_UP[6]) + I_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case O:
                        if (*(*(static_field + O_UP[0]) + O_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + O_UP[2]) + O_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + O_UP[4]) + O_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + O_UP[6]) + O_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case L:
                        if (*(*(static_field + L_UP[0]) + L_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + L_UP[2]) + L_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + L_UP[4]) + L_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + L_UP[6]) + L_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case J:
                        if (*(*(static_field + J_UP[0]) + J_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + J_UP[2]) + J_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + J_UP[4]) + J_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + J_UP[6]) + J_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case T:
                        if (*(*(static_field + T_UP[0]) + T_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + T_UP[2]) + T_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + T_UP[4]) + T_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + T_UP[6]) + T_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case Z:
                        if (*(*(static_field + Z_UP[0]) + Z_UP[1] + SPAWN_OFFSET_X) == 1 || *(*(static_field + Z_UP[2]) + Z_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + Z_UP[4]) + Z_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + Z_UP[6]) + Z_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
                case S:
                        if (*(*(static_field + S_UP[0]) + S_UP[1] + SPAWN_OFFSET_X) == 1 ||  *(*(static_field + S_UP[2]) + S_UP[3] + SPAWN_OFFSET_X) == 1 || *(*(static_field + S_UP[4]) + S_UP[5] + SPAWN_OFFSET_X) == 1 || *(*(static_field + S_UP[6]) + S_UP[7] + SPAWN_OFFSET_X) == 1) {
                                return FALSE;
                        }
                        break;
        }
        return TRUE;
}

int spawnNewTile(int **static_field, int **dynamic_field, enum TETROMINOS next_tile, int *x_pos, int *y_pos) {
        if (! checkSpaceSpawn(static_field, next_tile)) {
                return FALSE;
        }
        *x_pos = SPAWN_OFFSET_X;
        *y_pos = SPAWN_OFFSET_Y;
        initializeField(dynamic_field);
        switch (next_tile) {
                case I:
                        *(*(dynamic_field + I_UP[0] + SPAWN_OFFSET_Y) + I_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + I_UP[2] + SPAWN_OFFSET_Y) + I_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + I_UP[4] + SPAWN_OFFSET_Y) + I_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + I_UP[6] + SPAWN_OFFSET_Y) + I_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case O:
                        *(*(dynamic_field + O_UP[0] + SPAWN_OFFSET_Y) + O_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + O_UP[2] + SPAWN_OFFSET_Y) + O_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + O_UP[4] + SPAWN_OFFSET_Y) + O_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + O_UP[6] + SPAWN_OFFSET_Y) + O_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case L:
                        *(*(dynamic_field + L_UP[0] + SPAWN_OFFSET_Y) + L_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + L_UP[2] + SPAWN_OFFSET_Y) + L_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + L_UP[4] + SPAWN_OFFSET_Y) + L_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + L_UP[6] + SPAWN_OFFSET_Y) + L_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case J:
                        *(*(dynamic_field + J_UP[0] + SPAWN_OFFSET_Y) + J_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + J_UP[2] + SPAWN_OFFSET_Y) + J_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + J_UP[4] + SPAWN_OFFSET_Y) + J_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + J_UP[6] + SPAWN_OFFSET_Y) + J_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case T:
                        *(*(dynamic_field + T_UP[0] + SPAWN_OFFSET_Y) + T_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + T_UP[2] + SPAWN_OFFSET_Y) + T_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + T_UP[4] + SPAWN_OFFSET_Y) + T_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + T_UP[6] + SPAWN_OFFSET_Y) + T_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case Z:
                        *(*(dynamic_field + Z_UP[0] + SPAWN_OFFSET_Y) + Z_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + Z_UP[2] + SPAWN_OFFSET_Y) + Z_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + Z_UP[4] + SPAWN_OFFSET_Y) + Z_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + Z_UP[6] + SPAWN_OFFSET_Y) + Z_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
                case S:
                        *(*(dynamic_field + S_UP[0] + SPAWN_OFFSET_Y) + S_UP[1] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + S_UP[2] + SPAWN_OFFSET_Y) + S_UP[3] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + S_UP[4] + SPAWN_OFFSET_Y) + S_UP[5] + SPAWN_OFFSET_X) = 1;
                        *(*(dynamic_field + S_UP[6] + SPAWN_OFFSET_Y) + S_UP[7] + SPAWN_OFFSET_X) = 1;
                        break;
        }
        return TRUE;
}

int checkSpaceDown(int **static_field, int **dynamic_field, int x_pos, int y_pos) {
        int i, j;

        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                if (i == FIELD_HEIGHT - 1 || *(*(static_field + i + 1) + j) == 1) {
                                        return FALSE;
                                }
                        }
                }
        }
        return TRUE;
}

int checkSpaceLeft(int **static_field, int **dynamic_field, int x_pos, int y_pos) {
        int i, j;

        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                if (j == 0 || *(*(static_field + i) + j - 1) == 1) {
                                        return FALSE;
                                }
                        }
                }
        }
        return TRUE;
}

int checkSpaceRight(int **static_field, int **dynamic_field, int x_pos, int y_pos) {
        int i, j;

        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                if (j == FIELD_WIDTH - 1 || *(*(static_field + i) + j + 1) == 1) {
                                        return FALSE;
                                }
                        }
                }
        }
        return TRUE;
}

int checkRotationPossible(int **static_field, int **dynamic_field, enum TETROMINOS tile, int x_pos, int y_pos) {
        int i, j, x, y;
        float relative_x, relative_y, centered_x, centered_y, rotated_x, rotated_y;

        switch (tile) {
                case I:
                case O:
                        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                                calculateRelativePosition((float) j, (float) i, (float) x_pos, (float) y_pos, &relative_x, &relative_y);
                                                calculateCenteredFromLeftUpper4x4(relative_x, relative_y, &centered_x, &centered_y);
                                                calculateRotationClockwise90(centered_x, centered_y, &rotated_x, &rotated_y);
                                                calculateLeftUpperFromCentered4x4(rotated_x, rotated_y, &relative_x, &relative_y);
                                                calculateAbsolutePosition(relative_x, relative_y, x_pos, y_pos, &rotated_x, &rotated_y);
                                                x = (int) rotated_x;
                                                y = (int) rotated_y;
                                                if (! checkCoordinatesOnField(x, y) || *(*(static_field + y) + x) == 1) {
                                                        return FALSE;
                                                }
                                        }
                                }
                        }
                        break;
                case L:
                case J:
                case T:
                case Z:
                case S:
                        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                                calculateRelativePosition((float) j, (float) i, (float) x_pos, (float) y_pos, &relative_x, &relative_y);
                                                calculateCenteredFromLeftUpper3x3(relative_x, relative_y, &centered_x, &centered_y);
                                                calculateRotationClockwise90(centered_x, centered_y, &rotated_x, &rotated_y);
                                                calculateLeftUpperFromCentered3x3(rotated_x, rotated_y, &relative_x, &relative_y);
                                                calculateAbsolutePosition(relative_x, relative_y, x_pos, y_pos, &rotated_x, &rotated_y);
                                                x = (int) rotated_x;
                                                y = (int) rotated_y;
                                                if (! checkCoordinatesOnField(x, y) || *(*(static_field + y) + x) == 1) {
                                                        return FALSE;
                                                }
                                        }
                                }
                        }
                        break;
        }
        return TRUE;
}

int moveTileDown(int **static_field, int **dynamic_field, int *x_pos, int *y_pos) {
        int **cpy_field, i, j;

        if (! checkSpaceDown(static_field, dynamic_field, *x_pos, *y_pos)) {
                return FALSE;
        }
        cpy_field = declareField();
        initializeField(cpy_field);
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                *(*(cpy_field + i + 1) + j) = 1;
                        }
                }
        }
        initializeField(dynamic_field);
        *y_pos = *y_pos + 1;
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(cpy_field + i) + j) == 1) {
                                *(*(dynamic_field + i) + j) = 1;
                        }
                }
        }
        deleteField(cpy_field);
        return TRUE;
}

int moveTileLeft(int **static_field, int **dynamic_field, int *x_pos, int *y_pos) {
        int **cpy_field, i, j;

        if (! checkSpaceLeft(static_field, dynamic_field, *x_pos, *y_pos)) {
                return FALSE;
        }
        cpy_field = declareField();
        initializeField(cpy_field);
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                *(*(cpy_field + i) + j - 1) = 1;
                        }
                }
        }
        initializeField(dynamic_field);
        *x_pos = *x_pos - 1;
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(cpy_field + i) + j) == 1) {
                                *(*(dynamic_field + i) + j) = 1;
                        }
                }
        }
        deleteField(cpy_field);
        return TRUE;
}

int moveTileRight(int **static_field, int **dynamic_field, int *x_pos, int *y_pos) {
        int **cpy_field, i, j;

        if (! checkSpaceRight(static_field, dynamic_field, *x_pos, *y_pos)) {
                return FALSE;
        }
        cpy_field = declareField();
        initializeField(cpy_field);
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                *(*(cpy_field + i) + j + 1) = 1;
                        }
                }
        }
        initializeField(dynamic_field);
        *x_pos = *x_pos + 1;
        for (i = *y_pos; i < *y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = *x_pos; j < *x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(cpy_field + i) + j) == 1) {
                                *(*(dynamic_field + i) + j) = 1;
                        }
                }
        }
        deleteField(cpy_field);
        return TRUE;
}

int rotateTileClockwise(int **static_field, int **dynamic_field, enum TETROMINOS tile, int x_pos, int y_pos) {
        int i, j, x, y, **cpy_field;
        float relative_x, relative_y, centered_x, centered_y, rotated_x, rotated_y;

        if (! checkRotationPossible(static_field, dynamic_field, tile, x_pos, y_pos)) {
                return FALSE;
        }
        cpy_field = declareField();
        initializeField(cpy_field);
        switch (tile) {
                case I:
                        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                                calculateRelativePosition((float) j, (float) i, (float) x_pos, (float) y_pos, &relative_x, &relative_y);
                                                calculateCenteredFromLeftUpper4x4(relative_x, relative_y, &centered_x, &centered_y);
                                                calculateRotationClockwise90(centered_x, centered_y, &rotated_x, &rotated_y);
                                                calculateLeftUpperFromCentered4x4(rotated_x, rotated_y, &relative_x, &relative_y);
                                                calculateAbsolutePosition(relative_x, relative_y, x_pos, y_pos, &rotated_x, &rotated_y);
                                                x = (int) rotated_x;
                                                y = (int) rotated_y;
                                                *(*(cpy_field + y) + x) = 1;
                                        }
                                }
                        }
                        break;
                case O:
                        break;
                case L:
                case J:
                case T:
                case Z:
                case S:
                        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                                        if (checkCoordinatesOnField(j, i) && *(*(dynamic_field + i) + j) == 1) {
                                                calculateRelativePosition((float) j, (float) i, (float) x_pos, (float) y_pos, &relative_x, &relative_y);
                                                calculateCenteredFromLeftUpper3x3(relative_x, relative_y, &centered_x, &centered_y);
                                                calculateRotationClockwise90(centered_x, centered_y, &rotated_x, &rotated_y);
                                                calculateLeftUpperFromCentered3x3(rotated_x, rotated_y, &relative_x, &relative_y);
                                                calculateAbsolutePosition(relative_x, relative_y, x_pos, y_pos, &rotated_x, &rotated_y);
                                                x = (int) rotated_x;
                                                y = (int) rotated_y;
                                                *(*(cpy_field + y) + x) = 1;
                                        }
                                }
                        }
                        break;
        }
        initializeField(dynamic_field);
        for (i = y_pos; i < y_pos + MAX_BOUND_ALL_TILES_Y; i++) {
                for (j = x_pos; j < x_pos + MAX_BOUND_ALL_TILES_X; j++) {
                        if (checkCoordinatesOnField(j, i) && *(*(cpy_field + i) + j) == 1) {
                                *(*(dynamic_field + i) + j) = 1;
                        }
                }
        }
        deleteField(cpy_field);

        return TRUE;
}

void dropTileHard(int **static_field, int **dynamic_field, int *tile_pos_x, int *tile_pos_y) {
        while(moveTileDown(static_field, dynamic_field, tile_pos_x, tile_pos_y)) {}
}

void removeLine(int **static_field, int line) {
        int i, j;

        if (line < 0 || line >= FIELD_HEIGHT) {
                return;
        }
        for (j = line; j >= FIRST_VISIBLE_LINE_INDEX; j--) {
                for (i = 0; i < FIELD_WIDTH; i++) {
                        if (*(*(static_field + j - 1) + i) == 1) {
                                *(*(static_field + j) + i) = 1;
                        } else {
                                *(*(static_field + j) + i) = 0;
                        }
                }
        }
}

enum KEYS checkKeyHit() {
        int a, c;

        if (kbhit()) {
                a = getch();
                if (a == ESC || a == H || a == P || a == M || a == Q) {
                        return a;
                } else if (a == 0 || a == 0xE0) {
                        if (kbhit()) {
                                c = getch();
                                if (c == UP) {
                                        return UP;
                                } else if (c == LEFT) {
                                        return LEFT;
                                } else if (c == DOWN) {
                                        return DOWN;
                                } else if (c == RIGHT) {
                                        return RIGHT;
                                } else {
                                        return OTHER;
                                }
                        }
                } else {
                        return OTHER;
                }
        }
        return NONE;
}

enum KEYS  reactToKeyRun(int **static_field, int **dynamic_field, enum TETROMINOS activeTile, int *tile_pos_x, int *tile_pos_y, enum STATES *app_state) {
        enum KEYS key = checkKeyHit();

        switch (key) {
                case ESC:
                        *app_state = EXIT;
                        return ESC;
                case H:
                case P:
                case M:
                        return OTHER;
                case Q:
                        *app_state = GAME_OVER;
                        return Q;
                case UP:
                        rotateTileClockwise(static_field, dynamic_field, activeTile, *tile_pos_x, *tile_pos_y);
                        return UP;

                case DOWN:
                        dropTileHard(static_field, dynamic_field, tile_pos_x, tile_pos_y);
                        return DOWN;
                case LEFT:
                        moveTileLeft(static_field, dynamic_field, tile_pos_x, tile_pos_y);
                        return LEFT;
                case RIGHT:
                        moveTileRight(static_field, dynamic_field, tile_pos_x, tile_pos_y);
                        return RIGHT;
                case OTHER:
                        return OTHER;
                case NONE:
                default:
                        return NONE;
        }
}
enum KEYS reactToKeyStd(enum STATES *app_state) {
        enum KEYS key = checkKeyHit();
        switch (key) {
                case ESC:
                        *app_state = EXIT;
                        return ESC;
                case H:
                        if (*app_state == START) {
                                *app_state = HELP;
                                return H;
                        } else {
                                return OTHER;
                        }
                case P:
                        if (*app_state == START) {
                                *app_state = RUNNING;
                                return P;
                        } else {
                                return OTHER;
                        }
                        return P;
                case M:
                        if (*app_state == HELP || *app_state == GAME_OVER) {
                                *app_state = START;
                                return M;
                        } else {
                                return OTHER;
                        }
                case Q:
                        return OTHER;
                case UP:
                case DOWN:
                case LEFT:
                case RIGHT:
                case OTHER:
                        return OTHER;
                case NONE:
                default:
                        return NONE;
        }
}

int removeFullLines(int **static_field) {
        int i, cnt = 0;

        for (i = FIELD_HEIGHT - 1; i >= FIRST_VISIBLE_LINE_INDEX; i--) {
                if (checkLineFull(static_field, i)) {
                        removeLine(static_field, i);
                        cnt++;
                        i++;
                }
        }
        return cnt;
}

void awardPoints(int lines, int level, int *score) {
        switch (lines) {
                case 1:
                        *score += POINTS_1_LINE * level;
                        break;
                case 2:
                        *score += POINTS_2_LINE * level;
                        break;
                case 3:
                        *score += POINTS_3_LINE * level;
                        break;
                case 4:
                        *score += POINTS_4_LINE * level;
                        break;
        }
}

int updateLevel(int line_cnt, int *level) {
        if (line_cnt >= *level * LINES_FOR_NEXT_LEVEL) {
                *level += 1;
                return TRUE;
        } else {
                return FALSE;
        }
}

void updateTickSpeed(int *tick_speed, int level) {
        *tick_speed = MAX_TICK_SPEED - ((MAX_TICK_SPEED - MIN_TICK_SPEED) / MAX_LEVEL * level);
}

void reactToLineClear(int lines, int *line_cnt, int *tetris_cnt, int *level, int *score, int *tick_speed) {
        awardPoints(lines, *level, score);
        if (lines == LINES_FOR_TETRIS) {
                *tetris_cnt += 1;
        }
        *line_cnt += lines;
        if (updateLevel(*line_cnt, level)) {
                updateTickSpeed(tick_speed, *level);
        }
}

void initializeGame(int *score, int *line_cnt, int *tetris_cnt, int *level, int *tick_speed, time_t *start_time, enum TETROMINOS *active_tile, enum TETROMINOS *next_tile) {
        *start_time = time(NULL);
        *score = 0;
        *line_cnt = 0;
        *tetris_cnt = 0;
        *level = 1;
        *tick_speed = MAX_TICK_SPEED;
        *active_tile = getNextTile();
        *next_tile = getNextTile();
}

void runGame(enum STATES *app_state, int *score, int *line_cnt, int *tetris_cnt) {
        int **static_field, **dynamic_field, **preview, tile_pos_x, tile_pos_y, tick_speed, level;
        enum TETROMINOS active_tile, next_tile;
        clock_t last_move;
        time_t start_time;

        static_field = declareField();
        dynamic_field = declareField();
        preview = declarePreview();
        initializeField(static_field);
        initializeField(dynamic_field);
        initializePreview(preview);

        initializeGame(score, line_cnt, tetris_cnt, &level, &tick_speed, &start_time, &active_tile, &next_tile);

        if (! spawnNewTile(static_field, dynamic_field, active_tile, &tile_pos_x, &tile_pos_y)) {
                /*error*/
        }
        setPreview(preview, next_tile);
        last_move = clock();
        printGame(static_field, dynamic_field, preview, difftime(time(NULL), start_time), *line_cnt, *tetris_cnt, *score, level);
        while(*app_state == RUNNING) {
                switch (reactToKeyRun(static_field, dynamic_field, active_tile, &tile_pos_x, &tile_pos_y, app_state)) {
                        case ESC:
                        case H:
                        case P:
                        case M:
                        case Q:
                                break;
                        case UP:
                        case LEFT:
                        case RIGHT:
                                printGame(static_field, dynamic_field, preview, difftime(time(NULL), start_time), *line_cnt, *tetris_cnt, *score, level);
                                break;
                        case DOWN:
                                break;
                        case OTHER:
                        case NONE:
                                break;
                }
                if (((clock() - last_move) / (float) CLOCKS_PER_SEC) * 1000 >= tick_speed) {
                        last_move = clock();
                        if (! moveTileDown(static_field, dynamic_field, &tile_pos_x, &tile_pos_y)) {
                                addActiveTileToStaticField(static_field, dynamic_field, tile_pos_x, tile_pos_y);
                                reactToLineClear(removeFullLines(static_field), line_cnt, tetris_cnt, &level, score, &tick_speed);
                                active_tile = next_tile;
                                next_tile = getNextTile();
                                setPreview(preview, next_tile);
                                if (! spawnNewTile(static_field, dynamic_field, active_tile, &tile_pos_x, &tile_pos_y)) {
                                        *app_state = GAME_OVER;
                                }
                        }
                        printGame(static_field, dynamic_field, preview, difftime(time(NULL), start_time), *line_cnt, *tetris_cnt, *score, level);
                }
        }

        deleteField(static_field);
        deleteField(dynamic_field);
        deletePreview(preview);
}

void calculateXandYFromHDLFile(FILE *stream, int *x, int *y) {
        int c, cnt_x, cnt_y;

        cnt_x = 0;
        cnt_y = 0;
        c = fgetc(stream);
        while(c != EOF) {
                if (cnt_y == 0 && c != '\n') {
                        cnt_x++;
                } else if (c == '\n') {
                        cnt_y++;
                }
                c = fgetc(stream);
        }
        *x = cnt_x;
        *y = cnt_y;
}

int **calculateArrayFromHDLFile(FILE *stream, int x, int y) {
        int **array, i, j, c;

        array = (int **) malloc(y * sizeof(int *));
        if (array == NULL) {
                /*error*/
        }
        for (i = 0; i < y; i++) {
                *(array + i) = (int *) malloc(x * sizeof(int));
                if (*(array + i) == NULL) {
                        /*error*/
                }
        }
        for (i = 0; i < y; i++) {
                for (j = 0; j < x; j++) {
                        c = fgetc(stream);
                        if (c == EOF) {
                                /*error*/
                        } else if (c == 'x') {
                                *(*(array + i) + j) = 1;
                        } else if (c == ' ') {
                                *(*(array + i) + j) = 0;
                        } else {
                                /*error*/
                        }
                }
                if (fgetc(stream) != '\n') {
                        /*error*/
                }
        }
        if (fgetc(stream) != EOF) {
                /*error*/
        }

        return array;
}

void printFDLFile(int **field, int x, int y) {
        int i, j;

        printf("\n");
        for (i = 0; i < y; i++) {
                printf("<! ");
                for (j = 0; j < x; j++) {
                        if (*(*(field + i) + j) == 0) {
                                printf("   ");
                        } else if (*(*(field + i) + j) == 1) {
                                printf("[] ");
                        }
                }
                printf("!>\n");
        }
        printf("<! ");
        for (i = 0; i < x; i++) {
                printf("*  ");
        }
        printf("!>\n   ");
        for (i = 0; i < x; i++) {
                printf("\\/ ");
        }
}

void runStartMenu(enum STATES *app_state) {
        int **field, i, x, y;
        FILE *titelscr_file1, *titelscr_file2;

        clearConsole();
        titelscr_file1 = fopen("./files/tetris.hdl", "r");
        titelscr_file2 = fopen("./files/tetris.hdl", "r");
        if (titelscr_file1 == NULL || titelscr_file2 == NULL) {
                /*error*/
        }
        calculateXandYFromHDLFile(titelscr_file1, &x, &y);
        field = calculateArrayFromHDLFile(titelscr_file2, x, y);
        printFDLFile(field, x, y);
        printf("\n\nTo play the game press 'p' for help press 'h'\n");
        while (*app_state == START) {
                reactToKeyStd(app_state);
        }
        for (i = 0; i < y; i++) {
                free(*(field + 1));
        }
        free(field);
        if (fclose(titelscr_file1) == EOF || fclose(titelscr_file2) == EOF) {
                /*error*/
        }
}

void runHelpMenu(enum STATES *app_state) {
        int **field, i, x, y;
        FILE *titelscr_file1, *titelscr_file2;

        clearConsole();
        titelscr_file1 = fopen("./files/help.hdl", "r");
        titelscr_file2 = fopen("./files/help.hdl", "r");
        if (titelscr_file1 == NULL || titelscr_file2 == NULL) {
                /*error*/
        }
        calculateXandYFromHDLFile(titelscr_file1, &x, &y);
        field = calculateArrayFromHDLFile(titelscr_file2, x, y);
        printFDLFile(field, x, y);
        printf("\n\nsome future help text\n");
        while (*app_state == HELP) {
                reactToKeyStd(app_state);
        }
        for (i = 0; i < y; i++) {
                free(*(field + 1));
        }
        free(field);
        if (fclose(titelscr_file1) == EOF || fclose(titelscr_file2) == EOF) {
                /*error*/
        }
}

void runGameOverMenu(enum STATES *app_state, int score, int line_cnt, int tetris_cnt) {
        int **field, i, x, y;
        FILE *titelscr_file1, *titelscr_file2;

        clearConsole();
        titelscr_file1 = fopen("./files/game_over.hdl", "r");
        titelscr_file2 = fopen("./files/game_over.hdl", "r");
        if (titelscr_file1 == NULL || titelscr_file2 == NULL) {
                /*error*/
        }
        calculateXandYFromHDLFile(titelscr_file1, &x, &y);
        field = calculateArrayFromHDLFile(titelscr_file2, x, y);
        printFDLFile(field, x, y);
        printf("\n\nFinal score: %i\n\nTotal lines cleared: %i\n\nTotal tetris-clears: %i\n", score, line_cnt, tetris_cnt);
        while (*app_state == GAME_OVER) {
                reactToKeyStd(app_state);
        }
        for (i = 0; i < y; i++) {
                free(*(field + 1));
        }
        free(field);
        if (fclose(titelscr_file1) == EOF || fclose(titelscr_file2) == EOF) {
                /*error*/
        }
}

int main() {
        int score, line_cnt, tetris_cnt;
        enum STATES app_state;

        srand(time(NULL));

        app_state = START;
        while (app_state != EXIT) {
                switch (app_state) {
                        case START:
                                runStartMenu(&app_state);
                                break;
                        case RUNNING:
                                runGame(&app_state, &score, &line_cnt, &tetris_cnt);
                                break;
                        case GAME_OVER:
                                runGameOverMenu(&app_state, score, line_cnt, tetris_cnt);
                                break;
                        case HELP:
                                runHelpMenu(&app_state);
                                break;
                        case EXIT:
                                break;
                }
        }

        exit(0);
}
