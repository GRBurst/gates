/*
 * Noise.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#include "Noise.h"

Noise::Noise() : mWidth(256), mHeight(256), mSeed(1024), mNoiseType(PERLIN), mNoise(0)
{
	// TODO Auto-generated constructor stub

}

Noise::~Noise()
{
	// TODO Auto-generated destructor stub
}

void Noise::setParams(int x, int y, int seed){
	this->mWidth = x;
	this->mHeight = y;
	this->mSeed = seed;
	mNoise = new float[mWidth * mHeight];
}


//double Noise::clamp(double x, double min, double max){
//	if (x > max)
//		x = max;
//	if (x < min)
//		x = min;
//	return x;
//}


float *Noise::getTextureData(){
	return mNoise;
}


bool Noise::saveToFile(const char* filename){
	char* bmp = new char[mWidth*mHeight];
	char* it = bmp;
	for (int x = 0; x < this-> mWidth * this->mHeight; x++){
		*it = (char)(mNoise[x] * 255);
		it++;
			//std::cout<< (y + x) + y * mWidth << std::endl;
			//std::cout << mNoise[x][y] << std::endl;
	}


	uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, (uint16_t)mWidth, (uint16_t)mHeight, 8};
	std::ofstream file(filename, std::ios::binary);
	if(file.is_open()){
		file.write((char*) (&header), sizeof(header));
		file.write(bmp, sizeof(char)*mWidth*mHeight);
		file.close();
	}
	else{
		std::cout << "Error TGA Save: File not found" << std::endl;
		return false;
	}
	return true;

}
