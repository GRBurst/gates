/*
 * Noise.h
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#ifndef SRC_NOISE_H_
#define SRC_NOISE_H_

#include <random>
#include <vector>
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
		virtual void setOctavesFreqAmp(int octaves, double frequency, double amplitude)
        {
            mOctaves = octaves; mFrequency = frequency; mAmplitude = amplitude;
        };

		bool saveToFile(const char* filename);
		int getXDim(){ return mXDim;}
		int getYDim(){ return mYDim;}
		double calculateNoiseValue(double x, double y);
		glm::ivec2 getDimension(){ return glm::ivec2(mXDim, mYDim);}
	protected:
		double clamp(double x, double min, double max);
		int mXDim, mYDim, mZDim, mSeed;
		NoiseType mNoiseType;
		int mOctaves;
		double mFrequency, mAmplitude;

        std::vector<float> mNoiseValues;

};

#endif /* SRC_NOISE_H_ */


