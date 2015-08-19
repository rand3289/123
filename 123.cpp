// 123 is a text mode 2048 like game for UNIX terminal. It is written using ncurses library.
// to compile: g++ -o 123 -std=c++11 -lncurses 123.cpp
// to use a large interface type: 123 I am batman
// TODO: detect all spaces are exausted and quit instead of making the user press Q
#include <ncurses.h>
#include <cstdlib>    // rand() 
#include <functional> // mem_fn()
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


#define KEY_UP2    65 // my laptop made me do it :)
#define KEY_DOWN2  66
#define KEY_RIGHT2 67
#define KEY_LEFT2  68

#define EMPTY (-1)
#define SIZE(x) (sizeof(x)/sizeof(x[0]))


class Field{
    bool addRand;
    int score;
    int box[16];

    void addRandom(){ // add 0 or 1 in a random empty cell
	addRand = false;
	while(true){
	    int& val = box[ rand()%16 ];
	    if( EMPTY==val ){
		val = rand()%3 ? 0 : 1; // 66% chance of getting a 0
		return;
	    }
	}
    }


    void combine(int i){ // combine any adjacent equal squares to the left
	if( box[i]>=0 && box[i]==box[i+1] ){
	    ++box[i];
	    box[i+1] = EMPTY;
	    ++score;
	    addRand = true;
	}
    }


    void shift(int i){ // shift cells to the left if there are empty cells
	if( EMPTY==box[i] && box[i+1]>=0 ) {
	    swap( box[i], box[i+1] );
	    addRand = true;
	}
    }


    // This is how the grid looks like for the following two functions
    //  0  1  2  3
    //  4  5  6  7
    //  8  9 10 11
    // 12 13 14 15

    // call a member function on each active cell (left 3 in each line)
    void foreach( std::_Mem_fn<void (Field::*)(int)> f ){
	for(int i: {0,1,2, 4,5,6, 8,9,10, 12,13,14 } ){
	    f(this,i); // lol "f this"
	}
    }


    void rotate(){ // rotate 4x4 matrix 90 degrees clockwise
	static const int rotated[] = {12,8,4,12,13,9,9,8,14,10,10,13,15}; // guess why it works :)
	for(int i=0; i<SIZE(rotated); ++i){
	    int idx = rotated[i];
	    std::swap(box[i], box[idx]);
	}
    }


public:
    enum Direction { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 };
    int scores() const { return score; }
    int get(int x, int y) const { return box[y*4+x]; }

    Field(): addRand(false), score(0) {
	for(int i=0; i<16; ++i){ box[i] = EMPTY; } // init all empty
	addRandom();
    }


    // take one step in the game. instead of shifting in many directions, rotate and shift left
    void step(Direction dir){
	for(int i = 0; i < 4; ++i){ // rotate 4 times
	    if(dir == i){
		foreach( std::mem_fn(&Field::shift) );
		foreach( std::mem_fn(&Field::shift) );
		foreach( std::mem_fn(&Field::shift) );
		foreach( std::mem_fn(&Field::combine) );
		foreach( std::mem_fn(&Field::shift) );
		foreach( std::mem_fn(&Field::shift) );
	    }
	    rotate();
	}
	if( addRand ) { // add random if fied was modified by shift() or combine()
	    addRandom();
	}
    }
};



class Window { // large font interface
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
	box(win,0,0);
	if(num>=0){
	    int lo = num%10;
	    for(int i=0; i<7; ++i){
		wmove(win,i+1,9);
		for(int j=0; j<8; ++j){
		    waddch(win, font[i*80 + lo*8 + j] );
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
	    win.push_back( newwin(9, 18, 10*(i/4), 20*(i%4)) );
	}
    }
};



void show(const Field& field, bool large){
    static Window win;
    if(large){ return win.show(field); }

    // small font interface
    for(int y=0; y < 4; ++y){
	for(int x=0; x < 4; ++x){
	    mvprintw(y, x*2, (field.get(x,y) == EMPTY) ? "--" : "% 2d", field.get(x,y) );
	}
    }
    refresh(); // ncurses
}



int main(int argc, char* argv[]){
    initscr();   // ncurses mode
    cbreak();    // disable input buffering
    noecho();    // 
    curs_set(0); // hide the cursor
    srand(time(0));

    Field field;
    bool run = true;
    while(run) {
	show(field, argc>1);
	int key = getch(); //	cout << key << " ";
	switch(key){
	    case KEY_LEFT2:
	    case KEY_LEFT:  field.step(Field::LEFT);  break;
	    case KEY_RIGHT2:
	    case KEY_RIGHT: field.step(Field::RIGHT); break;
	    case KEY_UP2:
	    case KEY_UP:    field.step(Field::UP);    break;
	    case KEY_DOWN2:
	    case KEY_DOWN:  field.step(Field::DOWN);  break;
	    case 'Q':
	    case 'q': run = false;                    break;
	}
    }
    endwin(); // exit ncurses mode
    cout << field.scores() << endl;
}
