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

#include <unistd.h>
#include <linux/kd.h> //for keyboard i/p
#include <termios.h>  //contains the definitions used by the terminal I/O interfaces
#include <fcntl.h>
/*inside usr/include/linux there is a file called input.h which contains the #define values of all the keys on keyborad*/
char *keyboard = "/dev/input/event11";
int rawinput() {
	int fd, rd, value;
	char name[256] = "Unknown";
	char *device = NULL;  
	device = keyboard;
	if ((fd = open(device, O_RDONLY | O_NONBLOCK)) == -1)
		return 1;
	struct input_event ev[64];
	int size = sizeof(struct input_event);
	while (running) {
		rd = read (fd, ev, size * 64);
		switch (ev[0].code) {
/*16 : q */		case 16:
				running = 0;
				break;
/*57 : space */		case 57:
				shootd = 1;
				break;
/*103 : arrow up */	case 103:
				upd = 1;
				break;
/*108 : arrow down */	case 108:
				downd = 1;
				break;
		}

		switch (ev[1].code) {
			case 16:
				running = 0;
				break;
			case 57:
				shootd = 0;
				break;
			case 103:
				upd = 0;
				break;
			case 108:
				downd = 0;
				break;
		}
	}

	return 0;
}

/*below function decides what is to be done when user presses a key during the game run*/
void curses_input() {
	int d;
	while (running) {
		d = getch();
		if (d == 'q')
			running = 0;
		if (d == KEY_DOWN)
			down();
		if (d == KEY_UP)	
			up();
		if (d == ' ')
			addbullet();
		if(is2player())
			if (d == 'w' || d == 'W')
				up1();
		if(is2player())		
			if (d == 's' || d == 'S')
				down1();
		if(is2player()) 
			if (d == 'f' || d == 'F')
				addbulletleft();
	}
}

void *input() {
	curses_input();//This Simply ennables taking i/p from the keyboard jab game shuru hota hai
}	
