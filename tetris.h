#ifndef TETRIS_CONSTATNS_HEADER
#define TETRIS_CONSTATNS_HEADER

#define TRUE 1
#define FALSE 0
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 22
#define FIRST_VISIBLE_LINE_INDEX 2
#define MAX_BOUND_ALL_TILES_X 4
#define MAX_BOUND_ALL_TILES_Y 4
#define NUMBER_OF_TETROMINIOS 7
#define MAX_TICK_SPEED 300
#define MIN_TICK_SPEED 100
#define SPAWN_OFFSET_X 3
#define SPAWN_OFFSET_Y 0
#define LINES_FOR_NEXT_LEVEL 10
#define MAX_LEVEL 15
#define MIN_LEVEL 1
#define POINTS_1_LINE 40
#define POINTS_2_LINE 100
#define POINTS_3_LINE 300
#define POINTS_4_LINE 1200
#define LINES_FOR_TETRIS 4
#define PREVIEW_HIGHT 14

enum TETROMINOS {I = 1, O = 2, L = 3, J = 4, T = 5, Z = 6, S = 7};
enum KEYS {UP = 72, LEFT = 75, DOWN = 80, RIGHT = 77, ESC = 27, H = 104, P = 112, M = 109, Q = 113, OTHER = 0, NONE = -1};
enum STATES {START = 0, RUNNING = 1, GAME_OVER = 2, HELP = 3, EXIT = 4};

static int I_UP[8] = {1, 0, 1, 1, 1, 2, 1, 3};
static int O_UP[8] = {0, 1, 1, 1, 0, 2, 1, 2};
static int L_UP[8] = {1, 0, 1, 1, 1, 2, 0, 2};
static int J_UP[8] = {1, 0, 1, 1, 1, 2, 0, 0};
static int T_UP[8] = {1, 0, 1, 1, 1, 2, 0, 1};
static int Z_UP[8] = {0, 0, 0, 1, 1, 1, 1, 2};
static int S_UP[8] = {1, 0, 1, 1, 0, 1, 0, 2};

#endif
