/*
 * Noise.h
 *
 *  Created on: Jan 11, 2016
 *      Author: adrian
 */

#ifndef SRC_NOISE_H_
#define SRC_NOISE_H_

#include <random>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>

class Noise
{
	public:
		enum NoiseType
			        {
			            SIMPLEX = 0,
			            PERLIN,
						WHITE,
			            WORLEY
			        };
		Noise();
		Noise(int x, int y, NoiseType noiseType, int seed, int octaves, double frequency, double amplitude);
		virtual ~Noise();

        void setParams(int x, int y, NoiseType noiseType, int seed, int octaves, double frequency, double amplitude);
		void generate(int seed);
		void generateNoiseImage();
		double* getTextureData();
		float* getTextureDataF();
		bool saveToFile(const char* filename);
		int getWidth(){ return mWidth;}
		int getHeight(){return mHeight;}
		double calculateNoiseValue(double x, double y);
		glm::ivec2 getDimension(){ return glm::ivec2(mWidth, mHeight);}
	private:
		glm::dvec2 interpolationPolynomial2D(glm::dvec2 vec);
		double interpolationPolynomial2D(double t);
		void initPermutationTable();
		void initGradientTable();
//		double clamp(double x, double min, double max); not needed here
		double scaleToZeroToOne(double value);
		double gradient();
		double lerp(double alpha, double x0, double x1);

		int mSampleSize = 256;
		int mWidth, mHeight, mSeed, mOctaves;
		bool mStart = true;
		double mFrequency, mAmplitude, mMax = 0.5, mMin = 0.5;

		std::vector<int> mPermutationTable;
		std::vector<glm::dvec2> mGradientTable2d;
		double** mGradientNoise;
		double* mNoise;
		float* mNoiseF;
		double perlinNoise2D(double x, double y);

};

#endif /* SRC_NOISE_H_ */



