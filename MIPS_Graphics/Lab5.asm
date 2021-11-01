#Winter20 Lab5 Template File

# Macro that stores the value in %reg on the stack 
#  and moves the stack pointer.
.macro push(%reg)
subi $sp, $sp, 4
sw %reg, ($sp)
.end_macro 

# Macro takes the value on the top of the stack and 
#  loads it into %reg then moves the stack pointer.
.macro pop(%reg)
lw %reg, ($sp)
addi $sp, $sp, 4
.end_macro

# Macro that takes as input coordinates in the format
# (0x00XX00YY) and returns 0x000000XX in %x and 
# returns 0x000000YY in %y
.macro getCoordinates(%input %x %y)
push(%input)
lh %x, 2($sp)
lh %y, ($sp)
pop(%input)
.end_macro

# Macro that takes Coordinates in (%x,%y) where
# %x = 0x000000XX and %y= 0x000000YY and
# returns %output = (0x00XX00YY)
.macro formatCoordinates(%output %x %y)
subi $sp, $sp, 4
sh %y, ($sp)
sh %x, 2($sp)
pop(%output)
.end_macro 

#Macro that saves current register values in stack 
.macro preLog
#allocate memory for s registers and ra register
#then store 
subi $sp, $sp, 36
sw $s0, ($sp)
sw $s1, 4($sp)
sw $s2, 8($sp)
sw $s3, 12($sp)
sw $s4, 16($sp)
sw $s5, 20($sp)
sw $s6, 24($sp)
sw $s7, 28($sp)
sw $ra, 32($sp)
.end_macro 

#Macro that restores preLog registers from stack 
.macro postLog
#restore the values of the s registers 
#remove from stack 
lw $s0, ($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
lw $s3, 12($sp)
lw $s4, 16($sp)
lw $s5, 20($sp)
lw $s6, 24($sp)
lw $s7, 28($sp)
lw $ra, 32($sp)
addi $sp, $sp, 36
.end_macro 
.data
originAddress: .word 0xFFFF0000

.text
j done  #<not sure why this is here but was part of asm file 
    
    done: nop
    li $v0 10 
    syscall

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#  Subroutines defined below
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#*****************************************************
# clear_bitmap:
#  Given a clor in $a0, sets all pixels in the display to
#  that color.	
#-----------------------------------------------------
# $a0 =  color of pixel
#*****************************************************
clear_bitmap: nop
	#initialize adress at cords (0,0)
	#initialize counter
	lw $t0, originAddress
	li $t1, 0
	pixelHere:
	#draw a pixel until we fill all the pixels 
	sw $a0,($t0)
	addi $t0, $t0, 4
	add $t1, $t1, 1	
	ble $t1, 16383, pixelHere 
	jr $ra
	
#*****************************************************
# draw_pixel:
#  Given a coordinate in $a0, sets corresponding value
#  in memory to the color given by $a1
#  [(row * row_size) + column] to locate the correct pixel to color
#-----------------------------------------------------
# $a0 = coordinates of pixel in format (0x00XX00YY)
# $a1 = color of pixel
#*****************************************************
draw_pixel: nop
	preLog
	getCoordinates($a0 $s0 $s1)
	lw $s3, originAddress 
	#calculate adress offset based on cordinates 
	#then add offset to orgin adress and store color 
	mul $s4,$s1,128
	add $s4, $s0, $s4
	mul $s4, $s4, 4
	add $s4, $s3, $s4
	sw $a1, ($s4)
	postLog
	jr $ra
#*****************************************************
# get_pixel:
#  Given a coordinate, returns the color of that pixel	
#-----------------------------------------------------
# $a0 = coordinates of pixel in format (0x00XX00YY)
# returns pixel color in $v0	
#*****************************************************
get_pixel: nop
	preLog
	getCoordinates($a0, $s0, $s1)
	lw $s3, originAddress 
	#calculate adresss based on cordinates 
	#store color of pixel at that adress 
	mul $s2,$s1,128
	add $s2, $s0, $s2
	mul $s2, $s2, 4
	add $s2, $s3, $s2
	lw $v0, ($s2)
	postLog
	jr $ra
	

#***********************************************
# draw_line:
#  Given two coordinates, draws a line between them 
#  using Bresenham's incremental error line algorithm	
#-----------------------------------------------------
# 	Bresenham's line algorithm (incremental error)
# plotLine(int x0, int y0, int x1, int y1)
#    dx =  abs(x1-x0);
#    sx = x0<x1 ? 1 : -1;
#    dy = -abs(y1-y0);
#    sy = y0<y1 ? 1 : -1;
#    err = dx+dy;  /* error value e_xy */
#    while (true)   /* loop */
#        plot(x0, y0);
#        if (x0==x1 && y0==y1) break;
#        e2 = 2*err;
#        if (e2 >= dy) 
#           err += dy; /* e_xy+e_x > 0 */
#           x0 += sx;
#        end if
#        if (e2 <= dx) /* e_xy+e_y < 0 */
#           err += dx;
#           y0 += sy;
#        end if
#   end while
#-----------------------------------------------------
# $a0 = first coordinate (x0,y0) format: (0x00XX00YY)
# $a1 = second coordinate (x1,y1) format: (0x00XX00YY)
# $a2 = color of line format: (0x00RRGGBB)
#***************************************************
draw_line: nop
	
	preLog
	#t0=x0 t1=y0
	#t2=x1 t3=y1
	getCoordinates($a0, $t0, $t1)
	getCoordinates($a1, $t2, $s4)
	move $a1,$a2
	
	#t4=dx t5=dy
	sub $t4, $t2, $t0
	bge $t4, 0, xPositive
	mul $t4, $t4,-1
	xPositive:
	sub $t5, $s4, $t1
	ble $t5, 0, yNegative 
	mul $t5, $t5, -1
	yNegative:
	
	#s0=sx s1=sy
	sub $s0, $t2, $t0
	blt $s0, $zero, lessX
	li $s0, 1
	b moreX
	lessX:
	li $s0, -1
	moreX:
	sub $s1, $s4, $t1
	blt $s1, $zero, lessY
	li $s1, 1
	b next
	lessY:
	li $s1, -1
	next:
	
	#s2=err 
	add $s2, $t4, $t5
	
	#loop which calculates error and the draws a dot accordingly 
	printDot:
	formatCoordinates($a0, $t0, $t1)
	jal draw_pixel
	beq $t0,$t2, lineCheckx
	beq $t1,$s4, lineChecky
	checkDone:
	
	#calculate 2err=$s3
	mul $s3, $s2, 2
	
	#check current err with y
	blt $s3, $t5, lessYErr
	add $s2, $s2, $t5
	add $t0, $t0, $s0
	lessYErr:
	bgt $s3, $t4, printDot
	add $s2, $s2, $t4
	add $t1, $t1, $s1
	b printDot 
	
	#check if we are done 
	lineCheckx:
	beq $t1,$s4, lineDone
	b checkDone
	lineChecky:
	beq $t0,$t2, lineDone
	b checkDone
	lineDone:
	postLog
	jr $ra
	
#*****************************************************
# draw_rectangle:
#  Given two coordinates for the upper left and lower 
#  right coordinate, draws a solid rectangle	
#-----------------------------------------------------
# $a0 = first coordinate (x0,y0) format: (0x00XX00YY)
# $a1 = second coordinate (x1,y1) format: (0x00XX00YY)
# $a2 = color of line format: (0x00RRGGBB)
#***************************************************
draw_rectangle: nop
	preLog
	#x0=s0 y0=s1
	#x1=s2 y1=s3
	getCoordinates($a0, $s0, $s1)
	getCoordinates($a1, $s2, $s3)
	
	
	drawRow:
	#draw a line until the rectangle reaches proper size 
	#incriment y value to draw a line below the last line
	formatCoordinates($a0, $s0, $s1)
	formatCoordinates($a1, $s2, $s1)
	jal draw_line
	add $s1, $s1, 1
	bge $s3, $s1, drawRow
	postLog
	jr $ra
	
#*****************************************************
#Given three coordinates, draws a triangle
#-----------------------------------------------------
# $a0 = coordinate of point A (x0,y0) format: (0x00XX00YY)
# $a1 = coordinate of point B (x1,y1) format: (0x00XX00YY)
# $a2 = coordinate of traingle point C (x2, y2) format: (0x00XX00YY)
# $a3 = color of line format: (0x00RRGGBB)
#-----------------------------------------------------
# Traingle should look like:
#               B
#             /   \
#            A --  C
#***************************************************	
draw_triangle: nop
	preLog
	#save the values of triangle cordinates 
	move $s0,$a0
	move $s1, $a1
	move $s2, $a2
	#draw each line 
	move $a2, $a3	
	jal draw_line
	move $a0, $s0
	move $a1, $s2
	jal draw_line
	move $a0, $s1
	move $a1, $s2
	jal draw_line
	postLog
	jr $ra	
	
	
	
