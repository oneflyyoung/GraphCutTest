#pragma once

class CMaskManager
{
public:
	CMaskManager(void);
	~CMaskManager(void);
private:
	BYTE* m_mask;
	int m_width;
	int m_height;
public:
	int	 initMask(int width, int height, BYTE value);
	inline BYTE* getMask() {return m_mask;}
	inline int	 setMask(int x, int y, int w, int value) 
	{
		int i,j;
		for (i=y-w;i<=y+w;i++)
		{
			if (i>=m_height || i < 0)
				continue;

			for (j=x-w;j<=x+w;j++) 
			{
				if (j>=m_width || j < 0)
					continue;
				m_mask[i*m_width+j] = value;
			}
		}
		return 0;
	}
	inline int	 setMask(int index, int value) {return (m_mask[index] = value);}
};
