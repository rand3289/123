// text mode 2048 like game for UNIX terminal
// to compile: g++ -std=c++11 -lncurses 123.cpp
#include <ncurses.h>
#include <cstdlib> // rand() 
//#include <cstring> // memset()
#include <cstdio>
#include <string>
#include <iostream>
#include <functional> // bind(), mem_fn()
//using namespace std::placeholders; // _1
using namespace std;


class Field;
void show(const Field&, int x);


class Field{
    int score;
    int box[16];
    void rotate();

    void addRandom(){
	while(true){  // TODO: detect all spaces are exausted here
	    int idx = rand()%16;
	    if(-1==box[idx]){
		box[idx] = 0;
		return;
	    }
	}
    }


bool combine(int i){
    if( box[i]>=0 && box[i]==box[i+1] ){
	++box[i];
	box[i+1] = -1;
	return true;
    }
    return false;
}


bool shift(int i){
    if( box[i]<0 ) {
	box[i] = box[i+1];
	box[i+1] = -1;
	return true;
    }
    return false;
}


bool foreach( std::_Mem_fn<bool (Field::*)(int)> f){
    const static int bz [] = {0,1,2, 4,5,6, 8,9,10, 12,13,14 };
    bool ret = false;
    for(int i: bz){
	ret |= f(this,i);
    }
    return ret;
}


public:
    enum Direction { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 };
    int scores() const { return score; }
    int get(int x, int y) const { return box[y*4+x]; }

    Field(): score(0) {
	for(int i=0; i<16; ++i){ box[i] = -1; }
	addRandom();
    }


    void step(Direction dir){ // take one step in the game
	bool shifted = false;
	for(int i = 0; i < 4; ++i){ // rotate 4 times
	    if(dir == i){
		    shifted |= foreach( std::mem_fn(&Field::shift) );
		    shifted |= foreach( std::mem_fn(&Field::shift) );
		    shifted |= foreach( std::mem_fn(&Field::shift) );
		    shifted |= foreach( std::mem_fn(&Field::combine) );
	    }
	    rotate();
	    show(*this,i+1);
	}
	if( shifted ) { // add random only if anything shifted
	    addRandom();
	}
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


void show(const Field& field, int dx){
    static int dy = 0; // DEBUG
    char buff[32];
    for(int y=0; y < 4; ++y){ //TODO: iterate 0 through 15
	for(int x=0; x <4; ++x){
	    sprintf(buff,"% 2d",field.get(x,y) );
	    mvaddstr(y+dy, x*2+dx*20, (field.get(x,y) == -1) ? "--" : buff );
	}
    }
    refresh(); // ncurses
    if(4==dx){
	dy+=5; // DEBUG
    }
}


int main(int argc, char* argv[]){
    initscr(); // entering ncurses mode
    noecho();  // pressed symbols wont be printed to screen
    cbreak();  // disable line buffering
    //    raw();     // CTRL-C and others do not generate signals

    Field field;
    bool run = true;
    while(run) {
	show(field ,0);
	int key = getch();
	switch(key){
	    case 'j': field.step(Field::LEFT);  break;
	    case 'k': field.step(Field::RIGHT); break;
	    case 'i': field.step(Field::UP);    break;
	    case 'm': field.step(Field::DOWN);  break;
	    case 'q': run = false;              break;
	}
    }
    endwin(); // exit ncurses mode
    cout << field.scores() << endl;
}
