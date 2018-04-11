#include <graphics.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

typedef enum {false, true} bool;
typedef struct Item Item;
struct Item{
    int value;
    Item* above;
};

#define ENTER 13
#define BACKSPACE 8
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define VMAX 26
#define CMAX 4
#define BMAX 13
#define CHARH 18
#define CHARW 12
#define HALFCHARH CHARH / 2
#define HALFCHARW CHARW / 2
#define BOTTOM 480
#define BOXWIDTH CHARW * 4
#define BOXHEIGHT CHARH * 2
#define BOXINTERIORWIDTH CHARW * 3
#define BOXINTERIORHEIGHT CHARH
#define FALSE false
#define TRUE true

#define BLACK setcolor(0)
#define DEFAULT setcolor(7)
#define GREEN setcolor(10)
#define AQUA setcolor(11)
#define RED setcolor(12)
#define PURPLE setcolor(13)
#define YELLOW setcolor(14)
#define WHITE setcolor(15)
#define DRAWBOX(x, y) rectangle(x - HALFCHARW + 1, y - HALFCHARH + 1, x - HALFCHARW + 1 + BOXWIDTH - 1, y - HALFCHARH + 1 + BOXHEIGHT - 1)
#define DRAWHORIZONTAL(x, y, b) line(x + BOXINTERIORWIDTH / 2, y - BOXINTERIORHEIGHT / 2, x + BOXINTERIORWIDTH / 2, b - BOXINTERIORHEIGHT / 2);

void pressBackspace();                  // Wait for a backspace
void arrow(int, int, bool);             // Draw a arrow (visible?)
void printArray(int[VMAX], int);        // Print the array at 'y' Position
void printTail(Item*, int , bool);      // Print the Tail's items (visible?)
void generateValues(int[VMAX]);         // Generate random values for the array
int printMenu(char[], char*[], int);    // Print the menu and select a option
void changeValues(int[VMAX], int);      // Change array values at 'y' Position
void simulate(int[VMAX]);               // Simulate the bucket sort
void markComparison(int, int);          // Outline the comparisons
int scanInt(int, int);        		// Scan a int at 'y' position
char sleep(char);                       // Sleep

Item* createItem(int);                  // Create new item
Item* pushItem(Item*, int, int);        // Push a item into a tail
int popItem(Item**, int*);              // Pop a item from a tail

int main(){
    char *menu[] = {"Change array values",
		    "Generate random values for the array",
		    "View array values",
		    "Organize the array",
		    "Exit"};
    int array[VMAX], option, menuSize = 5, bottom = CHARH * (menuSize + 4);
    int gd = DETECT, gm; initgraph(&gd, &gm, "C:\\BGI");
    settextstyle(8,0,1);
    generateValues(array);
    do{
	option = printMenu("BUCKET SORT - MENU", menu, menuSize);
	switch(option){
	    case 1: changeValues(array, bottom);
		    break;
	    case 2: generateValues(array);
		    YELLOW;
		    outtextxy(0, bottom, "Array values have been randomized.");
		    pressBackspace();
		    break;
	    case 3: AQUA; outtextxy(0,  bottom, "CURRENT ARRAY VALUES");
		    printArray(array, bottom + 2 * CHARH);
		    pressBackspace();
		    break;
	    case 4: simulate(array);
		    break;
	}
	clearviewport();
    }while(option != menuSize);
    return 0;
}

Item* createItem(int value){
    Item* item = (Item*)malloc(sizeof(Item));
    item->value = value;
    item->above = NULL;
    return(item);
}

Item* pushItem(Item* tail, int value, int tailIndex, char* key){
    Item *p = tail, *q = NULL, *item = createItem(value);
    int tailPointer = 1;
    if (p) {
	YELLOW; markComparison(tailIndex, tailPointer);
	if((*key = sleep(*key)) == BACKSPACE) return(tail);
    }
    while(p != NULL && item->value > p->value){
	q = p;
	p = p->above;
	GREEN;      markComparison(tailIndex, tailPointer++);
	if (p) {
		YELLOW;     markComparison(tailIndex, tailPointer);
		if((*key = sleep(*key)) == BACKSPACE) return(tail);
	}
    }
    AQUA;       markComparison(tailIndex, tailPointer);
    if((*key = sleep(*key)) == BACKSPACE) return(tail);
    BLACK;      printTail(tail, tailIndex, FALSE);
    (q)?(q->above = item):(tail = item);
    item->above = p;
    return(tail);
}

int popItem(Item** tail, int* tailPointer){
    int value = (*tail)->value;
    Item *above = (*tail)->above;
    free((*tail));
    (*tail) = above;
    ++*tailPointer;
    return(value);
}

void changeValues(int array[VMAX], int y){
    char buffer[CMAX], key, *editTxt = "Press ENTER to edit a value.", *finishTxt = "Press ENTER to finish editing.";
    int xCorner = CHARW, yCorner = y + CHARH * 2;
    int i = 0, j = 0, index;

    AQUA; outtextxy(0, y, "SET THE ARRAY VALUES");
    printArray(array, y += 2 * CHARH);
    WHITE; outtextxy(0, y += CHARH * 5, "Use the arrow keys to move around the array.");
    outtextxy(0, y += CHARH, "Press ENTER to edit a value.");
    outtextxy(0, BOTTOM - CHARH * 1.5, "Press BACKSPACE to return.");

    do{
	RED; DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
	key = getch();
        if (key == UP || key == DOWN || key == LEFT || key == RIGHT){
	    GREEN; DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
            if(key == LEFT) j -= (j != 0)?1:-12;
	    else if(key == RIGHT) j += (j != 12)?1:-12;
            else if(key == UP) i -= (i != 0)?1:-1;
            else i += (i != 1)?1:-1;
        }
	else if(key == ENTER){
	    index = i * 13 + j;
	    itoa(array[index], buffer, 10);
	    BLACK; outtextxy(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i, buffer);
	    outtextxy(0, y, editTxt);
            YELLOW; DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
            outtextxy(0, y, finishTxt);

	    array[index] = scanInt(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
	    itoa(array[index], buffer, 10);
            BLACK; outtextxy(0, y, finishTxt);
            WHITE; outtextxy(0, y, editTxt);
        }
    }while(key != BACKSPACE);
}

void generateValues(int array[VMAX]){
    int i;
    srand(time(0));
    for(i = 0; i < VMAX; i++)
	array[i] = rand() % 1000;
}

void simulate(int array[VMAX]){
    int xCorner = CHARW, yCorner = CHARH * 2;
    int yTailPos = BOTTOM - CHARH - BOXHEIGHT;
    int c = 0, min, max, i = 0, j = 0, tailPointer = 0, tailIndex;
    char buffer[CMAX], key;
    Item *bucket[BMAX], *tail;

    clearviewport();
    min = max = array[c++];
    for(; c < VMAX; c++) (array[c] < min)?(min = array[c]):((array[c] > max)?(max = array[c]):NULL);
    GREEN; for(c = 0; c < BMAX; c++){
	bucket[c] = NULL;
	DRAWBOX(xCorner + BOXWIDTH * c, yTailPos);
	itoa(c, buffer, 10);
	outtextxy(xCorner + BOXWIDTH * c, yTailPos, buffer);
    }
    outtextxy(CHARW * 34, 0, "MAX =");
    outtextxy(CHARW * 44, 0, "MIN =");
    outtextxy(0, CHARH * 6, "OPERATION:    * (     -     ) /");
    AQUA;   outtextxy(0, CHARH * 7, "* Inserting");
    YELLOW; outtextxy(150, CHARH * 7, "* Comparing");
    RED;    outtextxy(290, CHARH * 7, "* Pushing to");
    PURPLE; outtextxy(440, CHARH * 7, "* Popping to");
    WHITE; itoa(max, buffer, 10); outtextxy(CHARW * 38, 0, buffer); outtextxy(CHARW * 21, CHARH * 6, buffer);
	    itoa(min, buffer, 10); outtextxy(CHARW * 50, 0, buffer); outtextxy(CHARW * 29, CHARH * 6, buffer);
	    outtextxy(CHARW * 10, CHARH * 6, "12");
    AQUA;   outtextxy(0, 0, "BUCKET SORT SIMULATION");
    WHITE;  outtextxy(0, BOTTOM - CHARH * 1.5, "Press BACKSPACE to return.");
	    outtextxy(380, BOTTOM - CHARH * 1.5, "Press N for next frame.");
    printArray(array, CHARH * 2);
    key = getch();

    for(c = 0; c < VMAX && key != BACKSPACE; c++){
	itoa(array[c], buffer, 10);
	tailIndex = (int)((BMAX - 1) * ((float)(array[c] - min)) / max);
	DEFAULT;outtextxy(CHARW * 15, CHARH * 6, buffer);
	RED;    DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos);

	bucket[tailIndex] = pushItem(bucket[tailIndex], array[c], tailIndex, &key);
	BLACK;  outtextxy(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i, buffer);
		outtextxy(CHARW * 16, CHARH * 6, buffer);
	WHITE;  printTail(bucket[tailIndex], tailIndex, TRUE);
	GREEN;  DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos);
	i += (c != 12)?0:1;
	j += (c != 12)?1:-12;
    }
    for(c = 0, tailIndex = 0, i = 0, j = 0; c < VMAX && key != BACKSPACE; c++){
	while(!bucket[tailIndex]) ++tailIndex;
	array[c] = popItem(&bucket[tailIndex], &tailPointer);
	itoa(array[c], buffer, 10);

	PURPLE; DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer);

	key = sleep(key);
	BLACK;  outtextxy(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer, buffer);
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer);
		arrow(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer, FALSE);
	WHITE;  outtextxy(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i, buffer);
	GREEN;  DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos);
	if(!bucket[tailIndex]){
	    BLACK;  DRAWHORIZONTAL(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer, yTailPos);
	    tailPointer = 0;
	}
	else {
	    WHITE;  DRAWHORIZONTAL(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * tailPointer, yTailPos);
	    if(bucket[tailIndex]->above){
		DRAWBOX(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * (tailPointer + 1));
		arrow(xCorner + BOXWIDTH * tailIndex, yTailPos - BOXHEIGHT * (tailPointer + 1), TRUE);
	    }
	}
	i += (c != 12)?0:1;
	j += (c != 12)?1:-12;
    }

    if (key == BACKSPACE) for(tailIndex = 0; tailIndex < BMAX;){
	if(bucket[tailIndex]) popItem(&bucket[tailIndex], &tailPointer);
	else ++tailIndex;
    }
    else{
	RED;    outtextxy(190, CHARH * 14, "The simulation has ended.");
    }

    while(key != BACKSPACE && getch() != BACKSPACE);
}

char sleep(char key){
    int ms;
    for(ms = 0; ms <= 2000; ms += 100){
	(kbhit())?(key = getch()):NULL;
	if (key == 'n' || key == 'N') return(key = getch());
	else if (key == BACKSPACE) return(key);
	else delay(100);
    }
    return(NULL);
}

void arrow(int xCorner, int yCorner, bool visible){
	int tri[6];
    int xOffset = BOXINTERIORWIDTH / 2;
	tri[0] = xCorner + xOffset; tri[1] = yCorner + BOXINTERIORHEIGHT + 2;
	tri[2] = xCorner + xOffset - 6; tri[3] = yCorner + BOXINTERIORHEIGHT + 14;
	tri[4] = xCorner + xOffset + 6; tri[5] = yCorner + BOXINTERIORHEIGHT + 14;
	setcolor(0); setfillstyle(1, visible?15:0);
	fillpoly(3, tri);
}

void printArray(int array[VMAX], int yCorner){
    char buffer[4];
    int xCorner = CHARW;
    int i, j;
    for(i = 0; i < 2; i++)
        for(j = 0; j < 13; j++){
	    itoa(array[i * 13 + j], buffer, 10);
            GREEN; DRAWBOX(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i);
            WHITE; outtextxy(xCorner + BOXWIDTH * j, yCorner + BOXHEIGHT * i, buffer);
        }
}

void printTail(Item* tail, int j, bool visible){
    int xCorner = CHARW, yCorner = BOTTOM - CHARH - BOXHEIGHT, i = 1;
    char buffer[CMAX];
    while(tail){
	itoa(tail->value, buffer, 10);
	tail = tail->above;
	(visible)?WHITE:BLACK; outtextxy(xCorner + BOXWIDTH * j, yCorner - BOXHEIGHT * i, buffer);
	(visible)?GREEN:BLACK; DRAWBOX(xCorner + BOXWIDTH * j, yCorner - BOXHEIGHT * i);
	++i;
    }
    if(i != 1) arrow(xCorner + BOXWIDTH * j, yCorner - BOXHEIGHT * (i - 1), visible);
}

void markComparison(int j, int i){
    int xCorner = CHARW, yCorner = BOTTOM - CHARH - BOXHEIGHT;
    DRAWBOX(xCorner + BOXWIDTH * j, yCorner - BOXHEIGHT * i);
    arrow(xCorner + BOXWIDTH * j, yCorner - BOXHEIGHT * i, TRUE);
}

int scanInt(int x, int y){
    int i = 0, strSize = CMAX;
    char str[CMAX], key;
    do{
	key = getch();
	if (i < strSize - 1 && key >= '0' && key <= '9'){
	    str[i] = key;
	    ++i;
	    str[i] = '\0';
	    WHITE; outtextxy(x, y, str);
	}
	else if(i != 0 && key == '\b'){
	    BLACK; outtextxy(x, y, str);
	    --i;
	    str[i] = '\0';
	    WHITE; outtextxy(x, y, str);
	}
    }while(key != ENTER);
    if(i != 0) return(atoi(str));
    WHITE; outtextxy(x, y, "0");
    return(0);
}

int printMenu(char title[], char* msg[], int menu_size){
    int i, option, y = 0;
    char *txt = "Select an option.", buffer[2];
    AQUA; outtextxy(0, 0, title);
    for(i = 0; i < menu_size; i++){
        itoa(i + 1, buffer, 10);
        WHITE;    outtextxy(CHARW * 1, y += CHARH, buffer);
	DEFAULT;      outtextxy(CHARW * 3, y, msg[i]);
    }
    WHITE; outtextxy(0, y += CHARH * 2, txt);
    do{
        option = getch() - '0';
    }while(!(option >= 1 && option <= menu_size));
    return(option);
}

void pressBackspace(){
     WHITE; outtextxy(0, BOTTOM - CHARH * 1.5, "Press BACKSPACE to return.");
     while(getch() != BACKSPACE);
}

