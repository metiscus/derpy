#ifndef TANKMAP_H_
#define TANKMAP_H_

#include "Noncopyable.h"

struct TankMapParams
{
	int width;
	int max_height;
	int min_height;
	float variance;
	float mean;
	int smoothing_passes;
	float smoothing_coeff;
};

class TankMap : public noncopyable
{
public:
	TankMap();
	~TankMap();

	void set(float* points, int width);

	void generate(unsigned int seed, TankMapParams params);
	float getHeight(int x) const;
	int getWidth() const;

	float* getHeights();

	void smoothRange(int begin, int end, int passes, float coeff);

private:
	int mWidth;
	float *mData;
};

#endif //TANKMAP_H_