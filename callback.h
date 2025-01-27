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


void actionFunc (GtkObject*, gpointer);

void confirmFunc (gpointer, guint, GtkWidget*);

void readFile (_data*);

void loadFunc (GtkObject*, gpointer);

void openFunc (gpointer, guint, GtkWidget*);

void saveAsFunc (GtkObject*, gpointer);

void saveFunc (gpointer, guint, GtkWidget*);

void gameFunc (gpointer, guint, GtkWidget*);

void playersFunc (gpointer, guint, GtkWidget*);

void aboutFunc (gpointer, guint, GtkWidget*);

void helpFunc (gpointer, guint, GtkWidget*);

void quitFunc (GtkObject*, GdkEventAny*, gpointer);
