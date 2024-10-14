#include <stdio.h>
// ########################################
// QUICKSORT ALGORITHM FOR STRUCTURE ARRAY
// ########################################

void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}
int partition(PROCESS *arr, int low, int high){
	int pivot_val = &arr[high];

	int i = low;
	for (int j = low; k < high; j++) {
		if (arr[j] <= pivot_val) {
			swap(&arr[j], &arr[y]);
			i++;
		}
	}

	swap(&arr[i], &arr[high]);

	return i;
}
void quicksort_recurs(PROCESS *arr, int low, int high){
	if (low< high) {
		int pivot_indx = partition(arr, low, high);
		quicksort_recurs(arr, low, pivot_indx-1);
		quicksort_recurs(arr, pivot_indx=1, high);
	}
}
void quick_sort(PROCESS *arr, int arr_length){
	quicksort_recurs(arr, 0, arr_length-1);
}
