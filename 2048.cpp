// 2048 like game

#include <ncurses.h>
#include <cstdlib> // rand() 
#include <cstring> // memset()
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;


class Field{
    int score;
    int box[16];
    void rotate();
    bool shift();

    void addRandom(){
	while(true){  // TODO: detect all spaces are exausted here
	    int idx = rand()%16;
	    if(-1==box[idx]){
		box[idx] = 0;
		return;
	    }
	}
    }
public:
    enum Direction { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 };
    int scores(){ return score; }
    int get(int x, int y){ return box[y*4+x]; }

    Field(): score(0) {
	memset(box, -1, sizeof(box) );
	addRandom();
    }


    void modify(Direction dir){ // take one step in the game
	for(int i = 0; i < 4; ++i){ // rotate 4 times
	    if(dir == i){
		if( shift() ) { // left
		    addRandom();
		}
	    }
	    rotate();
	}
    }
};


bool Field::shift(){
    bool modified = false;
    for(int i=0; i < 4; ++i){
        for(int j=1; j < 4; ++j){
            int idx = i*4+j;
	    if(box[idx]>=0){
	        if(box[idx-1]==-1){ // empty space to the left
	      	    box[idx-1]=box[idx];
		    box[idx]=-1;
		    modified = true;
		} else if(box[idx-1]==box[idx]){ // equal to the left
		    ++box[idx-1];
		    box[idx]=-1;
		    modified = true;
		    ++score;
		}
      	    }
        }
    }
    return modified;
}


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


void show(const Field& field){
}


int main(int argc, char* argv[]){
    initscr(); // entering ncurses mode
    noecho();  // pressed symbols wont be printed to screen
    cbreak();  // disable line buffering
    //    raw();     // CTRL-C and others do not generate signals

    Field field;
    while(true){
	show(field);
	int key = getch();
	switch(key){
	    case 'j': field.modify(Field::LEFT);
	    case 'k': field.modify(Field::RIGHT);
	    case 'i': field.modify(Field::UP);
	    case 'm': field.modify(Field::DOWN);
	    case 'q': break;
	}
    }
    endwin(); // exit ncurses mode
    cout << field.scores() << endl;
}
