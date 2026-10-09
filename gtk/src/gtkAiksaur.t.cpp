/*
 * AiksaurusGTK - A GTK interface to the AikSaurus library
 * Copyright (C) 2001 by Jared Davis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <AiksaurusGTK.h>
#include "config.h"
#include <gtk/gtk.h>
#include <iostream>
#include <string>
using namespace std;

// ---------------------------------------------------------------------
// System tray support (--tray).
//
// The thesaurus dialog is shown with runThesaurus(), which returns when
// the dialog is closed.  In tray mode we keep our own main loop running
// with a tray icon: clicking the icon opens the dialog (or closes it if
// it is already open); the menu offers Show/Hide and Quit.
// ---------------------------------------------------------------------

// GtkStatusIcon is deprecated in GTK+ 3 but has no replacement that works
// without extra libraries (it still works on Xfce and KDE).
G_GNUC_BEGIN_IGNORE_DEPRECATIONS

namespace
{
    struct TrayState
    {
        AiksaurusGTK* thesaurus;
        bool open;     // dialog currently running?
        bool quit;     // quit requested?
    };

    void trayToggle(TrayState* s, const char* word = 0)
    {
        if (s->open)
        {
            gtk_main_quit();    // ends the dialog's loop, hiding it
            return;
        }

        s->open = true;
        s->thesaurus->runThesaurus(word);
        s->open = false;

        if (s->quit)
            gtk_main_quit();
    }

    void trayActivate(GtkStatusIcon*, gpointer data)
    {
        trayToggle(static_cast<TrayState*>(data));
    }

    void trayMenuToggle(GtkMenuItem*, gpointer data)
    {
        trayToggle(static_cast<TrayState*>(data));
    }

    void trayMenuQuit(GtkMenuItem*, gpointer data)
    {
        TrayState* s = static_cast<TrayState*>(data);
        s->quit = true;
        gtk_main_quit();    // ends the dialog's loop, or our own
    }

    void trayPopup(GtkStatusIcon*, guint, guint, gpointer data)
    {
        static GtkWidget* menu = 0;
        if (!menu)
        {
            menu = gtk_menu_new();

            GtkWidget* show = gtk_menu_item_new_with_label("Show/Hide");
            g_signal_connect(show, "activate", G_CALLBACK(trayMenuToggle), data);
            gtk_menu_shell_append(GTK_MENU_SHELL(menu), show);

            GtkWidget* quit = gtk_menu_item_new_with_label("Quit");
            g_signal_connect(quit, "activate", G_CALLBACK(trayMenuQuit), data);
            gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit);

            gtk_widget_show_all(menu);
        }

        gtk_menu_popup_at_pointer(GTK_MENU(menu), 0);
    }
}

G_GNUC_END_IGNORE_DEPRECATIONS

int main(int argc, char** argv)
{
    const char* description = 
        "Aiksaurus is an English-language thesuarus library.  This\n"
        "is a graphical interface to it.\n\n"
        "   -t, --tray         Also show a system tray icon; click it to\n"
        "                      show or hide the thesaurus.\n"
        "   -m, --minimized    With --tray, start hidden in the tray.\n"
        "   -v, --version      Output version information and quit.\n"
        "   -h, --help         Output this help message and quit.\n\n"
        "For more information, visit\n"
        "   http://www.aiksaurus.com/\n\n";
    
    const char* license =
        "AiksaurusGTK, Version " VERSION "\n" 
        "Copyright (C) 2001 by Jared Davis.\n\n"
        "This program is free software; you can redistribute it and/or\n"
        "modify it under the terms of the GNU General Public License\n"
        "as published by the Free Software Foundation; either version 2\n"
        "of the License, or (at your option) any later version.\n\n"
        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU General Public License for more details.\n\n"
        "You should have received a copy of the GNU General Public License\n"
        "along with this program; if not, write to the Free Software\n"
        "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA\n"
        "02111-1307, USA.\n\n";
   
    const char* opening = 
        "Welcome to AiksaurusGTK\n"
        "    http://www.aiksaurus.com/\n\n"
        
        "To find synonyms for a word, enter the word into the \"look\n"
        "up\" box above.\n\n"
        
        "Command line arguments are:\n"
        "   --help, -h\n"
        "         Displays a help message, then exits.\n"
        "   --version, -v\n"
        "         Displays version and license, then exits.\n"
        "   --tray, -t\n"
        "         Also shows an icon in the system tray; click it\n"
        "         to show or hide this window.\n"
        "   --minimized, -m\n"
        "         With --tray, starts hidden in the tray.\n\n"

        "Also, you can send a word as an argument to this program \n"
        "to skip this screen and jump right to that word.";
    
	gtk_init(&argc, &argv);
	gtk_window_set_default_icon_name("gaiksaurus");
  
    bool tray = false;
    bool minimized = false;
    for(int i = 1;i < argc;++i)
    {
        string s(argv[i]);
        if ((s == "--tray") || (s == "-t"))
        {
            tray = true;
        }
        else if ((s == "--minimized") || (s == "-m"))
        {
            minimized = true;
        }
        else if ((s == "--help") || (s == "-h"))
        {
            cout << "Usage: " << argv[0] << " [word]\n\n" << description;
            exit(0);
        }
        else if ((s == "--version") || (s == "-v"))
        {
            cout << license;            
            exit(0);
        }
    }
   
    // the search word is the last argument, unless that is an option.
    const char* search = 0;
    if (argc > 1 && argv[argc-1][0] != '-')
        search = argv[argc-1];
    
    AiksaurusGTK thesaurus; 
    thesaurus.setTitle("AiksaurusGTK");
    thesaurus.setInitialMessage(opening);
    thesaurus.hideReplacebar();

    if (tray)
    {
        G_GNUC_BEGIN_IGNORE_DEPRECATIONS
        TrayState state = { &thesaurus, false, false };

        // "gaiksaurus" is installed in the hicolor icon theme.
        GtkStatusIcon* icon = gtk_status_icon_new_from_icon_name("gaiksaurus");
        gtk_status_icon_set_title(icon, "AiksaurusGTK");
        gtk_status_icon_set_tooltip_text(icon, "AiksaurusGTK thesaurus");
        g_signal_connect(icon, "activate", G_CALLBACK(trayActivate), &state);
        g_signal_connect(icon, "popup-menu", G_CALLBACK(trayPopup), &state);

        // open the window right away, unless asked to start minimized
        // (a word on the command line always opens it).
        if (search || !minimized)
        {
            trayToggle(&state, search);
            if (state.quit)
                return 0;
        }

        gtk_main();
        g_object_unref(icon);
        G_GNUC_END_IGNORE_DEPRECATIONS
        return 0;
    }

    thesaurus.runThesaurus(search);
    
	return 0;
};
