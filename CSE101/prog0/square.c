#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
// change to proper I/O format
// prog0.c or square.c?
// your makefile might contain: outpu1? 
void CairoSquare(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y, cairo_t *cr){
	
//	Draws square given coords on given cairo object

//	params:
// 		double p1x,  p2x, p2x, ... , p4x, p4y : corners of square
// 		cairo_t cr : cairo render state object
	
// 	set line width and color 		
	cairo_set_source_rgb (cr, 0, 0, 1);	
	cairo_set_line_width (cr, .75);

//	line connecting p1, p2	
	
	cairo_move_to (cr, p1x, p1y);			
	cairo_line_to (cr, p2x, p2y);
	cairo_stroke (cr);
//	line connecting p2, p3:	
	//cairo_set_source_rgb (cr, 1, 0, 1);	
	cairo_move_to (cr, p2x, p2y);
	cairo_line_to (cr, p3x, p3y );
	cairo_stroke (cr);
//	line connecting p3, p4;	
	//cairo_set_source_rgb (cr, 0, 1, 0);	
	cairo_move_to (cr, p3x, p3y);			
	cairo_line_to (cr, p4x, p4y);
	cairo_stroke (cr);
//	line connecting p4, p1
	//cairo_set_source_rgb (cr, 1, 0, 0);	
	cairo_move_to (cr, p4x, p4y);
	cairo_line_to (cr, p1x, p1y );

// write to object	
	cairo_stroke (cr);
}

double * ComputeHW(double p1x, double p1y, double p2x, double p2y){
// Computes the height and width of two points

// 	params
//		double p1x: x coord of point 1
//		double p1y: y coord of point 1
//		double p2x: x coord of point 2
//		double p2y: y coord of point 2

//	return: array (tuple) containg height and width of line connecting p1 and p2x

// 	declare array
	double * hw = malloc (sizeof (double) * 2);
// compute height (h[0]), width (h[1])
	hw[0] = abs(p2y - p1y);
	hw[1] = abs(p2x -p1x);

	
	return hw; 

}

double * ComputeNextCorners(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
			double p4x, double p4y, double frac){ 

//Computes the corners of the next square

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
	
//  Line p1x must alwasy be < p2x. If p1x >= p2x, we need to shift corners.
	if (p1x >= p2x){
		double keep1x = p1x;
		double keep1y = p1y;
		
		double keep2x = p2x;
		double keep2y = p2y;
		
		double keep3x = p3x;
		double keep3y = p3y;
		
		double keep4x = p4x;
		double keep4y = p4y;
		
		p1x = keep4x;
		p2x = keep1x;
		p3x = keep2x;
		p4x = keep3x; 
		
		p1y = keep4y; 
		p2y = keep1y; 
		p3y = keep2y; 
		p4y = keep3y;
		
		
	}
	

//	compute height and width of each line by calling ComputeHW
//  compute next corner given height and width
	double p1x_next = p1x + frac * ComputeHW(p1x, p1y, p2x, p2y)[1];

	double p1y_next = p1y - frac * ComputeHW(p1x, p1y, p2x, p2y)[0];
	
	double p2x_next = p2x - frac * ComputeHW(p2x, p2y, p3x, p3y)[1];
	double p2y_next = p2y - frac * ComputeHW(p2x, p2y, p3x, p3y)[0];
	
	double p3x_next = p3x - frac * ComputeHW(p3x, p3y, p4x, p4y)[1];
	double p3y_next = p3y + frac * ComputeHW(p3x, p3y, p4x, p4y)[0];
	
	double p4x_next = p4x + frac * ComputeHW(p4x, p4y, p1x, p1y)[1];
	double p4y_next = p4y + frac * ComputeHW(p4x, p4y, p1x, p1y)[0];


//	organize next corners into array then return
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

// read the input and parse data into array

// return: input as array of doubles
		
// declare input pointer, length,  and array to parse to 
	double * parsed_input = malloc(8);
	char * input = NULL;
	size_t len = 0;
	int index = 0;
	char * p;
		
// read, parse, and return
	getline(&input, &len, stdin);
	char * token = strtok(input, " ");
	while (token != NULL) {
		parsed_input[index] = strtod(token, &p);
		index = index + 1;
		token = strtok(NULL, " ");	
	}
	
	return parsed_input;	
}
	
int DrawSquare(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
			double p4x, double p4y, int dimx, int dimy, int rc_left, double frac, cairo_t *cr, cairo_surface_t * surface){ 


//
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
	if (rc_left == -1){
//		output to square.png
		cairo_destroy (cr);
		cairo_surface_write_to_png (surface, "square.png");
		cairo_surface_destroy (surface);
//		stop recursion
		return 0;
	}else{
// 		draw square 	
		CairoSquare(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, cr);
				
//		get next corners by calling ComputeNextCorners
		double * next_corners = ComputeNextCorners(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, frac);	
		
// 		decrement rc_left
		rc_left = rc_left - 1;

//  recursively call with updated corners keeping track of surface object and render state object
	return DrawSquare(next_corners[0], next_corners[1], next_corners[2], next_corners[3], next_corners[4], next_corners[5],
			next_corners[6], next_corners[7], dimx, dimy, rc_left, frac, cr, surface);
	}

}	
	
int main() {

//parse input data 
	double  * input_data = ParseInput();
	double p1x = input_data[4];
	double p1y = input_data[5];
	double p2x = input_data[6];
	double p2y = input_data[7];
	double frac = input_data[3];
	int rc_left = input_data[2];
	int dimx = input_data[0];
	int dimy = input_data[1];
	
	double wy = abs(p2y - p1y);
	double wx = abs(p2x - p1x);
	
	double p3x;
	double p3y; 
	double p4x;
	double p4y;
	
	
	if (p1x > p2x || p1y < p2y){
		frac = 1-frac; 
		printf("\nspecial case");
		double kp1x;
		double kp1y;
		double kp2x;
		double kp2y;
		double kp3x;
		double kp3y;
		double kp4x;
		double kp4y; 
		
		if (p1x >= p2x && p1y >= p2y){
			printf("\np1x >= p2x, p1y >= p2y");
			kp2x = p1x;
			kp3x = p2x;
			kp2y = p1y; 
			kp3y = p2y; 
			
			kp1x = p1x - wy; 
			kp1y = p1y + wx; 
			kp4x = p2x - wy; 
			kp4y = p2y + wx; 
			//frac = 1 - frac; 
		
		}else if(p1x >= p2x && p1y <= p2y){
			printf("\np1x >= p2x, p1y <= p2y");
			kp4x = p2x; 
			kp4y = p2y;
			kp3x = p1x; 
			kp3y = p1y; 
			
			kp1x = p2x + wy; 
			kp1y = p2y + wx; 
			kp2x = p1x + wy;
			kp2x = p1x + wy; 
			kp2y = p1y + wx; 
			
			//frac = 1 - frac; 
			
			
		
		
			
		} else if (p1x <= p2x && p1y <= p2y){
			printf("\np1x <= p2x, p1y <= p2y");
			kp1x = p2x;
			kp1y = p2y;
			kp4y = p1y;
			kp4x = p1x; 
			
			kp2x = p2x + wy;
			kp2y = p2y - wx;
			kp3x = p1x + wy; 
			kp3y = p1y - wx; 
				
			//frac = 1 - frac; 
			
		}
		
		p1x = kp1x;
		p1y = kp1y;
		p2x = kp2x;
		p2y = kp2y; 
		p3x = kp3x;
		p3y = kp3y;
		p4x = kp4x;
		p4y = kp4y; 
		
	
		
	}else{
	//compute p3x, p3y, p4x, p4y
		p3x = p2x - wy;
		p3y = p2y - wx;
		p4x = p1x - wy;
		p4y = p1y - wx;
	}
	printf("\np1: (%f , %f)", p1x, p1y);
	printf("\np2: (%f , %f)", p2x, p2y);
	printf("\np3: (%f , %f)", p3x, p3y);
	printf("\np4: (%f , %f\n)", p4x, p4y);
	//declare cairo objects
	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, dimx, dimy );
	cairo_t *cr  = cairo_create (surface);
	
	//begin recursion 
	DrawSquare(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, dimx, dimy, rc_left, frac, cr, surface);
	
	return 0;
}
