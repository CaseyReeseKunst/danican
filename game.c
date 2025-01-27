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
#include <stdio.h>
#include "board.h"
#include "eval.h"
#include "base.h"
#include "game.h"

void error (char*);

_step* newStep (_board* board)
{
    _step* step;

    step = (_step*) malloc (sizeof (_step));
    if (step == NULL) error ("Unable to malloc in newStep.\n");
    step->board = board;
    step->list = makePlyList (board);
    return step;
}

_game* initGame (void)
{
    _game* game;

    game = (_game*) malloc (sizeof (_game));
    if (game == NULL) error ("Unable to malloc in initGame.\n");

    game->step[0] = newStep (initBoard ());
    game->index = 0;
    game->length = 1;
    return game;
}

void clearGame (_game* game)
{
    int i;

    for (i = 0; i <= game->index; i++) free (game->step[i]);
    free (game);
}

_board* getBoard (_game* game)
{
    return game->step[game->index]->board;
}

_plyList* getPlyList (_game* game)
{
    return game->step[game->index]->list;
}

void doMove (_game* game, int i)
{
    _board* board;
    
    if (game->length == MAX_STEP) error ("Game too long.\n");
    game->step[game->index]->move = i;
    board = copyBoard (getBoard (game));
    doPly (board, getPlyList (game), i);
    game->index++;
    if (game->index == game->length) game->length++;
    else game->length = game->index + 1;
    game->step[game->index] = newStep (board);
}

void findMove (_game* game, int depth)
{
    int i;

    i = -1;
    if (game->index < 20) i = getOpeningMove (getBoard (game));
    if (i == -1) i = bestMove (getBoard (game), getPlyList (game), depth);
    if (i >= 0) doMove (game, i);
}

void gameNew (_game* game)
{
    game->index = 0; game->length = 1;
}

void gameUndo (_game* game)
{
    if (game->index > 0) game->index--;
}

void gameRedo (_game* game)
{
    if (game->index + 1  < game->length) game->index++;
}

void gameFirst (_game* game)
{
    game->index = 0;
}

void gameLast (_game* game)
{
    game->index = game->length - 1;
}

int addStep (_game* game, char start, char end)
{
    int i;

    i = findPly (getPlyList (game), start, end);
    if (i == (getPlyList (game))->index) return 0;
    doMove (game, i);
    return 1;
}

_game* readGame (FILE* file)
{
    _game* game;
    char buffer[11];
    int correct;

    correct = 1;
    game = initGame ();
    while (correct && fread (buffer, 1, 10, file)) {
	correct = correct && addStep (game, atoc (buffer, 4), atoc (buffer, 7));
	if (fread (buffer, 1, 6, file) == 0) break;
	correct = correct && addStep (game, atoc (buffer, 0), atoc (buffer, 3));
    }
    if (correct) return game;
    return NULL;
}

void writeGame (_game* game, FILE* file)
{
    int i;

    for (i = 0; i < game->index; i++) {
	if (i%2 == 0) fprintf (file, "%02i. ", i/2+1);
	fprintf
	    (file, "%02i%c%02i%c",
	    game->step[i]->list->ply[game->step[i]->move].start,
	    (game->step[i]->list->maxCaptures == 0)?'-':'x',
	    game->step[i]->list->ply[game->step[i]->move].end,
	    ((i+1)%10 == 0 || i+1 == game->index)?'\n':' ');
    }
}
