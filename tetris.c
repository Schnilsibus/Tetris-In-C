#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 22

#define MAX_BOUND_ALL_TILES_X 4
#define MAX_BOUND_ALL_TILES_Y 4

#define NUMBER_OF_TETROMINIOS 7

#define START_TICK_SPEED 300

#define SPAWN_OFFSET_X 3
#define SPAWN_OFFSET_Y 0

enum TETROMINOS {I = 1, O = 2, L = 3, J = 4, T = 5, Z = 6, S = 7};

static int I_UP[8] = {1, 0, 1, 1, 1, 2, 1, 3};
static int O_UP[8] = {0, 1, 1, 1, 0, 2, 1, 2};
static int L_UP[8] = {1, 0, 1, 1, 1, 2, 0, 2};
static int J_UP[8] = {1, 0, 1, 1, 1, 2, 0, 0};
static int T_UP[8] = {1, 0, 1, 1, 1, 2, 0, 1};
static int Z_UP[8] = {0, 0, 0, 1, 1, 1, 1, 2};
static int S_UP[8] = {1, 0, 1, 1, 0, 1, 0, 2};

void delay(int milliseconds) {
        time_t end;

        end = clock() + milliseconds*(CLOCKS_PER_SEC/1000);
        while(clock() < end) {

        }
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
        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                *(preview + i) = (int *) malloc(MAX_BOUND_ALL_TILES_X * sizeof(int));
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
        for (i = 0; i < FIELD_HEIGHT; i++) {
                *(field + i) = (int *) malloc(FIELD_WIDTH * sizeof(int));
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

void printGame(int **static_field, int **dynamic_field, int**preview) {
        char *printString;
        int i, j;

        printString = (char *) malloc(5 * sizeof(char));
        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                printf("<! ");
                for (j = 0; j < MAX_BOUND_ALL_TILES_X; j++) {
                        if (*(*(preview + i) + j) == 0) {
                                printString = "   ";
                        } else if (*(*(preview + i) + j) == 1) {
                                printString = "[] ";
                        }
                        printf("%s", printString);
                }
                printf("!>\n");
        }
        printf("<! ");
        for (i = 0; i < MAX_BOUND_ALL_TILES_X; i++) {
                printf("*  ");
        }
        printf("!>\n   ");
        for (i = 0; i < MAX_BOUND_ALL_TILES_X; i++) {
                printf("\\/ ");
        }
        printf("\n\n");
        for (i = 2; i < FIELD_HEIGHT; i++) {
                printf("<! ");
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
        printf("<! ");
        for (i = 0; i < FIELD_WIDTH; i++) {
                printf("*  ");
        }
        printf("!>\n   ");
        for (i = 0; i < FIELD_WIDTH; i++) {
                printf("\\/ ");
        }
        printf("\n");
        free(printString);
}

void printGameFull(int **static_field, int **dynamic_field, int**preview) {
        char *printString;
        int i, j;

        printString = (char *) malloc(5 * sizeof(char));
        for (i = 0; i < MAX_BOUND_ALL_TILES_Y; i++) {
                printf("<! ");
                for (j = 0; j < MAX_BOUND_ALL_TILES_X; j++) {
                        if (*(*(preview + i) + j) == 0) {
                                printString = "   ";
                        } else if (*(*(preview + i) + j) == 1) {
                                printString = "[] ";
                        }
                        printf("%s", printString);
                }
                printf("!>\n");
        }
        printf("<! ");
        for (i = 0; i < MAX_BOUND_ALL_TILES_X; i++) {
                printf("*  ");
        }
        printf("!>\n   ");
        for (i = 0; i < MAX_BOUND_ALL_TILES_X; i++) {
                printf("\\/ ");
        }
        printf("\n\n");
        for (i = 0; i < FIELD_HEIGHT; i++) {
                if (i == 2) {
                        printf("<!-");
                        for (j = 0; j < FIELD_WIDTH; j++) {
                                printf("---");
                        }
                        printf("!>\n");
                }
                printf("<! ");
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
        printf("<! ");
        for (i = 0; i < FIELD_WIDTH; i++) {
                printf("*  ");
        }
        printf("!>\n   ");
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
                                                if (! checkCoordinatesOnField(j, i) || *(*(static_field + y) + x) == 1) {
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
                                                if (! checkCoordinatesOnField(j, i) || *(*(static_field + y) + x) == 1) {
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

int main() {
        int **static_field, **dynamic_field, **preview, tile_pos_x = SPAWN_OFFSET_X, tile_pos_y = SPAWN_OFFSET_Y, i = 0;


        srand(time(NULL));
        static_field = declareField();
        dynamic_field = declareField();
        preview = declarePreview();
        initializeField(static_field);
        initializeField(dynamic_field);
        initializePreview(preview);

        printf("HelloWorld\n");

        deleteField(static_field);
        deleteField(dynamic_field);
        deletePreview(preview);

        return 0;
}
