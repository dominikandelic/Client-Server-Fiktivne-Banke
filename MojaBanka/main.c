#include "functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(void) {

	srand((unsigned)time(NULL));

	const char* nazivDatoteke = "data.bin";

	izbornik(nazivDatoteke);
}