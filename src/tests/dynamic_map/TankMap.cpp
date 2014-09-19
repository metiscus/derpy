#include "TankMap.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

template <typename T>
inline void iir_smoothing_filter(T* array, unsigned int arrayLength, double coeff)
{
	T value = array[0];
	for(unsigned int ii=0; ii<arrayLength; ++ii)
	{
		value = coeff * value + (1. - coeff) * array[ii];
		array[ii] = value;
	}
}

TankMap::TankMap()
	: mWidth(0)
	, mData(NULL)
{
	;
}

TankMap::~TankMap()
{
	delete [] mData;
}

void TankMap::set(float* points, int width)
{
	delete [] mData;
	mData = new float [width];

	for(int ii=0; ii<width; ++ii)
	{
		mData[ii] = points[ii];
	}

	mWidth = width;
}

void TankMap::generate(unsigned int seed, TankMapParams params)
{
	delete [] mData;
	mData = new float [params.width];

	mWidth = params.width;

	if(mData == NULL)
	{
		fprintf(stderr, "Error: Allocating tank map.\n");
		return;
	}

	// seed the random number generator
	srand(seed);

	// generate some spikes 
	for(unsigned int ii=0; ii<params.width; ++ii)
	{
		float U1 = (float)((double)rand() / (double)RAND_MAX);
		float U2 = (float)((double)rand() / (double)RAND_MAX);

		mData[ii] = sqrt(params.variance * -2.f * log(U1) ) * cosf(U2 * 3.14159f) + params.mean;
		mData[ii] = (mData[ii] < params.min_height) ? params.min_height : mData[ii];
		mData[ii] = (mData[ii] > params.max_height) ? params.max_height : mData[ii];
	}

	// use a simple IIR filter to smooth the terrain
	for(int pass=0; pass<params.smoothing_passes; ++pass)
	{
		iir_smoothing_filter(mData, params.width, params.smoothing_coeff);	
	}

}

float TankMap::getHeight(int x) const
{
	if(mData && x<mWidth)
	{
		return mData[x];
	}
	else 
	{
		return 0.0f;
	}
}

int TankMap::getWidth() const
{
	return mWidth;
}

float* TankMap::getHeights()
{
	return mData;
}

void TankMap::smoothRange(int begin, int end, int passes, float coeff)
{
	// use a simple IIR filter to smooth the terrain
	for(int pass=0; pass<passes; ++pass)
	{
		iir_smoothing_filter(mData + begin, end - begin, coeff);	
	}
}