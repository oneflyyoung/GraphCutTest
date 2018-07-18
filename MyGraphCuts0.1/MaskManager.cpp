#include "StdAfx.h"
#include ".\maskmanager.h"

#ifndef DELETE_SAFE
#define DELETE_SAFE(x) {if ((x)) delete [](x); (x)=NULL;}
#endif
CMaskManager::CMaskManager(void)
{
	m_mask = NULL;
}

CMaskManager::~CMaskManager(void)
{
	DELETE_SAFE(m_mask);
}

int	CMaskManager::initMask(int width, int height, BYTE value)
{
	DELETE_SAFE(m_mask);
	m_mask = new BYTE[width*height];
	m_width = width;
	m_height = height;
	int size = width * height;
	for ( int i = 0 ; i < size; i ++ )
		m_mask[i] = value;
	return 0;
}

