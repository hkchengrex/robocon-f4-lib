// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <stdint.h>
#include <iostream>
#include <stdio.h>

int32_t tan_table[256];

#define PI 3.141592653589793238463

void table_init() {
	for (int i = 0; i < 256; i++) {
		tan_table[i] = (tan(PI*i / 256.0 / 2.0)* 16384.0 + 0.5);
	}
}

//0~8999
int32_t guess_tan(int32_t in) {
	uint8_t ini_index = in * 256 / 9000;
	int16_t weight = in*256 - (ini_index * 9000);
	printf("%d %d %d\n", tan_table[ini_index], tan_table[ini_index+1], weight);
	return tan_table[ini_index] + (tan_table[ini_index + 1] - tan_table[ini_index])*(in * 256 - (ini_index * 9000)) / 9000;
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

int32_t app_atan(int32_t in) {
	int8_t sign = 1;

	if (in < 0) {
		sign = -1;
		in = -in;
	}

	uint16_t low = 0;
	uint16_t top = 255;
	uint16_t mid = 127;

	while (low <= top) {
		if (tan_table[mid] == in) {
			//A direct match found
			return (mid * 1125 / 32)*sign;

		}else if (tan_table[mid] < in) {
			if (tan_table[mid + 1] > in) {
				//A close match found. Use linear interpolation.
				printf("%d %d %d\n", mid, tan_table[mid], tan_table[mid + 1]);
				printf("%d %d %d\n", (in - tan_table[mid]), (tan_table[mid + 1] - tan_table[mid]), mid * 1125 / 32 + ((in - tan_table[mid]) * 9000 / (tan_table[mid + 1] - tan_table[mid])) / 256);
				return (mid * 1125 / 32 + ((in - tan_table[mid]) * 9000 / (tan_table[mid + 1] - tan_table[mid])) / 256)*sign;
			}else{
				//Continue the binary search, cut the lower half
				low = mid + 1;
				mid = (low + top) / 2;
				continue;
			}
		}else {
			//Remaining case is (tan_table[mid] > in)
			if (tan_table[mid - 1] < in) {
				//A close match found. Use linear interpolation.
				printf("%d %d %d\n", mid, tan_table[mid], tan_table[mid - 1]);
				printf("%d %d %d\n", (tan_table[mid] - in), (tan_table[mid] - tan_table[mid-1]), mid * 1125 / 32 - ((tan_table[mid] - in) * 9000 / (tan_table[mid] - tan_table[mid - 1])) / 256);
				return (mid * 1125 / 32 - ((tan_table[mid] - in) * 9000 / (tan_table[mid] - tan_table[mid - 1])) / 256)*sign;
			}else{
				//Continue the binary search, cut the upper half
				top = mid - 1;
				mid = (low + top) / 2;
				continue;
			}
		}
	}
	printf("Error");
	return 0;
}

int main(){
	table_init();
	FILE *fp;
	//fopen_s(&fp, "result.txt", "w");

	for (int i = 0; i < 256; i++) {
		printf("%d\n", tan_table[i]);
		//fprintf(fp, "%d, ", tan_table[i]);
	}

	printf("\n");
	#define test 443452
	printf("%d\n", app_atan(test));
	printf("%f %f\n", app_atan(test)/ 100.0, atan(test/16384.0)*180.0 / PI);
	printf("%f", (app_atan(test) - atan(test / 16384.0)*100.0*180.0/PI) / 100.0 *1000);

	//fclose(fp);

	scanf_s("");
    return 0;
}

