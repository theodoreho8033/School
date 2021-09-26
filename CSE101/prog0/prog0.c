

//prompt input 
#include <stdio.h>

int main() {

   printf("Hello, World!");



//parse input data 


//compute p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y

//call DrawSquare

	return 0;
}
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
//	call GraphicsDraw with current corners 
//	get next corners by calling ComputeNextCorners
// 	decrement rc_left
//   recursively call with updated corners 



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

	return: array with updated corners 
	

//	compute height and width of each line by calling ComputeHW
//	compute each next corner


//Function ComputeHW

// 	params
//		double p1x: x coord of point 1
//		double p1y: y coord of point 1
//		double p2x: x coord of point 2
//		double p2y: y coord of point 2

//	return: array containg height and width of line connecting p1 and p2x

