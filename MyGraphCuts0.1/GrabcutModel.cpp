#include "StdAfx.h"
#include ".\grabcutmodel.h"
#include <math.h>
#include "mygraph.h"

#define max(a,b) ( ((a)>(b)) ? (a):(b) )
#define min(a,b) ( ((a)>(b)) ? (b):(a) )


#ifndef DELETE_SAFE
#define DELETE_SAFE(x) {if (x) delete [](x); (x) = NULL;}
#endif
CGrabcutModel::CGrabcutModel(void)
{
	m_image = NULL;
	m_mask = NULL;
	m_result = NULL;
	m_pixel = NULL;
	m_Tb = NULL;
	m_To = NULL;

	m_lamda = 0.01;
	//m_sigma = 15;
}

CGrabcutModel::~CGrabcutModel(void)
{
	DELETE_SAFE(m_result);
	DELETE_SAFE(m_pixel);
	DELETE_SAFE(m_To);
	DELETE_SAFE(m_Tb);
}

int CGrabcutModel::setImage(BYTE* image, int width, int height) 
{
	m_width = width;
	m_height = height;
	m_size = width * height;
	m_image = (tagCOLOR*) image;
	DELETE_SAFE(m_pixel);
	m_pixel = new tagPixel[m_size];
	m_result = new BYTE[m_size];

	for (int i=0;i<height;i++) 
	{  // initialize neighborhood
		int offst = i * width;
		for (int j=0;j<width;j++) 
		{
			int t = offst + j;
			m_pixel[t].N[0] = (i==0)?(t):(t-width);
			m_pixel[t].N[1] = (i==height-1)?(t):(t+width);
			m_pixel[t].N[2] = (j==0)?(t):(t-1);
			m_pixel[t].N[3] = (j==width-1)?(t):(t+1);
		}
	}

	{
		float color1[3], color2[3];
		m_sigma=0;
		int cnt=0;
		int i,j;
		for (i=0; i<height-1; i++)
		{
			int offst = i * width;
			for (j=0;j<width-1;++j)
			{
				m_sigma += m_image[offst+j] - m_image[offst+j+1];
				m_sigma += m_image[offst+j] - m_image[offst+j+width];
				cnt+=2;
			}
		}
		m_sigma /= cnt;
		m_sigma = 0.5f / m_sigma;
	}


	for (int i=0;i<height;i++) 
	{  // initialize B(x)
		int offst = i * width;
		for (int j=0;j<width;j++) 
		{
			int c = offst + j;
			int nb = m_pixel[c].N[1];  // bottom neighbour
			int nr = m_pixel[c].N[3];  // right neighbour
			m_pixel[c].B[0] = m_pixel[c].B[2] = 0.0f;
 			m_pixel[c].B[1] = m_pixel[nb].B[0] = calcB(c, nb, m_sigma);
			m_pixel[c].B[3] = m_pixel[nr].B[2] = calcB(c, nr, m_sigma);
		}
	}
	return 0;
}
int CGrabcutModel::setMask(BYTE* mask)
{
	if (!m_image) return -1;
	m_mask = mask;
	DELETE_SAFE(m_To);
	DELETE_SAFE(m_Tb);
	return 0;
}

int CGrabcutModel::collect(void)
{
	int i;
	int sumTo, sumTb;

	if (m_Tb || m_To) return -1;

	sumTo = sumTb = 0;
	for (i=0;i<m_size;i++) 
	{  // how many pixels are OBJ or BKG
		if (m_mask[i] == OBJ)
			sumTo ++;
		else if (m_mask[i] == BKG)
			sumTb ++;
	}
	m_To = new int[sumTo];
	m_Tb = new int[sumTb];
	
	sumTo = sumTb = 0;
	for (i=0;i<m_size;i++) 
	{  // collect them
		if (m_mask[i] == OBJ) 
		{
			m_To[sumTo] = i;
			sumTo ++;
		} 
		else if (m_mask[i] == BKG) 
		{
			m_Tb[sumTb] = i;
			sumTb ++;
		}
	}
	m_numObj = sumTo;
	m_numBkg = sumTb;

	return 0;
}

BYTE* CGrabcutModel::doCut() 
{
	int i,j;

	collect();  // collect OBJ list and BKG list
	
	{ // build Obj and Bkg GMM
		float* * o_cov = new float* [m_numOfGMM];
		float * o_det_cov = new float [m_numOfGMM];
		float* * o_inv_cov = new float* [m_numOfGMM];
		float* * o_mean = new float* [m_numOfGMM];
		float * o_pi = new float [m_numOfGMM];

		float* * b_cov = new float* [m_numOfGMM];
		float * b_det_cov = new float [m_numOfGMM];
		float* * b_inv_cov = new float* [m_numOfGMM];
		float* * b_mean = new float* [m_numOfGMM];
		float * b_pi = new float [m_numOfGMM];

		buildGMM(m_To, m_numObj, m_numOfGMM, o_cov, o_det_cov, o_inv_cov, o_mean, o_pi);
		buildGMM(m_Tb, m_numBkg, m_numOfGMM, b_cov, b_det_cov, b_inv_cov, b_mean, b_pi);

		float t, Rb, Ro;
		for (i=0;i<m_size;i++) 
		{
			Ro = 10000; Rb = 10000;
			for (j=0;j<m_numOfGMM;j++)
			{
				t = calcP(m_image[i], o_mean[j], o_det_cov[j], o_inv_cov[j]);
				Ro = min(Ro, t);
				t = calcP(m_image[i], b_mean[j], b_det_cov[j], b_inv_cov[j]);
				Rb = min(Rb, t);
			}
			m_pixel[i].Ro = Ro;
			m_pixel[i].Rb = Rb;
		}

		for (i=0;i<m_numOfGMM;i++) 
		{
			delete [](o_cov[i]);
			delete [](o_inv_cov[i]);
			delete [](o_mean[i]);

			delete [](b_cov[i]);
			delete [](b_inv_cov[i]);
			delete [](b_mean[i]);
		}
		delete[] o_cov;
		delete[] o_det_cov ;
		delete[] o_inv_cov;
		delete[] o_mean;
		delete[] o_pi;

		delete[] b_cov;
		delete[] b_det_cov;
		delete[] b_inv_cov;
		delete[] b_mean;
		delete[] b_pi;

	}

	float K = 0.0f;
	for (i=0;i<m_size;i++)
	{
		float sum = m_pixel[i].B[0] + m_pixel[i].B[1] + 
					m_pixel[i].B[2] + m_pixel[i].B[3] ;
		K = max(K, sum);
	}
	K += 1.0f;

	Graph graph;
	for (i=0;i<m_size;i++) 
	{
		m_pixel[i].node = graph.add_node();
	}
	for (i=0;i<m_size;i++) 
	{
		if (m_mask[i] == CGrabcutModel::OBJ)
		{
			graph.add_tweights(m_pixel[i].node, K, 0);
		} 
		else if (m_mask[i] == CGrabcutModel::BKG) 
		{
			graph.add_tweights(m_pixel[i].node, 0, K);
		} 
		else
		{
			graph.add_tweights(m_pixel[i].node, m_lamda * m_pixel[i].Rb, m_lamda * m_pixel[i].Ro);
		}
	}

	for (i=0;i<m_height-1;i++) 
	{
		int offst = i * m_width;
		for (j=0;j<m_width-1;j++) 
		{
			int c,r,b;
			c = offst + j;
			r = m_pixel[c].N[3];
			b = m_pixel[c].N[1];
			graph.add_edge(m_pixel[c].node, m_pixel[r].node, m_pixel[c].B[3], m_pixel[c].B[3]);
			graph.add_edge(m_pixel[c].node, m_pixel[b].node, m_pixel[c].B[1], m_pixel[c].B[1]);
		}
	}
	
	graph.maxflow();

	for (i=0;i<m_size;i++) 
	{
		if (graph.what_segment(m_pixel[i].node) == graph.SOURCE)
			m_result[i] = 0; //CGrabcutModel::OBJ;
		else
			m_result[i] = 1; //CGrabcutModel::BKG;
	}	
	

	
	return m_result;
}

int CGrabcutModel::buildGMM(const int * pixels, const int numInstances, const int numOfGMM, float* * cov, float * det_cov, float* * inv_cov, float* * mean, float * pi)
{
	CvMat* pSample=cvCreateMat(numInstances,3,CV_32FC1);  // allocate memory for GMM

	for(int i=0;i<numInstances;i++) {  // assignment
		tagCOLOR color;
		color = m_image[pixels[i]];
		cvmSet(pSample,i,0,color.r);
		cvmSet(pSample,i,1,color.g);
		cvmSet(pSample,i,2,color.b);
	}

	int* m_pIDs = new int[numInstances];  
	CvMat* pIDMat = cvCreateMat(numInstances, 1, CV_32SC1);
	cvKMeans2(pSample,numOfGMM, pIDMat, cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 1000, 1e-10 ));  // K-mean
	
	for(int i=0; i<numInstances; i++) { // results from GMM
		m_pIDs[i] = pIDMat->data.i[i];
	}
	cvReleaseMat(&pIDMat);


	//统计每个高斯模型的元素个数
	int *pNumInstOfCluster = new int[numOfGMM];
	memset(pNumInstOfCluster, 0, sizeof(int)*numOfGMM);
	for(int i=0;i<numInstances;i++)
	{
		pNumInstOfCluster[m_pIDs[i]]++;
	}

	// 输出权重
	for (int i=0;i<numOfGMM;i++) 
	{
		pi[i] = (float) pNumInstOfCluster[i] / (float)numInstances;
	}

	// 建立数据
	int iCluster;
	CvMat*** pppVects = new CvMat**[numOfGMM];
	for(iCluster=0; iCluster<numOfGMM; iCluster++)
	{
		pppVects[iCluster] = (CvMat**)cvAlloc(sizeof(CvMat*)*pNumInstOfCluster[iCluster]);
	}

	int	*pVectIndex = new int[numOfGMM];	// index of current vector for each cluster
	memset(pVectIndex, 0, sizeof(int)*numOfGMM);

	int iIndex;
	for(int i=0;i<numInstances;i++)	
	{  // cluster collection
		iCluster = m_pIDs[i];
		iIndex = pVectIndex[iCluster]++;
		pppVects[iCluster][iIndex]=cvCreateMat(1,3,CV_32FC1);

		cvmSet(pppVects[iCluster][iIndex],0,0,cvmGet(pSample,i,0));  // red
		cvmSet(pppVects[iCluster][iIndex],0,1,cvmGet(pSample,i,1));  // green
		cvmSet(pppVects[iCluster][iIndex],0,2,cvmGet(pSample,i,2));  // blue
	}

	//对于每个高斯模型，计算均值和方差

	CvMat* cv_cov  = cvCreateMat(3,3,CV_32FC1);
	CvMat* cv_mean = cvCreateMat(1,3,CV_32FC1);
	CvMat* cv_inv_cov = cvCreateMat(3,3,CV_32FC1);

	for(iCluster=0; iCluster<numOfGMM; iCluster++) 
	{
		cov[iCluster]  = new float [9]; //cvCreateMat(3,3,CV_32FC1);
		mean[iCluster] = new float [3]; //cvCreateMat(1,3,CV_32FC1);
		inv_cov[iCluster] = new float [9]; //cvCreateMat(3,3,CV_32FC1);
		
		cvCalcCovarMatrix((const CvArr**)pppVects[iCluster], pNumInstOfCluster[iCluster],
			cv_cov, cv_mean, CV_COVAR_NORMAL + CV_COVAR_SCALE);			// 方差
		det_cov[iCluster] = (float)cvDet(cv_cov);
		cvInvert(cv_cov, cv_inv_cov);
		cov[iCluster][0] = cvmGet(cv_cov, 0, 0);
		cov[iCluster][1] = cvmGet(cv_cov, 0, 1);
		cov[iCluster][2] = cvmGet(cv_cov, 0, 2);
		cov[iCluster][3] = cvmGet(cv_cov, 1, 0);
		cov[iCluster][4] = cvmGet(cv_cov, 1, 1);
		cov[iCluster][5] = cvmGet(cv_cov, 1, 2);
		cov[iCluster][6] = cvmGet(cv_cov, 2, 0);
		cov[iCluster][7] = cvmGet(cv_cov, 2, 1);
		cov[iCluster][8] = cvmGet(cv_cov, 2, 2);
		mean[iCluster][0] = cvmGet(cv_mean, 0, 0);
		mean[iCluster][1] = cvmGet(cv_mean, 0, 1);
		mean[iCluster][2] = cvmGet(cv_mean, 0, 2);
		inv_cov[iCluster][0] = cvmGet(cv_inv_cov, 0, 0);
		inv_cov[iCluster][1] = cvmGet(cv_inv_cov, 0, 1);
		inv_cov[iCluster][2] = cvmGet(cv_inv_cov, 0, 2);
		inv_cov[iCluster][3] = cvmGet(cv_inv_cov, 1, 0);
		inv_cov[iCluster][4] = cvmGet(cv_inv_cov, 1, 1);
		inv_cov[iCluster][5] = cvmGet(cv_inv_cov, 1, 2);
		inv_cov[iCluster][6] = cvmGet(cv_inv_cov, 2, 0);
		inv_cov[iCluster][7] = cvmGet(cv_inv_cov, 2, 1);
		inv_cov[iCluster][8] = cvmGet(cv_inv_cov, 2, 2);		
		//float a = cvmGet(mean[iCluster],0,0);
		//float b = cvmGet(mean[iCluster],0,1);
		//float c = cvmGet(mean[iCluster],0,2);
	}

	for(int i=0;i<numOfGMM;i++)	
	{
		for (int j=0;j<pNumInstOfCluster[i];j++) 
		{
			cvReleaseMat(pppVects[i] + j);
		}
		cvFree((void**)&pppVects[i]);
	}
	cvReleaseMat(&cv_cov);
	cvReleaseMat(&cv_mean);
	cvReleaseMat(&cv_inv_cov);
	delete [] pVectIndex;
	delete [] pppVects;
	delete [] pNumInstOfCluster;
	return 0;
}
float CGrabcutModel::calcB(int Ip, int Iq, float sigma) 
{
	if (Ip == Iq) return 0.0f;
	float t = (m_image[Ip]-m_image[Iq])*sigma;
	int y1 = Ip % m_width;
	int y2 = Iq % m_width;
	int x1 = Ip / m_width;
	int x2 = Iq / m_width;
	return exp(-t); // / sqrt(float((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

float CGrabcutModel::calcP(tagCOLOR x, float* mu, float det_cov, float* inv_cov) {
	float d[3];
	float* v;
	float t[3];
	float sum;

	d[0] = float(x.r) - mu[0]; //cvmGet(mu,0,0);
	d[1] = float(x.g) - mu[1]; //cvmGet(mu,0,1);
	d[2] = float(x.b) - mu[2]; //cvmGet(mu,0,2);

	v = inv_cov;
	/*v[0] = cvmGet(inv_cov,0,0);
	v[1] = cvmGet(inv_cov,0,1);
	v[2] = cvmGet(inv_cov,0,2);
	v[3] = cvmGet(inv_cov,1,0);
	v[4] = cvmGet(inv_cov,1,1);
	v[5] = cvmGet(inv_cov,1,2);
	v[6] = cvmGet(inv_cov,2,0);
	v[7] = cvmGet(inv_cov,2,1);
	v[8] = cvmGet(inv_cov,2,2);*/

	t[0] = d[0] * v[0] + d[1] * v[3] + d[2] * v[6];
	t[1] = d[0] * v[1] + d[1] * v[4] + d[2] * v[7];
	t[2] = d[0] * v[2] + d[1] * v[5] + d[2] * v[8];

	sum = (t[0] * d[0] + t[1] * d[1] + t[2] * d[2]);

	return -log(exp(-sum)/sqrt(248.0502*fabs(det_cov)/65535.0));


}