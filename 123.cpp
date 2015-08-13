// 123 is a text mode 2048 like game for UNIX terminal
// to compile: g++ -std=c++11 -lncurses 123.cpp
#include <ncurses.h>  // in emacs Ctl-x-o on this guy to open
#include <cstdlib>    // rand() 
#include <functional> // mem_fn()
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;


#define KEY_UP2    65
#define KEY_DOWN2  66
#define KEY_RIGHT2 67
#define KEY_LEFT2  68

#define EMPTY (-1)
#define SIZE(x) (sizeof(x)/sizeof(x[0]))

class Field{
    bool addRand;
    int score;
    int box[16];

    void addRandom(){
	addRand = false;
	while(true){  // TODO: detect all spaces are exausted here instead of making the user press Q
	    int& val = box[ rand()%16 ];
	    if( EMPTY==val ){
		val = rand()%3 ? 0 : 1; // 66% chance of getting a 0
		return;
	    }
	}
    }


    void combine(int i){
	if( box[i]>=0 && box[i]==box[i+1] ){
	    ++box[i];
	    box[i+1] = EMPTY;
	    ++score;
	    addRand = true;
	}
    }


    void shift(int i){
	if( box[i]<0 && box[i+1]>=0 ) {
	    swap( box[i], box[i+1] );
	    addRand = true;
	}
    }


    // This is how the grid looks like for the followin two functions
    //  0  1  2  3
    //  4  5  6  7
    //  8  9 10 11
    // 12 13 14 15

    // for each active (left 3 in each line) square
    void foreach( std::_Mem_fn<void (Field::*)(int)> f ){
	for(int i: {0,1,2, 4,5,6, 8,9,10, 12,13,14 } ){
	    f(this,i); // lol "f this"
	}
    }


    void rotate(){
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
	for(int i=0; i<16; ++i){ box[i] = EMPTY; }
	addRandom();
    }


    void step(Direction dir){ // take one step in the game
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



void show(const Field& field){
    char buff[4];
    for(int y=0; y < 4; ++y){
	for(int x=0; x < 4; ++x){
	    sprintf(buff, "% 2d", field.get(x,y) );
	    mvaddstr(y, x*2, (field.get(x,y) == EMPTY) ? "--" : buff );
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
	show(field);
	int key = getch();
	//	cout << key << " ";
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
