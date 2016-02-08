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

class Noise
{
	public:

		Noise();
		Noise(int x, int y, int seed);
		Noise(int x, int y, int z, int seed);
		Noise(int x, int y, int z, int w, int seed);
		Noise(int x, int y, int z, int seed, int octaves, double frequency, double amplitude);
		Noise(int x, int y, int z, int w, int seed, int octaves, double frequency, double amplitude);
		virtual ~Noise();

        void setParams(int x, int y, int seed);
        void setParams(int x, int y, int z, int seed);
        virtual std::vector<float> getDataVector() { return mNoiseValues; };
		virtual float* getTextureData();
		virtual float* getInverseTextureData();
		virtual void generateTileableNoiseImage();
		virtual void generateNoiseImage() = 0;
		virtual void setOctavesFreqAmp(int octaves, double frequency, double amplitude)
		{
            mOctaves = octaves; mFrequency = frequency; mAmplitude = amplitude;
        };
		virtual void setScale(bool scale);
//		virtual float* getSphericalTexture();
		bool saveToFile(const char* filename);
		int getXDim(){ return mXDim;};
		int getYDim(){ return mYDim;};
		int getZDim(){ return mZDim;};
		int getUDim(){ return mUDim;};
		glm::ivec2 getDimension(){ return glm::ivec2(mXDim, mYDim);}
		float* getNormalMap();
	protected:
		double clamp(double x, double min, double max);
		int mXDim, mYDim, mZDim, mUDim, mVDim, mWDim, mSeed;
		int mOctaves;
		double mFrequency, mAmplitude;
		bool mTileable;
		bool mScaleZeroToOne;
        std::vector<float> mNoiseValues;
        std::vector<glm::vec3> mNormalMap;

};

#endif /* SRC_NOISE_H_ */


