// 2048 like game
// to compile: g++ 2048.cpp -lncurses
#include <ncurses.h>
#include <cstdlib> // rand() 
#include <cstring> // memset()
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;


class Field;
void show(const Field&, int x);


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
    int scores() const { return score; }
    int get(int x, int y) const { return box[y*4+x]; }

    Field(): score(0) {
	memset(box, -1, sizeof(box) );
	addRandom();
    }


    void modify(Direction dir){ // take one step in the game
	bool shifted = false;
	for(int i = 0; i < 4; ++i){ // rotate 4 times
	    if(dir == i){
		shifted = shift();
	    }
	    rotate();
	    show(*this,i+1);
	}
	if( shifted ) { // add random only if anything shifted
	    addRandom();
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


void show(const Field& field, int x){
    static int dy = 0; // DEBUG
    char buff[32];
    for(int y=0; y < 4; ++y){
	sprintf(buff,"% 2d% 2d% 2d% 2d",field.get(0,y),field.get(1,y),field.get(2,y), field.get(3,y));
	mvaddstr(y+dy,x*20,buff);
    }
    refresh(); // ncurses
    if(4==x){
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
	    case 'j': field.modify(Field::LEFT);  break;
	    case 'k': field.modify(Field::RIGHT); break;
	    case 'i': field.modify(Field::UP);    break;
	    case 'm': field.modify(Field::DOWN);  break;
	    case 'q': run = false;                break;
	}
    }
    endwin(); // exit ncurses mode
    cout << field.scores() << endl;
}
