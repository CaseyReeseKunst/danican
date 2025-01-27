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


#define MAX_CAPTURES 16

typedef struct {
    char start;
    char end;
    char capture [MAX_CAPTURES];
    char numCaptures;
} _ply;


#define MAX_PLIES 127

typedef struct {
    _ply ply [MAX_PLIES];
    char index;
    char maxCaptures;
} _plyList;


#define EMPTY 0
#define MAN_BLACK 1
#define KING_BLACK 2
#define MAN_WHITE 4
#define KING_WHITE 8
#define STOP 16
#define BLACK 3
#define WHITE 12
#define NORTH_EAST 0
#define NORTH_WEST 1
#define SOUTH_EAST 2
#define SOUTH_WEST 3

typedef struct {
    char place [51];
    char opponent;
    char man;
    char king;
    char right;
    char left;
} _board;

_board* initBoard (void);

_board* createBoard (void);

_board* copyBoard (const _board* source);

int testCapture (_board*);

_plyList* makePlyList (_board*);

void doPly (_board*, _plyList*, int);

int findPly (_plyList*, char, char);
