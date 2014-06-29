#include <cv.h>
#include <highgui.h>
#include "MyLib.h"
#include "Painter.h"


int main(int argc, char** argv)
{
	
	//**************************************************//
	//***********WINDOW AND CAMERA PROPERTIES***********//

	 //A full screen window that will be displaying the canvas.
	cvNamedWindow("canvas" , CV_WINDOW_NORMAL );
	cvSetWindowProperty("canvas" , CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );
	
	//A full screen window that will be displaying the camera.
	cvNamedWindow("camera" , CV_WINDOW_NORMAL );
	cvSetWindowProperty("camera" , CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );

	//A full screen window that will be useful for calibration.
	cvNamedWindow("calibrator" , CV_WINDOW_NORMAL );
	cvSetWindowProperty("calibrator" , CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );

	//Get input from the camera.
	CvCapture* capture = cvCreateCameraCapture( 1 );
	
	if( !capture )
	{
		printf("NOT FOUND!\n");
	}
	
	CvSize size = cvSize( (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH ) , (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT ));
	
	//**************************************************//

	


	//**************************************************//
	//***************ALL OF OUR IPL IMAGES**************//

	//The frame taken from the camera.
	IplImage* frame;

	//An image that will hold the canvas.
	IplImage* canvasImage = cvLoadImage("Canvas.jpg");

	//An image useful for the detection of the corners of the canvas.
	IplImage* corners = cvLoadImage("calibrator.jpg");

	//A gray-scale image of the input of the camera.
	IplImage* frameGray = cvCreateImage( size, IPL_DEPTH_8U, 1 );

	//An image useful for the differential.
	IplImage* prev = cvCreateImage( size, IPL_DEPTH_8U, 1 );

	//An image that we hold the differential of gray and prev.
	IplImage* diffImage = cvCreateImage( size, IPL_DEPTH_8U, 1 );

	//An image useful for applying canny algorithm.
	IplImage* cannyImage = cvCreateImage(size , IPL_DEPTH_8U , 1);

	//An image that we hold the edges found in canny algorithm.
	IplImage* edges = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	
	//An image useful for the laplacian smoothing.
	IplImage* laplaceImage = cvCreateImage( size, IPL_DEPTH_16S, 1 );

	//**************************************************//
	



	//**************************************************//
	//******************OTHER VARIABLES*****************//

	//Structure representing memory storage.
	CvMemStorage* store = cvCreateMemStorage( 0 );
	
	//CvSequence: dynamic data structure (kind of like a list).
	CvSeq* seq = 0;

	//Shows if we are ready to compute the differential or not.
	bool diff = false;
	
	//If this variable is set to true , then we paint.
	bool painting = false;

	//The biggest point of X-axis of our canvas.
	int maxX = -1;

	//The biggest point of Y-axis of our canvas.
	int maxY = -1;

	//The smallest point of X-axis of our canvas.
	int minX = 641;

	//The smallest point of Y-axis of our canvas.
	int minY = 481;

	//**************************************************//
	

	
		
	//**************************************************//
	//********FIRST LOOP. TARGET : PLACE CAMERA*********//
	
	while(1)
	{
		frame = cvQueryFrame( capture );

		if( !frame )
		{
			break;
		}

		cvShowImage("camera" , frame);

		char c = cvWaitKey( 20 );
		
		if( c == 'c' )
		{
			break;
		}
	}
	
	cvDestroyWindow("camera");

	//**************************************************//



	
	//**************************************************//
	//********SECOND LOOP. TARGET : CALIBRATION*********//
	
	while(1)
	{
		frame = cvQueryFrame( capture );

		if( !frame )
		{
			break;
		}

		cvSmooth(frame , frame );
		cvCvtColor(frame , frameGray , CV_BGR2GRAY);

		cvShowImage("calibrator" , corners);

		char c = cvWaitKey( 20 );
		
		if( c == 's' )
		{
			for( int y=0; y < frameGray->height; y++ )
			{
				uchar* ptr = (uchar*) (frameGray->imageData + y * frameGray->widthStep);
				for( int x=0; x < frameGray->width; x++ )
				{
					if(ptr[x] >= 220)
					{
						if( x < minX) minX = x;
						if( y < minY) minY = y;
						if( x > maxX) maxX = x;
						if( y > maxY) maxY = y;
					}
				}
			}

			break;
		}
	}
	
	minX  = (minX - 20) < 0?0:(minX - 20);
	minY  = (minY - 60) < 0?0:(minY - 60);

	maxX  = (maxX + 20) > 640?640:(maxX + 20);
	maxY  = (maxY + 80) > 480?480:(maxY + 80);
	
	cvDestroyWindow("calibrator");

	printf("minX = %d , minY = %d , maxX = %d , maxY = %d\n" ,minX, minY, maxX, maxY);

	//Initialize a painter object.
	Painter painter(canvasImage , minX , minY , maxX , maxY);

	//**************************************************//
	


	
	//**************************************************//
	//**********THIRD LOOP. TARGET : PAINTING***********//

	while(1)
	{
		//**************************************************//
		//*****************CAPTURING FRAMES*****************//

		//Capture a frame from the camera.
		frame = cvQueryFrame( capture );
		
		//If nothing was captured , break from the loop.
		if( !frame )
		{
			break;
		}
		
		//Convert the frame that was captured in a gray-scale image.
		cvCvtColor( frame , frameGray , CV_BGR2GRAY );

		//**************************************************//
				
		
		
		
		//**************************************************//
		//*************COMPUTE DIFFERENCE IMAGE*************//
		
		//1. Fill up gray2 with black pixels
		makeBlack(diffImage);
		
		//2. Creates differential image between current and previous frame (if exists).
		if( diff ) printDiff(frameGray , prev , diffImage , 30);
		
		//3. Keeps current frame (it will be "previous" at the next iteration).
		cvCopy(frameGray , prev); 
		
		//4.Smooth the differential image.
		//Apply Laplacian smoothing on the gray scale image.
		cvLaplace(diffImage , laplaceImage);
		cvConvertScale(laplaceImage , diffImage);

		//Apply a smooth for further noise reduction.
		cvSmooth(diffImage , diffImage);

		//**************************************************//




		//**************************************************//
		//******************MAKE AN ACTION******************//
		
		painter.findFinger(diffImage);
		
		int change = painter.action();
		
		if (change == 1)	   painter.changeToRed();

		else if (change == 2)  painter.changeToGreen();

		else if (change == 3)  painter.changeToBlue();

		else if (change == 4)  painter.activateEraser();
			
		else if (change == 5)  painter.activateLineTool();

		else if (change == 6)  painter.changeToSmall();
			
		else if (change == 7)  painter.changeToMedium();
			
		else if (change == 8)  painter.changeToLarge();

		else if (change == 10) painter.activateFreePaintingTool();

		else if (change == 11) painter.deactivateEraser();

		else if (change == 9)
		{
			if(painting && painter.validPoints())
			{
				makeBlack(cannyImage);
					
				painter.paint(cannyImage);

				//Get the canvas points.
				CvPoint canvasTopLeft = painter.getCanvasTopLeft();
				CvPoint canvasBottomRight = painter.getCanvasBottomRight();

				//This doesn't paint. It initializes a rectangle structure to use
				//in order to define a Rectangle Of Interest (ROI)
				CvRect rect = cvRect(canvasTopLeft.x + 5 , canvasTopLeft.y + 10 , (canvasBottomRight.x - canvasTopLeft.x - 5) , (canvasBottomRight.y - canvasTopLeft.y - 5) );

				//Apply the Canny edge detection algorithm
				//on image cannyImage with 
				//min threshold = 100 , max threshold = 300
				//and save the results to edges image.
				//cvCanny(cannyImage , edges , 100 , 300 );
					
				//Apply the roi.
				cvSetImageROI(cannyImage , rect);

				//Apply the Hough transform on the edges image.
				seq = cvHoughLines2(cannyImage , store , CV_HOUGH_PROBABILISTIC , 9 , 3*CV_PI/180 , 10 , 1 , 20 );

				//Reset the ROI.
				cvResetImageROI( cannyImage );
									
				//Paint every line that Hough transform detected.
				for ( int i = 0; i < seq->total; i++ )
				{
					CvPoint *aline = (CvPoint *) cvGetSeqElem (seq, i);
					
					CvPoint start , end;
				
					start.x = aline[0].x + canvasTopLeft.x + 5;
					start.y = aline[0].y + canvasTopLeft.y + 10;

					end.x = aline[1].x + canvasTopLeft.x; 
					end.y = aline[1].y + canvasTopLeft.y + 10;

					painter.paintLine(start , end);			
				}
						
				painter.prepareForNextPaint();
			}

			else
			{
				painter.clearFrameCounters();
			}
		}

		else if (change == 12)
		{
			if(painting)
			{
				painter.paintCircle();
			}
		}

		//**************************************************//

		
		

		//Show the canvas image.
		cvShowImage("canvas" , canvasImage);
		
		//We reset the allocated memory
		//to make sure that we will not
		//carry segments of the previous frame.
		cvClearMemStorage(store);

		//Wait for 10 milliseconds for an input from the user.
		char c = cvWaitKey( 10 );
		
		if( c == 27 )
		{
			break;
		}

		else if(c == 'p')
		{
			if(!painting) painting = true;
			else painting = false;
		}

		else if(c == 'c')
		{
			painter.clearCanvas();
		}

		else if(c == 'd')
		{
			if(diff) diff = false;
			else diff = true;
		}
	}
	
	
	
		
	//**************************************************//
	//***RELEASE WINDOWS AND IMAGES TO FREE THE MEMORY**//
	
	//Destroy the canvas window.
	cvDestroyWindow("canvas");

	//Close the input from the camera.
	cvReleaseCapture( &capture );
	
	//Release all the IPLImages.
	cvReleaseImage( &frameGray );
	cvReleaseImage( &diffImage );
	cvReleaseImage( &cannyImage );
	cvReleaseImage( &laplaceImage);
	cvReleaseImage( &prev );
	cvReleaseImage( &canvasImage );
	cvReleaseImage( &corners );
	cvReleaseImage( &edges );
	
	//Release the storage we used for the hough algorithm.
	cvReleaseMemStorage( &store );

	//**************************************************//

	return 0;
}
