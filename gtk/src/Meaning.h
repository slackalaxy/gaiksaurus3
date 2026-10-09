#ifndef INCLUDED_MEANING_H
#define INCLUDED_MEANING_H

#include <gtk/gtk.h>
#include <vector>
#include <string>
using namespace std;

namespace AiksaurusGTK_impl 
{
    class Display;
    class Meaning
    {
        private:
            string d_title;
            vector<string> d_words;
            Display& d_display;
            vector<GtkWidget*> d_lists;
            
            GtkWidget* d_masterLayout;
            GtkWidget* d_mainLayout;
            GtkWidget* d_labelLayout;
            GtkWidget* d_subLayout;
            GtkWidget* d_label;
        
            static void _appendWord(GtkWidget* list, const char* str);
            static char* _wordAt(GtkWidget* list, GtkTreePath* path);
            static void _selectionChanged(GtkTreeSelection* sel, gpointer data);
            static void _rowActivated(GtkTreeView* list, GtkTreePath* path,
                                      GtkTreeViewColumn* col, gpointer data);

        public:
       
            Meaning(const string& title, vector<string>& words, Display& display)
                ;
       
            ~Meaning() ;
        
            GtkWidget* getLayout() ;
            
            void unselectListsExcept(GtkWidget* me) ;
    };
}

#endif // INCLUDED_MEANING_H
