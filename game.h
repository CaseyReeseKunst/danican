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


typedef struct {
    _board* board;
    _plyList* list;
    char move;
} _step;

#define MAX_STEP 512

typedef struct {
    _step* step[MAX_STEP];
    int index, length;
} _game;

_game* initGame (void);

void clearGame (_game*);

_board* getBoard (_game*);

_plyList* getPlyList (_game*);

void doMove (_game*, int);

void findMove (_game*, int);

void gameNew (_game*);

void gameUndo (_game*);

void gameRedo (_game*);

void gameFirst (_game*);

void gameLast (_game*);

_game* readGame (FILE*);

void writeGame (_game*, FILE*);
