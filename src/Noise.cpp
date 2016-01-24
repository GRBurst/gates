/*
 * Noise.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#include "Noise.h"

Noise::Noise() : mXDim(256), mYDim(256), mZDim(1), mSeed(1024), mOctaves(1), mFrequency(1.0), mAmplitude(1.0), mNoiseType(PERLIN)
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


double Noise::clamp(double x, double min, double max){
	if (x > max)
		x = max;
	if (x < min)
		x = min;
	return x;
}


float *Noise::getTextureData(){
	return &mNoiseValues[0];
}


bool Noise::saveToFile(const char* filename){
	char* bmp = new char[mXDim*mYDim];
	char* it = bmp;
	for (int x = 0; x < this-> mXDim * this->mYDim; x++){
		*it = (char)(mNoiseValues[x] * 255);
		it++;
			//std::cout<< (y + x) + y * mXDim << std::endl;
			//std::cout << mNoiseValues[x][y] << std::endl;
	}


	uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, (uint16_t)mXDim, (uint16_t)mYDim, 8};
	std::ofstream file(filename, std::ios::binary);
	if(file.is_open()){
		file.write((char*) (&header), sizeof(header));
		file.write(bmp, sizeof(char)*mXDim*mYDim);
		file.close();
	}
	else{
		std::cout << "Error TGA Save: File not found" << std::endl;
		return false;
	}
	return true;

}
