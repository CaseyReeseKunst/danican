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

void initPixmap (_data* data, int);

_data* initData (GtkWidget*, GtkWidget*, int, int, char*);

void initConfirmWin (_data*);

void initWarningWin (_data*);

void initErrorWin (char*);

void initOpenWin (_data*);

void initSaveAsWin (_data*);

void initAboutWin (_data*);

void initHelpWin (_data*);
