

//prompt input 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double * ComputeHW(double p1x, double p1y, double p2x, double p2y){
//Function ComputeHW

// 	params
//		double p1x: x coord of point 1
//		double p1y: y coord of point 1
//		double p2x: x coord of point 2
//		double p2y: y coord of point 2

//	return: array containg height and width of line connecting p1 and p2x
	double * hw = malloc (sizeof (double) * 2);
	hw[0] = abs(p2y - p1y);
	hw[1] = abs(p2x -p1x);
	printf("\nHeight: %.2f", hw[0]);
	printf("\nWidth: %.2f", hw[1]);
	printf("\n---\n");
	
	return hw; 

}

double * ComputeNextCorners(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
			double p4x, double p4y, double frac){ 
//Function ComputeNextCorners
//	params
//		double p1x: x coord of corner 1
//		double p1y: y coord of corner 1
//		double p2x: x coord of corner 2
//		double p2y: y coord of corner 2
//		double p3x: x coord of corner 3
//		double p3y: y coord of corner 3
//		double p4x: x coord of corner 4
//		double p4y: y coord of corner 4	

//	return: array with updated corners 
	

//	compute height and width of each line by calling ComputeHW
	double p1x_next = p1x + frac * ComputeHW(p1x, p1y, p2x, p2y)[1];
	printf("\n\n%f\n\n", frac);
	double p1y_next = p1y - frac * ComputeHW(p1x, p1y, p2x, p2y)[0];
	
	double p2x_next = p2x - frac * ComputeHW(p2x, p2y, p3x, p3y)[1];
	double p2y_next = p2y - frac * ComputeHW(p2x, p2y, p3x, p3y)[0];
	
	double p3x_next = p3x - frac * ComputeHW(p3x, p3y, p4x, p4y)[1];
	double p3y_next = p3y + frac * ComputeHW(p3x, p3y, p4x, p4y)[0];
	
	double p4x_next = p4x + frac * ComputeHW(p4x, p4y, p1x, p1y)[1];
	double p4y_next = p4y + frac * ComputeHW(p4x, p4y, p1x, p1y)[0];
//	compute each next corner
	double * next_corners = malloc (sizeof (double) * 8);
	next_corners[0] = p1x_next;
	next_corners[1] = p1y_next;
	
	next_corners[2] = p2x_next;
	next_corners[3] = p2y_next;
	
	next_corners[4] = p3x_next;
	next_corners[5] = p3y_next;
	
	next_corners[6] = p4x_next;
	next_corners[7] = p4y_next;
	
	return next_corners;

}

double * ParseInput(){

	
	
	

	double * parsed_input = malloc(8);
	char * input = NULL;
	size_t len = 0;
	
	getline(&input, &len, stdin);
	
	
	
	char * token = strtok(input, " ");
	
	int index = 0;
	char * p;
	while (token != NULL) {
		parsed_input[index] = strtod(token, &p);
		index = index + 1;
		token = strtok(NULL, " ");
	
	}

 
	free(input);
	
	return parsed_input;
  
	
	
	
}
	
	
	
	
	


int main() {

  
 


//parse input data 
double  * input_data = ParseInput();


//compute p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y

double p1x = input_data[4];
double p1y = input_data[5];
double p2x = input_data[6];
double p2y = input_data[7];
double frac = input_data[3];

double wx = -(p2y - p1y);
double wy = -(p2x - p1x);
double p3x = p2x - wx;
double p3y = p2y + wy;
double p4x = p3x - wx;
double p4y = p3y - wy;
double * next_corners = ComputeNextCorners(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, frac); 

printf("\n Current Corners: p1: %.2f,%.2f | p2: %.2f,%.2f | p3: %.2f,%.2f | p4: %.2f,%.2f |\n\n", p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y);
printf("\n Next Corners");
	for(int i = 0; i < 8; i++) {
        printf("\n%.2f",next_corners[i]);
    }

//call DrawSquare

	return 0;
}


int DrawSquare(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
			double p4x, double p4y, int dimx, int dimy, int rc_left, double frac){ 
//Function DrawSquare
//	params
//		double p1x: x coord of corner 1
//		double p1y: y coord of corner 1
//		double p2x: x coord of corner 2
//		double p2y: y coord of corner 2
//		double p3x: x coord of corner 3
//		double p3y: y coord of corner 3
//		double p4x: x coord of corner 4
//		double p4y: y coord of corner 4

//		int dimx: image width
//		int dimy: image height 
//		int rc_left: recursions remaining 
//		double frac: fraction of square

//		return: 0 if rc_left == 0,  recursion if rc_left > 0



//	check if rec_left == 0
	if (rc_left == 0){
		return 0;
	}else{
		return 1;
				
//	call GraphicsDraw with current corners 
//	get next corners by calling ComputeNextCorners
	double * next_corners = ComputeNextCorners(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, frac);
	
// 	decrement rc_left
//   recursively call with updated corners 
	}

}




