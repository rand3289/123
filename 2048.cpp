class Field{
    int box[16];
    void rotate();
    void shift();
public:
    enum Direction { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 };

    Field(){
	memset(box, -1, sizeof(box) );
	addRandom();
    }

    void addRandom(){
	while(true){  // TODO: detect all spaces are exausted here
	    int idx = rand()%16;
	    if(-1==box[idx]){
		box[idx] = 0;
		return;
	    }
	}
    }

    void modify(Direction dir){
	for(int i = 0; i < 4; ++i){ // rotate 4 times
	    if(dir == i){
		shift(); // left
	    }
	    rotate();
	}
    }
    
    void print(){
	for(int i=0; i < 16; ++i){
	    cout << (i%4? "" : endl);
	    printf("%2d", box[i]); // do not mix
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


int input(){
    char c;
    while(true){
	cin >> c;
	switch(c){
	    case 'j': return j;
	    case 'k': return k;
	    case 'i': return i;
	    case 'm': return m;
	}
    }
}


int main(int argc, char* argv[]){
    Field field;
    while(true){
	show(field);
	int key = input();
	switch(key){
	    case 'j': field.modify(Field::LEFT);
	    case 'k': field.modify(Field::RIGHT);
	    case 'i': field.modify(Field::UP);
	    case 'm': field.modify(Field::DOWN);
	}
	field.addRandom();
    }
}
