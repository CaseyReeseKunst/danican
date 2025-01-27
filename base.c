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
#include <string.h>
#include <time.h>
#include <math.h>
#include "board.h"
#include "base.h"

void error (char*);

#define MAX_HASH 3001
#define MAX_TIME 600

struct hashEntry {
    char state[51];
    int amount[MAX_PLIES];
    int sum;
    struct hashEntry *next;
};

typedef struct hashEntry _hashEntry;

_hashEntry* hashTable[MAX_HASH];

char atoc (char * buffer, int i)
{
    return 10*(buffer[i]-'0')+buffer[i+1]-'0';
}

unsigned int hashFunc (_board* board)
{
    int i;
    unsigned int r = 0, s = 0;

    for (i = 1; i < 25; i++) {
	r = r << 1;
	s = s << 1;
	if (board->place[i] != EMPTY) r++;
	if (board->place[i+25] != EMPTY) s++;
    }
    return (r+s) % MAX_HASH;
}

_hashEntry* lookUp (_board* board, unsigned int index)
{
    _hashEntry* entry;

    entry = hashTable[index];
    while (entry) {
	if (memcmp (entry->state, board->place+1, 51) == 0)
	    return entry;
	entry = entry->next;
    }
    return NULL;
}

_hashEntry* createEntry (_board* board, unsigned int index)
{
    _hashEntry* entry;
    int i;

    entry = (_hashEntry*) malloc (sizeof (_hashEntry));
    if (entry == NULL) error ("Unable to malloc in createEntry.\n");
    memcpy (entry->state, board->place+1, 51);
    for (i = 0; i < MAX_PLIES; i++) entry->amount[i] = 0;
    entry->sum = 0;
    entry->next = hashTable [index];
    hashTable [index] = entry;
    return entry;
}

void handlePly (_board* board, char start, char end)
{
    _plyList* list;
    int move;
    unsigned int index;
    _hashEntry* entry;

    list = makePlyList (board);
    move = findPly (list, start, end);
    if (move == list->index) error ("Corrupted database\n");
    index = hashFunc (board);
    entry = lookUp (board, index);
    if (entry == NULL) entry = createEntry (board, index);
    entry->amount[move]++;
    entry->sum++;
    doPly (board, list, move);
    free (list);
}

int createHash (void)
{
    _board* board;
    FILE *file;
    clock_t t;
    int depth;
    char buffer[10];

    file = fopen (LIBDIR "base.pdn", "r");
    if (!file) error ("Could not open base.pdn.\n");
    srandom ((unsigned int) time (NULL));
    t = clock ();
    board = initBoard ();
    while (fread (buffer, 1, 1, file)) {
	if (buffer[0] == '\n') {
	    free (board);
	    board = initBoard ();
	    fread (buffer, 1, 9, file);
	} else fread (buffer+1, 1, 8, file);
	handlePly (board, atoc (buffer, 4), atoc (buffer, 7));
	fread (buffer, 1, 1, file);
	if (buffer[0] == '\n') continue;
	fread (buffer, 1, 6, file);
	handlePly (board, atoc (buffer, 0), atoc (buffer, 3));
    }
    fclose (file);
    t = clock () - t;
    depth = 4 + (int)floor (log(MAX_TIME*.1*CLOCKS_PER_SEC/t)/(2*M_LN2));
    if (depth < 3) depth = 3;
    if (depth > 10) depth = 10;
    return depth;
}

int getOpeningMove (_board* board)
{
    int i, r;
    _hashEntry* entry;

    entry = lookUp (board, hashFunc (board));
    if (entry == NULL) return -1;
    r = 1+random ()%entry->sum;
    i = -1;
    while (r>0) {
	i++;
	r -= entry->amount[i];
    }
    return i;
}
