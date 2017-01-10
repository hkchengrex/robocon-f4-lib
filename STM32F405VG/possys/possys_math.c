//All the math stuff needed by possys
//Migrate so that approx math is not needed anymore

#include "possys_math.h"

#define Sqr(x) (x*x)

s16 tanval[91] = {		//scale 100
0,2,3,5,7,9,11,12,14,16,18,19,21,23,25,27,29,31,32,34,36,38,40,42,
45,47,49,51,53,55,58,60,62,65,67,70,73,75,78,81,84,87,90,93,97,100,104,107,
111,115,119,123,128,133,138,143,148,154,160,166,173,180,188,196,205,214,225,236,248,261,275,290,
308,327,349,373,401,433,470,514,567,631,712,814,951,1143,1430,1908,2864,5729,-1};

u16 possys_pythag(COORD3 *p1, COORD3 *p2) {
	return Sqrt(Sqr(p2->y - p1->y) + Sqr(p2->x - p1->x));
}

u16 possys_mod(s16 x, u16 m) {
	return (x%m + m)%m;
}

u16 possys_abs(s16 x) {
	return (x < 0) ? -x : x;
}

u16 possys_arctan(COORD3 *p1, COORD3 *p2) {
	s16 x, y, d;
	x = p2->x - p1->x;
	y = p2->y - p1->y;
	d = 0;
	if (!y)
		return (x > 0) ? 900 : ((x < 0) ? 2700 : 0);
	
	s16 ratio = x*100/y;
	while(d < 90) {
		if (tanval[d] > possys_abs(ratio)) break; 
		d++;
	}
	d--;
	d*=(ratio/possys_abs(ratio)*10);
	
	if (y < 0) d+=1800;
	else if (x < 0) d+=3600;
	return d;
}
