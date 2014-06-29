#include "MyLib.h"


float percentDiff( const IplImage * src1, const IplImage * src2, int sensitivity )
{
	int changed = 0;
	int allofthem = (src1->height) * (src1->width);
	for( int y=0; y < src1->height; y++ )
	{
		uchar* ptr1 = (uchar*) (src1->imageData + y * src1->widthStep);
		uchar* ptr2 = (uchar*) (src2->imageData + y * src2->widthStep);
		for( int x=0; x < src1->width; x++ )
		{
			if( abs( ptr1[x] - ptr2[x] ) > sensitivity ) changed++;
		}
	}

	return (float)changed/(float)allofthem;
}




void printDiff(const IplImage * src1, const IplImage * src2, IplImage * dst, int sensitivity)
{
	for( int y=0; y < src1->height; y++ )
	{
		uchar* ptr1 = (uchar*) (src1->imageData + y * src1->widthStep);
		uchar* ptr2 = (uchar*) (src2->imageData + y * src2->widthStep);
		uchar* ptr3 = (uchar*) (dst->imageData + y * dst->widthStep);
		
		for( int x=0; x < src1->width; x++ )
		{
			if( abs( ptr1[x] - ptr2[x] ) > sensitivity )
			{
				if(ptr1[x] >= 70) ptr3[x] = 0;
				else              ptr3[x] = 255;
			}
		}
	}
}




void paintRed( IplImage * dst, const CvPoint point, int distance )
{
	for(int i = (point.y - distance); i < (point.y + distance); i++)
	{
		if (i >= dst->height) break;
		uchar* ptr = (uchar*) (dst->imageData + i * dst->widthStep);
		for( int j=(point.x - distance); j < (point.x + distance); j++ )
		{
			if (j >= dst->width) break;
			ptr[3*j] = 0; ptr[3*j+1] = 0; ptr[3*j+2] = 255;
		}
	}
}




void makeWhite( IplImage * dst )
{
	for( int y=0; y < dst->height; y++ )
	{
		uchar* ptr = (uchar*) (dst->imageData + y * dst->widthStep);

		for( int x=0; x < dst->width; x++ )
		{
			if(dst->nChannels > 1)
			{
				ptr[3*x] = 255; ptr[3*x+1] = 255; ptr[3*x+2] = 255;
			}
			
			else ptr[x] = 255;
		}
	}
}




void makeBlack( IplImage * dst )
{
	for( int y=0; y < dst->height; y++ )
	{
		uchar* ptr = (uchar*) (dst->imageData + y * dst->widthStep);

		for( int x=0; x < dst->width; x++ )
		{
			if(dst->nChannels > 1)
			{
				ptr[3*x] = 0; ptr[3*x+1] = 0; ptr[3*x+2] = 0;
			}
			
			else ptr[x] = 0;
		}
	}
}