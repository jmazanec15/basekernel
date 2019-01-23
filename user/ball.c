/*
Copyright (C) 2017 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

/*
	This program is a simple graphics demonstration that shows the text
	"basekernel" bouncing around the screen and changing colors. It is an
	excellent example of how to use syscall_object_size to get the dimensions
	of the screen and use them to create a flexible graphic. It also shows how to
	create graphics inside a window. Last but not least, it could potentially
	function as a cool screen saver.
*/

#include "library/syscalls.h"
#include "library/user-io.h"
#include "library/string.h"

typedef unsigned int uint32_t;

/* Function Declarations */
uint32_t randint(uint32_t min, uint32_t max);
void move(int *x, int *d, int min, int max);

int main(int argc, char *argv[])
{
	/* Initialize variables */
	int r = 255;
	int g = 0;
	int b = 0;
	int x1 = 12;
	int y1 = 12;
	int dx1 = 4;
	int dy1 = 1;
	int dr = -1;
	int dg = 2;
	int db = 3;
	int dims[2], width, height;

	/*
		Get the dimensions of the standard window. The syscall takes in the
		file descriptor of the object you want to get the dimensions of, an array
		of integers to be filled with dimensions and the number of dimensions you
		expect to get back (2, Width and Height)
	*/
	syscall_object_size(KNO_STDWIN, dims, 2);
	width = dims[0];
	height = dims[1];

	/* Set up the window */
	draw_window(KNO_STDWIN); // sets the window to be written to
	draw_clear(0, 0, width, height); // clears all pixels to black
	draw_flush(); // flush all changes to the screen

	char stop = -1; // used to quit the program
	while(stop == -1) {
		draw_window(KNO_STDWIN);
		move(&x1, &dx1, 0, width - 80);
		move(&y1, &dy1, 0, height - 1);
		move(&r, &dr, 0, 255);
		move(&g, &dg, 0, 255);
		move(&b, &db, 0, 255);
		draw_color(r, g, b); // set the color of the pixels to be drawn
		draw_string(x1, y1, "basekernel");
		draw_flush();

		syscall_process_sleep(75); // sleeps for 75 ms and yields the cpu
		syscall_object_read_nonblock(KNO_STDIN,&stop,1); // see if user wants out
	}

	/* Clean up */
	draw_clear(0, 0, width, height);
	draw_color(255, 255, 255);
	draw_flush();
	return 0;
}

/* Helper functions */
uint32_t randint(uint32_t min, uint32_t max)
{
	/* Generates random integer */
	static uint32_t state = 0xF3DC1A24;
	state = (state * 1299721) + 29443;
	return min + ((state >> 16) % (max - min + 1));
}

void move(int *x, int *d, int min, int max)
{
	/* Move the positions of the x and d coords */
	*x += *d;
	if(*x < min) {
		*x = min;
		*d = randint(1, 10);
	}
	if(*x > max) {
		*x = max;
		*d = -randint(1, 10);
	}
}
