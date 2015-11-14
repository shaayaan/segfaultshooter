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

char *highscoresname = ".1";
char *tmphighscorespath = "/tmp/shayananwar";
char highscorespath[512];

/*This function gets the name and the corresponding score from the file*/
void getnamescore(char *line, char *name, char *score) {
	int i, s;
	s = 0;
	for (i = 0; i < 80; i++) {
		if (line[i] == ':') {
			s = i + 1;
			break;
		}
		name[i] = line[i]; 
	}
	for (i = s; i < 80 && line[i] != '\n' && line[i]; i++)
		score[i - s] = line[i];
}

/*Reads from the file & Displays the score on the screen*/
void highscores() {
	FILE *hi;
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	hi = fopen(highscorespath, "r");
	wattron(stdscr, COLOR_PAIR(2));
	if (hi == NULL) {
		attron(COLOR_PAIR(4));
		drawmessage("You don't seem to have any highscores!", 0);
		return;
	}
	wattroff(stdscr, COLOR_PAIR(2));    
	int max = 0;
	int n = 0;
	while (1) {
		char line[512] = {'\0'};
		char hname[256] = {'\0'};;
		char hscore[128] = {'\0'};;
		if (!fgets(line, sizeof line, hi))
			break;
		getnamescore(line, hname, hscore);
		if (LEN(hname) > max)
			max = LEN(hname);
		n++;
	}
	fclose(hi);
	hi = fopen(highscorespath, "r");
	if (hi == NULL) {
		drawmessage("Cant read highscores file!", 0);
		return;
	}
	int r = rmax / 2 - n / 2;
	int nc = cmax / 2 - max / 2 - 2; // name coloumn
	int sc = cmax / 2 + max / 2 + 2; // score coloumn
	drawstring("Highscores", r - 2, cmax / 2 - 4);
	while (1) {
		char line[512] = {'\0'};
		char hname[256] = {'\0'};;
		char hscore[128] = {'\0'};;
		if (!fgets(line, sizeof line, hi)) 
			break;
		getnamescore(line, hname, hscore);
		drawstring(hname, r, nc);
		drawstring(hscore, r, sc);
		r++;
	}
}

/*Checks whether the score is in top 10 and a/c puts it in proper place*/
int checkhighscore(FILE *hi) {
	int place = 0;
	while (1) {
		char line[512] = {'\0'};;
		char hname[256] = {'\0'};;
		char hscore[128] = {'\0'};
		if (!fgets(line, sizeof line, hi))
			break;
		getnamescore(line, hname, hscore);
		if (score > atoi(hscore)) {
			break;
		}
		place++;
		}
	return place;
}

/*Enables user to enter the name and score at the time of highscore*/
char* getstring(char *buf, int r, int c) {
	int i = 0;
	int j, d, offset;
	curs_set(1);
	move(r, c);
	while (1) {
		d = getch(); 
		if (d == '\n' && buf[0]) {
			break;
		}
		else if (d == KEY_BACKSPACE || d == KEY_DC || d == 127) {
			if (i > 0) {
				i--;
				buf[i] = '\0';
			}
		} 
		else if (d == KEY_LEFT) {
			if (i > 0)
				i--;
		}
		else if (d == KEY_RIGHT) {
			if (i < LEN(buf))
				i++;
		}
		else if (d == KEY_UP) {
		}
		else if (d == KEY_DOWN) {      
		}
		else {
			for (j = LEN(buf); j > i; j--)
				buf[j] = buf[j - 1];
			buf[i] = d;
			i++;
		}
		offset = LEN(buf) / 2;
		clearrow(r);
		drawstring(buf, r, c - offset);
		move(r, c + i - offset);
		refresh();
	}
	curs_set(0);
	return buf;
}

/*Writes the score in the file from the temporary file*/
void addhighscore(int place, char *name, int score) {
	FILE *hi, *ho;
	int i;
	ho = fopen(tmphighscorespath, "w");
	hi = fopen(highscorespath, "r");
	if (!hi || !ho) {
		drawmessage("Cant write or read highscores!", 0);
		return;
	}  
	for (i = 0; i < place; i++) {
		char line[512] = {'\0'}; 
		fgets(line, sizeof line, hi);
		fprintf(ho, "%s", line);
	}
	fprintf(ho, "%s:%i\n", name, score);
	for (i++; i < 10; i++) {
		char line[80] = {'\0'}; 
		if (!fgets(line, sizeof line, hi))
			break;
		fprintf(ho, "%s", line);
	}
	fclose(hi);
	fclose(ho);
	hi = fopen(tmphighscorespath, "r");
	ho = fopen(highscorespath, "w");
	if (!hi || !ho) {
		drawmessage("Cant write or read highscores!", 0);
		return;
	}
	while (1) {
		char line[80] = {'\0'}; 
		if (!fgets(line, sizeof line, hi)) 
			break;
		fprintf(ho, "%s", line);
	}    
	fclose(hi);
	fclose(ho);    
	remove(tmphighscorespath);
}

/*Stores score which user enters in the file*/
void highscore() {
	FILE *hi;
	int ishighscore = 0;
	int place = -1;
	hi = fopen(highscorespath, "r");

	if (hi == NULL) {
		hi = fopen(highscorespath, "w");
		if (hi == NULL) {
			drawmessage("Cant create new highscores file and there is none to read!", 0);
			getch();
			return;
		}
		drawstring("Creating highscores file...", rmax - 1, cmax / 2 - 9);
		ishighscore = 1;
	}

	else {
		place = checkhighscore(hi);
		if (place < 10)
			ishighscore = 1;
	}
	fclose(hi);
	if (ishighscore) {
		drawmessage("Highscore!", 1);
		drawmessage("Enter your name below", 2);
		int r, c;
		r = rmax / 2 + 3;
		c = cmax / 2;
		char name[128] = {'\0'};
		*name = *getstring(name, r, c);
		addhighscore(place, name, score);
	}
}
