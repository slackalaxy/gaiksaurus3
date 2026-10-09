#ifndef INCLUDED_DISPLAY_H
#define INCLUDED_DISPLAY_H

#include <Aiksaurus.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <new>
#include "Exception.h"

namespace AiksaurusGTK_impl 
{
    class DialogMediator;
    class Meaning;
    class Display
    {
        friend class Meaning;

        private:
            Display(const Display& rhs);
            Display& operator=(const Display& rhs);

            DialogMediator& d_mediator;
        
            Aiksaurus d_thesaurus;
            GtkWidget* d_scroller;
            GtkWidget* d_white;
            GtkWidget* d_layout;
            std::vector<Meaning*> d_meanings;

            void _handleSelection(GtkWidget* list) ;
            void _handleClick(bool isDoubleClick, const char* text) ;
        
            void _resetDisplay() ;
            
            void _createMeaning(const std::string& title, std::vector<std::string>& words) 
                ;
            
            void _displayResults(const char* word) ;
            void _displayAlternatives() ;
            
            void _checkThesaurus() ;
            
            static void _initResources() ;

        public:
            Display(DialogMediator& parent) ;
            ~Display() ;
        
            const Aiksaurus& getThesaurus() const;
            GtkWidget* getDisplay() ;

            void search(const char* word) ;
            void showMessage(const char* message) ;
    };

}

#endif // INCLUDED_DISPLAY_H
