#ifndef _PAINTER_H_
#define _PAINTER_H_


#include <cv.h>
#include <highgui.h>
#include "MyLib.h"


#define BLUE             CV_RGB(0,0,255)
#define GREEN            CV_RGB(0,255,0)
#define RED              CV_RGB(255,0,0)
#define WHITE            CV_RGB(255,255,255)
#define BLACK            CV_RGB(0,0,0)
#define YELLOW           CV_RGB(255,255,0)


class Painter
{
	public :

		//Constructor.
		Painter(IplImage *canvasImage , int minX , int minY , int maxX , int maxY);

		//Tries to find the user's finger in the differential image.
		void findFinger(IplImage *diffImage);

		//Returns true if the points that we have
		//to paint a line are valid. False otherwise.
		bool validPoints();

		//Clears the canvas.
		void clearCanvas();

		//Changes the size to small.
		void changeToSmall();

		//Changes the size to medium.
		void changeToMedium();

		//Changes the size to large.
		void changeToLarge();

		//Activates the line tool.
		void activateLineTool();

		//Deactivates the line tool.
		void activateFreePaintingTool();

		//Changes our color to red.
		void changeToRed();

		//Changes our color to green.
		void changeToGreen();

		//Changes our color to Blue.
		void changeToBlue();

		//Activates the eraser.
		void activateEraser();

		//Deactivates the eraser.
		void deactivateEraser();

		/*
		 * Returns a code according to the action that we need to do.
		 *
		 * 0 :  no action
		 * 1 :  change color to red.
		 * 2 :  change color to green.
		 * 3 :  change color to blue.
		 * 4 :  activate eraser.
		 * 5 :  activate line tool.
		 * 6 :  change to small size.
		 * 7 :  change to medium size.
		 * 8 :  change to large size.
		 * 9 :  paint a line.
		 * 10 : activate free painting tool.
		 * 11 : deactivate eraser.
		 * 12 : paint a small circle.
		 */
		int action();

		//Paints a white line , using the
		//points we have , in the image given
		//as parameter.
		void paint(IplImage * img);

		//Paints a line on the canvas.
		void paintLine(const CvPoint &start , const CvPoint &end);

		//Paints a small circle on the canvas , if the line tool is not selected.
		void paintCircle();

		//Changes some variables in order to be used 
		//for the next line to be painted after 
		//an already painted one.
		void prepareForNextPaint();

		//Returns the top left point of the canvas.
		CvPoint getCanvasTopLeft();

		//Returns the bottom right point of the canvas.
		CvPoint getCanvasBottomRight();

		//If we have not located the finger in the current frame , 
		//then make all counters for changing the tools zero.
		void clearFrameCounters();



	private :

	//The canvas image that we will paint on it.
	IplImage *canvasImage;




	//**************************************************//
	//*******************PIXEL VARIABLES****************//

	//The biggest point of X-axis of our canvas.
	int maxX;

	//The biggest point of Y-axis of our canvas.
	int maxY;

	//The smallest point of X-axis of our canvas.
	int minX;

	//The smallest point of Y-axis of our canvas.
	int minY;

	//The first pixel in X-Axis that indicates the existence of finger.
	int fingerX;

	//The first pixel in Y-Axis that indicates the existence of finger.
	int fingerY;

	//The X coordinate of the start point , of a line that we want to draw.
	int x1Paint;

	//The Y coordinate of the start point , of a line that we want to draw.
	int y1Paint;

	//The X coordinate of the end point , of a line that we want to draw.
	int x2Paint;

	//The Y coordinate of the end point , of a line that we want to draw.
	int y2Paint;

	//The X coordinate of the circle we are going to paint , if the line tool is not selected.
	int circleX;

	//The Y coordinate of the circle we are going to paint , if the line tool is not selected.
	int circleY;

	//**************************************************//




	//**************************************************//
	//******************PAINTING VARIABLES**************//

	//The current color we are using.
	CvScalar currentColor;

	//The current size of the pen.
	int penSize;

	//The small size pen or eraser.
	static const int smallSize = 3;

	//The medium size pen or eraser.
	static const int mediumSize = 5;

	//The large size pen or eraser.
	static const int largeSize = 7;

	//The number of frames to wait before change a tool.
	static const int framesToWait = 10;

	//How many frames we have to wait before painting.
	static const int framesToPaint = 5;

	//**************************************************//




	//**************************************************//
	//**BOOLEAN VARIABLES USEFUL FOR OUR IMPLEMENTATION*//

	//Indicates if the small size pen is selected.
	bool smallSelected;

	//Indicates if the medium size pen is selected.
	bool mediumSelected;

	//Indicates if the large size pen is selected.
	bool largeSelected;

	//Indicates if the line tool is selected.
	bool lineToolSelected;

	//Indicates if the free painting tool is selected.
	bool freePaintingSelected;

	//Indicates if the eraser is selected.
	bool eraserSelected;

	//Indicates if the color we are using is red.
	bool redSelected;

	//Indicates if the color we are using is green.
	bool greenSelected;

	//Indicates if the color we are using is blue.
	bool blueSelected;
	
	//**************************************************//	




	//**************************************************//
	//*******COUNTERS USEFUL FOR MAKING AN ACTION*******//

	//For how many frames , the finger is in "small size pen box".
	int framesInSmall;

	//For how many frames , the finger is in "medium size pen box".
	int framesInMedium;

	//For how many frames , the finger is in "large size pen box".
	int framesInLarge;

	//For how many frames , the finger is in the "line tool box".
	int framesInLineTool;

	//For how many frames , the finger is in the "eraser box".
	int framesInEraser;

	//For how many frames , the finger is in the "red color box".
	int framesInRed;

	//For how many frames , the finger is in the "green color box".
	int framesInGreen;

	//For how many frames , the finger is in the "blue color box".
	int framesInBlue;

	//The number of frames that elapsed before painting.
	int framesInCanvas;

	//**************************************************//




	//**************************************************//
	//****************CVPOINTS OF OUR TOOLS*************//

	//The top left point of the canvas i.e. the are that we can paint.
	CvPoint canvasTopLeft;

	//The bottom right point of our canvas.
	CvPoint canvasBottomRight;



	//The top left point of the small size line tool.
	CvPoint smallTopLeft;

	//The bottom right point of small size line tool.
	CvPoint smallBottomRight;



	//The top left point of the medium size ine tool.
	CvPoint mediumTopLeft;

	//The bottom right point of the medium size line tool.
	CvPoint mediumBottomRight;



	//The top left point of the large sie line tool.
	CvPoint largeTopLeft;

	//The bottom right point of the large size line tool.
	CvPoint largeBottomRight;



	//The top left point of the red color.
	CvPoint redTopLeft;

	//The bottom right point of red color.
	CvPoint redBottomRight;



	//The top left point of the green color. 
	CvPoint greenTopLeft;

	//The bottom right point green color.
	CvPoint greenBottomRight;



	//The top left point of the blue color.
	CvPoint blueTopLeft;

	//The bottom right point of the blue color.
	CvPoint blueBottomRight;



	//The top left point of the line tool. 
	CvPoint lineToolTopLeft;

	//The bottom right point of the line tool.
	CvPoint lineToolBottomRight;



	//The top left point of the eraser. 
	CvPoint eraserTopLeft;

	//The bottom right point of the eraser.
	CvPoint eraserBottomRight;

	//**************************************************//




	//**************************************************//
	//*********************FUNCTIONS********************//

	//Counts how many pixels of the finger 
	//are in the area of the small size tool.
	int PixelsInSmall();

	//Counts how many pixels of the finger 
	//are in the area of the medium size tool.
	int PixelsInMedium();

	//Counts how many pixels of the finger 
	//are in the area of the large size tool.
	int PixelsInLarge();

	//Counts how many pixels of the finger 
	//are in the area of the line tool.
	int PixelsInLineTool();

	//Counts how many pixels of the finger 
	//are in the area of the red color.
	int PixelsInRed();

	//Counts how many pixels of the finger 
	//are in the area of the green color.
	int PixelsInGreen();

	//Counts how many pixels of the finger 
	//are in the area of the small color.
	int PixelsInBlue();

	//Counts how many pixels of the finger 
	//are in the area of the eraser tool.
	int PixelsInEraser();

	//Counts how many pixels of the finger 
	//are in the area of the painting part of canvas.
	int PixelsInCanvas();

	//Initializes the cv points(start , end)
	//of our tools.
	void initializeCvPoints();

	//Paints some rectangles in all of tools
	//and also around the canvas , so that we know
	//were is the border of the canvas and which 
	//tools are currently selected.
	void paintRectangles();

	//Returns true if the X and Y points of the finger are valid.
	bool validFingerCoordinates();

	//Sets the X and Y values of the finger to -1.
	void disableFinger();
	
	//**************************************************//
};

#endif