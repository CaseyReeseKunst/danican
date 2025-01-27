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
#include "base.h"
#include "game.h"
#include "main.h"
#include "init.h"
#include "callback.h"

static GtkItemFactoryEntry menuEntry[] =
{
  {"/_File", NULL, NULL, 0, "<Branch>"},
  {"/File/_New", "<control>N", confirmFunc, 0, "<Item>"},
  {"/File/_Open...", "<control>O", openFunc, 0, "<Item>"},
  {"/File/_Save", "<control>S", saveFunc, 0, "<Item>"},
  {"/File/Save _As...", NULL, saveFunc, 1, "<Item>"},
  {"/File/sep", NULL, NULL, 0, "<Separator>"},
  {"/File/_Quit", "<control>Q", confirmFunc, 1, "<Item>"},
  {"/_Game", NULL, NULL, 0, "<Branch>"},
  {"/Game/_Undo", "<control>U", gameFunc, 0, "<Item>"},
  {"/Game/_Redo", "<control>R", gameFunc, 1, "<Item>"},
  {"/Game/sep", NULL, NULL, 0, "<Separator>"},
  {"/Game/_First", "<control>F", gameFunc, 2, "<Item>"},
  {"/Game/_Last", "<control>L", gameFunc, 3, "<Item>"},
  {"/_Players", NULL, NULL, 0, "<Branch>"},
  {"/Players/White human", NULL, playersFunc, 0, "<RadioItem>"},
  {"/Players/White computer", NULL, playersFunc, 1, "/Players/White human"},
  {"/Players/sep", NULL, NULL, 0, "<Separator>"},
  {"/Players/Black human", NULL, playersFunc, 2, "<RadioItem>"},
  {"/Players/Black computer", NULL, playersFunc, 3, "/Players/Black human"},
  {"/_Help", NULL, NULL, 0, "<LastBranch>"},
  {"/Help/_About", NULL, aboutFunc, 0,"<Item>"},
  {"/Help/sep", NULL, NULL, 0, "<Separator>"},
  {"/Help/_Rules", "<control>H", helpFunc, 1, "<Item>"},
  {"/Help/_Man page", NULL, helpFunc, 2, "<Item>"},
  {"/Help/_Copying", NULL, helpFunc, 3, "<Item>"}
};

int main (int argc, char** argv)
{
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget* menubar;
    GtkWidget* item;
    GtkAccelGroup *accel;
    GtkItemFactory *items;
    _data* data;
    int size, depth;
    char* name;

    depth = createHash ();
    
    size = INIT_SIZE;
    name = NULL;
    if (argc == 2 || argc == 3) {
	if (argv[1][0] == '-' && argv[1][1] == 's') {
	    size = atoi (argv[1]+2);
	    if (size < 30 || size > 70) size = INIT_SIZE;
	} else name = argv[1];
    }
    if (argc == 3) name = argv[2];

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Danican (White to play)");
    gtk_window_set_policy (GTK_WINDOW (window), FALSE, FALSE, FALSE);

    box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), box);

    data = initData (window, box, size, depth, name);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
	                (GtkSignalFunc) quitFunc, data);
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
	                (GtkSignalFunc) quitFunc, data);
    
    accel = gtk_accel_group_new ();
    items = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<Main>", accel);
    
    gtk_item_factory_create_items (items, 25, menuEntry, data);

    gtk_accel_group_attach (accel, GTK_OBJECT (window));

    item = gtk_item_factory_get_widget (items, "/Players/Black computer");
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(item), TRUE);
    
    menubar = gtk_item_factory_get_widget (items, "<Main>");
    gtk_box_pack_start (GTK_BOX (box), menubar, FALSE, FALSE, 0);

    gtk_widget_show_all (window);
    if (name) readFile (data);
    
    gtk_main ();

    return 0;
}
