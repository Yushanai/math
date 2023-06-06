#include "aggregate.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static double _min(double* arr, int size);
static double _max(double* arr, int size);
static double _count(double* arr, int size);
static double _sum(double* arr, int size);
static double _avg(double* arr, int size);
static double _pavg(double* arr, int size);

typedef double (*aggregate_fuc)(double*, int);
static const aggregate_fuc farray[] = {&_min, &_max, &_count, &_sum, &_avg, &_pavg};
static const char* funcname[] = {"MIN", "MAX", "COUNT", "SUM", "AVG", "PAVG"};

double aggregate (const char* func, double* arr, int size){
	if (arr == NULL || size < 1){
		printf("FATAL ERROR in line%d\n", __LINE__);
		exit(1);
	}
	else{
		int index =-1;
		for (int i = 0; i < 6; i++){
			if(strcasecmp(func, funcname[i])==0){
				index = i;
				break;	
			}
		}
		//exit if the function name if not match
	        if (index == -1){
			printf("FATAL ERROR in line %d\n", __LINE__);
      			exit(1);
			}
		return farray[index](arr,size);
	 }
}

static double _min(double* arr, int size){
	double min;
	if(size == 1){
	   	min = arr[0]; 
	}
	else{
	 	 min = arr[0];
		for (int i = 1; i<size; i++){
			if ( min>arr[i] ){
			 min = arr[i];
			}
		}
	}
	return min;
}
static double _max(double* arr, int size){
	double max;
	if(size == 1){
	   	max = arr[0]; 
	}
	else{
		max = arr[0];
		for (int i = 1; i<size; i++){
			if ( max < arr[i] ){
				 max = arr[i];
			}
		}
	}
	return max;
}
static double _count(double* arr, int size){
	
	return (double)size;
}
static double _sum(double* arr, int size){
	double total = 0;
	for (int i = 0; i<size; i++){
		total += arr[i];
	}
	return total;
}
static double _avg(double* arr, int size){
	double avg = 0;
	double total = _sum(arr, size);
	avg = total / size;
	return avg;
}
static double _pavg(double* arr, int size){
	if(size==1){
		return arr[0];
	}
	double min = _min(arr, size);
	double max = _max(arr, size);		
	double pavg = (min + max)/2;
	return pavg;
}

