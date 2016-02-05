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
	initPermutationTable();
}

SimplexNoise::SimplexNoise(int x, int y, int z, int seed) : PerlinNoise(x, y, z, seed)
{
	initPermutationTable();
}

SimplexNoise::SimplexNoise(int x, int y, int z, int u, int seed) : PerlinNoise(x, y, z, u, seed)
{
	initPermutationTable();
}

SimplexNoise::SimplexNoise(int x, int y, int z, int seed, int octaves, double frequency, double amplitude) : PerlinNoise(x, y, z, seed, octaves, frequency, amplitude)
{
	initPermutationTable();
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

void SimplexNoise::generateTileableNoiseImage(int dimensions){
	switch (dimensions){
	case 1:
		mNoiseValues.resize(mXDim * mYDim * mZDim);
		break;
	case 2:
		mNoiseValues.resize(mXDim * mYDim * mZDim);
		break;
	case 3:
		mNoiseValues.resize(mXDim * mYDim * mZDim);
		break;
	}
	if (mUDim > 1){
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
		for(int z = 0; z < mZDim; z++){
			for(int y = 0; y < mYDim; y++){
				for(int x = 0; x < mXDim; x++){
					float value = static_cast<float>(fbmTileableX(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)));
					if (dimensions == 1)
						mNoiseValues[z * mYDim * mXDim + y * mXDim + x] = value;
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
	int corners[4][3]; //corners of tetrahedral simplex
	double distances[4][3];
	double skew = (x+y+z) * F3;
	corners[0][0] = std::floor(x + skew);
	corners[0][1] = std::floor(y + skew);
	corners[0][2] = std::floor(z + skew);

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
	corners[3][0] = 1;
	corners[3][1] = 1;
	corners[3][2] = 1;
	for (int i = 1; i < 4; i++){
		for(int j = 0; j < 3; j++)
		{
			distances[i][j] = distances[0][j] - corners[i][j] + G3 * i;
		}
	}
	int cornersMod[3];
	for (int i = 0; i < 3; i++)
	{
		cornersMod[i] = corners[0][i] & (mSampleSize - 1);
	}
	int gradient_indices[4];
	gradient_indices[0] = mPermutationTable[cornersMod[0] + mPermutationTable[cornersMod[1] + mPermutationTable[cornersMod[2]]]] & 11;
	for (int i = 1; i < 4; i++)
	{
			gradient_indices[i] = mPermutationTable[cornersMod[0] + corners[i][0] + mPermutationTable[cornersMod[1] + corners[i][1]
																 + mPermutationTable[cornersMod[2] + corners[i][2]]]] & 11;
	}
	double final_sum = 0.0;
	double t[4];
	for (int i = 0; i < 4; i++){
		t[i] = 0.6 - distances[i][0] * distances[i][0] - distances[i][1] * distances[i][1] - distances[i][2] * distances[i][2];
		if (t[i] < 0) t[i] = 0.0;
		else{
			t[i] *= t[i];
			final_sum += t[i] * t[i] * dot(mGradientTable3d.at(gradient_indices[i]), distances[i]);
//			for  (int j = 0; j < 3; j++)
//				std::cout << mGradientTable3d.at(gradient_indices[i])[j] << ", ";
//			std::cout <<std::endl;
		}

	}
	return 32.0 * final_sum;
//	std::cout << res << std::endl;
}

double SimplexNoise::calculateNoiseValue(double x, double y, double z, double u){
	int corners[5][4]; //corners of tetrahedral simplex
	double distances[5][4];
	double skew = (x+y+z+u) * F4;
	corners[0][0] = std::floor(x + skew);
	corners[0][1] = std::floor(y + skew);
	corners[0][2] = std::floor(z + skew);
	corners[0][3] = std::floor(u + skew);

	double unskew = (corners[0][0] + corners[0][1] + corners[0][2] + corners[0][3]) * G4;
	distances[0][0] = x - corners[0][0] + unskew;
	distances[0][1] = y - corners[0][1] + unskew;
	distances[0][2] = z - corners[0][2] + unskew;
	distances[0][3] = u - corners[0][3] + unskew;

//	int ranks[4];
//	if(distances[0][0] > distances[0][1]) ranks[0]++;
//	else ranks[1]++;
//	if(distances[0][0] > distances[0][2]) ranks[0]++;
//	else ranks[2]++;
//	if(distances[0][0] > distances[0][3]) ranks[0]++;
//	else ranks[3]++;
//	if(distances[0][1] > distances[0][2]) ranks[1]++;
//	else ranks[2]++;
//	if(distances[0][1] > distances[0][3]) ranks[1]++;
//	else ranks[3]++;
//	if(distances[0][2] > distances[0][3]) ranks[2]++;
//	else ranks[3]++;
	int c1 = (distances[0][0] > distances[0][1]) ? 32 : 0;
	int c2 = (distances[0][0] > distances[0][2]) ? 16 : 0;
	int c3 = (distances[0][1] > distances[0][2]) ? 8 : 0;
	int c4 = (distances[0][0] > distances[0][3]) ? 4 : 0;
	int c5 = (distances[0][1] > distances[0][3]) ? 2 : 0;
	int c6 = (distances[0][2] > distances[0][3]) ? 1 : 0;
    int c = c1 + c2 + c3 + c4 + c5 + c6;
	for (int i = 1; i < 4; i++){
		for(int j = 0; j < 4; j++)
		{
			if (simplex[c][j] >= 4 - i)
				corners[i][j] = 1;
			else
				corners[i][j] = 0;
		}
	}

	corners[4][0] = 1;
	corners[4][1] = 1;
	corners[4][2] = 1;
	corners[4][3] = 1;


	for (int i = 1; i < 5; i++){
		for(int j = 0; j < 4; j++)
		{
			distances[i][j] = distances[0][j] - corners[i][j] + G4 * i;
		}
	}
	int cornersMod[4];
	for (int i = 0; i < 4; i++)
	{
		cornersMod[i] = corners[0][i] & (mSampleSize - 1);
	}
	int gradient_indices[5];
	gradient_indices[0] = mPermutationTable[cornersMod[0] + mPermutationTable[cornersMod[1] + mPermutationTable[cornersMod[2] + mPermutationTable[cornersMod[3]]]]] & 31;
	for (int i = 1; i < 5; i++)
	{
			gradient_indices[i] = mPermutationTable[cornersMod[0] + corners[i][0] + mPermutationTable[cornersMod[1] + corners[i][1]
																 + mPermutationTable[cornersMod[2] + corners[i][2] + mPermutationTable[cornersMod[3] + corners[i][3]]]]] & 31;
	}
	double final_sum = 0.0;
	double t[5];
	for (int i = 0; i < 5; i++){
		t[i] = 0.6 - distances[i][0] * distances[i][0] - distances[i][1] * distances[i][1] - distances[i][2] * distances[i][2] - distances[i][3] * distances[i][3];
		if (t[i] < 0) t[i] = 0.0;
		else{
			t[i] *= t[i];
			final_sum += t[i] * t[i] * dot(mGradientTable4d.at(gradient_indices[i]), distances[i]);
		}

	}
//	std::cout << cnt++ << std::endl;
	return 27.0 * final_sum;
}

double SimplexNoise::fbmTileableX(double x, double y, double z){
	double result = 0.0;
	double amp = mAmplitude;
	x = x / (double)mXDim;
	y = y / (double)mYDim;
	z = z / (double)mZDim;
	float frequency = mFrequency;
	float multiplier = 1.0f / ( 2.0f * glm::pi<double>() );
	for(int i = 0; i < mOctaves; i++){
		result += calculateNoiseValue((std::cos(x * glm::pi<double>()))*frequency, (std::cos(y * 2.0 * glm::pi<double>()) * multiplier)*frequency, (std::sin(y * 2.0 * glm::pi<double>()) * multiplier) * frequency, z) * amp;


		z *= 2.0;
		frequency *= 2.0;
		amp *= 0.5;
	}
	return result;
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


