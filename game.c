#include <linux/input.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include"game.h"

int nbugparts = 5;

char *bugparts[5] = {"OO\nOO", "OOOOO\nO     \nO     \nO     \nOOOOO\n", "OOOO\nO  O\nO  O\nOOOO\n", "OOO\nOOO\nOOO", "OOOOO\n"};

/*bugparts basically is the variety of enemies*/

int rmax, cmax, nbullets, nbugs, delay = 25000, flag = 0, godmode, flagvs = 0, pauseflag = 0;
object *bullets;
object *bugs;
object arrow, arrow1;
short running, score, level, levels;
int shootd, upd, downd, lives;

#include "highscores.c"
#include "input.c"

short LEN(const char *str) {
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

short randr(short min, short max) {
	return (rand() % (max - min) + min);
}

void draw(char dc, short r, short c) {
	if (r >= rmax || c >= cmax) return;
	attron(A_BOLD);
	
	mvaddch(r, c, dc);
	attroff(A_BOLD);		
}

void drawbullet(char dc, short r, short c) {
	if (r >= rmax || c >= cmax) return;
	attron(A_BOLD);
	mvaddch(r, c, dc);
	attroff(A_BOLD);		
}


void drawstring(char *string, short r, short c) {
	attron(A_BOLD);
	mvaddnstr(r, c, string, cmax - c);
	attroff(A_BOLD);
}

void drawmessage(char *message, short offset) {
	short r = rmax / 2 + offset;
	short c = cmax / 2 - LEN(message) / 2;
	drawstring(message, r, c);
}

void map(struct _object *o) {
	int size = LEN((*o).str) * 2;
	int *map = malloc(sizeof(int) * size);
	int i, j;
	int r, c;
	int health = 0;
	i = r = c = 0;
	for (j = 0; (*o).str[j] != '\0'; j++) {
		if ((*o).str[j] == '\n') {
			r++;
			c = 0;
			continue;
		}
		else if ((*o).str[j] != ' ') {
			map[i] = r;
			map[i + 1] = c;      
			i += 2;
			health++;
		}
		c++;
	}
	map[i] = -1;
	(*o).map = map;
	(*o).health = health / 7 + 1;
}

void clearrow(int r) {
	char space[cmax];
	int i;
	for (i = 0; i < cmax - 1; i++) space[i] = ' ';
	space[i] = '\0';
	drawstring(space, r, 0);
}

void clearrow1(int r) {
	char space[cmax];
	int i;
	for (i = 0; i < cmax - 1; i++) space[i] = ' ';
	space[i] = '\0';
	drawstring(space, r, 0);
}
void drawobject(object o) {
	int i, j, l, pr;
	i = pr = 0;
	for (j = 0; o.str[j] != '\0'; j++) {
		if (o.str[j] == '\n') {
			for (l = 0; l < j - i; l++) {
				if (o.str[l + i] != ' ')
					if(!flag)
						draw(o.str[l + i], o.r + pr, o.c + l);
					else 
						drawbullet(o.str[l + i], o.r + pr, o.c + l);
			}
			pr++;
			i = j + 1;
		}
	}
}

short collides(object a, object b) {
	int i, j;
	for (i = 0; a.map[i] != -1; i += 2) {
		for (j = 0; b.map[j] != -1; j += 2) {
			if (a.map[i] + a.r == b.map[j] + b.r) {
				if (a.map[i + 1] + a.c == b.map[j + 1] + b.c) {
					return 1;
				}
			}
		}
	}  
	return 0;
}

void newbug(struct _object *b) {
	(*b).speed = 1;
	(*b).r = randr(10, rmax);
	(*b).c = -15 - randr(0, 20);
	(*b).str = bugparts[randr(0, nbugparts)];
	map(&(*b));
}

void newbullet(struct _object *b, int r, int c) {
	(*b).r = r;
	(*b).c = c - 6;
	(*b).w = 4;
	(*b).speed = 2;
	(*b).str = "---\n";
	map(&(*b));
}


void newbulletleft(struct _object *b, int r, int c) {
	(*b).r = r;
	(*b).c = c - 6;
	(*b).w = 4;
	(*b).speed = 2;
	(*b).str = "---\n";
	map(&(*b));
}



void copyobject(struct _object *n, struct _object *o) {
	(*n).w = (*o).w;
	(*n).h = (*o).h;
	(*n).speed = (*o).speed;
	(*n).str = (*o).str;
	(*n).r = (*o).r;
	(*n).c = (*o).c;
	(*n).map = (*o).map;
}

void game() {
	char dsa;
	short i;
	while (running) {
		if (lives < 1)
			running = 0;
		clear();
		if (shootd)
			addbullet();
		if (upd)
			up();
		if (downd)
			down();
		if (arrow.r + arrow.h / 2 > rmax - 1)
			arrow.r = rmax - 1 - arrow.h / 2;
		if (arrow.r + arrow.h / 2 < 0) 
			arrow.r = -arrow.h / 2;
		flag = 1;
		drawobject(arrow);
	    // draw and update bugs
		for (i = 0; i < nbugs; i++) {
			flag = 0;
			drawobject(bugs[i]);
			bugs[i].c += bugs[i].speed;
			if (bugs[i].c > cmax) {
				if(pauseflag == 0) {
					newbug(&bugs[i]);
					lives--;
				}
			}
		// check for collisions with bullets
		short j;
		for (j = 0; j < nbullets; j++)
			if (bullets[j].speed && collides(bugs[i], bullets[j])) {
				bullets[j].speed = bullets[j].c = 0;
				score++;
				bugs[i].health--;
				if (bugs[i].health < 0) {
					if(pauseflag == 0)
						newbug(&bugs[i]);
				}
			}
		}
		// draw and update bullets
		for (i = 0; i < nbullets; i++) {
			if (!bullets[i].speed) 
				continue;
			flag = 0;
			drawobject(bullets[i]);
			bullets[i].c -= bullets[i].speed;
			if (bullets[i].c < -bullets[i].w)
				bullets[i].speed = 0;
		}
		char string[200] = {'\0'};
		int diffi = senddiff();
		sprintf(string, "CONTROLS");
		drawstring(string, 1, cmax - strlen(string) - 4);
	
		sprintf(string, "Shoot : Spacebar");
		drawstring(string, 2, cmax - strlen(string) - 1);


		sprintf(string, "Up : arrow up");
		drawstring(string, 3, cmax - strlen(string) - 1);


		sprintf(string, "Down : arrow down");
		drawstring(string, 4, cmax - strlen(string) - 1);
		if(diffi == 1)
			sprintf(string, "score: %d   lives: %d   Beginner", score, lives);
		else if(diffi == 2)
			sprintf(string, "score: %d   lives: %d   Normal", score, lives);
		else if(diffi == 3)
				sprintf(string, "score: %d   lives: %d   Expert", score, lives);
		else if(diffi == 99)
				sprintf(string, "score: %d   lives: %d   GOD MODE", score, lives);
		else 
			sprintf(string, "score: %d   lives: %d	 Beginner ", score, lives);
		drawstring(string, 0, 0);
		
		
		// if time to add another bug... add another bug
		if (score > level) {
			level *= 2;
			levels++;
			object *old;
			old = malloc(nbugs * sizeof(object));
			for (i = 0; i < nbugs; i++) {
				copyobject(&old[i], &bugs[i]);
			}
			int oldn = nbugs;
			nbugs += levels;;
			bugs = realloc(bugs, nbugs * sizeof(object));
			for (i = 0; i < oldn; i++) {
				copyobject(&bugs[i], &old[i]);
			}
			for (; i < nbugs; i++) {
				if(pauseflag == 0) 
					newbug(&bugs[i]);
			}
		}    
		refresh();
		int fl = senddiff();
		if(fl == 1) {
			if(score >=  0 && score <= 10)
				usleep(70000);
			else if ( score > 10 && score <= 20)
				usleep(50000);
			else if (score > 20 && score <= 40)
				usleep(40000);
			else 
				usleep(35000);
		}

		else if (fl == 2) {
			if(score >=  0 && score <= 10)
				usleep(50000);
			else if ( score > 10 && score <= 20)
				usleep(40000);
			else if (score > 20 && score <= 40)
				usleep(30000);
			else 
				usleep(27000);
		}

		else if(fl == 3) {
			if(score >=  0 && score <= 10)
				usleep(50000);
			else if ( score > 10 && score <= 20)
				usleep(40000);
			else if (score > 20 && score <= 40)
				usleep(30000);
			else 
				usleep(25000);
		}
		else if (fl == 99) {
			if(score >=  0 && score <= 10)
				usleep(30000);
			else if ( score > 10 && score <= 20)
				usleep(25000);
			else if (score > 20 && score <= 40)
				usleep(20000);
			else 
				usleep(10000);
		}		
		else {
			if(score >=  0 && score <= 10)
				usleep(70000);
			else if ( score > 10 && score <= 20)
				usleep(60000);
			else if (score > 20 && score <= 40)
				usleep(40000);
			else 
				usleep(35000);
		}
	}
	
}

void up() {
	arrow.r -= 2;
}

void down() {
	arrow.r += 2;
}

void up1() {
	arrow1.r -= 2;
}

void down1() {
	arrow1.r += 2;
}

void addbullet() {
	int i;
	for (i = 0; i < nbullets; i++) {
		if (!bullets[i].speed) {
			newbullet(&bullets[i], arrow.r + arrow.h / 2, cmax);
			break;
		}
	}
}

void addbulletleft() {
	int i;
	for (i = 0; i < nbullets; i++) {
		if (!bullets[i].speed) {
			newbulletleft(&bullets[i], arrow1.r + arrow1.h / 2, cmax);
			break;
		}
	}
}

void gamepause(int d) {
	while((d = getch()) != 'p'){
		keypad(stdscr, FALSE);
		pauseflag = 1;
	}
	keypad(stdscr, TRUE);
	pauseflag = 0;
			
}

void gameresume() {

}

void newgame() {
	clear();
	refresh();
	char d; 
	short i;
	running = 1;  
	nbullets = cmax / 2;
	nbugs = 5;
	if(getlives())
		lives = getlives();
	else {
		int fl = senddiff();
		if(fl == 1)
			lives = 10;
		else if(fl == 2)
			lives = 10;
		else if(fl == 3)
			lives = 8;
		else if (fl == 99)
			lives = 999999;
	}
	if(lives == 0)
		lives = 10;
	
	score = 0;
	level = 25;
	levels = 1;
/*arrow.r and .c are positions of the gun on teh window rows and colmsms*/
	arrow.r = rmax / 2;
	arrow.c = cmax - 4;
	arrow.w = 3;
	arrow.h = 7;
	arrow.str =
	"  * \n"
	" ** \n"
	"*** \n"
	"0000\n"
	"*** \n"
	" ** \n"
	"  * \n";
	bullets = malloc(nbullets * sizeof(object));
	for (i = 0; i < nbullets; i++) {
		newbullet(&bullets[i], 0, 0);
	}  




	bugs = malloc(nbugs * sizeof(object));
	for (i = 0; i < nbugs; i++) {
		if(pauseflag == 0)
			newbug(&bugs[i]);
	}  
	drawmessage("SEGFAULT SHOOTER", -3);
	drawmessage("Stop the SEGFAULTS from getting past you", -1);
	drawmessage("Arrow keys to move, space to shoot", 0);
	drawmessage("Q to exit", 1);
	drawmessage("Anything else to start", 2);
	d = getch();
	if (d == 'q') {
		clear();
		refresh();
		return;
	}	
/*Enables the keyboard arrow keys*/  
	pthread_t pth;
	pthread_create(&pth, NULL, input, "get keys");
	game();
	pthread_cancel(pth);  
	clear();
/* This is printed on the window when game is over*/
	drawmessage("Game Over", -1);
	char string[10] = {'\0'};
	sprintf(string, "%d", score);
	drawmessage(string, 0);
	drawmessage("Return to continue", 2);
	while ((d = getch()) && d != '\n')
		if (d == 'q') {
			
			return;
		}
	clear();
	highscore();
	clear();
	highscores();
	while ((d = getch()) && d != '\n')
		if (d == 'q') {
			clear();
			refresh();
			return;
		}
}

void game2() {
	char dsa;
	short i;
	while (running) {
		if (lives < 1)
			running = 0;
		clear();
		if (shootd)
			addbullet();
		if (upd)
			up();
		if (downd)
			down();
		if (arrow.r + arrow.h / 2 > rmax - 1)
			arrow.r = rmax - 1 - arrow.h / 2;
		if (arrow.r + arrow.h / 2 < 0) 
			arrow.r = -arrow.h / 2;
		if (arrow1.r + arrow1.h / 2 > rmax - 1)
			arrow1.r = rmax - 1 - arrow1.h / 2;
		if (arrow1.r + arrow1.h / 2 < 0) 
			arrow1.r = -arrow1.h / 2;
		flag = 1;
		drawobject(arrow);
		drawobject(arrow1);
	    // draw and update bugs
		for (i = 0; i < nbugs; i++) {
			flag = 0;
			drawobject(bugs[i]);
			bugs[i].c += bugs[i].speed;
			if (bugs[i].c > cmax) {
				if(pauseflag == 0)
					newbug(&bugs[i]);
				lives--;
			}
		// check for collisions with bullets
		short j;
		for (j = 0; j < nbullets; j++)
			if (bullets[j].speed && collides(bugs[i], bullets[j])) {
				bullets[j].speed = bullets[j].c = 0;
				score++;
				bugs[i].health--;
				if (bugs[i].health < 0) {
				if(pauseflag == 0)
					newbug(&bugs[i]);
				}
			}
		}
		// draw and update bullets
		for (i = 0; i < nbullets; i++) {
			if (!bullets[i].speed) 
				continue;
			flag = 0;
			drawobject(bullets[i]);
			bullets[i].c -= bullets[i].speed;
			if (bullets[i].c < -bullets[i].w)
				bullets[i].speed = 0;
		}
		char string[200] = {'\0'};
		int diffi = senddiff();
		sprintf(string, "PLAYER 1");
		drawstring(string, 1, cmax - strlen(string) - 4 - 15);
	
		sprintf(string, "Shoot : Spacebar");
		drawstring(string, 2, cmax - strlen(string) - 1 - 15);


		sprintf(string, "Up : arrow up");
		drawstring(string, 3, cmax - strlen(string) - 1 - 15);


		sprintf(string, "Down : arrow down");
		drawstring(string, 4, cmax - strlen(string) - 1 - 15);




		sprintf(string, "PLAYER 2");
		drawstring(string, 1, cmax - strlen(string) - 1);
	
		sprintf(string, "Shoot : f");
		drawstring(string, 2, cmax - strlen(string) - 1);


		sprintf(string, "Up : w");
		drawstring(string, 3, cmax - strlen(string) - 1);


		sprintf(string, "Down : s");
		drawstring(string, 4, cmax - strlen(string) - 1);
		if(diffi == 1)
			sprintf(string, "score: %d   lives: %d   Beginner", score, lives);
		else if(diffi == 2)
			sprintf(string, "score: %d   lives: %d   Normal", score, lives);
		else if(diffi == 3)
				sprintf(string, "score: %d   lives: %d   Expert", score, lives);
		else if(diffi == 99)
				sprintf(string, "score: %d   lives: %d   GOD MODE", score, lives);
		else 
			sprintf(string, "score: %d   lives: %d	 Beginner ", score, lives);
		drawstring(string, 0, 0);
		
		
		// if time to add another bug... add another bug
		if (score > level) {
			level *= 2;
			levels++;
			object *old;
			old = malloc(nbugs * sizeof(object));
			for (i = 0; i < nbugs; i++) {
				copyobject(&old[i], &bugs[i]);
			}
			int oldn = nbugs;
			nbugs += levels;;
			bugs = realloc(bugs, nbugs * sizeof(object));
			for (i = 0; i < oldn; i++) {
				copyobject(&bugs[i], &old[i]);
			}
			for (; i < nbugs; i++) {
				if(pauseflag == 0)
				newbug(&bugs[i]);
			}
		}    
		refresh();
		int fl = senddiff();
		if(fl == 1) {
			if(score >=  0 && score <= 10)
				usleep(70000);
			else if ( score > 10 && score <= 20)
				usleep(50000);
			else if (score > 20 && score <= 40)
				usleep(40000);
			else 
				usleep(35000);
		}

		else if (fl == 2) {
			if(score >=  0 && score <= 10)
				usleep(50000);
			else if ( score > 10 && score <= 20)
				usleep(40000);
			else if (score > 20 && score <= 40)
				usleep(30000);
			else 
				usleep(27000);
		}

		else if(fl == 3) {
			if(score >=  0 && score <= 10)
				usleep(50000);
			else if ( score > 10 && score <= 20)
				usleep(40000);
			else if (score > 20 && score <= 40)
				usleep(30000);
			else 
				usleep(25000);
		}
		else if (fl == 99) {
			if(score >=  0 && score <= 10)
				usleep(30000);
			else if ( score > 10 && score <= 20)
				usleep(25000);
			else if (score > 20 && score <= 40)
				usleep(20000);
			else 
				usleep(10000);
		}		
		else {
			if(score >=  0 && score <= 10)
				usleep(70000);
			else if ( score > 10 && score <= 20)
				usleep(60000);
			else if (score > 20 && score <= 40)
				usleep(40000);
			else 
				usleep(35000);
		}
	}
	
}

void mainn(){
	clear();
	refresh();
	initscr();
	refresh();	
	WINDOW *wnd;
	sprintf(highscorespath, "%s/%s", getenv("HOME"), highscoresname);
	srand(time(NULL));
	wnd = initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	wbkgd(wnd, COLOR_PAIR(1));
	refresh();
	wrefresh(wnd);
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);
	getmaxyx(wnd, rmax, cmax);
	clear();
	newgame();
	endwin();
	clear();
	refresh();
	clear();
}

void mainn2(){
	clear();
	refresh();
	initscr();
	refresh();	
	WINDOW *wnd;
	sprintf(highscorespath, "%s/%s", getenv("HOME"), highscoresname);
	srand(time(NULL));
	wnd = initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	wbkgd(wnd, COLOR_PAIR(1));
	refresh();
	wrefresh(wnd);
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);
	getmaxyx(wnd, rmax, cmax);
	clear();
	newgame2();
	endwin();
	clear();
	refresh();
	clear();
	flagvs = 0;
}

void newgame2() {
	clear();
	refresh();
	char d; 
	short i;
	running = 1;  
	nbullets = cmax / 2;
	nbugs = 5;
	if(getlives())
		lives = getlives();
	else {
		int fl = senddiff();
		if(fl == 1)
			lives = 10;
		else if(fl == 2)
			lives = 10;
		else if(fl == 3)
			lives = 8;
		else if (fl == 99)
			lives = 999999;
	}
	if(lives == 0)
		lives = 10;

	score = 0;
	level = 25;
	levels = 1;
/*arrow.r and .c are positions of the gun on teh window rows and colmsms*/
	arrow.r = rmax / 2;
	arrow.c = cmax - 4;
	arrow.w = 3;
	arrow.h = 7;
	arrow.str =
	"  * \n"
	" ** \n"
	"*** \n"
	"0000\n"
	"*** \n"
	" ** \n"
	"  * \n";


	arrow1.r = rmax / 2 - 10;
	arrow1.c = cmax - 4;
	arrow1.w = 3;
	arrow1.h = 7;
	arrow1.str =
	"  * \n"
	" ** \n"
	"*** \n"
	"0000\n"
	"*** \n"
	" ** \n"
	"  * \n";
	bullets = malloc(nbullets * sizeof(object));
	for (i = 0; i < nbullets; i++) {
		newbullet(&bullets[i], 0, 0);
	}  

	for (i = 0; i < nbullets; i++) {
		newbulletleft(&bullets[i], 0, 0);

	}

	bugs = malloc(nbugs * sizeof(object));
	for (i = 0; i < nbugs; i++) {
		if(pauseflag == 0)
		newbug(&bugs[i]);
	}  
	drawmessage("SEGFAULT SHOOTER", -3);
	drawmessage("Stop the SEGFAULTS from getting past you", -1);
	drawmessage("Arrow keys to move, space to shoot", 0);
	drawmessage("Q to exit", 1);
	drawmessage("Anything else to start", 2);
	d = getch();
	if (d == 'q') {
		clear();
		refresh();
		return;
	}	
/*Enables the keyboard arrow keys*/  
	pthread_t pth;
	pthread_create(&pth, NULL, input, "get keys");
	game2();
	pthread_cancel(pth);  
	clear();
/* This is printed on the window when game is over*/
	drawmessage("Game Over", -1);
	char string[10] = {'\0'};
	sprintf(string, "%d", score);
	drawmessage(string, 0);
	drawmessage("Return to continue", 2);
	while ((d = getch()) && d != '\n')
		if (d == 'q') {
			
			return;
		}
	clear();
	highscore();
	clear();
	highscores();
	while ((d = getch()) && d != '\n')
		if (d == 'q') {
			clear();
			refresh();
			return;
		}
}


void clearscores() {
	clear();
	refresh();
	remove(highscorespath);
	drawmessage("High Scores Cleared", 0);
	clear();
	refresh();
	clear();
}

void twoplayer() {
	flagvs = 1;
	mainn2();
}


int is2player() {
	return flagvs;
}
