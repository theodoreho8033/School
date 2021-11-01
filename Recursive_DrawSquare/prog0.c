#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>

//	Cruz ID: theho, Name: Theodore Ho, CSE 101 Fall 2021
// 	Given a two points, p1 and p2, this program recursively draws squares within eachother at a given offset to produce a sprial-like pattern to a .png file. 
// 	Additional parameters are given to specify the image size, recursive depth, and output file name.

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
// 	This is because the calculations assume p1x < p2x
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
	
int DrawSquare(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y,
			double p4x, double p4y, int dimx, int dimy, int rc_left, double frac, cairo_t *cr, cairo_surface_t * surface, char * name){ 


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
// 		char * name: name of output png file 

//		return: 0 if rc_left < 0,  recursion if rc_left >= 0


//	check if rec_left == -1
	if (rc_left == -1){

//		output to name.png
		cairo_destroy (cr);
		cairo_surface_write_to_png(surface, strcat(name, ".png"));
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
			next_corners[6], next_corners[7], dimx, dimy, rc_left, frac, cr, surface, name);
	}

}	
	
int main() {
	
//	we first parse the input 
	char * input; 
	size_t len = 0; 
	char * p; 
	int index = 0; 

// 	allocating sufficent memory for input data (more than needed to be on the safe side)
	char name[800]; 
	double input_data[35]; 

//	begin reading input 
	getline(&input, &len, stdin);
	char * tk;
	tk = strtok(input, " ,	");

// 	save the name of the file first 
	const char * name_read = tk; 
	strcpy(name, name_read);

// we must skip over the first token as that is used for the name of the file and cannot be added to an array of doubles
	tk = strtok(NULL, " ,	");
	while (tk != NULL){		
		input_data[index] = strtod(tk, &p);
		index = index + 1;
		tk = strtok(NULL, " ,	");
	}
		
//	write input data to variables
	double p1x = input_data[4];
	double p1y = input_data[5];
	double p2x = input_data[6];
	double p2y = input_data[7];
	double frac = input_data[3];
	int rc_left = input_data[2];
	int dimx = input_data[0];
	int dimy = input_data[1];
	
//	we need the slopes to compute additional points 
	double wy = abs(p2y - p1y);
	double wx = abs(p2x - p1x);

//	declare the remaining corners
	double p3x;
	double p3y; 
	double p4x;
	double p4y;
	 	
//	The calculations done in ComputeNextCorners assume that p1x >= p2x &&  p1y >= p2y. In every square, there will be two points
// 	which meet this condition. Thus, if this conditions are not true for the given p1 and p2, we must redefine p1 and p2 to other corners of the square 
// 	which satisfy this condition. 
	if (p1x > p2x || p1y < p2y){

//		place holders so we don't lose track of values when swapping. 
		double kp1x;
		double kp1y;
		double kp2x;
		double kp2y;
		double kp3x;
		double kp3y;
		double kp4x;
		double kp4y; 

//		depending on the placement of the given corners we must compute the remaining corners and adjustments of p1 and p2 differently		
		if (p1x >= p2x && p1y >= p2y){
			kp2x = p1x;
			kp3x = p2x;
			kp2y = p1y; 
			kp3y = p2y; 
			
			kp1x = p1x - wy; 
			kp1y = p1y + wx; 
			kp4x = p2x - wy; 
			kp4y = p2y + wx; 
			
		}else if(p1x >= p2x && p1y <= p2y){
			kp4x = p2x; 
			kp4y = p2y;
			kp3x = p1x; 
			kp3y = p1y; 
			
			kp1x = p2x + wy; 
			kp1y = p2y + wx; 
			kp2x = p1x + wy;
			kp2x = p1x + wy; 
			kp2y = p1y + wx; 
					
		} else if (p1x <= p2x && p1y <= p2y){
			kp1x = p2x;
			kp1y = p2y;
			kp4y = p1y;
			kp4x = p1x; 
			
			kp2x = p2x + wy;
			kp2y = p2y - wx;
			kp3x = p1x + wy; 
			kp3y = p1y - wx; 
				
		
		}

// 		finally, write computed values to the corner variables	
		p1x = kp1x;
		p1y = kp1y;
		p2x = kp2x;
		p2y = kp2y; 
		p3x = kp3x;
		p3y = kp3y;
		p4x = kp4x;
		p4y = kp4y; 
		
	
		
	}else{
	
//		Base condition when p1x >= p2x &&  p1y >= p2y
		p3x = p2x - wy;
		p3y = p2y - wx;
		p4x = p1x - wy;
		p4y = p1y - wx;
	}
	
//	declare cairo objects
	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, dimx, dimy );
	cairo_t *cr  = cairo_create (surface);
	
//	begin recursion 
	DrawSquare(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, dimx, dimy, rc_left, frac, cr, surface, name);
	
	return 0;
}
