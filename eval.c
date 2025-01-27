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
#include "eval.h"

void error (char*);


int bestMove (_board* source, _plyList* list, int depth)
{
    int i, e, best, move;
    _board* board;

    if (list->index <= 1) return list->index-1;
    board = createBoard ();
    best = (source->opponent == BLACK)?-2000:2000;
    for (i = 0; i < list->index; i++) {
	memcpy (board, source, sizeof (_board));
	doPly (board, list, i);
        e = alphabeta (board, -2000, 2000, depth);
        if (source->opponent == BLACK) {
	    if (e > best) {
		best = e;
		move = i;
	    }
	} else {
	    if (e < best) {
		best = e;
		move = i;
	    }
	}
    }
    free (board);
    return move;
}

int alphabeta (_board* source, int alpha, int beta, int depth)
{
    _plyList* list;
    _board* board;
    int i, e;

    
    if (depth == 0) {
        if (testCapture (source))
	    depth = 1;
        else return evaluate (source);
    }
    list = makePlyList (source);
    if (list->index == 0)
        return (source->opponent == BLACK)?-1000:1000;
    board = createBoard ();
    for (i = 0; i < list->index; i++) {
        memcpy (board, source, sizeof (_board));
	doPly (board, list, i);
        e = alphabeta (board, alpha, beta, depth-1);
        if (source->opponent == BLACK) {
	    if (e >= beta) {
                free (board);
		free (list);
	        return e;
	    }
	    if (e > alpha)
	        alpha = e;
	}
        else {
	    if (e <= alpha) {
                free (board);
		free (list);
	        return e;
	    }
	    if (e < beta)
	        beta = e;
	}
    }
    free (board);
    free (list);
    if (source->opponent == BLACK) return alpha;
    return beta;
}

const int zero[50];

const int whiteMan[50] = {
     17,      17,      17,      17,      17, 
 16,      16,      16,      16,      16,
     15,      15,      15,      15,      15,
 14,      14,      14,      14,      14,
     13,      13,      14,      15,      13, 
 12,      14,      14,      12,      12,
     11,      11,      11,      11,      11,
 10,      10,      10,      10,      10,
      9,       9,       9,       9,       9, 
  8,       8,       9,       8,       8
};

const int blackMan[50] = {
     -8,      -8,      -9,      -8,      -8,
 -9,      -9,      -9,      -9,      -9, 
    -10,     -10,     -10,     -10,     -10,
-11,     -11,     -11,     -11,     -11,
    -12,     -12,     -14,     -14,     -12,
-13,     -15,     -14,     -13,     -13, 
    -14,     -14,     -14,     -14,     -14,
-15,     -15,     -15,     -15,     -15,
    -16,     -16,     -16,     -16,     -16,
-17,     -17,     -17,     -17,     -17
};

const int whiteKing[50] = {
     20,      20,      20,      20,      20,
 20,      20,      20,      20,      20,
     20,      20,      20,      20,      20,
 20,      20,      22,      22,      20,
     20,      23,      23,      20,      20,
 20,      20,      23,      23,      20,
     20,      22,      22,      20,      20,
 20,      20,      20,      20,      20,
     20,      20,      20,      20,      20,
 20,      20,      20,      20,      20
};

const int blackKing[50] = {
    -20,     -20,     -20,     -20,     -20,
-20,     -20,     -20,     -20,     -20,
    -20,     -20,     -20,     -20,     -20,
-20,     -20,     -22,     -22,     -20,
    -20,     -23,     -23,     -20,     -20,
-20,     -20,     -23,     -23,     -20,
    -20,     -22,     -22,     -20,     -20,
-20,     -20,     -20,     -20,     -20,
    -20,     -20,     -20,     -20,     -20,
-20,     -20,     -20,     -20,     -20
};

const int* value[9] = {
    zero, blackMan, blackKing, NULL, whiteMan, NULL, NULL, NULL, whiteKing
};

int evaluate (_board* board)
{
    int i, e = 0;

    for (i = 1; i < 51; i++)
	e += value[board->place[i]][i-1];
    return e;
}
