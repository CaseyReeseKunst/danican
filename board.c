/*
 *  Danican  version 0.5.2
 *  Copyright (C) 2002 Christian Obrecht
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <stdlib.h>
#include <string.h>
#include "board.h"


void error (char*);


const char oneStep [4][51] = {
0,
     0,      0,      0,      0,     0,
 1,      2,      3,      4,      5,
     7,      8,      9,     10,     0,
11,     12,     13,     14,     15,
    17,     18,     19,     20,     0,
21,     22,     23,     24,     25,
    27,     28,     29,     30,     0,
31,     32,     33,     34,     35,
    37,     38,     39,     40,     0,
41,     42,     43,     44,     45,
0,
    0,      0,      0,      0,      0,
0,      1,      2,      3,      4,    
    6,      7,      8,      9,     10,
0,     11,     12,     13,     14,    
   16,     17,     18,     19,     20,
0,     21,     22,     23,     24,    
   26,     27,     28,     29,     30,
0,     31,     32,     33,     34,    
   36,     37,     38,     39,     40,
0,     41,     42,     43,     44,    
0,
     7,      8,      9,     10,     0,
11,     12,     13,     14,     15,
    17,     18,     19,     20,     0,
21,     22,     23,     24,     25,
    27,     28,     29,     30,     0,
31,     32,     33,     34,     35,
    37,     38,     39,     40,     0,
41,     42,     43,     44,     45,
    47,     48,     49,     50,     0,
 0,      0,      0,      0,      0,
0,
    6,      7,      8,      9,     10,
0,     11,     12,     13,     14,    
   16,     17,     18,     19,     20,
0,     21,     22,     23,     24,    
   26,     27,     28,     29,     30,
0,     31,     32,     33,     34,    
   36,     37,     38,     39,     40,
0,     41,     42,     43,     44,    
   46,     47,     48,     49,     50,
0,      0,      0,      0,      0   
};

const char twoStep [4][51] = {
0,
     0,      0,      0,      0,     0,
 0,      0,      0,      0,      0,
     2,      3,      4,      5,     0,
 7,      8,      9,     10,      0,
    12,     13,     14,     15,     0,
17,     18,     19,     20,      0,
    22,     23,     24,     25,     0,
27,     28,     29,     30,      0,
    32,     33,     34,     35,     0,
37,     38,     39,     40,      0,
0,
    0,      0,      0,      0,      0,
0,      0,      0,      0,      0,
    0,      1,      2,      3,      4,
0,      6,      7,      8,      9,
    0,     11,     12,     13,     14,
0,     16,     17,     18,     19,
    0,     21,     22,     23,     24,
0,     26,     27,     28,     29,
    0,     31,     32,     33,     34,
0,     36,     37,     38,     39,
0,
    12,     13,     14,     15,     0,
17,     18,     19,     20,      0,
    22,     23,     24,     25,     0,
27,     28,     29,     30,      0,
    32,     33,     34,     35,     0,
37,     38,     39,     40,      0,
    42,     43,     44,     45,     0,
47,     48,     49,     50,      0,
     0,      0,      0,      0,     0,
 0,      0,      0,      0,      0,
0,
    0,     11,     12,     13,     14,
0,     16,     17,     18,     19,
    0,     21,     22,     23,     24,
0,     26,     27,     28,     29,
    0,     31,     32,     33,     34,
0,     36,     37,     38,     39,
    0,     41,     42,     43,     44,
0,     46,     47,     48,     49,
    0,      0,      0,      0,      0,
0,      0,      0,      0,      0
};

const _board initialBoard = {
STOP,
MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK,
     MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK,
MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK,
     MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK, MAN_BLACK,
EMPTY,     EMPTY,     EMPTY,     EMPTY,     EMPTY,
     EMPTY,     EMPTY,     EMPTY,     EMPTY,     EMPTY,
MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE,
     MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE,
MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE,
     MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE, MAN_WHITE,
BLACK, MAN_WHITE, KING_WHITE, NORTH_EAST, NORTH_WEST
};

const int right [4] = {SOUTH_EAST, NORTH_EAST, SOUTH_WEST, NORTH_WEST};

const int left [4] = {NORTH_WEST, SOUTH_WEST, NORTH_EAST, SOUTH_EAST};

_ply* newPly (void)
{
    _ply* ply;

    ply = (_ply*) malloc (sizeof (_ply));
    if (ply == NULL) error ("Unable to malloc in newPly.\n");
    return ply;
}

_ply* newCapture (char start, char end, char captured)
{
    _ply* ply;

    ply = newPly ();
    ply->start = start;
    ply->end = end;
    ply->capture[0] = captured;
    ply->numCaptures = 1;
    return ply;
}

void addCapture (_ply* ply, char end, char captured)
{
    if (ply->numCaptures == MAX_CAPTURES) error ("Capture list overflow.\n");
    ply->end = end;
    ply->capture[ply->numCaptures] = captured;
    ply->numCaptures++;
}

_ply* extendCapture (_ply* source, char end, char captured)
{
    _ply* ply;

    if (source->numCaptures == MAX_CAPTURES) error ("Capture list overflow.\n");
    ply = newPly ();
    memcpy (ply, source, sizeof (_ply));
    addCapture (ply, end, captured);
    return ply;
}

void modifyCapture (_ply* ply, char end)
{
    ply->end = end;
}

_plyList* newPlyList (void)
{
    _plyList* plyList;

    plyList = (_plyList*) malloc (sizeof (_plyList));
    if (plyList == NULL) error ("Unable to malloc in newPlyList.\n");
    plyList->index = 0;
    plyList->maxCaptures = 0;
    return plyList;
}

void checkPlyList (_plyList* list)
{
    if (list->index == MAX_PLIES) error ("Ply list overflow.\n");
}

void addMove (_plyList* list, char start, char end)
{
    checkPlyList (list);
    list->ply[list->index].start = start;
    list->ply[list->index].end = end;
    list->ply[list->index].numCaptures = 0;
    list->index++;
}

void copyPly (_plyList* list, _ply* ply)
{
    memcpy (&(list->ply[list->index]), ply, sizeof (_ply));
    list->index++;
}

void addPly (_plyList* list, _ply* ply)
{
    if (ply->numCaptures < list->maxCaptures) return;
    checkPlyList (list);
    copyPly (list, ply);
    if (ply->numCaptures > list->maxCaptures)
	list->maxCaptures = ply->numCaptures;
}

_plyList* cleanPlyList (_plyList* old)
{
    _plyList* list;
    int i;

    list = newPlyList ();
    for (i = 0; i < old->index; i++)
        if (old->ply[i].numCaptures == old->maxCaptures)
	    copyPly (list, &(old->ply[i]));
    list->maxCaptures = old->maxCaptures;
    free (old);
    return list;
}


_board* createBoard (void)
{
    _board* board;

    board = (_board*) malloc (sizeof (_board));
    if (board == NULL) error ("Unable to malloc in createBoard.\n");
    return board;
}
    
_board* copyBoard (const _board* source)
{
    _board* board;

    board = createBoard ();
    memcpy (board, source, sizeof (_board));
    return board;
}

_board* initBoard (void)
{
    return copyBoard (&initialBoard);
}

_board* makeTempBoard (_board* source, int start, int captured)
{
    _board* board;

    board = copyBoard (source);
    board->place[start] = EMPTY;
    board->place[captured] = STOP;
    return board;
}

void modifyTempBoard (_board* board, int captured)
{
    board->place[captured] = STOP;
}

void switchTurn (_board* board)
{
    if (board->opponent == WHITE) {
        board->opponent = BLACK;
        board->man = MAN_WHITE;
        board->king = KING_WHITE;
	board->right = NORTH_EAST;
	board->left = NORTH_WEST;
    } else {
        board->opponent = WHITE;
        board->man = MAN_BLACK;
	board->king = KING_BLACK;
	board->right = SOUTH_WEST;
	board->left = SOUTH_EAST;
   }
}

void manCapture (_board* source, _plyList* list, _ply* ply, int i, int t)
{
    _board* board;
    _ply* new;
    int j;

    board = makeTempBoard (source, i, t);
    for (j = 0; j < 4; j++)
	if ((board->place[oneStep[j][i]] & board->opponent)
	        && (board->place[twoStep[j][i]] == EMPTY))
	{
	    new = extendCapture (ply, twoStep[j][i], oneStep[j][i]);
	    manCapture (board, list, new, twoStep[j][i], oneStep[j][i]);
            free (new);
	}
    addPly (list, ply);
    free (board);
}

void tryMoveMan (_board* board, _plyList* list, int i)
{
    _ply* ply;
    int j;

    if (list->maxCaptures == 0) {
	if (board->place[oneStep[board->right][i]] == EMPTY)
	    addMove (list, i, oneStep[board->right][i]);
	if (board->place[oneStep[board->left][i]] == EMPTY)
	    addMove (list, i, oneStep[board->left][i]);
    }
    for (j = 0; j < 4; j++)
	if ((board->place[oneStep[j][i]] & board->opponent)
	        && (board->place[twoStep[j][i]] == EMPTY))
	{
	    ply = newCapture (i, twoStep[j][i], oneStep[j][i]);
	    manCapture (board, list, ply, twoStep[j][i], oneStep[j][i]);
	    free(ply);
	}
}

#define STOP_MODE 0
#define MOVE_MODE 1
#define CAPTURE_MODE 2

void kingCapture (_board* source, _plyList* list, _ply* old, int j, int i)
{
    _board* board;
    _ply* ply;
    int k, mode;

    board = source;
    k = i;
    mode = MOVE_MODE;
    while (mode) {
	k = oneStep[j][k];
	if (mode == MOVE_MODE) {
	    if (board->place[k] & board->opponent)
		if (board->place[oneStep[j][k]] == EMPTY) {
		    ply = extendCapture (old, oneStep[j][k], k);
		    addPly (list, ply);
		    board = makeTempBoard (source, i, k);
		    k = oneStep[j][k];
		    kingCapture (board, list, ply, left[j], k);
		    kingCapture (board, list, ply, right[j], k);
		    mode = CAPTURE_MODE;
		} else mode = STOP_MODE;
	    else if (board->place[k] != EMPTY) mode = STOP_MODE;
	}
	else
	    if (board->place[k] == EMPTY) {
		modifyCapture (ply, k);
		addPly (list, ply);
		kingCapture (board, list, ply, left[j], k);
		kingCapture (board, list, ply, right[j], k);
	    }
	    else if (board->place[k] & board->opponent)
		if (board->place[oneStep[j][k]] == EMPTY) {
		    addCapture (ply, oneStep[j][k], k);
		    addPly (list, ply);
		    modifyTempBoard (board, k);
		    k = oneStep[j][k];
		    kingCapture (board, list, ply, left[j], k);
		    kingCapture (board, list, ply, right[j], k);
		} else {
		    mode = STOP_MODE;
		    free (board);
		    free (ply);
		}
	    else if (board->place[k] != EMPTY) {
		    mode = STOP_MODE;
		    free (board);
		    free (ply);
	    }
    }
}

void tryMoveKing (_board* source, _plyList* list, int i)
{
    _board* board;
    _ply* ply;
    int j, k, mode;

    for (j = 0; j < 4; j++) {
        board = source;
	k = i;
	mode = MOVE_MODE;
        while (mode) {
	    k = oneStep[j][k];
	    if (mode == MOVE_MODE)
       	        if (board->place[k] == EMPTY)
		    addMove (list, i, k);
		else if (board->place[k] & board->opponent)
                    if (board->place[oneStep[j][k]] == EMPTY) {
			ply = newCapture (i, oneStep[j][k], k);
			addPly (list, ply);
			board = makeTempBoard (source, i, k);
			k = oneStep[j][k];
			kingCapture (board, list, ply, left[j], k);
			kingCapture (board, list, ply, right[j], k);
		        mode = CAPTURE_MODE;
		    } else mode = STOP_MODE;
                else mode = STOP_MODE;
	    else
		if (board->place[k] == EMPTY) {
		    modifyCapture (ply, k);
       		    addPly (list, ply);
		    kingCapture (board, list, ply, left[j], k);
		    kingCapture (board, list, ply, right[j], k);
		}
		else if (board->place[k] & board->opponent)
                    if (board->place[oneStep[j][k]] == EMPTY) {
			addCapture (ply, oneStep[j][k], k);
			addPly (list, ply);
		        modifyTempBoard (board, k);
			k = oneStep[j][k];
			kingCapture (board, list, ply, left[j], k);
			kingCapture (board, list, ply, right[j], k);
		    } else {
		        mode = STOP_MODE;
			free (board);
			free (ply);
		    }
		else {
		        mode = STOP_MODE;
			free (board);
			free (ply);
		}
	}
    }
}

_plyList* makePlyList (_board* board)
{
    _plyList* list;
    int i;

    list = newPlyList ();

    for (i = 1; i < 51; i++)
        if (board->place[i] == board->man)
	    tryMoveMan (board, list, i);
	else if (board->place[i] == board->king)
	    tryMoveKing (board, list, i);
    return cleanPlyList (list);
}

int testCapture (_board* board)
{
    int i, j, k, mode;

    for (i = 1; i < 51; i++)
	if (board->place[i] == board->man)
	    for (j = 0; j < 4; j++) {
	        if ((board->place[oneStep[j][i]] & board->opponent)
	            && (board->place[twoStep[j][i]] == EMPTY)) return 1;
	    }
	else if (board->place[i] == board->king)
	    for (j = 0; j < 4; j++) {
	        k = i;
	        mode = MOVE_MODE;
                while (mode) {
	            k = oneStep[j][k];
		    if ((board->place[k] & board->opponent)
                        && (board->place[oneStep[j][k]] == EMPTY)) return 1;
		    else if (board->place[k] != EMPTY) mode = STOP_MODE;
		}
	    }
    return 0;
}

void doPly (_board* board, _plyList* list, int i)
{
    char start, end;
    int j;

    start = list->ply[i].start;
    end = list->ply[i].end;
    if (board->opponent == BLACK && end < 6)
	board->place[end] = KING_WHITE;
    else if (board->opponent == WHITE && end > 45)
	board->place[end] = KING_BLACK;
    else
        board->place[end] = board->place[start];
    board->place[start] = EMPTY;
    for (j = 0; j < list->maxCaptures ; j++)
        board->place[list->ply[i].capture[j]] = EMPTY;
    switchTurn (board);
}

int findPly (_plyList* list, char start, char end)
{
    int i = 0;

    while (i < list->index) {
	if ((list->ply[i].start == start) && (list->ply[i].end == end))
	    return i;
	i++;
    }
    return i;
}
