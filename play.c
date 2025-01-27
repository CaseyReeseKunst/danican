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


#include <stdio.h>
#include <gtk/gtk.h>
#include "board.h"
#include "game.h"
#include "main.h"


void drawAspect (_data* data, int i, int j)
{
    gdk_draw_pixmap (data->pixmap, data->gc, data->aspect[j],
	             0, 0,
		     (2*(i%5)+(1+i/5)%2)*data->size, (i/5)*data->size,
		     data->size, data->size);
}

void drawPixmap (_data* data)
{
    int i;

    for (i = 0; i < 50; i++)
	switch (data->visible->place[i+1]) {
	    case EMPTY:
		drawAspect (data, i, 0);
		break;
	    case MAN_WHITE:
		drawAspect (data, i, 1);
		break;
	    case MAN_BLACK:
		drawAspect (data, i, 2);
		break;
	    case KING_WHITE:
		drawAspect (data, i, 3);
		break;
	    case KING_BLACK:
		drawAspect (data, i, 4);
		break;
	    case STOP:
		drawAspect (data, i, 5);
	}
}

gint exposeFunc (GtkWidget* area, GdkEventExpose* event, gpointer data)
{
    gdk_draw_pixmap (area->window,
	             ((_data*)data)->gc,
		     ((_data*)data)->pixmap,
		     event->area.x, event->area.y,
		     event->area.x, event->area.y,
		     event->area.width, event->area.height);
    return FALSE;
}

void refreshVisible (_data* data)
{
    int i;

    drawPixmap (data);
    gdk_draw_pixmap (data->area->window, data->gc, data->pixmap,
		     0, 0, 0, 0, data->size*10, data->size*10);
}

void getVisible (_data* data)
{
    memcpy (data->visible, getBoard (data->game), sizeof (_board));
}

void refreshWindow (_data* data)
{
    getVisible (data);
    refreshVisible (data);
    if (data->visible->opponent == WHITE)
	gtk_window_set_title (GTK_WINDOW (data->window),
	           	      "Danican (Black to play)");
    else
	gtk_window_set_title (GTK_WINDOW (data->window),
	           	      "Danican (White to play)");
}

void computerPlay (_data* data)
{
    findMove (data->game, data->depth);
    data->saved = 0;
    refreshWindow (data);
}

int isComputerTurn (_data* data)
{
    return data->visible->opponent == WHITE && data->blackPlayer == COMPUTER
	   || data->visible->opponent == BLACK && data->whitePlayer == COMPUTER;
}

void humanPlay (_data* data, int i)
{
    doMove (data->game, i);
    data->saved = 0;
    refreshWindow (data);
    while (gtk_events_pending ()) gtk_main_iteration ();
    if (isComputerTurn ((_data*)data)) computerPlay ((_data*)data);
}

void handleWaiting (_data* data, int i)
{
    int j, k, l;
    _plyList* list;

    k = 0;
    list = getPlyList (data->game);
    for (j = 0; j < list->index; j++)
	if (list->ply[j].start == i) {
	   data->visible->place[list->ply[j].end] = STOP;
	   k++; l = j;
	}
    if (k == 0) return;
    if (k == 1)
	humanPlay (data, l);
    else {
	data->state = PENDING;
	data->start = i;
	refreshVisible (data);
    }
}

void handlePending (_data* data, int i)
{
    int j;
    _plyList* list;

    if (data->visible->place[i] != STOP) {
	getVisible (data);
        refreshVisible (data);
	data->state = WAITING;
	if (data->start != i) handleWaiting (data, i);
	return;
    }
    list = getPlyList (data->game);
    j = 0;
    while (j < list->index
	   && (list->ply[j].start != data->start || list->ply[j].end != i)) j++;
    humanPlay (data, j);
    data->state = WAITING;
}   

int getIndex (int x, int y)
{
    return ((x+y)%2)?x/2+5*y+1:0;
}

gint button_pressFunc (GtkWidget* area, GdkEventButton* event, gpointer data)
{
    int i;

    if (isComputerTurn ((_data*)data)) {
	computerPlay ((_data*)data);
	return;
    }
    i = getIndex ((int)event->x/((_data*)data)->size,
	          (int)event->y/((_data*)data)->size);
    if (((_data*)data)->state == WAITING) handleWaiting ((_data*)data, i);
    else handlePending ((_data*)data, i);
    return FALSE;
}
