#include "Painter.h"


Painter::Painter(IplImage *canvasImage , int minX , int minY , int maxX , int maxY)
{
	this->canvasImage = canvasImage;
	this-> minX = minX;
	this-> minY = minY;
	this-> maxX = maxX;
	this-> maxY = maxY;
	this-> fingerX = -1;
	this-> fingerY = -1;
	this-> penSize = smallSize;
	this-> x1Paint = 0;
	this-> y1Paint = 0;
	this-> x2Paint = 0;
	this-> y2Paint = 0;
	this-> smallSelected = true;
	this-> mediumSelected = false;
	this-> largeSelected = false;
	this-> lineToolSelected = false;
	this-> redSelected = true;
	this-> greenSelected = false;
	this-> blueSelected = false;
	this-> eraserSelected = false;
	this-> freePaintingSelected = true;
	this-> framesInSmall = 0;
	this-> framesInMedium = 0;
	this-> framesInLarge = 0;
	this-> framesInLineTool = 0;
	this-> framesInRed = 0;
	this-> framesInGreen = 0;
	this-> framesInBlue = 0;
	this-> framesInEraser = 0;
	this-> framesInCanvas = 0;
	this-> currentColor = RED;

	this-> initializeCvPoints();
	this-> paintRectangles();
}




void Painter::initializeCvPoints()
{
	//**************************************************//
	//NOW THAT WE HAVE THE POINTS OF OUR WINDOW,NEED TO //
	//INITIALIZE THE POINTS OF ITS DIFFERENT TOOLS.		//

	canvasTopLeft = cvPoint(minX , (int)( (maxY - minY)/4) );
	canvasBottomRight = cvPoint(maxX , maxY);

	lineToolTopLeft = cvPoint((int)( 3*(maxX - minX)/4) , minY);
	lineToolBottomRight = cvPoint(maxX - minX , (int)( (maxY - minY)/8));

	eraserTopLeft = cvPoint((int)( 3*(maxX - minX)/4) , (int)( (maxY - minY)/8));
	eraserBottomRight = cvPoint(maxX - minX , (int)( (maxY - minY)/4));

	redTopLeft = cvPoint(minX , (int)( (maxY - minY)/8));
	redBottomRight = cvPoint((int)( 1*(maxX - minX)/4) , (int)( (maxY - minY)/4));

	greenTopLeft = cvPoint((int)( 1*(maxX - minX)/4) , (int)( (maxY - minY)/8));
	greenBottomRight = cvPoint((int)( 2*(maxX - minX)/4) , (int)( (maxY - minY)/4));

	blueTopLeft = cvPoint((int)( 2*(maxX - minX)/4) , (int)( (maxY - minY)/8));
	blueBottomRight = cvPoint((int)( 3*(maxX - minX)/4) , (int)( (maxY - minY)/4));

	smallTopLeft = cvPoint(minX , minY);
	smallBottomRight = cvPoint((int)( 1*(maxX - minX)/4) , (int)( (maxY - minY)/8));

	mediumTopLeft = cvPoint((int)(1*(maxX - minX)/4) , minY);
	mediumBottomRight = cvPoint((int)(2*(maxX - minX)/4) ,  (int)( (maxY - minY)/8));

	largeTopLeft = cvPoint((int)(2*(maxX - minX)/4) , minY);
	largeBottomRight = cvPoint((int)(3*(maxX - minX)/4) ,  (int)( (maxY - minY)/8));

	//**************************************************//
}




void Painter::paintRectangles()
{
	//**************************************************//
	//INITIALIZE SOME RECTANGLES , SO WE KNOW EVERY TIME//
	//WHICH TOOL WE ARE USING.//
	
	//Red is selected.
	cvRectangle(canvasImage , cvPoint(redTopLeft.x + 10 , redTopLeft.y+10) , cvPoint(redTopLeft.x + 20 , redTopLeft.y+20) , YELLOW , 3);
	
	//Small size is selected.
	cvRectangle(canvasImage , cvPoint(smallTopLeft.x + 10 , smallTopLeft.y+10) , cvPoint(smallTopLeft.x + 20 , smallTopLeft.y+20) , YELLOW , 3);

	//Line tool is not selected.
	cvRectangle(canvasImage , cvPoint(lineToolTopLeft.x + 10 , lineToolTopLeft.y + 10) , cvPoint(lineToolTopLeft.x + 20 , lineToolTopLeft.y+20) , BLACK , 3);

	//Green is not selected.
	cvRectangle(canvasImage , cvPoint(greenTopLeft.x + 10  , greenTopLeft.y+10) , cvPoint(greenTopLeft.x + 20  , greenTopLeft.y+20) , BLACK , 3);

	//Blue is not selected.
	cvRectangle(canvasImage , cvPoint(blueTopLeft.x + 10  , blueTopLeft.y+10) , cvPoint(blueTopLeft.x + 20  , blueTopLeft.y+20) , BLACK , 3);

	//Eraser is not selected.
	cvRectangle(canvasImage , cvPoint(eraserTopLeft.x + 10  , eraserTopLeft.y+10) , cvPoint(eraserTopLeft.x + 20  , eraserTopLeft.y+20) , BLACK , 3);
	
	//Medium is not selected.
	cvRectangle(canvasImage , cvPoint(mediumTopLeft.x + 10  , mediumTopLeft.y+10) , cvPoint(mediumTopLeft.x + 20 , mediumTopLeft.y+20) , BLACK , 3);
	
	//Large is not selected.
	cvRectangle(canvasImage , cvPoint(largeTopLeft.x + 10 , largeTopLeft.y+10) , cvPoint(largeTopLeft.x + 20  , largeTopLeft.y+20) , BLACK , 3);

	//Paint a rectangle around the canvas.
	cvRectangle(canvasImage , canvasTopLeft, canvasBottomRight, YELLOW, 1);
	
	//**************************************************//
}




void Painter::findFinger(IplImage *diffImage)
{
	int count = 0;
	
	int tempX = maxX + 1;
	int tempY = maxY + 1;

	for( int i = 0; i < diffImage->height; i++ )
	{
		uchar* ptr = (uchar*) (diffImage->imageData + i * diffImage->widthStep);
		
		for( int j = 0; j < diffImage->width; j++ )
		{
			if( ptr[j] > 0  && j >= minX && j <= maxX && i >= minY && i <= maxY)
			{
				if( j < tempX) tempX = j;
				if( i < tempY) tempY = i;
				count++;
			}
		}
	}
	
	//tempY -= minY;
	//tempX -= minX;
	
	//Propably finger and not noise
	if(count >= 50)
	{
		fingerX = tempX;
		fingerY = tempY;
	}

	else
	{
		fingerX = fingerX;
		fingerY = fingerY;
	}
}




int Painter::PixelsInSmall()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= smallTopLeft.x && i < smallBottomRight.x && j >= smallTopLeft.y && j < smallBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInMedium()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= mediumTopLeft.x && i < mediumBottomRight.x && j >= mediumTopLeft.y && j < mediumBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInLarge()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= largeTopLeft.x && i < largeBottomRight.x && j >= largeTopLeft.y && j < largeBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInLineTool()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= lineToolTopLeft.x && i < lineToolBottomRight.x && j >= lineToolTopLeft.y && j < lineToolBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInRed()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= redTopLeft.x && i< redBottomRight.x && j>= redTopLeft.y && j<redBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInGreen()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= greenTopLeft.x && i < greenBottomRight.x && j >= greenTopLeft.y && j < greenBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInBlue()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= blueTopLeft.x && i < blueBottomRight.x && j >= blueTopLeft.y && j < blueBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInEraser()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= eraserTopLeft.x && i < eraserBottomRight.x && j >= eraserTopLeft.y && j < eraserBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




int Painter::PixelsInCanvas()
{
	int howMany = 0;

	for(int i = fingerX + 35 ; i < fingerX + 45; i++)
	{
		for(int j = fingerY ; j < fingerY + 10; j++)
		{
			if( i >= canvasTopLeft.x && i < canvasBottomRight.x && j >= canvasTopLeft.y && j < canvasBottomRight.y)
			{
				howMany++;
			}
		}
	}

	return howMany;
}




void Painter::clearCanvas()
{
	for( int y=0; y < canvasImage->height; y++ )
	{
		uchar* ptr = (uchar*) (canvasImage->imageData + y * canvasImage->widthStep);
		
		for( int x=0; x < canvasImage->width; x++ )
		{
			if(x>= canvasTopLeft.x + 2 && x<=canvasBottomRight.x - 1 && y>=canvasTopLeft.y + 2 && y<=canvasBottomRight.y - 1)
			{
				ptr[3*x + 0] = 255;
				ptr[3*x + 1] = 255;
				ptr[3*x + 2] = 255;
			}
		}
	}
}




int Painter::action()
{
	if(!validFingerCoordinates()) return 0;

	int howManyinGreen    = PixelsInGreen();
	int howManyinRed      = PixelsInRed();
	int howManyinBlue     = PixelsInBlue();
	int howManyinEraser   = PixelsInEraser();
	int howManyinSmall    = PixelsInSmall();
	int howManyinMedium   = PixelsInMedium();
	int howManyinLarge    = PixelsInLarge();
	int howManyinLineTool = PixelsInLineTool();
	int howManyInCanvas   = PixelsInCanvas();

	if(howManyinGreen > 50)
	{
		framesInGreen++;
	}

	else
	{
		framesInGreen = 0;
	}

	if(howManyinRed > 50)
	{
		framesInRed++;
	}

	else
	{
		framesInRed = 0;
	}

	if(howManyinBlue > 50)
	{
		framesInBlue++;
	}

	else
	{
		framesInBlue = 0;
	}

	if(howManyinEraser > 50)
	{
		framesInEraser++;
	}

	else
	{
		framesInEraser = 0;
	}

	if(howManyinSmall > 50)
	{
		framesInSmall++;
	}

	else
	{
		framesInSmall = 0;
	}

	if(howManyinMedium > 50)
	{
		framesInMedium++;
	}

	else
	{
		framesInMedium = 0;
	}

	if(howManyinLarge > 50)
	{
		framesInLarge++;
	}

	else
	{
		framesInLarge = 0;
	}

	if(howManyinLineTool > 50)
	{
		framesInLineTool++;
	}

	else
	{
		framesInLineTool = 0;
	}

	if(howManyInCanvas > 50)
	{
		if(freePaintingSelected)
		{
			circleX = fingerX + 40;
			circleY = fingerY + 5;
			disableFinger();

			return 12;
		}

		else if(lineToolSelected)
		{
			if(x1Paint == 0 || y1Paint == 0)
			{
				x1Paint = fingerX + 40;
				y1Paint = fingerY + 5;
			}

			else 
			{
				x2Paint = fingerX + 40;
				y2Paint = fingerY + 5;
			}

			framesInCanvas++;
		}
	}

	else
	{
		framesInCanvas = 0;
		x1Paint = 0;
		y1Paint = 0;
		x2Paint = 0;
		y2Paint = 0;
	}

	//Now change the tool if needed.
	if(framesInRed == framesToWait)
	{
		framesInRed = 0;
		disableFinger();

		return 1;
	}
	
	else if(framesInGreen == framesToWait)
	{
		framesInGreen = 0;
		disableFinger();

		return 2;
	}

	else if(framesInBlue == framesToWait)
	{
		framesInBlue = 0;
		disableFinger();

		return 3;
	}

	else if(framesInEraser == framesToWait)
	{
		framesInEraser = 0;
		disableFinger();

		if (this->eraserSelected) return 11;
		else return 4;
	}

	else if(framesInLineTool == framesToWait)
	{
		framesInLineTool = 0;
		disableFinger();
		
		if(this->lineToolSelected) return 10;
		else return 5;
	}

	else if(framesInSmall == framesToWait)
	{
		framesInSmall = 0;
		disableFinger();

		return 6;
	}

	else if(framesInMedium == framesToWait)
	{
		framesInMedium = 0;
		disableFinger();

		return 7;
	}

	else if(framesInLarge == framesToWait)
	{
		framesInLarge = 0;
		disableFinger();

		return 8;
	}

	else if(framesInCanvas == framesToPaint)
	{
		framesInCanvas = 0;
		disableFinger();

		return 9;
	}

	else
	{
		return 0;
	}
}




void Painter::changeToSmall()
{
	smallSelected = true;
	cvRectangle(canvasImage , cvPoint(smallTopLeft.x + 10 , smallTopLeft.y+10) , cvPoint(smallTopLeft.x + 20 , smallTopLeft.y+20) , YELLOW , 3);	
	
	mediumSelected = false;
	cvRectangle(canvasImage , cvPoint(mediumTopLeft.x + 10  , mediumTopLeft.y+10) , cvPoint(mediumTopLeft.x + 20 , mediumTopLeft.y+20) , BLACK , 3);

	largeSelected = false;
	cvRectangle(canvasImage , cvPoint(largeTopLeft.x + 10 , largeTopLeft.y+10) , cvPoint(largeTopLeft.x + 20  , largeTopLeft.y+20) , BLACK , 3);

	penSize = smallSize;
}




void Painter::changeToMedium()
{
	smallSelected = false;
	cvRectangle(canvasImage , cvPoint(smallTopLeft.x + 10 , smallTopLeft.y+10) , cvPoint(smallTopLeft.x + 20 , smallTopLeft.y+20) , BLACK , 3);
	
	mediumSelected = true;
	cvRectangle(canvasImage , cvPoint(mediumTopLeft.x + 10  , mediumTopLeft.y+10) , cvPoint(mediumTopLeft.x + 20 , mediumTopLeft.y+20) , YELLOW , 3);

	largeSelected = false;
	cvRectangle(canvasImage , cvPoint(largeTopLeft.x + 10 , largeTopLeft.y+10) , cvPoint(largeTopLeft.x + 20  , largeTopLeft.y+20) , BLACK , 3);

	penSize = mediumSize;
}




void Painter::changeToLarge()
{
	smallSelected = false;
	cvRectangle(canvasImage , cvPoint(smallTopLeft.x + 10 , smallTopLeft.y+10) , cvPoint(smallTopLeft.x + 20 , smallTopLeft.y+20) , BLACK , 3);

	mediumSelected = false;
	cvRectangle(canvasImage , cvPoint(mediumTopLeft.x + 10  , mediumTopLeft.y+10) , cvPoint(mediumTopLeft.x + 20 , mediumTopLeft.y+20) , BLACK , 3);

	largeSelected = false;
	cvRectangle(canvasImage , cvPoint(largeTopLeft.x + 10 , largeTopLeft.y+10) , cvPoint(largeTopLeft.x + 20  , largeTopLeft.y+20) , YELLOW , 3);

	penSize = largeSize;
}




void Painter::activateLineTool()
{
	freePaintingSelected = false;
	lineToolSelected = true;

	cvRectangle(canvasImage , cvPoint(lineToolTopLeft.x + 10 , lineToolTopLeft.y + 10) , cvPoint(lineToolTopLeft.x + 20 , lineToolTopLeft.y+20) , YELLOW , 3);

	if(!eraserSelected)
	{
		if      (redSelected)   currentColor = RED;
		else if (greenSelected) currentColor = GREEN;
		else					currentColor = BLUE;
	}				
}




void Painter::activateFreePaintingTool()
{
	freePaintingSelected = true;
	lineToolSelected = false;

	cvRectangle(canvasImage , cvPoint(lineToolTopLeft.x + 10 , lineToolTopLeft.y + 10) , cvPoint(lineToolTopLeft.x + 20 , lineToolTopLeft.y+20) , BLACK , 3);

	if(!eraserSelected)
	{
		if      (redSelected)   currentColor = RED;
		else if (greenSelected) currentColor = GREEN;
		else					currentColor = BLUE;
	}
}




void Painter::changeToRed()
{
	redSelected = true;
	cvRectangle(canvasImage , cvPoint(redTopLeft.x + 10 , redTopLeft.y+10) , cvPoint(redTopLeft.x + 20 , redTopLeft.y+20) , YELLOW , 3);

	greenSelected = false;
	cvRectangle(canvasImage , cvPoint(greenTopLeft.x + 10  , greenTopLeft.y+10) , cvPoint(greenTopLeft.x + 20  , greenTopLeft.y+20) , BLACK , 3);
				
	blueSelected = false;
	cvRectangle(canvasImage , cvPoint(blueTopLeft.x + 10  , blueTopLeft.y+10) , cvPoint(blueTopLeft.x + 20  , blueTopLeft.y+20) , BLACK , 3);
			
	if(!eraserSelected) currentColor = RED;
}




void Painter::changeToGreen()
{
	redSelected = false;
	cvRectangle(canvasImage , cvPoint(redTopLeft.x + 10 , redTopLeft.y+10) , cvPoint(redTopLeft.x + 20 , redTopLeft.y+20) , BLACK, 3);

	greenSelected = true;
	cvRectangle(canvasImage , cvPoint(greenTopLeft.x + 10  , greenTopLeft.y+10) , cvPoint(greenTopLeft.x + 20  , greenTopLeft.y+20) , YELLOW , 3);


	blueSelected = false;
	cvRectangle(canvasImage , cvPoint(blueTopLeft.x + 10  , blueTopLeft.y+10) , cvPoint(blueTopLeft.x + 20  , blueTopLeft.y+20) , BLACK , 3);
				
	if(!eraserSelected) currentColor = GREEN;
}




void Painter::changeToBlue()
{
	redSelected = false;
	cvRectangle(canvasImage , cvPoint(redTopLeft.x + 10 , redTopLeft.y+10) , cvPoint(redTopLeft.x + 20 , redTopLeft.y+20) , BLACK , 3);

	greenSelected = false;
	cvRectangle(canvasImage , cvPoint(greenTopLeft.x + 10  , greenTopLeft.y+10) , cvPoint(greenTopLeft.x + 20  , greenTopLeft.y+20) , BLACK , 3);

	blueSelected = true;
	cvRectangle(canvasImage , cvPoint(blueTopLeft.x + 10  , blueTopLeft.y+10) , cvPoint(blueTopLeft.x + 20  , blueTopLeft.y+20) , YELLOW , 3);
	
	if(!eraserSelected) currentColor = BLUE;
}




void Painter::activateEraser()
{
	eraserSelected = true;

	cvRectangle(canvasImage , cvPoint(eraserTopLeft.x + 10  , eraserTopLeft.y+10) , cvPoint(eraserTopLeft.x + 20  , eraserTopLeft.y+20) , YELLOW , 3);
	
	currentColor = WHITE;
}




void Painter::deactivateEraser()
{
	eraserSelected = false;

	cvRectangle(canvasImage , cvPoint(eraserTopLeft.x + 10  , eraserTopLeft.y+10) , cvPoint(eraserTopLeft.x + 20  , eraserTopLeft.y+20) , BLACK , 3);

	if (this->lineToolSelected)			 this -> activateLineTool();
	else if(this->freePaintingSelected)	 this -> activateFreePaintingTool();
}




bool Painter::validPoints()
{
	return (x1Paint > 0 && x2Paint > 0 && y1Paint > 0 && y2Paint > 0);
}




void Painter::paint(IplImage *img)
{
	cvLine(img , cvPoint(x1Paint , y1Paint) , cvPoint(x2Paint , y2Paint) , WHITE , penSize);
}




void Painter::paintLine(const CvPoint &start , const CvPoint &end)
{
	cvLine(canvasImage , start , end , currentColor, penSize, 8 , 0 );
}




void Painter::paintCircle()
{
	cvCircle(canvasImage , cvPoint(circleX , circleY) , 1 , currentColor , penSize);
}




void Painter::prepareForNextPaint()
{
	x1Paint = x2Paint;
	y1Paint = y2Paint;
	x2Paint = 0;
	y2Paint = 0;
	framesInCanvas = 0;
}




CvPoint Painter:: getCanvasTopLeft() 
{
	return canvasTopLeft;
}




CvPoint Painter:: getCanvasBottomRight() 
{
	return canvasBottomRight;
}




void Painter::clearFrameCounters()
{
	this->framesInSmall = 0;
	this->framesInMedium = 0;
	this->framesInLarge = 0;
	this->framesInLineTool = 0;
	this->framesInRed = 0;
	this->framesInGreen = 0;
	this->framesInBlue = 0;
	this->framesInEraser = 0;
	this->fingerX = -1;
	this->fingerY = -1;
	this->x1Paint = 0;
	this->y1Paint = 0;
	this->x2Paint = 0;
	this->y2Paint = 0;
}




void Painter::disableFinger()
{
	this->fingerX = -1;
	this->fingerY = -1;
}




bool Painter::validFingerCoordinates()
{
	return (this->fingerX >= 0 && this->fingerY >=0);
}