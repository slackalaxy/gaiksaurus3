
#include "Meaning.h"
#include "Display.h"
#include <cctype>  
using namespace std;

namespace AiksaurusGTK_impl 
{
    
    static void ucwords(string& str) 
    {
        bool ws = true;
        for(int i = 0;i < static_cast<int>(str.size());++i)
        {
            if (isspace(str[i]))
                ws = true;
                
            else if (ws)
            {
                str[i] = toupper(str[i]);
                ws = false;
            }
        }
    }

    Meaning::Meaning(const string& title, vector<string>& words, Display& display) 
    
        : d_title(title), d_words(words), d_display(display), d_lists(4)
    {
        d_masterLayout = gtk_event_box_new();
        
        ucwords(d_title);
        
        gtk_widget_set_name(d_masterLayout, "ybg");

        d_mainLayout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add( GTK_CONTAINER(d_masterLayout), d_mainLayout );

        d_labelLayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        gtk_box_pack_start( GTK_BOX(d_mainLayout), d_labelLayout, 0, 0, 2);

        d_label = gtk_label_new(d_title.c_str());
        gtk_widget_set_name(d_label, "mst");
        gtk_label_set_justify( GTK_LABEL(d_label), GTK_JUSTIFY_LEFT );
        gtk_box_pack_start( GTK_BOX(d_labelLayout), d_label, 0, 0, 4 );

        d_subLayout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_set_homogeneous(GTK_BOX(d_subLayout), TRUE);  // equal columns
        gtk_box_pack_start( GTK_BOX(d_mainLayout), d_subLayout, 0, 0, 0 );

        int i;
        for(i = 0;i < 4;++i)
        {
            GtkListStore* store = gtk_list_store_new(1, G_TYPE_STRING);
            d_lists[i] = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
            g_object_unref(store);

            GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes(
                "", gtk_cell_renderer_text_new(), "text", 0, NULL
            );
            gtk_tree_view_append_column(GTK_TREE_VIEW(d_lists[i]), column);
            gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(d_lists[i]), FALSE);

            gtk_container_set_border_width( GTK_CONTAINER(d_lists[i]), 0 );
            gtk_widget_set_name(d_lists[i], "wbg");  // white list background
            gtk_box_pack_start( GTK_BOX(d_subLayout), d_lists[i], 1, 1, 0 );

            // single click selects a word, double click looks it up.
            g_signal_connect(
                G_OBJECT(gtk_tree_view_get_selection(GTK_TREE_VIEW(d_lists[i]))),
                "changed", G_CALLBACK(_selectionChanged), this
            );
            g_signal_connect(
                G_OBJECT(d_lists[i]), "row-activated",
                G_CALLBACK(_rowActivated), this
            );
        }

        int n = d_words.size();
        int q = n / 4;
        int r = n % 4;
        int stop1, stop2, stop3;
        
        switch(r)
        {
            case 0:
                stop1 = q;
                stop2 = stop1 + q;
                stop3 = stop2 + q;
                break;    
                
            case 1:
                stop1 = q + 1;
                stop2 = stop1 + q;
                stop3 = stop2 + q;
                break;
                
            case 2:
                stop1 = q + 1;
                stop2 = stop1 + (q + 1);
                stop3 = stop2 + q;
                break;
                
            default: // r = 3
                stop1 = q + 1;
                stop2 = stop1 + (q + 1);
                stop3 = stop2 + (q + 1);
                break;
        }

        for(i = 0;i < stop1;++i)
        {
            const char* str = d_words[i].c_str();
            _appendWord(d_lists[0], str);
        }

        for(; i < stop2;++i)
        {
            const char* str = d_words[i].c_str();
            _appendWord(d_lists[1], str);
        }

        for(; i < stop3;++i)
        {
            const char* str = d_words[i].c_str();
            _appendWord(d_lists[2], str);
        }

        for(; i < static_cast<int>(d_words.size());++i)
        {
            const char* str = d_words[i].c_str();
            _appendWord(d_lists[3], str);
        }
        
    }

    Meaning::~Meaning() 
    {
    
    }

    GtkWidget* Meaning::getLayout() 
    {
        return d_masterLayout;
    }


    void Meaning::_appendWord(GtkWidget* list, const char* str)
    {
        GtkListStore* store = GTK_LIST_STORE(
            gtk_tree_view_get_model(GTK_TREE_VIEW(list))
        );
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, str, -1);
    }


    // Returns the word at 'path' (or at the selection if path is null),
    // as a newly allocated string, or 0 if there is none.
    char* Meaning::_wordAt(GtkWidget* list, GtkTreePath* path)
    {
        GtkTreeModel* model = 0;
        GtkTreeIter iter;
        char* text = 0;

        if (path)
        {
            model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
            if (!gtk_tree_model_get_iter(model, &iter, path))
                return 0;
        }
        else
        {
            GtkTreeSelection* sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
            if (!gtk_tree_selection_get_selected(sel, &model, &iter))
                return 0;
        }

        gtk_tree_model_get(model, &iter, 0, &text, -1);
        return text;
    }


    void Meaning::_selectionChanged(GtkTreeSelection* sel, gpointer data)
    {
        Meaning *m = static_cast<Meaning*>(data);
        GtkWidget* list = GTK_WIDGET(gtk_tree_selection_get_tree_view(sel));

        // Also fired when another list is being cleared: nothing selected.
        char* text = _wordAt(list, 0);
        if (!text)
            return;

        m->d_display._handleSelection(list);

        try { m->d_display._handleClick(false, text); }
        catch(...) { }
        g_free(text);
    }


    void Meaning::_rowActivated
    (GtkTreeView* list, GtkTreePath* path, GtkTreeViewColumn* col, gpointer data)
    {
        Meaning *m = static_cast<Meaning*>(data);

        char* text = _wordAt(GTK_WIDGET(list), path);
        if (!text)
            return;

        try { m->d_display._handleClick(true, text); }
        catch(...) { }
        g_free(text);
    }


    void Meaning::unselectListsExcept(GtkWidget* list) 
    {
        for(int i = 0;i < static_cast<int>(d_lists.size());++i)
        {
            if (d_lists[i] != list)
                gtk_tree_selection_unselect_all(
                    gtk_tree_view_get_selection(GTK_TREE_VIEW(d_lists[i])));
        }
    }

}

