#ifndef INCLUDED_WIN_H
#define INCLUDED_WIN_H

#include <ncurses.h>
#include <string>
#include <vector>
using namespace std;

class Field;

class Window{
    vector<WINDOW*> win;
    const string font = 
	"  ###      #     #####   #####  #       #######  #####  #######  #####   #####  " \
	" #   #    ##    #     # #     # #    #  #       #     # #    #  #     # #     # " \
	"# #   #  # #          #       # #    #  #       #           #   #     # #     # " \
	"#  #  #    #     #####   #####  #######  #####  ######     #     #####   ###### " \
	"#   # #    #    #             #      #        # #     #   #     #     #       # " \
	" #   #     #    #       #     #      #  #     # #     #   #     #     # #     # " \
	"  ###    #####  #######  #####       #   #####   #####    #      #####   #####  " ;

    void showWin(WINDOW* win, int num){
	werase(win);
	if(num>=0){
	    for(int i=0; i<7; ++i){
		wmove(win,i,0);
		for(int j=0; j<8; ++j){
		    waddch(win, font[i*80 + num*8 + j] );
		}
	    }
	}
	wrefresh(win);
    }

public:

    void show(const Field& field) {
	for(int i=0; i< 16; ++i) {
	    showWin( win[i], field.get(i%4,i/4) );
	}
    }

    Window() {
	for(int i = 0; i< 16; ++i){
	    win.push_back( newwin(8, 16, 10*(i/4), 20*(i%4)) );
	}
    }
};


#endif // INCLUDED_WIN_H
