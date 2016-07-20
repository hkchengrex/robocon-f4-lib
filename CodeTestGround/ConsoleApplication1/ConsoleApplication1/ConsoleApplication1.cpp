// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <stdint.h>
#include <iostream>
#include <stdio.h>

int32_t tan_table[512];

#define PI 3.141592653589793238463

void table_init() {
	for (int i = 0; i < 512; i++) {
		tan_table[i] = (tan(PI*i / 512.0 / 2.0)* 4096.0 + 0.5);
	}
}

//0~8999
int32_t guess_tan(int32_t in) {
	uint8_t ini_index = in * 512 / 9000;
	int16_t weight = in*100 - (ini_index * 9000 *100 / 512);
	printf("%d %d %d\n", tan_table[ini_index], tan_table[ini_index+1], weight);
	return tan_table[ini_index] + (tan_table[ini_index + 1] - tan_table[ini_index])*weight * 512 / 9000 /100;
}

int32_t app_tan(int32_t in) {
	while (in > 18000) {
		in -= 18000;
	}
	while (in < 0) {
		in += 18000;
	}

	if (in < 9000) {
		return guess_tan(in);
	}
	else {
		return -guess_tan(18000 - in);
	}
}


int main(){
	table_init();
	FILE *fp;
	fopen_s(&fp, "result.txt", "w");

	for (int i = 0; i < 512; i++) {
		printf("%d\n", tan_table[i]);
		fprintf(fp, "%d, ", tan_table[i]);
	}

	#define test 17512
	printf("%d\n", app_tan(test));
	printf("%f %f\n", app_tan(test)/ 4096.0, tan(test*PI/180.0/100.0));
	printf("%f", (app_tan(test) - tan(test*PI / 180.0 / 100.0)*4096.0) / 4096.0 * 1000);

	fclose(fp);

	scanf_s("");
    return 0;
}

