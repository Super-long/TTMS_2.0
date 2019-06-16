#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/io.h>
//#include "./TTMSReal/TTMS_SCU_Play_UI/Play_UI_MgEnt.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);

	Main_Menu();
	return EXIT_SUCCESS;
}
