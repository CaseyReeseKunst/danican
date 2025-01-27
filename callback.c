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
#include "init.h"
#include "play.h"
#include "callback.h"

void error (char* msg)
{
    initErrorWin (msg);
}

void warning (_data* data, char* message)
{
    if (!data->warningWin) initWarningWin (data);
    gtk_label_set_text (GTK_LABEL (data->warningMessage), message);
    gtk_widget_show (data->warningWin);
}

void actionFunc (GtkObject* object, gpointer ptr)
{
    if (((_data*)ptr)->action == 0) {
	gameNew (((_data*)ptr)->game);
	((_data*)ptr)->name = NULL;
	((_data*)ptr)->saved = 1;
	refreshWindow ((_data*)ptr);
    } else gtk_main_quit ();
}

void confirmFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    ((_data*)ptr)->action = action;
    if (!((_data*)ptr)->saved && getPlyList (((_data*)ptr)->game)->index) {
	if (!((_data*)ptr)->confirmWin) initConfirmWin ((_data*)ptr);
	gtk_widget_show (((_data*)ptr)->confirmWin);
    } else actionFunc (NULL, ptr);
}

void readFile (_data* data)
{
    FILE* file;
    _game* game;

    file = fopen (data->name, "r");
    if (file == NULL) {
	warning (data, "Unable to open file.");
	return;
    }
    game = readGame (file);
    fclose (file);
    if (game == NULL) {
	warning (data, "Unable to read file.");
	return;
    }
    data->saved = 1;
    clearGame (data->game);
    data->game = game;
    refreshWindow (data);
}

void loadFunc (GtkObject* object, gpointer ptr)
{
    ((_data*)ptr)->name
	   = gtk_file_selection_get_filename (((_data*)ptr)->openWin);
    readFile ((_data*)ptr);
}

void openFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    if (!((_data*)ptr)->openWin) initOpenWin ((_data*)ptr);
    gtk_file_selection_set_filename (((_data*)ptr)->openWin, "");
    gtk_widget_show (GTK_WIDGET(((_data*)ptr)->openWin));
}

void saveAsFunc (GtkObject* object, gpointer ptr)
{
    FILE* file;

    if (object)
       ((_data*)ptr)->name
	   = gtk_file_selection_get_filename (((_data*)ptr)->saveAsWin);
    file = fopen (((_data*)ptr)->name, "w");
    if (file == NULL) {
	warning ((_data*)ptr, "Unable to write file.");
	return;
    }
    writeGame ((_game*)((_data*)ptr)->game, file);
    fclose (file);
    ((_data*)ptr)->saved = 1;
}

void saveFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    if (!((_data*)ptr)->name || action == 1) {
	if (!((_data*)ptr)->saveAsWin) initSaveAsWin ((_data*)ptr);
	gtk_file_selection_set_filename (((_data*)ptr)->saveAsWin, "");
	gtk_widget_show (GTK_WIDGET(((_data*)ptr)->saveAsWin));
    } else saveAsFunc (NULL, ptr);
}

void gameFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    switch (action) {
	case 0: gameUndo (((_data*)ptr)->game);
		break;
	case 1: gameRedo (((_data*)ptr)->game);
		break;
	case 2: gameFirst (((_data*)ptr)->game);
		break;
	case 3: gameLast (((_data*)ptr)->game);
    }
    refreshWindow ((_data*)ptr);
} 
	
void playersFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    switch (action) {
	case 0: ((_data*)ptr)->whitePlayer = HUMAN;
		break;
	case 1: ((_data*)ptr)->whitePlayer = COMPUTER;
		break;
	case 2: ((_data*)ptr)->blackPlayer = HUMAN;
		break;
	case 3: ((_data*)ptr)->blackPlayer = COMPUTER;
    }
}    

void aboutFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    if (!((_data*)ptr)->aboutWin) initAboutWin ((_data*)ptr);
    gtk_widget_show (((_data*)ptr)->aboutWin);
}

int openTxtFile (_data* data, unsigned int i)
{
    FILE* file;
    char* buffer;
    int sizeRead, numRead, sizeBuf;

    if (gtk_text_get_length (GTK_TEXT (data->text)))
        gtk_editable_delete_text (GTK_EDITABLE (data->text), 0, -1);
    switch (i) {
	case 1: file = fopen (LIBDIR "rules.txt", "r");
		gtk_window_set_title (GTK_WINDOW (data->helpWin),
				      "Rules of International Draughts");
		break;
	case 2: file = fopen (LIBDIR "manual.txt", "r");
		gtk_window_set_title (GTK_WINDOW (data->helpWin),
				      "Man page");
		break;
	case 3: file = fopen (LIBDIR "COPYING", "r");
		gtk_window_set_title (GTK_WINDOW (data->helpWin),
				      "License");
    }
    if (file == NULL) {
       	warning (data, "Unable to open file.");
	return 0;
    }
    sizeRead = 0;
    numRead = sizeBuf = 1024;
    buffer = (char *) malloc (sizeBuf);
    if (buffer == NULL) error ("Unable to malloc in openTxtFile.\n");
    while (numRead >= 1024) {
	if (sizeBuf < sizeRead+1024) {
	    sizeBuf *= 2;
	    buffer = (char *) realloc (buffer, sizeBuf);
	    if (buffer == NULL) error ("Unable to realloc in openTxtFile.\n");
	}
	numRead = fread (buffer+sizeRead, 1, 1024, file);
	sizeRead += numRead;
    }
    gtk_text_insert (GTK_TEXT (data->text), data->font, NULL, NULL,
	             buffer, sizeRead);
    free (buffer);
    fclose (file);
    return 1;
}

void helpFunc (gpointer ptr, guint action, GtkWidget* widget)
{
    int loaded;

    if (!((_data*)ptr)->helpWin) initHelpWin ((_data*)ptr);
    if (((_data*)ptr)->lastText != action)
       	loaded = openTxtFile ((_data*)ptr, action);	
    if (loaded) gtk_widget_show (((_data*)ptr)->helpWin);
}

void quitFunc (GtkObject* object, GdkEventAny* event, gpointer ptr)
{
    confirmFunc (ptr, 2, NULL);
}
