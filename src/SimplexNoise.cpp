/*
 * SimplexNoise.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: adrian
 */

#include "SimplexNoise.h"

SimplexNoise::SimplexNoise()
{
	// TODO Auto-generated constructor stub

}

SimplexNoise::~SimplexNoise()
{
	// TODO Auto-generated destructor stub
}

void SimplexNoise::generateNoiseImage(){
	initPermutationTable();


//    printParams();
	if (mZDim == 1){
		for(int y = 0; y < mYDim; y++){
			for(int x = 0; x < mXDim; x++){
				float value = static_cast<float>(fbm(static_cast<double>(x), static_cast<double>(y)));
				//value = value -floor(value);
				mNoiseValues[y * mXDim + x] = value;
//				std::cout << value << std::endl;
				if (value < mMin)
					mMin = value;
				if (value > mMax)
					mMax = value;
			}
		}
//		mMax = mMax - mMin;
//		if (mMax != 0)
//			for(int y = 0; y < mYDim; y++){
//				for(int x = 0; x < mXDim; x++){
//					mNoiseValues[y * mXDim + x] = (mNoiseValues[y * mXDim + x] - mMin) / mMax;
//				}
//			}
	}
	else
	{
		for(int z = 0; z < mZDim; z++){
			for(int y = 0; y < mYDim; y++){
				for(int x = 0; x < mXDim; x++){
					float value = static_cast<float>(fbm(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)));
					mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = value;
//					std::cout << value << std::endl;
//					if (value < mMin)
//						mMin = value;
//					if (value > mMax)
//						mMax = value;
				}
			}
		}
//		if (mMax != 0)
//			for(int z = 0; z < mZDim; z++){
//				for(int y = 0; y < mYDim; y++){
//					for(int x = 0; x < mXDim; x++){
//						mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = (mNoiseValues[z * mYDim * mXDim + y * mXDim + x] - mMin) / mMax;
//
//					}
//				}
//			}
	}

}

double SimplexNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double SimplexNoise::calculateNoiseValue(double x, double y, double z) {

    int X = static_cast<int>(std::floor(x)) % (mSampleSize);
    int Y = static_cast<int>(std::floor(y)) % (mSampleSize);
    int Z = static_cast<int>(std::floor(z)) % (mSampleSize);

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    double u = interpolationPolynomial(x);
    double v = interpolationPolynomial(y);
    double w = interpolationPolynomial(z);

    int B0 = mPermutationTable[X] + Y;
    int B00 = mPermutationTable[B0] + Z;
    int B01 = mPermutationTable[B0 + 1] + Z;
    int B1 = mPermutationTable[X + 1] + Y;
    int B10 = mPermutationTable[B1] + Z;
    int B11 = mPermutationTable[B1 + 1] + Z;

    int PAA = mPermutationTable[B00];
    int PBA = mPermutationTable[B10];
    int PAB = mPermutationTable[B01];
    int PBB = mPermutationTable[B11];
    int PAA1 = mPermutationTable[B00 + 1];
    int PBA1 = mPermutationTable[B10 + 1];
    int PAB1 = mPermutationTable[B01 + 1];
    int PBB1 = mPermutationTable[B11 + 1];

    double a = lerp(v,
        lerp(u, grad(PAA, x, y, z), grad(PBA, x-1, y, z)),
        lerp(u, grad(PAB, x, y-1, z), grad(PBB, x-1, y-1, z))
    );

    double b = lerp(v,
        lerp(u, grad(PAA1, x, y, z-1), grad(PBA1, x-1, y, z-1)),
        lerp(u, grad(PAB1, x, y-1, z-1), grad(PBB1, x-1, y-1, z-1))
    );
    return lerp(w, a, b);
}

void SimplexNoise::initPermutationTable(){
	std::default_random_engine generator(mSeed);
	mPermutationTable = std::vector<int>(mSampleSize);
	iota(mPermutationTable.begin(), mPermutationTable.end(), 0);
	std::shuffle(mPermutationTable.begin(), mPermutationTable.end(), generator);
	mPermutationTable.insert(std::end(mPermutationTable), std::begin(mPermutationTable), std::end(mPermutationTable));

}

double SimplexNoise::fbm(double x, double y, double z){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
	z = z / (double)mZDim;
	x *= mFrequency;
	y *= mFrequency;
	z *= mFrequency;

	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue(x, y, z) * amp;
		//std::cout << result << std::endl;
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		amp *= 0.5;
	}
	return result;
}

double SimplexNoise::interpolationPolynomial(double t){
	return t * t * t * (t * (t * 6 - 15) + 10); //simplex
}
