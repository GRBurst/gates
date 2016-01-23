/*
 * PerlinNoise.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: adrian
 */

#include "PerlinNoise.h"
PerlinNoise::PerlinNoise()
{
	mGradientNoise = new double* [mXDim];
	for (int i = 0; i < mXDim; i++){
		mGradientNoise[i] = new double[mYDim]();
	}
}


PerlinNoise::~PerlinNoise()
{
	// TODO Auto-generated destructor stub
}

void PerlinNoise::printParams()
{
    std::cout << "mXDim = " << mXDim << std::endl;
    std::cout << "mYDim = " << mYDim << std::endl;
    std::cout << "mSeed = " << mSeed << std::endl;
    std::cout << "mOctaves = " << mOctaves << std::endl;
    std::cout << "mStart = " << mStart << std::endl;
    std::cout << "mFrequency = " << mFrequency << std::endl;
    std::cout << "mAmplitude = " << mAmplitude << std::endl;
    std::cout << "mMin = " << mMin << std::endl;
    std::cout << "mMax = " << mMax << std::endl;
	std::cout << "mSampleSize = " << mSampleSize << std::endl;// = 256;

    /* std::cout << "first gradients = " */ 

    std::cout << "mNoiseValues values:" << std::endl;
	for(int y = 0; y < mYDim; y++){
	    for(int x = 0; x < mXDim; x++){
            if(x == mXDim-1) std::cout << mNoiseValues[y * mXDim + x] << std::endl;
            else std::cout << mNoiseValues[y * mXDim + x] << ", ";
		}
	}
	
    /* std::cout << "mGradientTable2d values:" << std::endl; */
    /* for (std::vector<glm::dvec2>::iterator it = mGradientTable2d.begin() ; it != mGradientTable2d.end(); ++it) */
    /*         std::cout << "x = " << it->x << ", y =  " <<  it->y << std::endl; */
		
	/* for (int i = 0; i < mXDim; i++){ */
	/* 	mGradientNoise[i] = new double[mYDim](); */
	/* } */
    std::cout << "mPermutationTable values:" << std::endl;
    for (std::vector<int>::iterator it = mPermutationTable.begin() ; it != mPermutationTable.end(); ++it)
        std::cout << ' ' << *it;
}

void PerlinNoise::generateNoiseImage(){
	if(mStart){
		mStart = false;
		initPermutationTable();
		initGradientTable();
	}

    /* printParams(); */
	for(int x = 0; x < mXDim; x++){
		for(int y = 0; y < mYDim; y++){
			float value = static_cast<float>(noise(static_cast<double>(x), static_cast<double>(y)));
			mNoiseValues[y * mXDim + x] = value;
			if (value < mMin)
				mMin = value;
			if (value > mMax)
				mMax = value;
		}
	}
    if((mMin < 0.0) || (mMax > 1.0))
    {
	    mMax = mMax - mMin;
        for(int x = 0; x < mXDim; x++){
            for(int y = 0; y < mYDim; y++){
                mNoiseValues[y * mXDim + x] = (mNoiseValues[y * mXDim + x] - mMin) / mMax;
            }
        }
    }
    std::cout << "min = " << mMin << ", max = " << mMax << std::endl;


}

double PerlinNoise::noise(double x, double y){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
	x *= mFrequency;
	y *= mFrequency;
	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue(x, y) * amp;
		x *= 2.0;
		y *= 2.0;
		amp *= 0.5;
	}
	return result;
}

//double clamp(double x, double min, double max){
//	if (x > max)
//		x = max;
//	if (x < min)
//		x = min;
//	return x;
//}

double PerlinNoise::calculateNoiseValue(double x, double y){
	int N = 4096;
	glm::dvec2 inputVector(x, y);
	int b00, b10, b01, b11;
	double a, b, u, v;
	int i, j;
	glm::ivec2 bx, by;
	glm::dvec2 t, rx, ry, s, q;
	t = inputVector + glm::dvec2(N, N);
	//std::cout << t.x << ", " << t.y << std::endl;
	bx.x = ((int)t.x) & (mSampleSize - 1); //faster modulo operation for 2^n values
	bx.y = (bx.x + 1) & (mSampleSize - 1);

	rx.x = t.x - (int)t.x;
	rx.y = rx.x - 1.0;
//	std::cout << bx.x << ", " << bx.y << std::endl;
//	std::cout << rx.x << ", " << rx.y << std::endl;

	by.x = ((int)t.y) & (mSampleSize - 1);
	by.y = (by.x + 1) & (mSampleSize - 1);

	ry.x = t.y - (int)t.y;
	ry.y = ry.x - 1.0;
//	std::cout << by.x << ", " << by.y << std::endl;
//	std::cout << ry.x << ", " << ry.y << std::endl;
//	std::cin.get();
	i = mPermutationTable[bx.x];
	j = mPermutationTable[bx.y];

	b00 = mPermutationTable[i + by.x];
	b10 = mPermutationTable[j + by.x];
	b01 = mPermutationTable[i + by.y];
	b11 = mPermutationTable[j + by.y];

	s = interpolationPolynomial2D(glm::dvec2(rx.x, ry.x));

	q = glm::dvec2(mGradientTable2d.at(b00));
	u = glm::dot(q, glm::dvec2(rx.x, ry.x));
	q = glm::dvec2(mGradientTable2d.at(b10));
	v = glm::dot(q, glm::dvec2(rx.y, ry.x));
	a = lerp(s.x, u, v);


	q = glm::dvec2(mGradientTable2d.at(b01));
	u = glm::dot(q, glm::dvec2(rx.x, ry.y));
	q = glm::dvec2(mGradientTable2d.at(b11));
	v = glm::dot(q, glm::dvec2(rx.y, ry.y));
	b = lerp(s.x, u, v);

//	return clamp(lerp(s.y, a, b), 0.0, 1.0);
	return lerp(s.y, a, b);
}




glm::dvec2 PerlinNoise::interpolationPolynomial2D(glm::dvec2 vec){
	double x = interpolationPolynomial2D(vec.x);
	double y = interpolationPolynomial2D(vec.y);
	return glm::dvec2(x, y);
}

double PerlinNoise::interpolationPolynomial2D(double t){
	return t * t *(3 - 2 * t);
				//t * t * t * (t * (t * 6 - 15) + 10); simplex
}

void PerlinNoise::initPermutationTable(){
	std::default_random_engine generator(mSeed);
	mPermutationTable = std::vector<int>(mSampleSize);
	iota(mPermutationTable.begin(), mPermutationTable.end(), 0);
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}
	std::shuffle(mPermutationTable.begin(), mPermutationTable.end(), generator);
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}
	mPermutationTable.insert(std::end(mPermutationTable), std::begin(mPermutationTable), std::end(mPermutationTable));
//	for(std::vector<int>::iterator it = mPermutationTable.begin(); it != mPermutationTable.end(); it++){
//		std::cout << *it << std::endl;
//	}

}

void PerlinNoise::initGradientTable(){
	std::default_random_engine generator(mSeed);
	std::uniform_real_distribution<double> distribution (-1.0, 1.0);
	mGradientTable2d = std::vector<glm::dvec2>(mSampleSize);
	for(int i = 0; i < mSampleSize; i++){
	//mGradientTable2d.at(i) = glm::dvec2();
		for (int j = 0; j < 2; j++)
			mGradientTable2d.at(i)[j] = distribution(generator);
		mGradientTable2d.at(i) = glm::normalize(mGradientTable2d.at(i));
		//std::cout << mGradientTable2d.at(i).x << ", b0  " <<  mGradientTable2d.at(i).y << std::endl;
	}
	//std::cin.get();

	mGradientTable2d.insert(std::end(mGradientTable2d), std::begin(mGradientTable2d), std::end(mGradientTable2d));
}


double PerlinNoise::lerp(double alpha, double x0, double x1){
	return x0 + alpha * (x1 - x0);
}



