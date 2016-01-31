/*
 * Noise.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#include "Noise.h"
Noise::Noise() : mXDim(256), mYDim(256), mZDim(1), mWDim(1), mSeed(1024), mOctaves(1), mFrequency(1.0), mAmplitude(1.0), mNoiseType(PERLIN)
{
	// TODO Auto-generated constructor stub

}

Noise::~Noise()
{
	// TODO Auto-generated destructor stub
}

void Noise::setParams(int x, int y, int seed){
	this->mXDim = x;
	this->mYDim = y;
	this->mSeed = seed;
	mNoiseValues.resize(mXDim * mYDim);
}

void Noise::setParams(int x, int y, int z, int seed){
	this->mXDim = x;
	this->mYDim = y;
	this->mZDim = z;
	this->mSeed = seed;
	mNoiseValues.resize(mXDim * mYDim * mZDim);
}


double Noise::clamp(double x, double min, double max){
	if (x > max)
		x = max;
	if (x < min)
		x = min;
	return x;
}

//float* Noise::getSphericalTexture(){
//	for (int y = 0; y < mYDim; y++)
//	{
//		for (int x = 0; x < mXDim; x++)
//		{
//			mNoiseValues[x+y * mXDim] = mNoiseValues[x+y * mXDim] + 1;
//		}
//	}
//}

float *Noise::getTextureData(){
	return mNoiseValues.data();
}

float *Noise::getInverseTextureData(){
	for (int i = 0; i < mXDim * mYDim; i++)
	{
		mNoiseValues[i] = -mNoiseValues[i] + 1;
	}
	return &mNoiseValues[0];
}

bool Noise::saveToFile(const char* filename){
	char* bmp = new char[mXDim*mYDim*mZDim];
	char* it = bmp;
	float* itF = &mNoiseValues.front();
	for(int i = 0; i < mZDim; i++){
		for (int x = 0; x < this-> mXDim * this->mYDim; x++){
			*it = (char)(*itF * 255);
			it++;
			itF++;
				//std::cout<< (y + x) + y * mXDim << std::endl;
				//std::cout << mNoiseValues[x][y] << std::endl;
		}

	}
    uint16_t d = static_cast<uint16_t>(mYDim)*static_cast<uint16_t>(mZDim);
	uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, static_cast<uint16_t>(mXDim), d, 8};
	std::ofstream file(filename, std::ios::binary);
	if(file.is_open()){
		file.write((char*) (&header), sizeof(header));
		file.write(bmp, sizeof(char)*mXDim*mYDim * mZDim);
		file.close();
	}
	else{
		std::cout << "Error TGA Save: File not found" << std::endl;
		return false;
	}

	return true;

}
