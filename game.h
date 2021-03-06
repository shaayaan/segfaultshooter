/*****************************************************************************
 * Copyright (C) Shayan Khan shayananwar786@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

typedef struct _object {
	short r, c, w, h, speed;
	char *str;
	int *map;
	int health;
}object;

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
short LEN(const char *str);
short randr(short min, short max);
void draw(char dc, short r, short c);
void drawstring(char *string, short r, short c);
void drawmessage(char *message, short offset);
void map(object *o);
void clearrow(int r);
void drawobject(object o);
short collides(object a, object b);
void newbug(object *b);
void newbullet(object *b, int r, int c);
void newbulletleft(object *b, int r, int c);	
void copyobject(object *n, object *o);
void up();
void up1();
void down1();
void down();
void addbullet();
void addbulletleft();
void game();
void game2();
void newgame2();
void mainn2();
void gamepause(int d);
void gameresume();
void newgame();
void mainn();
void instruct();
int getlives();
int pspeed();
void godmod();
void twoplayervs();
int is2player();
void survive();
int ismodesurvive();

//functions used in menu.c

void settings();
int mainsettings();
void showhighscore();
void plives();
void difficulty();
int senddiff();
void pro();
void inter();
void beginner();
void about();
void endgame();
void clearscores();
void twoplayer();
int isscoreclear();

//fnctions used in highscore.c

void getnamescore(char *line, char *name, char *score);
void highscores();
int checkhighscore(FILE *hi);
char* getstring(char *buf, int r, int c);
void addhighscore(int place, char *name, int score);
void highscore();


