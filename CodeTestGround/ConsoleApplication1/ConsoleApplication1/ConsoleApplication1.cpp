// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <stdint.h>
#include <iostream>

int32_t tan_table[256];

#define PI 3.141592653589793238463

void table_init() {
	for (int i = 0; i < 256; i++) {
		tan_table[i] = (tan(PI*i / 256.0 / 2.0)*4096.0 + 0.5);
	}
}

//0~8999
int32_t guess_tan(int32_t in) {
	uint8_t ini_index = in * 256 / 9000;
	int16_t weight = in*100 - (ini_index * 9000 *100 / 256);
	printf("%d %d %d\n", ini_index, weight, tan_table[ini_index]);
	return tan_table[ini_index] + (tan_table[ini_index + 1] - tan_table[ini_index])*weight *256 / 9000 /100;
}

int main(){
	table_init();
	for (int i = 0; i < 256; i++) {
		printf("%d\n", tan_table[i]);
	}
	#define test 6504
	printf("%d\n", guess_tan(test));
	printf("%f %f\n", guess_tan(test)/ 4096.0, tan(test*PI/180.0/100.0));
	printf("%f", (guess_tan(test) - tan(test*PI / 180.0 / 100.0)*4096.0) / 4096.0 * 1000);
	scanf_s("");
    return 0;
}

