#include "quick_math.h"

/** This function tries to cap a s16 within a specific range.
** The inline property tries to remove the function call and make it runs faster
**/
int16_t s16_cap(int16_t in_num, int16_t upper_bound, int16_t lower_bound){
	return in_num>upper_bound?upper_bound:in_num<lower_bound?lower_bound:in_num;
}


/** This function tries to cap a u16 within a specific range.
** The inline property tries to remove the function call and make it runs faster
**/
uint16_t u16_cap(uint16_t in_num, uint16_t upper_bound, uint16_t lower_bound){
	return in_num>upper_bound?upper_bound:in_num<lower_bound?lower_bound:in_num;
}

int32_t s32_sqrt(int32_t in_num){
	return (s32)(__sqrtf(in_num)*1024.0f + 0.5f);
}



/** A quick select algorithm from N. Wirth's "Algorithms + data structures = programs"
** It is used to find the kth smallest element in the array
** It can be used to find median (median filter), ~25 times faster than quicksort method
** Feel free to change ELEMENT_TYPE to implement versions for different variable type, as long as they are comparable
**
** @param list: Array of items to be searched.
** @param len: Length of array.
** @param k: which element to be searched (kth smallest)
** @param copy_array: whether this method should change the original array or not, if it is set to TRUE, 
** the array will be copied and the original array will NOT be modified as a side effect
*/
#define ELEMENT_TYPE int16_t
#define SWAP_ELEMENT(a,b) {ELEMENT_TYPE t=(a);(a)=(b);(b)=t;}
ELEMENT_TYPE s16_kth_smallest(ELEMENT_TYPE list[], u16 len, u16 k, bool copy_array){
	u16 i, j, l, m;
  ELEMENT_TYPE x;
	ELEMENT_TYPE a[len];
	
	if (copy_array){
		memcpy(a, list, len*sizeof(ELEMENT_TYPE));
		list = a;
	}
	
	l = 0; //Left
	m = len - 1; //Right
	while (l < m) {
		x = list[k]; //Element at k is always the pivot
		i = l;
		j = m;
		do {
				while (list[i] < x){
					i++; //Search for a item >= pivot on the left side
				}
				while (list[j] > x){
					j--; //Search for a item <= pivot on the right side
				}
				if (i<=j) {
					SWAP_ELEMENT(list[i],list[j]); //Swap these "improperly" placed items
					i++;
					j--;
				}
		} while (i <= j);
		//Select the "unarranged" portion
		if (j < k){
			//if j<k, this means all elements with index > k are larger than the current pivot
			//and the current pivot should already been swapped into the left region of k
			l = i;
		}
		if (i > k){
			m = j;
		}
	}
	return list[k];
}
#undef SWAP_ELEMENT
#undef ELEMENT_TYPE
