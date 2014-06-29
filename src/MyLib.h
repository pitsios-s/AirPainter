#ifndef MYLIB_H
#define	MYLIB_H

#include <cv.h>
#include <highgui.h>

//Prints the percent difference in the values of 2 grayscale frames, given a certain sensitivity
float percentDiff( const IplImage * src1, const IplImage * src2, int sensitivity );


//Calculates the differential frame of 2 grayscale frames, given a certain sensitivity
void printDiff(const IplImage * src1, const IplImage * src2, IplImage * dst, int sensitivity);


//Pains the distance x distance neighborhood around a CvPoint red. dst is a 3-channel frame
//See what you have to change, in order to paint different sized regions with different colors
void paintRed( IplImage * dst, const CvPoint point, int distance );


//Makes all pixels of dst white. dst can have one or more channels of colors
void makeWhite( IplImage * dst );


//Makes all pixels of dst black. dst can have one or more channels of colors
void makeBlack( IplImage * dst );

#endif