#include "stdafx.h"

#include "imageManipulation.h"

#ifndef DELETE_SAFE
#define DELETE_SAFE(x) {if ((x)) delete [](x);(x)=NULL;}
#endif


#define _4_NEIGHBOUR



int inflate(const unsigned char* image, const int width, const int height, unsigned char* output)
{
	int i,j;
	int size;
	int offst, offst_p, offst_n;
	size = width * height;
	memset(output, 0, size);

	for (i=1;i<height-1;i++)
	{
		offst = i * width;
		offst_p = offst - width;
		offst_n = offst + width;
		
		for (j=1;j<width-1;j++)
		{
#ifdef _4_NEIGHBOUR
			// 4 neighbours
			output[offst+j] = (image[offst+j] || image[offst+j+1] || image[offst+j-1] || image[offst_p + j] ||	image[offst_n + j]);
#else
			// 8 neighbours
			output[offst+j] = image[offst+j] || image[offst+j+1] || image[offst+j-1] || 
							  image[offst_p+j] || image[offst_p+j+1] || image[offst_p+j-1] ||
							  image[offst_n+j] || image[offst_n+j+1] || image[offst_n+j-1]);
#endif
		
		}
	}
	return 0;

}

int erode(const unsigned char* image, const int width, const int height, unsigned char* output)
{
	int i,j;
	int size;
	int offst, offst_p, offst_n;
	size = width * height;
	memset(output, 0, size);

	for (i=1;i<height-1;i++)
	{
		offst = i * width;
		offst_p = offst - width;
		offst_n = offst + width;
		
		for (j=1;j<width-1;j++)
		{
#ifdef _4_NEIGHBOUR
			// 4 neighbours
			output[offst+j] = (image[offst+j] && image[offst+j+1] && image[offst+j-1] && image[offst_p + j] && image[offst_n + j]);
#else
			// 8 neighbours
			output[offst+j] = image[offst+j] && image[offst+j+1] && image[offst+j-1] && 
							  image[offst_p+j] && image[offst_p+j+1] && image[offst_p+j-1] &&
							  image[offst_n+j] && image[offst_n+j+1] && image[offst_n+j-1]);
#endif
		
		}
	}
	return 0;

}

int openOp(const unsigned char* image, const int width, const int height, unsigned char* output)
{
	unsigned char* temp = NULL;
	int flag = -1;
	__try
	{
		temp = new unsigned char[width*height];
		if (!temp) __leave;
		if (flag = erode(image, width, height, temp)) __leave;
		if (flag = inflate(temp, width, height, output)) __leave;
		flag = 0;
	}
	__finally
	{
		DELETE_SAFE(temp);
		return flag;
	}
}

int closeOp(const unsigned char* image, const int width, const int height, unsigned char* output)
{
	unsigned char* temp = NULL;
	int flag = -1;
	__try
	{
		temp = new unsigned char[width*height];
		if (!temp) __leave;
		if (flag = inflate(image, width, height, temp)) __leave;
		if (flag = erode(temp, width, height, output)) __leave;
		flag = 0;
	}
	__finally
	{
		DELETE_SAFE(temp);
		return flag;
	}
}

int substract(const unsigned char* image1, const unsigned char* image2, const int width, const int height, unsigned char* output)
{
	int i;
	int size;
	size = width * height;
	for (i=0;i<size;i++)
		output[i] = (image1[i] && !image2[i]);
	return 0;
}

int openContour(const unsigned char* image, const int width, const int height, unsigned char* output)
{
	unsigned char* temp = NULL;
	int flag = -1;
	__try
	{
		temp = new unsigned char[width*height];
		if (!temp) __leave;
		if (flag = inflate(image, width, height, temp)) __leave;
		if (flag = substract(temp, image, width, height, output)) __leave;
		flag = 0;
	}
	__finally
	{
		DELETE_SAFE(temp);
		return flag;
	}
}
int closeContour(const unsigned char* image, const int width, const int height, unsigned char* output)
{
		unsigned char* temp = NULL;
	int flag = -1;
	__try
	{
		temp = new unsigned char[width*height];
		if (!temp) __leave;
		if (flag = erode(image, width, height, temp)) __leave;
		if (flag = substract(image, temp, width, height, output)) __leave;
		flag = 0;
	}
	__finally
	{
		DELETE_SAFE(temp);
		return flag;
	}
}