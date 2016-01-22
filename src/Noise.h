/*
 * Noise.h
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#ifndef SRC_NOISE_H_
#define SRC_NOISE_H_

#include <random>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>
#include "NoiseType.h"
//Factory Class
class Noise
{
	public:

		Noise();

		virtual ~Noise();

        void setParams(int x, int y, int seed);

		virtual float* getTextureData();
		virtual void generateNoiseImage() = 0;
		virtual void setOctavesFreqAmp(int octaves, double frequency, double amplitude) = 0;

		bool saveToFile(const char* filename);
		int getWidth(){ return mWidth;}
		int getHeight(){return mHeight;}
		double calculateNoiseValue(double x, double y);
		glm::ivec2 getDimension(){ return glm::ivec2(mWidth, mHeight);}
	protected:

		int mWidth, mHeight, mSeed;
		NoiseType mNoiseType;

		float* mNoise;

};

#endif /* SRC_NOISE_H_ */


