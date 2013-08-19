/* vim: set tabstop=2 shiftwidth=2 noexpandtab */ 
/*
	UNIX Life: The Game of Life following the UNIX Philosophy
*/
#define DEBUG 0
#if DEBUG
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG(...)
#endif

#define MAX_SIZE 1024

#define CELL_ON 'X'
#define CELL_OFF '.'

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MOD(a, b) (((a < 0) ? ((a % b) + b) : a) % b)

void
append_char(char *strn, size_t maxsz, char c)
{
	size_t len = strlen(strn);
	if (maxsz > len) {
		strn[len++] = c;
		strn[len] = '\0';
	}

}

typedef struct {
	int width;
	int height;
	char **board;
} Life;

char*
dump_life(Life *);

Life *
parse_life(FILE *input) {
	Life *game = malloc(sizeof(Life));
	int width = 0;
	int height = 0;

	int rowIdx = 0;
	int c;

	char **board = (char **)malloc(MAX_SIZE * sizeof(char *));
	board[0] = (char *)malloc(MAX_SIZE * sizeof(char));
	board[0][0] = '\0';
	while((c = getchar()) != EOF) {
		/* append c to board. */
		append_char(board[rowIdx], MAX_SIZE, (char)c);

        LOG("board[%d] = %s\n", rowIdx, board[rowIdx]);
		if (c == '\n') {
			rowIdx++;
			board[rowIdx] = (char *)malloc(width * sizeof(char));
			board[rowIdx][0] = '\0';
		} else if (rowIdx == 0) {
			width++;
		}
	}
	height = rowIdx;
	game->width = width;
	game->height = height;
    for (int i = 0; i < game->height; i++)
        LOG("Game board %d: %s\n", i, board[i]);
	game->board = board;
	return game;
}

/*
Any live cell with fewer than two live neighbours dies, as if caused by under-population.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overcrowding.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

char
state(int neighbors, char curr) {
	if (curr == '\n' || curr == '\0')
		return curr;
	switch (neighbors) {
		case 0:
		case 1:
			return CELL_OFF;
			break;
		case 2:
			return curr;
			break;
		case 3:
			return CELL_ON;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			return CELL_OFF;
			break;
	}
}

int
neighbors(Life *life, int ci, int cj) {
	int neighbors = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++) {
			neighbors += (int)(life->board[MOD(ci + i, life->height)][MOD(cj + j, life->width)] == CELL_ON);
        }
    /* Don't count *this* cell */
    neighbors -= life->board[ci][cj] == CELL_ON;
	return neighbors;
}

Life *
life(Life *life) {
	Life *new_life = (Life *)malloc(sizeof(Life));
	new_life->width = life->width;
	new_life->height = life->height;
	new_life->board = (char **)malloc(new_life->height * sizeof(char *));
	for (int rowIdx = 0; rowIdx < life->height; rowIdx++)
		new_life->board[rowIdx] = (char *)malloc((life->width + 2) * sizeof(char));
	for (int i = 0; i < life->height; i++)
		for (int j = 0; j < life->width + 2; j++) {
			/* 2 includes newlines and nullchar */
				LOG("Life! %dx%d\n%s\n%d %d\n", life->height, life->width,
						dump_life(life), i, j);
			LOG("Neighbors: %d, %d = %d\n", i, j, neighbors(life, i, j));
			new_life->board[i][j] = state(neighbors(life, i, j), life->board[i][j]);
		}

	return new_life;
}

char *
dump_life(Life *life) {
	/* w x h + w + 1 (w for newlines, 1 for last char) */
	size_t life_chars = life->height * (life->width + 1) + 1;
	char *result = (char *)malloc(life_chars * sizeof(char));
    for (int z = 0; z < life_chars; z++)
        result[z] = '\0';
	int i, j;
	for (i = 0; i < life->height; i++)
		for (j = 0; j < life->width + 1; j++)
			result[i * (life->width + 1) + j] = life->board[i][j];
	return result;
}

void
description(Life *life) {
    LOG("Life (%dx%d):\n%s", life->width, life->height, dump_life(life));
}

int
main(int argc, char *argv[]) {
	Life *life_in = parse_life(stdin);
    LOG("Parsed life:\n");
    description(life_in);
    LOG("\n");
	Life *next_life = life(life_in);
	fprintf(stdout, "%s", dump_life(next_life));
}
