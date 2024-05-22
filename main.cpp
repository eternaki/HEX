/* board uses odd-q vertical layout */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <climits>

#define BOARD_MAX_SIZ 11
#define BOARD_WIDTH 25
#define BOARD_HEIGHT 14
#define CMD_LEN 20

#define BEGINNING_COL (BOARD_WIDTH / 2)
#define BEGINNING_ROW 2

#define IS_OPPOSITE(c1, c2) ((c1 + 1) == c2)

/* VOID is not used in game */
/* VOID_RED/BLUE means winning edge */
/* the number differentiate the other side to which we trace path */
typedef enum {
    VOID,
    VOID_RED1,
    VOID_RED2,
    VOID_BLUE1,
    VOID_BLUE2,
    NONE,
    RED,
    BLUE,
} Player;

typedef struct {
    int red_count;
    int blue_count;
    int siz;
} Data;

/* debug printing */
void setb(Player board[][BOARD_HEIGHT], int col, int row, Player player)
{
    board[col][row] = player;
    char name[3];
    switch (player) {
        case VOID: case NONE: strcpy(name, ""); break;
        case RED:        strcpy(name, "R");  break;
        case BLUE:       strcpy(name, "B");  break;
        case VOID_RED1:  strcpy(name, "R1"); break;
        case VOID_RED2:  strcpy(name, "R2"); break;
        case VOID_BLUE1: strcpy(name, "B1"); break;
        case VOID_BLUE2: strcpy(name, "B2"); break;
    }
    fprintf(stderr, "[%-2s, %2d, %2d]", name, col, row);
}

/* return the player count and board size */
Data boardread(Player board[][BOARD_HEIGHT])
{
    int ch;
    int beg_col = BEGINNING_COL;
    int col = beg_col;
    int row = BEGINNING_ROW;
    Data data = {0};
    Player pl = NONE;
    char last = 0;
    int midpoint = 0;
    fprintf(stderr, "INPUT BEG %d\n", beg_col);
    /* skip to board first row */
    while ((last = getchar()) != '-' && last != EOF);
    if (last == EOF) {
        data.siz = -1;
        return data;
    }
    while ((last = getchar()) != '\n')
        ;
    /* this is the tip of the board, connect it with correct player */
    setb(board, col - 1, 0, VOID_RED1);
    setb(board, col + 1, 0, VOID_BLUE1);
    fprintf(stderr, "\n");
    setb(board, col - 2, row / 2, VOID_RED1);
    while ((ch = getchar()) != EOF) {
        switch (ch) {
            case 'r': pl = RED;  break;
            case 'b': pl = BLUE; break;
            case '>':	/* new field */
                setb(board, col, row / 2, pl);
                if (pl > NONE) {
                    if (pl == RED) {
                        data.red_count++;
                    } else {
                        data.blue_count++;
                    }
                }
                col += 2;
                pl = NONE;
                break;
            case '<':
                if (last == '\n') midpoint = 1;
                break;
            case '\n':
                setb(board, col, row / 2, midpoint ? VOID_RED2 : VOID_BLUE1);
                fprintf(stderr, "\n");
                row++;
                col = (midpoint ? ++beg_col : --beg_col);
                pl = NONE;
                if (col >= BEGINNING_COL + 1) {
                    while (getchar() != '\n'); /* skip last board row */
                    /* connect the other tip of the board */
                    setb(board, col - 2, row / 2, VOID_BLUE2);
                    setb(board, col, row / 2, VOID_RED2);
                    fprintf(stderr, "\n");
                    fprintf(stderr, "INPUT END\n");
                    data.siz = row / 2;
                    return data;
                }
                setb(board, col - 2, row / 2,  midpoint ? VOID_BLUE2 : VOID_RED1);
        }
        last = ch;
    }
    data.siz = -1;
    return data;
}

typedef struct {
    int col, row;
} Field;

typedef struct {
    int count;
    Field fields[BOARD_MAX_SIZ * BOARD_MAX_SIZ];
} FieldStack;

void pushfield(FieldStack *st, Field field)
{
    st->fields[st->count++] = field;
}

Field popfield(FieldStack *st)
{
    return st->fields[--st->count];
}

void getsides(Field fields[6], int col, int row)
{
    if (col % 2) {
        fields[0].col = col + 1; fields[0].row = row + 1;
        fields[1].col = col + 1; fields[1].row = row;
        fields[2].col = col;     fields[2].row = row - 1;
        fields[3].col = col - 1; fields[3].row = row;
        fields[4].col = col - 1; fields[4].row = row + 1;
        fields[5].col = col;     fields[5].row = row + 1;
    } else {
        fields[0].col = col + 1; fields[0].row = row;
        fields[1].col = col + 1; fields[1].row = row - 1;
        fields[2].col = col;     fields[2].row = row - 1;
        fields[3].col = col - 1; fields[3].row = row - 1;
        fields[4].col = col - 1; fields[4].row = row;
        fields[5].col = col;     fields[5].row = row + 1;
    }
}


int do_player_win(Player winner, Player board[][BOARD_HEIGHT], Data data)
{
    int already_visited[BOARD_WIDTH][BOARD_HEIGHT] = {0};
    FieldStack tovisit = {0};
    Player start = (winner == RED ? VOID_RED1 : VOID_BLUE1);
    int adv_col = (winner == RED ? -1 : 1);
    /* iterate over the sides of VOID_COLOR to get the starting edges to trace */

    for (int row = 2, col = BEGINNING_COL + adv_col * 2; /* left/right VOID col */
         col != BEGINNING_COL + ((data.siz + 2) * adv_col);
         col += adv_col, row++)
    {
        int start_c = col + -adv_col * 2;
        int start_r = row / 2;
        if (board[start_c][start_r] != winner) continue;
        already_visited[start_c][start_r] = 1;
        Field field = Field{start_c, start_r};
        pushfield(&tovisit, field);
        fprintf(stderr, "pushed: %d, %d\n", start_c, start_r);
    }

    while (tovisit.count > 0) { // DFS 
        Field field = popfield(&tovisit);
        fprintf(stderr, "poped: %d, %d\n", field.col, field.row);
        Field sides[6];
        getsides(sides, field.col, field.row);
        for (int i = 0; i < 6; i++) {
            int c = sides[i].col;
            int r = sides[i].row;
            if (IS_OPPOSITE(start, board[c][r])) return 1;
            if (board[c][r] != winner || already_visited[c][r]) continue;
            already_visited[c][r] = 1;
            Field field =  {c, r};
            pushfield(&tovisit, field);
            fprintf(stderr, "pushed: %d, %d\n", c, r);

        }
    }
    return 0;
}


int has_multiple_paths(Player test, Player board[][BOARD_HEIGHT], Data ret)
{
    int found = 0;

    for (int i = 0; i < BOARD_WIDTH && !found; i++) {
        for (int j = 0; j < BOARD_HEIGHT && !found; j++) {
            if (board[i][j] == test) {
                board[i][j] = NONE;
                if (!do_player_win(test, board, ret)) {
                    found = 1;
                }
                board[i][j] = test;
            }
        }
    }
    return !found;
}

int main()
{
    Player board[BOARD_WIDTH][BOARD_HEIGHT] = {Player::NONE};
    char cmd[CMD_LEN];
    Data data;
    while ((data = boardread(board)).siz != -1) {
        fprintf(stderr, "RED: %d\n", data.red_count);
        fprintf(stderr, "BLUE: %d\n", data.blue_count);
        fprintf(stderr, "SIZE: %d\n", data.siz);
        scanf("%s", cmd);
        fprintf(stderr, "CMD: %s\n", cmd);
        if (strcmp(cmd, "BOARD_SIZE") == 0) {
            printf("%d\n", data.siz);
        } else if (strcmp(cmd, "PAWNS_NUMBER") == 0) {
            printf("%d\n", data.blue_count + data.red_count);
        } else if (strcmp(cmd, "IS_BOARD_CORRECT") == 0) {
            int diff = (int)data.red_count - (int)data.blue_count;
            printf("%s\n", 0 <= diff && diff <= 1 ? "YES" : "NO");
        } else if (strcmp(cmd, "IS_GAME_OVER") == 0) {
            int diff = (int)data.red_count - (int)data.blue_count;
            if (!(0 <= diff && diff <= 1)) {
                printf("NO\n\n");
                continue;
            }
            fprintf(stderr, "[CHECK FOR BLUE]\n");
            if (do_player_win(BLUE, board, data)) {
                printf("YES BLUE\n\n");
                continue;
            }
            fprintf(stderr, "[CHECK FOR RED]\n");
            if (do_player_win(RED, board, data)) {
                printf("YES RED\n\n");
                continue;
            }
            printf("NO\n\n");
        } else if (strcmp(cmd, "IS_BOARD_POSSIBLE") == 0) {
            int diff = (int)data.red_count - (int)data.blue_count;
            if (!(0 <= diff && diff <= 1)) {
                printf("NO\n\n");
                continue;
            }
            int is_blue_win = do_player_win(BLUE, board, data);
            int is_red_win = do_player_win(RED, board, data);
            if (!(is_blue_win || is_red_win)) { // no one wins the game
                printf("YES\n\n");
                continue;
            }
            if (is_blue_win && is_red_win) {
                printf("NO\n\n");
                continue;
            }
            if (is_blue_win && data.blue_count != data.red_count) {
                printf("NO\n\n");
                continue;
            }
            if (is_red_win && data.red_count <= data.blue_count) {
                printf("NO\n\n");
                continue;
            }
            if (is_red_win && has_multiple_paths(RED, board, data)) {
                printf("NO\n\n");
                continue;
            }
            if (is_blue_win && has_multiple_paths(BLUE, board, data)) {
                printf("NO\n\n");
                continue;
            }
            printf("YES\n\n");
            continue;
        } else if (strcmp(cmd, "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT") == 0) {
            for (int i = 0; i < 4; i++) {
                while (getchar() != '\n')
                    ;
            }
        } else if (strcmp(cmd, "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT") == 0) {
            for (int i = 0; i < 4; i++) {
                while (getchar() != '\n')
                    ;
            }
        }
    }
}
