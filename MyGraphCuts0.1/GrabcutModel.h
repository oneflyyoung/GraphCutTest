#pragma once

#include <cv.h>

struct tagPixel
{
	int N[4];  // neighbour   top bottom left right
	float B[4];
	float Ro;
	float Rb;
	void* node;
};

struct tagCOLOR 
{
	BYTE b;
	BYTE g;
	BYTE r;
	int operator - (const tagCOLOR & x)
	{
		return (int(r)-int(x.r))*(int(r)-int(x.r)) +
			(int(g)-int(x.g))*(int(g)-int(x.g)) +
			(int(b)-int(x.b))*(int(b)-int(x.b));
	}
};

class CGrabcutModel
{
public:
	CGrabcutModel(void);
	~CGrabcutModel(void);

private:
	tagCOLOR*	m_image;
	BYTE*		m_mask;
	BYTE*		m_result;
	tagPixel*	m_pixel;

	int*		m_To;		// Obj pixel list
	int*		m_Tb;		// Bkg pixel list
	int			m_numObj;
	int			m_numBkg;


	float		m_sigma;
	static const int m_numOfGMM = 5;

	int			m_width;
	int			m_height;
	int			m_size;

public:
	static const BYTE OBJ = 1;
	static const BYTE BKG = 2;
	static const BYTE UND = 0;
	float		m_lamda;


private:
	int   collect(void);
	float calcB(int Ip, int Iq, float sigma);
	int   buildGMM(const int * pixels, const int numInstances, const int numOfGMM, float* * cov, float * det_cov, float* * inv_cov, float* * mean, float * pi);
	float calcP(tagCOLOR x, float* mu, float det_cov, float* inv_cov);

public:
	int setImage(BYTE* image, int width, int height);
	int setMask(BYTE* mask);
	BYTE* doCut();
};
