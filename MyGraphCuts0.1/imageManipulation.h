#ifndef _IMAGE_MANIPULATION_H_
#define _IMAGE_MANIPLUATION_H_

int inflate(const unsigned char* image, const int width, const int height, unsigned char* output);
int erode(const unsigned char* image, const int width, const int height, unsigned char* output);
int substract(const unsigned char* image1, const unsigned char* image2, const int width, const int height, unsigned char* output);
int openOp(const unsigned char* image, const int width, const int height, unsigned char* output);
int closeOp(const unsigned char* image, const int width, const int height, unsigned char* output);
int openContour(const unsigned char* image, const int width, const int height, unsigned char* output);
int closeContour(const unsigned char* image, const int width, const int height, unsigned char* output);


#endif