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


#define INIT_SIZE 50

#define WAITING 0
#define PENDING 1

#define HUMAN 0
#define COMPUTER 1

typedef struct {
    _game* game;
    _board* visible;
    int depth; 
    int size;
    GtkWidget* window;
    GtkWidget* area;
    GdkPixmap* pixmap;
    GdkPixmap* aspect[6];
    GdkGC* gc;
    GdkColor black, white, dark, light, back;
    int state, start;
    int whitePlayer, blackPlayer;
    int action, saved;
    char* name;
    GtkWidget* confirmWin;
    GtkWidget* warningWin;
    GtkWidget* warningMessage;
    GtkFileSelection* openWin;
    GtkFileSelection* saveAsWin;
    GtkWidget* aboutWin;
    GtkWidget* helpWin;
    GtkWidget* text;
    GdkFont* font;
    int lastText;
} _data;
