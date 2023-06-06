#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <math.h>
#include"aggregate.h"
#include "singular.h"
double roundDecimal(double num, int precision) {
    double multiplier = pow(10, precision);
    return round(num * multiplier) / multiplier;
}
int precision = 6;
int main(int argc, char* argv[]){
	int capacity = 10;
	//Check if the input argument is enough
	if (argc<2 || argc>6){
	 fprintf(stderr, "Error: Not enough argument.\n");
	 exit(0);
	}
	
	//read the file and put into array
	double* array = (double*)malloc(capacity * sizeof(double));
        if (array == NULL) {
        printf("Failed to allocate memory.\n");
        return 1;
       }
	
	int size = 0;
	double num;
	int firstRowElements = 0; // Variable to store the number of elements in the first row
	int grow = 2;
	
	//read all numbers in the file
	while (scanf("%lf", &num) == 1) {
		array[size++] = num;
	//Check if the array is full and reallocate memory if needed
	if (size == capacity) {
	    capacity *= grow;
	    double* new_array = (double*)realloc(array, capacity * sizeof(double));
	if (new_array == NULL) {
	    printf("Failed to reallocate memory.\n");
	    free(array);
	    return 1;
	}
            array = new_array;
	}

        // Check if a newline character is encountered
        //-9999 means a new line start
	if (getchar() == '\n') {
	    array[size++] = -9999.0; // Add -999 to the array
	   if (firstRowElements == 0) {
		firstRowElements = size - 1; // Store the number of elements in the first row
	    }
	}
	}
	int dsize = firstRowElements; //default size
	char* function;
	int reslut = 0;
	int count = 0;
	double by = 0;
	double threshold = 0;
	enum filter_type t;
	
	//set the size and presision value
	for (int i = 1;i<argc ;i++){
		if(strncmp("-size=", argv[i], 6) == 0){
			dsize = atoi(argv[i] + 6);
		}
		else if(strncmp("-prec=", argv[i], 6) == 0){
			precision = atoi(argv[i] + 6);
			}
		}
		
	//find the operations in the input
	for (int i = 1;i<argc ;i++){
		//check for the aggregate function
		if (count>1){
			fprintf(stderr, "Error: Can not do more than operations.\n");
			exit(0);
		}
		//process aggregate functions
		else if (strcasecmp(argv[i], "COUNT") == 0 || strcasecmp(argv[i], "MIN") == 0 || 
		strcasecmp(argv[i], "MAX")  == 0 || strcasecmp(argv[i], "SUM") == 0 || 
		strcasecmp(argv[i], "AVG") == 0 || strcasecmp(argv[i], "PAVG") == 0 ) {
		function =  argv[i];
		count++;
		double fresult;
		double* subArray = (double*)malloc(dsize * sizeof(double));
		int index = -1;
		int inIndex = -1;
		int i;
		int j;
		for ( i = 0; i< size;i+=dsize){
			if(array[i]==-9999){
				i++;
				inIndex++;
			}
			for ( j = i; j < dsize+i; j++){
				if(array[j] != -9999.0){
				   index++;
				   inIndex++;
				   subArray[index] = array[inIndex];
				    if(inIndex == size-1){
				   	break;
				   }
				}
				else{
					inIndex++;
					if((j+1) != (i+dsize) ){
					i= i-(dsize -1 -(index+1));
					}
					break;
				}
			}
			//call the aggregate function
			fresult = aggregate (function, subArray, index+1);
			// Printing the number with the specified decimal points
			double roundedNum = roundDecimal(fresult, precision);
			printf("%.*f ", precision, roundedNum);
			index = -1;
		}
		free(subArray);		
		} 
		
		//process print function
		else if  (strcasecmp(argv[i], "PRINT") == 0 ){
			count++;
			//divide the bigarray into subarrays
			double* subArray = (double*)malloc(dsize * sizeof(double));
			int index = -1;
			int inIndex = -1;
			//for each row in the files, call print with the input size
			for (int i = 0; i< size-1;i+=dsize){
				for (int j = i; j < dsize+i; j++){
					if(array[j] != -9999.0){
					   index++;
					   inIndex++;
					   subArray[index] = array[inIndex];
					}
					else{
						inIndex++;
						if((j+1) != (i+dsize) ){
							i= i-(dsize -1 -(index+1));
						}
						break;
					}
				}
				//call the print function
				print(subArray, index+1);
				index = -1;
			}
			free(subArray);
		}
		//process shift function
		else if  (strcasecmp(argv[i], "SHIFT") == 0 ){
			by = strtod(argv[i+1],NULL);
			count++;
			int c_size=0;
			int index = -1;
			double* subArray = (double*)malloc(dsize * sizeof(double));
			
			for(int i = 0;i < size;i++){
				int j;
				for (j = 0;j < dsize;j++){
					index++;
					if(array[index]==-9999){
						break;
					}
					c_size++;
					subArray[j] = array[index];
				}
				//call the shift function.
				shift(subArray, c_size, by);
				int x =0;
				for (int k = i; k < c_size + i;k++){
					array[k] = subArray[x];
					x++;		
				}
				i= i+ c_size;
				index = i;
				while(array[i]!=-9999){
					index++;
					i++;
				}
				c_size=0;	
			}
			free(subArray);
			//print the array after shift
			//for(int i = 0;i < size;i++){
			//	if(array[i]!= -9999){
			//	printf("%f ", array[i]);
			//	} 
			//	else{
			//	printf("\n");
			//	}
			//}
		}		
		// process the filter function
		else if  (strcasecmp(argv[i], "FILTER") == 0 ){
		     //check the filter type
		     if (strcasecmp(argv[i+1], "EQ") == 0) {
			t = EQ;
		    } else if (strcasecmp(argv[i+1], "NEQ") == 0) {
			t = NEQ;
		    } else if (strcasecmp(argv[i+1], "GEQ") == 0) {
			t = GEQ;
		    } else if (strcasecmp(argv[i+1], "LEQ") == 0) {
			t = LEQ;
		    } else if (strcasecmp(argv[i+1], "LESS") == 0) {
			t = LESS;
		    } else if (strcasecmp(argv[i+1], "GREATER") == 0) {
			t = GREATER;
		    } else {
			printf("Invalid filter type\n");
			return 1;
		  	}
			threshold = strtod(argv[i+2],NULL);
			count++;
			int c_size=0;
			int index = -1;
			double* subArray = (double*)malloc(dsize * sizeof(double));
			
			for(int i = 0;i < size;i++){
				int j;
				for (j = 0;j < dsize;j++){
					index++;
					if(array[index]==-9999){
						break;
					}
					c_size++;
					subArray[j] = array[index];
				}
				//return the size_t after call filter function
				size_t s = filter(subArray, c_size,t, threshold);
				printf("%zu\n", s);
				
				int x =0;
				for (int k = i; k < c_size + i;k++){
					array[k] = subArray[x];
					x++;		
				}
				i= i+ c_size;
				index = i;
				while(array[i]!=-9999){
					index++;
					i++;
				}
				c_size=0;	
			}
			free(subArray);
			//print the array after filter
			//for(int i = 0;i < size-1;i++){
			//	if(array[i]!= -9999){
			//	printf("%f ", array[i]);
			//	} 
			//	else{
			//	printf("\n");
			//	}
			//}
		 }
	 }  
	        // Free the dynamically allocated memory
		free(array);
  		  return 0;
 }	
 
//gcc mathpipe.c singular.c aggregate.c -o mathpipe -lm
//cat sample.txt | ./mathpipe min -size=2
   	 
