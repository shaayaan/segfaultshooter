#include <unistd.h>
#include <linux/kd.h>
#include <termios.h>
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
			case 16:
				running = 0;
				break;
			case 57:
				shootd = 1;
				break;
			case 103:
				upd = 1;
				break;
			case 108:
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
		if(d == 'p') {
			gamepause(d);
		}
	}
}
void *input() {
	curses_input();//This Simply ennables taking i/p from the keyboard jab game shuru hota hai
}	
