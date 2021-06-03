#include "functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")


int main(void) {

	srand((unsigned)time(NULL));
	//stvoriKonekciju();

	const char* nazivDatoteke = "data.bin";

	izbornik(nazivDatoteke);
}