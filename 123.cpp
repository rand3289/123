// text mode 2048 like game for UNIX terminal
// to compile: g++ -std=c++11 -lncurses 123.cpp
#include <ncurses.h>  // in emacs Ctl-x-o on this guy to open
#include <cstdlib>    // rand() 
#include <functional> // mem_fn()
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

// TODO: change to arrow keys, addRand is not set correctly when nothing is shifted



class Field{
    bool addRand;
    int score;
    int box[16];
    void rotate();

    int addRandom(){
	addRand = false;
	while(true){  // TODO: detect all spaces are exausted here
	    int idx = rand()%16;
	    if(-1==box[idx]){
		box[idx] = 0;
		return idx;
	    }
	}
    }


void combine(int i){
    if( box[i]>=0 && box[i]==box[i+1] ){
	++box[i];
	box[i+1] = -1;
	++score;
	addRand = true;
    }
}


void shift(int i){
    if( box[i]<0 ) {
	box[i] = box[i+1];
	box[i+1] = -1;
	addRand = true;
    }
}


// for each active squares (left 3 in each line)
void foreach( std::_Mem_fn<void (Field::*)(int)> f ){
    const static int bz [] = {0,1,2, 4,5,6, 8,9,10, 12,13,14 };
    for(int i: bz){
	f(this,i); // lol "f this"
    }
}


public:
    enum Direction { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 };
    int scores() const { return score; }
    int get(int x, int y) const { return box[y*4+x]; }

    Field(): addRand(false), score(0) {
	for(int i=0; i<16; ++i){ box[i] = -1; }
	addRandom();
    }


    int step(Direction dir){ // take one step in the game
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
	    return addRandom();
	}
	return -1;
    }
};



void Field::rotate(){
    int temp = box[0];
    box[0] = box[12];
    box[12] = box[15];
    box[15] = box[3];
    box[3] = temp;

    temp = box[1];
    box[1] = box[8];
    box[8] = box[14];
    box[14] = box[7];
    box[7] = temp;

    temp = box[2];
    box[2] = box[4];
    box[4] = box[13];
    box[13] = box[11];
    box[11] = temp;

    temp = box[5];
    box[5] = box[9];
    box[9] = box[10];
    box[10] = box[6];
    box[6] = temp;
}


void show(const Field& field, int added){
    char buff[32];
    for(int y=0; y < 4; ++y){ //TODO: iterate 0 through 15
	for(int x=0; x <4; ++x){
	    sprintf(buff,"% 2d",field.get(x,y) );
	    mvaddstr(y, x*2, (field.get(x,y) == -1) ? "--" : buff );
	}
    }
    if(added >=0){ move( (added/4)*2, (added%4)*2 ); } // leave the cursor wher the last entry was added
    refresh(); // ncurses
}


#define KEY_UP2    65
#define KEY_DOWN2  66
#define KEY_RIGHT2 67
#define KEY_LEFT2  68


int main(int argc, char* argv[]){
    initscr();   // entering ncurses mode
    //    cbreak();    // disable line buffering
    noecho();    // pressed symbols wont be printed to screen
    curs_set(0); // hide the cursor
    srand(time(0));

    Field field;
    bool run = true;
    while(run) {
	int added = -1;
	show(field, added);
	int key = getch();
	//	cout << key << " ";
	switch(key){
	    case KEY_LEFT2:
	    case KEY_LEFT:  added = field.step(Field::LEFT);  break;
	    case KEY_RIGHT2:
	    case KEY_RIGHT: added = field.step(Field::RIGHT); break;
	    case KEY_UP2:
	    case KEY_UP:    added = field.step(Field::UP);    break;
	    case KEY_DOWN2:
	    case KEY_DOWN:  added = field.step(Field::DOWN);  break;
	    case 'Q':
	    case 'q': run = false;              break;
	}
    }
    endwin(); // exit ncurses mode
    cout << field.scores() << endl;
}
