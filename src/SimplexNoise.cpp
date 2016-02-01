/*
 * SimplexNoise.cpp
 *
 *  Created on: Jan 26, 2016
 *      Author: adrian
 */

#include "SimplexNoise.h"
#include <iostream>
SimplexNoise::SimplexNoise()
{
	// TODO Auto-generated constructor stub

}

SimplexNoise::~SimplexNoise()
{
	// TODO Auto-generated destructor stub
}

SimplexNoise::SimplexNoise(int x, int y, int seed) : PerlinNoise(x, y, seed)
{

}

SimplexNoise::SimplexNoise(int x, int y, int z, int seed) : PerlinNoise(x, y, z, seed)
{

}

SimplexNoise::SimplexNoise(int x, int y, int z, int seed, int octaves, double frequency, double amplitude) : PerlinNoise(x, y, z, seed, octaves, frequency, amplitude)
{

}

double dot(std::vector<int> grad, double* x){
	double result = 0.0;
	for (unsigned int i = 0; i < grad.size(); i++)
	{
		result += grad.at(i) * x[i];
	}
	return result;

}

void SimplexNoise::initPermutationTable(){
	std::default_random_engine generator(mSeed);
	mPermutationTable = std::vector<int>(mSampleSize);
	iota(mPermutationTable.begin(), mPermutationTable.end(), 0);
	std::shuffle(mPermutationTable.begin(), mPermutationTable.end(), generator);
	mPermutationTable.insert(std::end(mPermutationTable), std::begin(mPermutationTable), std::end(mPermutationTable));

}

void SimplexNoise::generateNoiseImage(){
	initPermutationTable();
	if (mUDim == 1){
		for(int z = 0; z < mZDim; z++){
			for(int y = 0; y < mYDim; y++){
				for(int x = 0; x < mXDim; x++){
					float value = static_cast<float>(fbm(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)));
					//value = value -floor(value);
					mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = value;
	//				std::cout << value << std::endl;
					if (value < mMin)
						mMin = value;
					if (value > mMax)
						mMax = value;
				}
			}
		}
		if (mScaleZeroToOne){
			mMax = mMax - mMin;
			if (mMax != 0)
			for(int z = 0; z < mZDim; z++){
				for(int y = 0; y < mYDim; y++){
					for(int x = 0; x < mXDim; x++){
						mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = (mNoiseValues[z * mYDim * mXDim + y * mXDim + x] - mMin) / mMax;
					}
				}
			}
		}
	}
	else
	{
		for(int u = 0; u < mUDim; u++){
			for(int z = 0; z < mZDim; z++){
				for(int y = 0; y < mYDim; y++){
					for(int x = 0; x < mXDim; x++){
						float value = static_cast<float>(fbm(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z), static_cast<double>(u)));
						mNoiseValues[u * mZDim * mYDim * mXDim + z * mYDim * mXDim + y * mXDim + x] = value;
						if (value < mMin)
							mMin = value;
						if (value > mMax)
							mMax = value;
					}
				}
			}
		}
		if (mScaleZeroToOne){
			mMax = mMax - mMin;
			if (mMax != 0)
				for(int u = 0; u < mUDim; u++){
					for(int z = 0; z < mZDim; z++){
						for(int y = 0; y < mYDim; y++){
							for(int x = 0; x < mXDim; x++){
								mNoiseValues[u * mZDim * mYDim * mXDim + z * mYDim * mXDim + y * mXDim + x] = (mNoiseValues[u * mZDim * mYDim * mXDim + z * mYDim * mXDim + y * mXDim + x] - mMin) / mMax;

							}
						}
					}
				}
			}
		}
}

//void SimplexNoise::calculateNoiseValue(double x, double y){
//
//}
//

float  grad( int hash, float x, float y , float z ) {
    int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
    float u = h<8 ? x : y; // gradient directions, and compute dot product.
    float v = h<4 ? y : h==12||h==14 ? x : z; // Fix repeats at h = 12 to 15
    return ((h&1)? -u : u) + ((h&2)? -v : v);
}
double SimplexNoise::calculateNoiseValue(double x, double y, double z){
//	int corners[4][3]; //corners of tetrahedral simplex
//	double distances[4][3];
//	double skew = (x+y+z) * F3;
//	corners[0][0] = (int)(std::floor(x + skew));
//	corners[0][1] = (int)(std::floor(y + skew));
//	corners[0][2] = (int)(std::floor(z + skew));
//
//	double unskew = (corners[0][0] + corners[0][1] + corners[0][2]) * G3;
//	distances[0][0] = x - corners[0][0] + unskew;
//	distances[0][1] = y - corners[0][1] + unskew;
//	distances[0][2] = z - corners[0][2] + unskew;
//
//	if(distances[0][0] < distances[0][1]){
//		if (distances[0][1] < distances[0][2]){
//			corners[1][0] = 0; corners[1][1] = 0; corners[1][2] = 1;
//			corners[2][0] = 0; corners[2][1] = 1; corners[2][2] = 1;
//		}
//		else if (distances[0][0] < distances[0][2]){
//			corners[1][0] = 0; corners[1][1] = 1; corners[1][2] = 0;
//			corners[2][0] = 0; corners[2][1] = 1; corners[2][2] = 1;
//		}
//		else{
//			corners[1][0] = 0; corners[1][1] = 1; corners[1][2] = 0;
//			corners[2][0] = 1; corners[2][1] = 1; corners[2][2] = 0;
//		}
//	}
//	else{
//		if (distances[0][0] < distances[0][2]){
//			corners[1][0] = 0; corners[1][1] = 0; corners[1][2] = 1;
//			corners[2][0] = 1; corners[2][1] = 0; corners[2][2] = 1;
//		}
//		else if (distances[0][1] < distances[0][2]){
//			corners[1][0] = 1; corners[1][1] = 0; corners[1][2] = 0;
//			corners[2][0] = 1; corners[2][1] = 0; corners[2][2] = 1;
//		}
//		else{
//			corners[1][0] = 1; corners[1][1] = 0; corners[1][2] = 0;
//			corners[2][0] = 1; corners[2][1] = 1; corners[2][2] = 0;
//		}
//
//	}
//	corners[3][0] = 1;
//	corners[3][1] = 1;
//	corners[3][2] = 1;
//	for (int i = 1; i < 4; i++){
//		for(int j = 0; j < 3; j++)
//		{
//			distances[i][j] = distances[0][j] - corners[i][j] + unskew * i;
//		}
//	}
//	int cornersMod[3];
//	for (int i = 0; i < 3; i++)
//	{
//		cornersMod[i] = corners[0][i] & (mSampleSize - 1);
//	}
//	int gradient_indices[4];
//	gradient_indices[0] = mPermutationTable[cornersMod[0] + mPermutationTable[cornersMod[1] + mPermutationTable[cornersMod[2]]]] % 12;
//	for (int i = 1; i < 4; i++)
//	{
//			gradient_indices[i] = mPermutationTable[cornersMod[0] + corners[i][0] + mPermutationTable[cornersMod[1] + corners[i][1]
//																 + mPermutationTable[cornersMod[2] + corners[i][2]]]] % 12;
//	}
//	double final_sum = 0.0;
//	double t[4];
//	for (int i = 0; i < 4; i++){
//		t[i] = 0.6 - distances[i][0] * distances[i][0] - distances[i][1] * distances[i][1] - distances[i][2] * distances[i][2];
//		if (t[i] < 0) t[i] = 0.0;
//		else{
//			t[i] *= t[i];
//			final_sum += t[i] * t[i] * dot(mGradientTable3d.at(gradient_indices[i]), distances[i]);
////			for  (int j = 0; j < 3; j++)
////				std::cout << mGradientTable3d.at(gradient_indices[i])[j] << ", ";
////			std::cout <<std::endl;
//		}
//
//	}
//	double res = 32.0 * final_sum;
////	std::cout << res << std::endl;
//	return res;
#define F3 0.333333333f
#define G3 0.166666667f

    float n0, n1, n2, n3; // Noise contributions from the four corners

    // Skew the input space to determine which simplex cell we're in
    float s = (x+y+z)*F3; // Very nice and simple skew factor for 3D
    float xs = x+s;
    float ys = y+s;
    float zs = z+s;
    int i = std::floor(xs);
    int j = std::floor(ys);
    int k = std::floor(zs);

    float t = (float)(i+j+k)*G3;
    float X0 = i-t; // Unskew the cell origin back to (x,y,z) space
    float Y0 = j-t;
    float Z0 = k-t;
    float x0 = x-X0; // The x,y,z distances from the cell origin
    float y0 = y-Y0;
    float z0 = z-Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

/* This code would benefit from a backport from the GLSL version! */
    if(x0>=y0) {
      if(y0>=z0)
        { i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
        else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
        else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
      }
    else { // x0<y0
      if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
      else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
      else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.

    float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    float y1 = y0 - j1 + G3;
    float z1 = z0 - k1 + G3;
    float x2 = x0 - i2 + 2.0f*G3; // Offsets for third corner in (x,y,z) coords
    float y2 = y0 - j2 + 2.0f*G3;
    float z2 = z0 - k2 + 2.0f*G3;
    float x3 = x0 - 1.0f + 3.0f*G3; // Offsets for last corner in (x,y,z) coords
    float y3 = y0 - 1.0f + 3.0f*G3;
    float z3 = z0 - 1.0f + 3.0f*G3;

    // Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;
    int kk = k & 0xff;

    // Calculate the contribution from the four corners
    float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
    if(t0 < 0.0f) n0 = 0.0f;
    else {
      t0 *= t0;
      n0 = t0 * t0 * grad(mPermutationTable[ii+mPermutationTable[jj+mPermutationTable[kk]]], x0, y0, z0);
    }

    float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
    if(t1 < 0.0f) n1 = 0.0f;
    else {
      t1 *= t1;
      n1 = t1 * t1 * grad(mPermutationTable[ii+i1+mPermutationTable[jj+j1+mPermutationTable[kk+k1]]], x1, y1, z1);
    }

    float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
    if(t2 < 0.0f) n2 = 0.0f;
    else {
      t2 *= t2;
      n2 = t2 * t2 * grad(mPermutationTable[ii+i2+mPermutationTable[jj+j2+mPermutationTable[kk+k2]]], x2, y2, z2);
    }

    float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
    if(t3<0.0f) n3 = 0.0f;
    else {
      t3 *= t3;
      n3 = t3 * t3 * grad(mPermutationTable[ii+1+mPermutationTable[jj+1+mPermutationTable[kk+1]]], x3, y3, z3);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 32.0f * (n0 + n1 + n2 + n3); // TODO: The scale factor is preliminary!

}

double SimplexNoise::calculateNoiseValue(double x, double y, double z, double u){
	int corners[5][4]; //corners of tetrahedral simplex
	double distances[5][4];
	double skew = (x+y+z) * F3;
	corners[0][0] = static_cast<int>(std::floor(x + skew));
	corners[0][1] = static_cast<int>(std::floor(y + skew));
	corners[0][2] = static_cast<int>(std::floor(z + skew));


	double unskew = (corners[0][0] + corners[0][1] + corners[0][2]) * G3;
	distances[0][0] = x - corners[0][0] + unskew;
	distances[0][1] = y - corners[0][1] + unskew;
	distances[0][2] = z - corners[0][2] + unskew;

	if(distances[0][0] >= distances[0][1]){
		if (distances[0][1] >= distances[0][2]){
			corners[1][0] = 1; corners[1][1] = 0; corners[1][2] = 0;
			corners[2][0] = 1; corners[2][1] = 1; corners[2][2] = 0;
		}
		else if (distances[0][0] >= distances[0][2]){
			corners[1][0] = 1; corners[1][1] = 0; corners[1][2] = 0;
			corners[2][0] = 1; corners[2][1] = 0; corners[2][2] = 1;
		}
		else{
			corners[1][0] = 0; corners[1][1] = 0; corners[1][2] = 1;
			corners[2][0] = 1; corners[2][1] = 0; corners[2][2] = 1;
		}
	}
	else{
		if (distances[0][1] < distances[0][2]){
			corners[1][0] = 0; corners[1][1] = 0; corners[1][2] = 1;
			corners[2][0] = 0; corners[2][1] = 1; corners[2][2] = 1;
		}
		else if (distances[0][0] < distances[0][2]){
			corners[1][0] = 0; corners[1][1] = 1; corners[1][2] = 0;
			corners[2][0] = 0; corners[2][1] = 1; corners[2][2] = 1;
		}
		else{
			corners[1][0] = 0; corners[1][1] = 1; corners[1][2] = 0;
			corners[2][0] = 1; corners[2][1] = 1; corners[2][2] = 0;
		}

	}

	for (int i = 1; i < 3; i++){
		corners[3][i] = 1;
		for(int j = 0; j < 3; j++)
		{
			distances[i][j] = distances[0][j] - double(corners[i][j]) + unskew * (double)i;
		}
	}
	int cornersMod[3];
	for (int i = 0; i < 3; i++)
	{
		cornersMod[i] = corners[0][i] & (mSampleSize - 1);
	}
	int gradient_indices[4];

	gradient_indices[0] = mPermutationTable[cornersMod[0] + mPermutationTable[cornersMod[1] + mPermutationTable[cornersMod[2]]]];
	for (int i = 1; i < 4; i++)
	{
			gradient_indices[i] = mPermutationTable[cornersMod[0] + corners[i][0] + mPermutationTable[cornersMod[1] + corners[i][1]
																 + mPermutationTable[cornersMod[2] + corners[i][2]]]];
	}
	double final_sum = 0.0;
	double t[4];
	for (int i = 0; i < 4; i++){
		t[i] = 0.6 - corners[i][0] - corners[i][1] - corners[i][2];
		if (t[i] < 0) t[0] = 0.0;
		else{
			t[i] *= t[i];
			final_sum += t[i] * t[i] * dot(mGradientTable3d.at(gradient_indices[i]), distances[i]);
		}
	}
	return (32.0f * final_sum);
}



double SimplexNoise::fbm(double x, double y, double z, double u){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
	z = z / (double)mZDim;
	u = u / (double)mUDim;
	x *= mFrequency;
	y *= mFrequency;
	z *= mFrequency;
	u *= mFrequency;
	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue(x, y, z, u) * amp;
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		u *= 2.0;
		amp *= 0.5;
	}
	return result;
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
		x *= 2.0;
		y *= 2.0;
		z *= 2.0;
		amp *= 0.5;
	}
	result = result / 2.0 + 1/2.0;
	return result / mAmplitude;
}


