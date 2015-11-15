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
#include <ncurses.h>
#include<stdio.h>
#include <menu.h>
#include<string.h>
#include<stdlib.h>
#include"game.h"
#include"menu.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] = {
			"                     NEW GAME                                                  ",
			"                     HOW TO PLAY                                                  ",
			"                     SETTINGS                                                   ",
			"                     HIGHSCORES                                                    ",
			"                     ABOUT ME                                                    ",
			"                     EXIT    ",
		};

char *sett[] = {	"                     DIFFICULTY                                                  ",
			"                     2 PLAYER MODE                                                ",
			"                     GOD MODE                                                   ",
			"                     SURVIVAL MODE                                                   ",
			"                     CHANGE PLAYER LIVES                                                ",
			"                     CLEAR SCORES                                                ",
			                 
		};


char *diff[] = {                       
			"                     BEGINNER                                                  ",
			"                     INTERMEDIATE                                                  ",
			"                     EXPERT                                                   ",

		};
int l = 0, slp = 70000, flagdif = 10, godmode = 0, flagsurvive = 0;
int main() {
	clear();
	refresh();
	curs_set(0);
	ITEM **my_items;
	int c, xc = 0, y = 2;
	box(stdscr,0, 0);
	WINDOW *gamewin;
	WINDOW *top = newwin(LINES - (LINES - LINES / 8), COLS,0, 0);
	box(top, 0, 0);
	wbkgd(top, COLOR_PAIR(2));
	mvwprintw(top, (LINES - (LINES - LINES / 8)) / 2 , COLS / 2 - 8, "SEGFAULT SHOOTER");
	wrefresh(top);
	MENU *my_menu;
	int n_choices = 6, i;
	ITEM *cur_item;
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(9, COLOR_WHITE, COLOR_WHITE);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);
	init_pair(12, COLOR_BLUE, COLOR_BLUE);
	init_pair(13, COLOR_GREEN, COLOR_GREEN);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(7, COLOR_RED, COLOR_RED);
	init_pair(8, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(10, COLOR_CYAN, COLOR_CYAN);

	attrset(COLOR_PAIR(1));
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	mvwprintw(stdscr, LINES - 2, 2, "Press F1 to EXIT");

	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], NULL);
	my_items[n_choices] = (ITEM *)NULL;
	set_item_userptr(my_items[0], mainn);
	set_item_userptr(my_items[1], instruct);
	set_item_userptr(my_items[2], settings);
	set_item_userptr(my_items[3], showhighscore);
	set_item_userptr(my_items[4], about);
	set_item_userptr(my_items[5], endgame);
	my_menu = new_menu((ITEM **)my_items);
	gamewin = newwin(20, 60, (LINES - 20) / 2, (COLS - 60) / 2);
	keypad(gamewin, TRUE);
	set_menu_win(my_menu, gamewin);
	set_menu_sub(my_menu, derwin(gamewin, 6, 58, 3, 1));
	box(gamewin, 0, 0);
	print_in_middle(gamewin, 1, 0, 58, " MAIN MENU", COLOR_PAIR(4));
	mvwaddch(gamewin, 2, 0, ACS_LTEE);
	mvwhline(gamewin, 2, 1, ACS_HLINE, 58);
	mvwaddch(gamewin, 2, 60, ACS_RTEE);
	attron(COLOR_PAIR(6));
	mvprintw(LINES - 2, 2, "Press F1 to EXIT");
	while(xc != COLS / 2 - 43) {
		clear();
//s
		mvprintw(y, xc, "####");
		mvprintw(y + 1, xc, "#");
		mvprintw(y + 2, xc, "####");
		mvprintw(y + 3, xc + 3, "#");
		mvprintw(y + 4, xc, "####");
//e		
		mvprintw(y, xc + 5, "####");
		mvprintw(y + 1, xc + 5, "#");
		mvprintw(y + 2, xc + 5, "####");
		mvprintw(y + 3, xc + 5, "#");
		mvprintw(y + 4, xc + 5, "####");					
		mvprintw(y, xc + 10, "####");

//g
		mvprintw(y + 1, xc + 10, "#");
		mvprintw(y + 2, xc + 10, "#");
		mvprintw(y + 3, xc + 10, "#");
		mvprintw(y + 4, xc + 10, "####");
		mvprintw(y + 3, xc + 10 + 3, "#");
		mvprintw(y + 2, xc + 10 + 3, "#");
		mvprintw(y + 2, xc + 10 + 2, "#");	
//f							
		mvprintw(y, xc + 18, "#");
		mvprintw(y + 1, xc + 18, "#");
		mvprintw(y + 2, xc + 18, "#");
		mvprintw(y + 3, xc + 18, "#");
		mvprintw(y + 4, xc + 18, "#");
		mvprintw(y , xc + 18 + 1, "####");
		mvprintw(y + 2 , xc + 18 + 1, "###");
//a
		mvprintw(y, xc + 24, "#");
		mvprintw(y + 1, xc + 24, "#");
		mvprintw(y + 2, xc + 24, "#");
		mvprintw(y + 3, xc + 24, "#");
		mvprintw(y + 4, xc + 24, "#");
		mvprintw(y , xc + 25 , "####");
		mvprintw(y, xc + 255, "#");
		mvprintw(y + 1, xc + 25 + 3, "#");
		mvprintw(y + 2, xc + 25 + 3, "#");
		mvprintw(y + 3, xc + 25 + 3, "#");
		mvprintw(y + 4, xc + 25 + 3, "#");
		mvprintw(y + 2, xc + 25 + 1, "#");
//u
		mvprintw(y, xc + 30, "#");
		mvprintw(y + 1, xc + 30, "#");
		mvprintw(y + 2, xc + 30, "#");
		mvprintw(y + 3, xc + 30, "#");
		mvprintw(y + 4, xc + 30, "#");
		mvprintw(y + 4 , xc + 31 , "####");
		mvprintw(y + 1, xc + 31 + 3, "#");
		mvprintw(y + 2, xc + 31 + 3, "#");
		mvprintw(y + 3, xc + 31 + 3, "#");
		mvprintw(y + 4, xc + 31 + 3, "#");
		mvprintw(y , xc + 31 + 3, "#");
//l
		mvprintw(y, xc + 30 + 6, "#");
		mvprintw(y + 1, xc + 36, "#");
		mvprintw(y + 2, xc + 36, "#");
		mvprintw(y + 3, xc + 36, "#");
		mvprintw(y + 4, xc + 36, "#");
		mvprintw(y + 4 , xc + 37 , "####");
//t		
		mvprintw(y , xc + 41 , "#####");
		mvprintw(y, xc + 43, "#");
		mvprintw(y + 1, xc + 43, "#");
		mvprintw(y + 2, xc + 43, "#");
		mvprintw(y + 3, xc + 43, "#");
		mvprintw(y + 4, xc + 43, "#");
//s	
		mvprintw(y, xc + 50, "####");
		mvprintw(y + 1, xc + 50, "#");
		mvprintw(y + 2, xc + 50, "####");
		mvprintw(y + 3, xc + 53, "#");
		mvprintw(y + 4, xc + 50, "####");

//h
		mvprintw(y, xc + 55, "#");
		mvprintw(y + 1, xc + 55, "#");
		mvprintw(y + 2, xc + 55, "#");
		mvprintw(y + 3, xc + 55, "#");
		mvprintw(y + 4, xc + 55, "#");		
		mvprintw(y, xc + 59, "#");
		mvprintw(y + 1, xc + 59, "#");
		mvprintw(y + 2, xc + 59, "#");
		mvprintw(y + 3, xc + 59, "#");
		mvprintw(y + 4, xc + 59, "#");
		mvprintw(y + 2 , xc + 57, "#");		
//o	
		mvprintw(y, xc + 61, "#");
		mvprintw(y + 1, xc + 61, "#");
		mvprintw(y + 2, xc + 61, "#");
		mvprintw(y + 3, xc + 61, "#");
		mvprintw(y + 4, xc + 61, "#");
		mvprintw(y + 4 , xc + 61 , "####");
		mvprintw(y, xc + 65, "#");
		mvprintw(y + 1, xc + 65, "#");
		mvprintw(y + 2, xc + 65, "#");
		mvprintw(y + 3, xc + 65, "#");
		mvprintw(y + 4, xc + 65, "#");
		mvprintw(y , xc + 61 , "####");		
//o							
		mvprintw(y, xc + 67, "#");
		mvprintw(y + 1, xc + 67, "#");
		mvprintw(y + 2, xc + 67, "#");
		mvprintw(y + 3, xc + 67, "#");
		mvprintw(y + 4, xc + 67, "#");
		mvprintw(y + 4 , xc + 67 , "####");
		mvprintw(y, xc + 71, "#");
		mvprintw(y + 1, xc + 71, "#");
		mvprintw(y + 2, xc + 71, "#");
		mvprintw(y + 3, xc + 71, "#");
		mvprintw(y + 4, xc + 71, "#");
		mvprintw(y , xc + 67 , "####");	
//t	
		mvprintw(y , xc + 73 , "#####");
		mvprintw(y, xc + 75, "#");
		mvprintw(y + 1, xc + 75, "#");
		mvprintw(y + 2, xc + 75, "#");
		mvprintw(y + 3, xc + 75, "#");
		mvprintw(y + 4, xc + 75, "#");
//e
		mvprintw(y, xc + 79, "####");
		mvprintw(y + 1, xc + 79, "#");
		mvprintw(y + 2, xc + 79, "####");
		mvprintw(y + 3, xc + 79, "#");
		mvprintw(y + 4, xc + 79, "####");
	
//r		
		mvprintw(y, xc + 84, "####");
		mvprintw(y + 1, xc + 84, "#");
		mvprintw(y + 2, xc + 84, "####");
		mvprintw(y + 3, xc + 84, "#");
		mvprintw(y + 4, xc + 84, "#");
		mvprintw(y + 1, xc + 87, "#");
		mvprintw(y + 4, xc + 87, "#");
		mvprintw(y + 3, xc + 86, "#");

		refresh();
		usleep(35000);// Shorter delay between movements
		xc++;   
	}
	xc--;
	attroff(COLOR_PAIR(4));
	refresh();
	post_menu(my_menu);
	wrefresh(gamewin);
	wrefresh(top);	
	while((c = wgetch(gamewin)) != KEY_F(1)) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break ;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break ;
			case 10:
			{
				unpost_menu(my_menu);
				endwin();
				ITEM *cur;
				void (*p) (char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				pos_menu_cursor(my_menu);
				clear();
				endwin();
				refresh();
				break;
			}
			refresh();
			break;
		}
		clear();
		refresh();
		endwin();
		initscr();
		init_pair(4, COLOR_GREEN, COLOR_BLACK);
		bkgd(COLOR_PAIR(4));
		attrset(COLOR_PAIR(4));
		post_menu(my_menu);
		box(gamewin, 0, 0);
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 2, 2, "Press F1 to EXIT");
		attroff(COLOR_PAIR(4));
		int rcol = randr(5,10);
		attron(COLOR_PAIR(rcol));	
//s		
		mvprintw(y, xc, "####");
		mvprintw(y + 1, xc, "#");
		mvprintw(y + 2, xc, "####");
		mvprintw(y + 3, xc + 3, "#");
		mvprintw(y + 4, xc, "####");
//e		
		mvprintw(y, xc + 5, "####");
		mvprintw(y + 1, xc + 5, "#");
		mvprintw(y + 2, xc + 5, "####");
		mvprintw(y + 3, xc + 5, "#");
		mvprintw(y + 4, xc + 5, "####");					
		mvprintw(y, xc + 10, "####");

//g
		mvprintw(y + 1, xc + 10, "#");
		mvprintw(y + 2, xc + 10, "#");
		mvprintw(y + 3, xc + 10, "#");
		mvprintw(y + 4, xc + 10, "####");
		mvprintw(y + 3, xc + 10 + 3, "#");
		mvprintw(y + 2, xc + 10 + 3, "#");
		mvprintw(y + 2, xc + 10 + 2, "#");	
//f							
		mvprintw(y, xc + 18, "#");
		mvprintw(y + 1, xc + 18, "#");
		mvprintw(y + 2, xc + 18, "#");
		mvprintw(y + 3, xc + 18, "#");
		mvprintw(y + 4, xc + 18, "#");
		mvprintw(y , xc + 18 + 1, "####");
		mvprintw(y + 2 , xc + 18 + 1, "###");
//a
		mvprintw(y, xc + 24, "#");
		mvprintw(y + 1, xc + 24, "#");
		mvprintw(y + 2, xc + 24, "#");
		mvprintw(y + 3, xc + 24, "#");
		mvprintw(y + 4, xc + 24, "#");
		mvprintw(y , xc + 25 , "####");
		mvprintw(y, xc + 255, "#");
		mvprintw(y + 1, xc + 25 + 3, "#");
		mvprintw(y + 2, xc + 25 + 3, "#");
		mvprintw(y + 3, xc + 25 + 3, "#");
		mvprintw(y + 4, xc + 25 + 3, "#");
		mvprintw(y + 2, xc + 25 + 1, "#");
//u
		mvprintw(y, xc + 30, "#");
		mvprintw(y + 1, xc + 30, "#");
		mvprintw(y + 2, xc + 30, "#");
		mvprintw(y + 3, xc + 30, "#");
		mvprintw(y + 4, xc + 30, "#");
		mvprintw(y + 4 , xc + 31 , "####");
		mvprintw(y + 1, xc + 31 + 3, "#");
		mvprintw(y + 2, xc + 31 + 3, "#");
		mvprintw(y + 3, xc + 31 + 3, "#");
		mvprintw(y + 4, xc + 31 + 3, "#");
		mvprintw(y , xc + 31 + 3, "#");
//l
		mvprintw(y, xc + 30 + 6, "#");
		mvprintw(y + 1, xc + 36, "#");
		mvprintw(y + 2, xc + 36, "#");
		mvprintw(y + 3, xc + 36, "#");
		mvprintw(y + 4, xc + 36, "#");
		mvprintw(y + 4 , xc + 37 , "####");
//t		
		mvprintw(y , xc + 41 , "#####");
		mvprintw(y, xc + 43, "#");
		mvprintw(y + 1, xc + 43, "#");
		mvprintw(y + 2, xc + 43, "#");
		mvprintw(y + 3, xc + 43, "#");
		mvprintw(y + 4, xc + 43, "#");
//s	
		mvprintw(y, xc + 50, "####");
		mvprintw(y + 1, xc + 50, "#");
		mvprintw(y + 2, xc + 50, "####");
		mvprintw(y + 3, xc + 53, "#");
		mvprintw(y + 4, xc + 50, "####");

//h
		mvprintw(y, xc + 55, "#");
		mvprintw(y + 1, xc + 55, "#");
		mvprintw(y + 2, xc + 55, "#");
		mvprintw(y + 3, xc + 55, "#");
		mvprintw(y + 4, xc + 55, "#");		
		mvprintw(y, xc + 59, "#");
		mvprintw(y + 1, xc + 59, "#");
		mvprintw(y + 2, xc + 59, "#");
		mvprintw(y + 3, xc + 59, "#");
		mvprintw(y + 4, xc + 59, "#");
		mvprintw(y + 2 , xc + 57, "#");		
//o	
		mvprintw(y, xc + 61, "#");
		mvprintw(y + 1, xc + 61, "#");
		mvprintw(y + 2, xc + 61, "#");
		mvprintw(y + 3, xc + 61, "#");
		mvprintw(y + 4, xc + 61, "#");
		mvprintw(y + 4 , xc + 61 , "####");
		mvprintw(y, xc + 65, "#");
		mvprintw(y + 1, xc + 65, "#");
		mvprintw(y + 2, xc + 65, "#");
		mvprintw(y + 3, xc + 65, "#");
		mvprintw(y + 4, xc + 65, "#");
		mvprintw(y , xc + 61 , "####");		
//o							
		mvprintw(y, xc + 67, "#");
		mvprintw(y + 1, xc + 67, "#");
		mvprintw(y + 2, xc + 67, "#");
		mvprintw(y + 3, xc + 67, "#");
		mvprintw(y + 4, xc + 67, "#");
		mvprintw(y + 4 , xc + 67 , "####");
		mvprintw(y, xc + 71, "#");
		mvprintw(y + 1, xc + 71, "#");
		mvprintw(y + 2, xc + 71, "#");
		mvprintw(y + 3, xc + 71, "#");
		mvprintw(y + 4, xc + 71, "#");
		mvprintw(y , xc + 67 , "####");	
//t	
		mvprintw(y , xc + 73 , "#####");
		mvprintw(y, xc + 75, "#");
		mvprintw(y + 1, xc + 75, "#");
		mvprintw(y + 2, xc + 75, "#");
		mvprintw(y + 3, xc + 75, "#");
		mvprintw(y + 4, xc + 75, "#");
//e
		mvprintw(y, xc + 79, "####");
		mvprintw(y + 1, xc + 79, "#");
		mvprintw(y + 2, xc + 79, "####");
		mvprintw(y + 3, xc + 79, "#");
		mvprintw(y + 4, xc + 79, "####");
	
//r		
		mvprintw(y, xc + 84, "####");
		mvprintw(y + 1, xc + 84, "#");
		mvprintw(y + 2, xc + 84, "####");
		mvprintw(y + 3, xc + 84, "#");
		mvprintw(y + 4, xc + 84, "#");
		mvprintw(y + 1, xc + 87, "#");
		mvprintw(y + 4, xc + 87, "#");
		mvprintw(y + 3, xc + 86, "#");


		attroff(COLOR_PAIR(4));
		refresh();
		wrefresh(gamewin);
	}
	unpost_menu(my_menu);
	for(i = 0; i < n_choices; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

/*Shows a window having the stored top 10 highscores*/
void showhighscore() {
	clear();
	refresh();
	char c;
	start_color();
	init_pair(2, COLOR_BLACK, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	WINDOW *ins = newwin(40,150,0,0);
	wbkgd(ins, COLOR_PAIR(4));
	mvprintw(LINES - 4, 2, "q to go back");
	wrefresh(ins);	
	box(ins,0,0);
	wrefresh(ins);
	highscores();
	wrefresh(ins);
	while((c = getch()) != 'q') {
	}
	clear();
	refresh();		
}
/*Opens the settings menu*/
void settings() {
	clear();
	refresh();
	curs_set(0);
	ITEM **my_items;
	int c;
	WINDOW *gamewin;
	MENU *my_menu;
	int n_sett = 6, i;
	ITEM *cur_item;
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	my_items = (ITEM **)calloc(n_sett + 1, sizeof(ITEM *));
	mvwprintw(stdscr, LINES - 2, 2, "Press F1 to EXIT");
	for(i = 0; i < n_sett; ++i)
		my_items[i] = new_item(sett[i], NULL);
	my_items[n_sett] = (ITEM *)NULL;
	set_item_userptr(my_items[0], difficulty);
	set_item_userptr(my_items[1], twoplayer);
	set_item_userptr(my_items[2], godmod);
	set_item_userptr(my_items[3], survive);
	set_item_userptr(my_items[4], plives);
	set_item_userptr(my_items[5], clearscores);
	my_menu = new_menu((ITEM **)my_items);
	gamewin = newwin(20, 60, (LINES - 20) / 2, (COLS - 60) / 2);
	keypad(gamewin, TRUE);
	set_menu_win(my_menu, gamewin);
	set_menu_sub(my_menu, derwin(gamewin, 6, 58, 3, 1));
	box(gamewin, 0, 0);
	print_in_middle(gamewin, 1, 0, 58, " MAIN MENU", COLOR_PAIR(1));
	mvwaddch(gamewin, 2, 0, ACS_LTEE);
	mvwhline(gamewin, 2, 1, ACS_HLINE, 58);
	mvwaddch(gamewin, 2, 60, ACS_RTEE);
	refresh();
	clear();
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 2, "q to go back");
	attroff(COLOR_PAIR(4));
	refresh();
	post_menu(my_menu);
	wrefresh(gamewin);
	while((c = wgetch(gamewin)) != 'q') {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break ;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break ;
			case 10:
			{	
				unpost_menu(my_menu);
				endwin();
				ITEM *cur;
				void (*p) (char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				refresh();
				pos_menu_cursor(my_menu);
				refresh();
				clear();
				endwin();
				refresh();
				init_pair(1, COLOR_GREEN, COLOR_BLACK);
				break;
			}
			break;
		}
		mvprintw(10, 10, "CLEARED SCORE");
		refresh();
		clear();
		refresh();
		endwin();
		initscr();
		init_pair(3, COLOR_GREEN, COLOR_BLACK);
		bkgd(COLOR_PAIR(4));
		attrset(COLOR_PAIR(4));
		post_menu(my_menu);
		box(gamewin, 0, 0);
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 2, 2, "q to go back");
		attroff(COLOR_PAIR(4));
		refresh();
		wrefresh(gamewin);
	}
	
	unpost_menu(my_menu);
	for(i = 0; i < n_sett; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();

}

/*Opens the difficulty menu*/
void difficulty() {
	clear();
	refresh();
	curs_set(0);
	ITEM **my_items;
	int c;
	WINDOW *gamewin;
	MENU *my_menu;
	int n_diff = 3, i;
	ITEM *cur_item;
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	attrset(COLOR_PAIR(1));
	my_items = (ITEM **)calloc(n_diff + 1, sizeof(ITEM *));
	mvprintw(LINES - 2, 2, "q to go back");
	for(i = 0; i < n_diff; ++i)
		my_items[i] = new_item(diff[i], NULL);
	my_items[n_diff] = (ITEM *)NULL;
	set_item_userptr(my_items[0], beginner);
	set_item_userptr(my_items[1], inter);
	set_item_userptr(my_items[2], pro);
	my_menu = new_menu((ITEM **)my_items);
	gamewin = newwin(20, 60, (LINES - 20) / 2, (COLS - 60) / 2);
	keypad(gamewin, TRUE);
	set_menu_win(my_menu, gamewin);
	set_menu_sub(my_menu, derwin(gamewin, 6, 58, 3, 1));
	box(gamewin, 0, 0);
	print_in_middle(gamewin, 1, 0, 58, " MAIN MENU", COLOR_PAIR(4));
	mvwaddch(gamewin, 2, 0, ACS_LTEE);
	mvwhline(gamewin, 2, 1, ACS_HLINE, 58);
	mvwaddch(gamewin, 2, 60, ACS_RTEE);
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 2, "q to go back");
	attroff(COLOR_PAIR(4));
	refresh();
	post_menu(my_menu);
	wrefresh(gamewin);
	while((c = wgetch(gamewin)) != 'q') {
		switch(c) {
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break ;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break ;
			case 10:
			{	unpost_menu(my_menu);
				endwin();
				ITEM *cur;
				void (*p)(char *);
				cur = current_item(my_menu);
				p = item_userptr(cur);
				p((char *)item_name(cur));
				pos_menu_cursor(my_menu);
				clear();
				endwin();
				refresh();
				init_pair(1, COLOR_GREEN, COLOR_BLACK);
				break;
			}
			break;
		}
		clear();
		refresh();
		endwin();
		initscr();
		init_pair(3, COLOR_GREEN, COLOR_BLACK);
		bkgd(COLOR_PAIR(4));
		attrset(COLOR_PAIR(4));
		post_menu(my_menu);
		box(gamewin, 0, 0);
		attron(COLOR_PAIR(4));
		mvprintw(LINES - 2, 2, "q to go back");
		attroff(COLOR_PAIR(4));
		refresh();
		wrefresh(gamewin);
	}
	unpost_menu(my_menu);
	for(i = 0; i < n_diff; i++)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

/*A function which allows user to set his lives manually*/
 
void plives() {
	clear();
	refresh();
	char c, v;
	start_color();
	init_pair(2, COLOR_YELLOW , COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_GREEN);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	noecho();
	curs_set(FALSE);
	WINDOW *top = newwin(LINES - (LINES - LINES / 8), COLS,0, 0);
	box(top, 0, 0);
	wbkgd(top, COLOR_PAIR(2));
	mvwprintw(top, (LINES - (LINES - LINES / 8)) / 2 , COLS / 2 - 8, "SEGFAULT SHOOTER");
	wrefresh(top);
	WINDOW *mainwin = newwin(LINES - LINES / 8 , COLS, 5, 0);
	box(mainwin, 0, 0);
	wbkgd(mainwin, COLOR_PAIR(1));
	mvwprintw(mainwin, LINES - 10, COLS - (COLS - 10) , "Press q to go back");
	mvwprintw(mainwin, LINES / 2 - 11, COLS/2 -40, "ENTER THE LIVES YOU WANT (LESS THAN 100)");
	curs_set(TRUE);
	wrefresh(mainwin);
	scanf("%d", &l);
	curs_set(FALSE);
	mvwprintw(mainwin, LINES / 2 - 9, 35, "Lives have been changed to :%d", l);
	wrefresh(mainwin);	
	mvwprintw(mainwin, LINES / 2 - 7, 35, "SUCCESS");
	mvwprintw(mainwin, LINES / 2 - 5, 35, "Press ' s ' to start game or ' q ' to go back to settings");
	wrefresh(mainwin);
	wrefresh(top);
	if((c = getch()) == 's') {
		clear();
		refresh();
		mainn();
	}
	else {
		while(c != 'q') {
		}
		clear();
		refresh();
	}
}

/*A function to print in the middle*/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color){
	int length, x, y;
	float temp;
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;
	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

/*Enter into godmode*/
void godmod() {
	refresh();
	clear();
	flagdif = 99;
	mainn();
	flagdif = 1;
}

/*Returns the value of l a/c to which the lives in different game modes are selected... eg. l = 1 is for beginners */
int getlives() {
	return l;
}

/*Starts the game in beginners mode*/
void beginner(){
	clear();
	refresh();
	flagdif = 1;
	mainn();
	clear();
	refresh();
}
/*Starts the game in Expert mode*/
void pro(){
	flagdif = 3;	
	mainn();
}
/*Starts the game in Intermeiate mode*/
void inter() {
	flagdif = 2;
	mainn();

}

/*Returns the value of flagdif a/c to which the game modes are selected... eg. flagdif = 1 is for beginners */
int senddiff(){
	return flagdif;
}

/*A function to display the About Me Window*/
void about() {
	clear();
	initscr();
	refresh();
	char c;
	start_color();
	init_pair(2, COLOR_YELLOW , COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	noecho();
	curs_set(FALSE);
	WINDOW *top = newwin(LINES - (LINES - LINES / 8), COLS,0, 0);
	box(top, 0, 0);
	wbkgd(top, COLOR_PAIR(2));
	mvwprintw(top, (LINES - (LINES - LINES / 8)) / 2 , COLS / 2 - 8, "SEGFAULT SHOOTER");
	wrefresh(top);
	WINDOW *mainwin = newwin(LINES - LINES / 8 , COLS, LINES - (LINES - LINES / 8), 0);
	box(mainwin, '*', '*');
	wbkgd(mainwin, COLOR_PAIR(3));
	mvwprintw(mainwin, LINES / 2 - 11, COLS / 2 - 25, "This game has been developed by Shayan Anwar");
	mvwprintw(mainwin, LINES / 2 - 9, COLS / 2 - 25, "SY B.Tech");
	mvwprintw(mainwin, LINES / 2 - 7, COLS / 2 - 25, "111403073");
	mvwprintw(mainwin, LINES / 2 - 5, COLS / 2 - 25, "9503623940");
	mvwprintw(mainwin, LINES / 2 - 3, COLS / 2 - 25, "Please contact me if you find any bugs");
	mvwprintw(mainwin, LINES - 10, COLS - (COLS - 10), "Press q to go back");
	wrefresh(mainwin);
	wrefresh(top);
	while((c = getch()) != 'q') {
	}
	clear();
	refresh();
}

void endgame() {
	clear();
	endwin();
	exit(0);
}

/*A function to display the instruction window*/
void instruct() {
	clear();
	initscr();
	refresh();
	char c;
	start_color();
	init_pair(2, COLOR_YELLOW , COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	noecho();
	curs_set(FALSE);
	WINDOW *top = newwin(LINES - (LINES - LINES / 8), COLS,0, 0);
	box(top, 0, 0);
	wbkgd(top, COLOR_PAIR(2));
	mvwprintw(top, (LINES - (LINES - LINES / 8)) / 2 , COLS / 2 - 8, "SEGFAULT SHOOTER");
	wrefresh(top);
	WINDOW *mainwin = newwin(LINES - LINES / 8 , COLS, 5, 0);
	box(mainwin, '*', '*');
	wbkgd(mainwin, COLOR_PAIR(3));
	mvwprintw(mainwin, LINES / 2 - 11, COLS/2 - 40, "SEGFAULT SHOOTER IS A SIMPLE SHOOTING GAME. THERE IS A MACHINE WHICH FIRES BULLET.");
	mvwprintw(mainwin, LINES / 2 - 9, 35, "YOU CAN USE THE ARROW KEYS TO MOVE IT UP AND DOWN.TO FIRE YOU NEED TO PRESS OR HOLD SPACE BAR.");
	mvwprintw(mainwin, LINES / 2 - 7, 35, "YOUR TARGET IS TO KILL AS MANY SEGAFULTS AS YOU CAN BEFORE YOUR LIFE BECOMES ZERO.");
	mvwprintw(mainwin, LINES / 2 - 5, 35, "YOU HAVE CERTAIN LIVES INITIALLY, WHICH REDUCES IF ANY OF THE SEGFAULTS COLLIDES YOU OR YOUR WALL.");
	mvwprintw(mainwin, LINES / 2 - 3, 35, "YOU LOSE THE GAME WHEN LIFE BECOMES ZERO.YOU CAN ALSO INCREASE OR DECREASE PLAYER'S LIFE.");
	mvwprintw(mainwin, LINES / 2 - 1, 35, "DONT FORGET TO GO THROUGH THE VARIOUS MODES LIKE GOD MODE, 2 PLAYER MODE ETC ");
	mvwprintw(mainwin, LINES / 2 + 1, 35, "SO WHAT ARE YOU WAITING FOR, KILL THE SEGFAULTS BEFORE THEY INVADE YOUR CODE !!");
	mvwprintw(mainwin, LINES - 10, COLS - (COLS - 10), "Press q to go back");
	wrefresh(mainwin);
	wrefresh(top);
	while((c = getch()) != 'q') {
	}
	clear();
	refresh();
}

/*Starts the game in Survival mode*/
void survive() {
	flagdif = 99;
	flagsurvive = 1;
	mainn();
	flagdif = 1;
	flagsurvive = 0;
}

/*Returns a val a/c to whcih the survival mode is started*/
int ismodesurvive() {
	return flagsurvive;
}
