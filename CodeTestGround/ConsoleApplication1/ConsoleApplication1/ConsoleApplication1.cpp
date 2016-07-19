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
		tan_table[i] = (tan(PI*i / 256.0 / 2.0)*1024 + 0.5);
	}
}

//0~8999
int32_t guess_tan(int32_t in) {
	uint8_t ini_index = in * 256 / 9000;
	int16_t est_in = ini_index * 9000 / 256;
	int16_t fract = in - est_in;
	int16_t fract_sq = fract*fract;
	int16_t fract_cube = fract_sq*fract;
	printf("%d %d %d\n", ini_index, est_in, fract);
}

int main(){
	table_init();
	for (int i = 0; i < 256; i++) {
		printf("%d\n", tan_table[i]);
	}
	guess_tan(50);
	scanf_s("");
    return 0;
}

