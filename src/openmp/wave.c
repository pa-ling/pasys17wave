/**
 * @file add.c
 *
 * This is the main driver of the program, i.e.,
 * the program, which is then used by the user.
 */
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "gui.h"

int main(int argc, char **argv)
{

	//seq init
	init();
	
	simulate();
	
	output();

	simulate();

	output();

	draw(getNewValues(), getArraySize());

	return EXIT_SUCCESS;
}
