#include "singular.h"
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
extern int precision;

void print(double a[], size_t size){
	if(size>0){
		double multiplier = pow(10, precision);
   		double roundedNum;
		for(size_t i = 0; i < size; i++){
		if(a[i]==-9999){
			break;
		}
		roundedNum = round(a[i] * multiplier) / multiplier;
		printf("%.*f ", precision, roundedNum);
		}
		printf("\n");
	}
}
void shift(double a[], size_t size, double by){
		for (size_t i = 0; i < size; i++){
			a[i] = a[i] + by;
		}
		
}
size_t filter(double a[], size_t count, enum filter_type t, double threshold){
	int counter = 0;
	 for (size_t i = 0; i < count; i++) {
	 	switch(t){
		   case EQ:
			 if (a[i] == threshold){
		            a[counter++] = a[i];
			}
			else{
			    a[i] = -999;
			}	
			break; 
		   case NEQ:
             		 if (a[i] != threshold) {
                            a[counter++] = a[i];
             	 	  }
             	   	else{
			    a[i] = -999;
			}
              		break;
		    case GEQ:
		        if (a[i] >= threshold) {
		            a[counter++] = a[i];
		        }
		        else{
			    a[i] = -999;
			}
		        break;
		    case LEQ:
		        if (a[i] <= threshold) {
		            a[counter++] = a[i];
		     }
		    	 else{
			    a[i] = -999;
			}
		        break;
		    case LESS:
		        if (a[i] < threshold) {
		            a[counter++] = a[i];
		        }
		        else{
			    a[i] = -999;
			}
		        break;
		    case GREATER:
		        if (a[i] > threshold) {
		            a[counter++] = a[i];
		        }
		        else{
			    a[i] = -999;
			}
		        break;
		    default:
		     	printf("1Invalid type\n");
          	        return 1;
		 }  
	 }
	 	return counter;	
}

