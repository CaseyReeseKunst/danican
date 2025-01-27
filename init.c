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
#include <stdlib.h>
#include <gtk/gtk.h>
#include "board.h"
#include "game.h"
#include "main.h"
#include "play.h"
#include "callback.h"

void error (char*);

GdkPixmap* initManAspect (GdkWindow* window, GdkGC* gc, int s,
			  GdkColor* back, GdkColor* fill, GdkColor* draw)
{
    GdkPixmap* pixmap;
    int x, y, w, h;

    pixmap = gdk_pixmap_new (window, s, s, -1);
    gdk_gc_set_foreground (gc, back);
    gdk_draw_rectangle (pixmap, gc, TRUE, 0, 0, s, s);
    x = s/2 - (9*s+2)/20; y = s/2 - s/4;
    w = (9*s-1)/10; h = (2*s)/5;
    gdk_gc_set_foreground (gc, fill);
    gdk_draw_arc (pixmap, gc, TRUE, x, y + s/10, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, draw);
    gdk_draw_arc (pixmap, gc, FALSE, x, y + s/10, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, fill);
    gdk_draw_arc (pixmap, gc, TRUE, x, y, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, draw);
    gdk_draw_arc (pixmap, gc, FALSE, x, y, w, h, 0, 360*64);
    return pixmap;
}

GdkPixmap* initKingAspect (GdkWindow* window, GdkGC* gc, int s,
			   GdkColor* back, GdkColor* fill, GdkColor* draw)
{
    GdkPixmap* pixmap;
    int x, y, w, h;

    pixmap = gdk_pixmap_new (window, s, s, -1);
    gdk_gc_set_foreground (gc, back);
    gdk_draw_rectangle (pixmap, gc, TRUE, 0, 0, s, s);
    x = s/2 - (9*s+2)/20; y = s/2 - (3*s)/10;
    w = (9*s-1)/10; h = (2*s)/5;
    gdk_gc_set_foreground (gc, fill);
    gdk_draw_arc (pixmap, gc, TRUE, x, y + s/5, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, draw);
    gdk_draw_arc (pixmap, gc, FALSE, x, y + s/5, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, fill);
    gdk_draw_arc (pixmap, gc, TRUE, x, y + s/10, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, draw);
    gdk_draw_arc (pixmap, gc, FALSE, x, y + s/10, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, fill);
    gdk_draw_arc (pixmap, gc, TRUE, x, y, w, h, 0, 360*64);
    gdk_gc_set_foreground (gc, draw);
    gdk_draw_arc (pixmap, gc, FALSE, x, y, w, h, 0, 360*64);
    return pixmap;
}

void initColor (_data* data)
{
    GdkColormap* colormap;
    
    colormap = gdk_window_get_colormap(data->area->window);

    gdk_color_white (colormap, &data->white);
    gdk_color_black (colormap, &data->black);
    
    data->dark.red = 0x6b00;
    data->dark.green = 0x8e00;
    data->dark.blue = 0x2300;
    gdk_colormap_alloc_color(colormap, &data->dark, FALSE, TRUE);

    data->light.red = 0xff00;
    data->light.green = 0xe400;
    data->light.blue = 0xc400;
    gdk_colormap_alloc_color(colormap, &data->light, FALSE, TRUE);

    data->back.red = 0xfa00;
    data->back.green = 0x8000;
    data->back.blue = 0x7200;
    gdk_colormap_alloc_color(colormap, &data->back, FALSE, TRUE);
}
    
void initPixmap (_data* data, int size)
{
    data->size = size;
    data->pixmap = gdk_pixmap_new (data->area->window, 10*size, 10*size, -1);
    gdk_gc_set_foreground (data->gc, &data->light);
    gdk_draw_rectangle (data->pixmap, data->gc, TRUE, 0, 0, 10*size, 10*size);
    
    data->aspect[0] = gdk_pixmap_new (data->area->window, size, size, -1);
    gdk_gc_set_foreground (data->gc, &data->dark);
    gdk_draw_rectangle (data->aspect[0], data->gc, TRUE, 0, 0, size, size);
    
    data->aspect[1] = initManAspect (data->area->window, data->gc, size,
    				     &data->dark, &data->white, &data->black);

    data->aspect[2] = initManAspect (data->area->window, data->gc, size,
    				     &data->dark, &data->black, &data->white);

    data->aspect[3] = initKingAspect (data->area->window, data->gc, size,
    				      &data->dark, &data->white, &data->black);

    data->aspect[4] = initKingAspect (data->area->window, data->gc, size,
    				      &data->dark, &data->black, &data->white);

    data->aspect[5] = gdk_pixmap_new (data->area->window, size, size, -1);
    gdk_gc_set_foreground (data->gc, &data->back);
    gdk_draw_rectangle (data->aspect[5], data->gc, TRUE, 0, 0, size, size);
    
    gdk_gc_set_foreground (data->gc, &data->black);
}

_data* initData (GtkWidget* window, GtkWidget* vbox,
        	 int size, int depth, char* name)
{
    GtkWidget* box;
    _data* data;

    data = (_data*) malloc (sizeof (_data));
    if (data == NULL) error ("Unable to malloc in initData.\n");

    data->window = window;
    data->game = initGame ();
    data->visible = initBoard ();
    data->depth = depth;
    data->state = WAITING;
    data->whitePlayer = HUMAN;
    data->blackPlayer = COMPUTER;
    data->action = 0;
    data->saved = 1;
    data->name = name;
    data->lastText = 0;
    data->confirmWin = NULL;
    data->warningWin = NULL;
    data->openWin = NULL;
    data->saveAsWin = NULL;
    data->aboutWin = NULL;
    data->helpWin = NULL;

    box = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
    gtk_box_pack_end (GTK_BOX (vbox), box, TRUE, FALSE, 0);

    data->area = gtk_drawing_area_new ();
    gtk_box_pack_start (GTK_BOX (box), data->area, TRUE, FALSE, 0);
    gtk_widget_set_events (data->area,
	                   GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
    gtk_drawing_area_size (GTK_DRAWING_AREA (data->area),
	                   10*size, 10*size);
    gtk_signal_connect (GTK_OBJECT (data->area), "expose_event",
	                (GtkSignalFunc) exposeFunc, (gpointer) data);
    gtk_signal_connect (GTK_OBJECT (data->area), "button_press_event",
	                (GtkSignalFunc) button_pressFunc, (gpointer) data);
    gtk_widget_realize (data->area);
    data->gc = gdk_gc_new (data->area->window);

    initColor (data);
    initPixmap (data, size);
    drawPixmap (data);

    return data;
}

void initConfirmWin (_data* data)
{
    GtkWidget *confirm, *box, *message, *button;
    
    confirm = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (confirm), "Confirm");
    gtk_signal_connect (GTK_OBJECT (confirm), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (confirm), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_window_set_policy (GTK_WINDOW (confirm), FALSE, FALSE, FALSE);

    box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (confirm)->vbox), box);
    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
    gtk_widget_show (box);
    message = gtk_label_new ("Abort current game?");
    gtk_box_pack_start_defaults (GTK_BOX (box), message);
    gtk_widget_show (message);
    button = gtk_button_new_with_label ("OK");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                (GtkSignalFunc) actionFunc, data);
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
	                       (GtkSignalFunc) gtk_widget_hide,
			       GTK_OBJECT (confirm));
    gtk_box_pack_start_defaults
       	(GTK_BOX (GTK_DIALOG (confirm)->action_area), button);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
    button = gtk_button_new_with_label ("Cancel");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
	                       (GtkSignalFunc) gtk_widget_hide,
			       GTK_OBJECT (confirm));
    gtk_box_pack_start_defaults
       	(GTK_BOX (GTK_DIALOG (confirm)->action_area), button);
    gtk_widget_show (button);
    data->confirmWin = confirm;
}

void initWarningWin (_data* data)
{
    GtkWidget *warning, *box, *message, *button;
    
    warning = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (warning), "Error");
    gtk_signal_connect (GTK_OBJECT (warning), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (warning), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_window_set_policy (GTK_WINDOW (warning), FALSE, FALSE, FALSE);

    box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (warning)->vbox), box);
    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
    gtk_widget_show (box);
    message = gtk_label_new ("");
    gtk_box_pack_start_defaults (GTK_BOX (box), message);
    gtk_widget_show (message);
    button = gtk_button_new_with_label ("OK");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
	                       (GtkSignalFunc) gtk_widget_hide,
			       GTK_OBJECT (warning));
    gtk_box_pack_start_defaults
       	(GTK_BOX (GTK_DIALOG (warning)->action_area), button);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
    data->warningWin = warning;
    data->warningMessage = message;
}

void initErrorWin (char* msg)
{
    GtkWidget *error, *box, *message, *button;
    
    error = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (error), "Fatal Error");
    gtk_signal_connect (GTK_OBJECT (error), "delete_event",
	                (GtkSignalFunc) gtk_main_quit, NULL);
    gtk_signal_connect (GTK_OBJECT (error), "destroy",
	                (GtkSignalFunc) gtk_main_quit, NULL);
    gtk_window_set_policy (GTK_WINDOW (error), FALSE, FALSE, FALSE);

    box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (error)->vbox), box);
    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
    gtk_widget_show (box);
    message = gtk_label_new (msg);
    gtk_box_pack_start_defaults (GTK_BOX (box), message);
    gtk_widget_show (message);
    message = gtk_label_new ("Please send a bug report to:\n"
	                     "<obrecht@altern.org>");
    gtk_box_pack_start_defaults (GTK_BOX (box), message);
    gtk_widget_show (message);
    button = gtk_button_new_with_label ("OK");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			(GtkSignalFunc) gtk_main_quit, NULL);
    gtk_box_pack_start_defaults
       	(GTK_BOX (GTK_DIALOG (error)->action_area), button);
    gtk_widget_show (button);
    gtk_widget_show (error);
}

void initOpenWin (_data* data)
{
    GtkFileSelection* window;
    
    window = (GtkFileSelection *)gtk_file_selection_new("Open...");
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (window->ok_button), "clicked",
	                (GtkSignalFunc) loadFunc, data);
    gtk_signal_connect_object
       	(GTK_OBJECT (window->ok_button), "clicked",
	 (GtkSignalFunc) gtk_widget_hide, GTK_OBJECT (window));
    gtk_signal_connect_object
       	(GTK_OBJECT (window->cancel_button), "clicked",
         (GtkSignalFunc) gtk_widget_hide, GTK_OBJECT (window));
    data->openWin = window;
}

void initSaveAsWin (_data* data)
{
    GtkFileSelection* window;
    
    window = (GtkFileSelection *)gtk_file_selection_new("Save As...");
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (window->ok_button), "clicked",
	                (GtkSignalFunc) saveAsFunc, data);
    gtk_signal_connect_object
       	(GTK_OBJECT (window->ok_button), "clicked",
	 (GtkSignalFunc) gtk_widget_hide, GTK_OBJECT (window));
    gtk_signal_connect_object
       	(GTK_OBJECT (window->cancel_button), "clicked",
	 (GtkSignalFunc) gtk_widget_hide, GTK_OBJECT (window));
    data->saveAsWin = window;
}

void initAboutWin (_data* data)
{
    GtkWidget *about, *box, *message, *button;
    
    about = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (about), "About Danican");
    gtk_signal_connect (GTK_OBJECT (about), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (about), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_window_set_policy (GTK_WINDOW (about), FALSE, FALSE, FALSE);

    box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (about)->vbox), box);
    gtk_container_set_border_width (GTK_CONTAINER (box), 10);
    gtk_widget_show (box);
    message = gtk_label_new ("DANICAN version 0.5.2\n\n"
	                     "Copyright (C) 2002 Christian Obrecht\n\n"
	                     "<obrecht@altern.org>");
    gtk_box_pack_start_defaults (GTK_BOX (box), message);
    gtk_widget_show (message);
    button = gtk_button_new_with_label ("OK");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
	                       (GtkSignalFunc) gtk_widget_hide,
			       GTK_OBJECT (about));
    gtk_box_pack_start_defaults
       	(GTK_BOX (GTK_DIALOG (about)->action_area), button);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
    data->aboutWin = about;
}

void initHelpWin (_data* data)
{
    GtkWidget *help, *table, *text, *scrollbar;

    help = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_signal_connect (GTK_OBJECT (help), "delete_event",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_signal_connect (GTK_OBJECT (help), "destroy",
	                (GtkSignalFunc) gtk_widget_hide, NULL);
    gtk_widget_set_usize (help, 585, 400);

    table = gtk_table_new (2, 1, FALSE);
    gtk_container_add (GTK_CONTAINER (help), table);
    gtk_container_set_border_width (GTK_CONTAINER (table), 2);
    gtk_widget_show (table);
    text = gtk_text_new (NULL, NULL);
    gtk_table_attach (GTK_TABLE (table), text, 0, 1, 0, 1,
		      GTK_EXPAND | GTK_SHRINK | GTK_FILL,
		      GTK_EXPAND | GTK_SHRINK | GTK_FILL, 2, 2);
    gtk_widget_show (text);
    scrollbar = gtk_vscrollbar_new (GTK_TEXT (text)->vadj);
    gtk_table_attach (GTK_TABLE (table), scrollbar, 1, 2, 0, 1,
		      GTK_FILL, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 2, 2);
    gtk_widget_show (scrollbar);
    data->helpWin = help;
    data->text = text;
    data->font =
	gdk_font_load ("-*-courier-medium-r-*-*-*-120-*-*-*-*-iso8859-1");
}
