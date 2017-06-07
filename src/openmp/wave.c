/**
 * @file add.c
 *
 * This is the main driver of the program, i.e.,
 * the program, which is then used by the user.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "core.h"
#include "gui.h"

int main(int argc, char **argv)
{
	//evaluate program arguments
	bool nogui = false;
	if (argc > 1 && strcmp(argv[1], "--nogui") == 0)
	{
		nogui = true;
	}

	//TODO: evaluate config file

	//seq init
	init();

	if (nogui){
		output();
	} else {
		init_gui();
	}

	return EXIT_SUCCESS;
}
