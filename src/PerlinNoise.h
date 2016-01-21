/*
 * PerlinNoise.h
 *
 *  Created on: Jan 21, 2016
 *      Author: adrian
 */

#ifndef SRC_PERLINNOISE_H_
#define SRC_PERLINNOISE_H_
#include "Noise.h"
#include <random>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>


class PerlinNoise : public Noise
{
	public:
		PerlinNoise();
		virtual ~PerlinNoise();
		void setOctavesFreqAmp(int octaves, double frequency, double amplitude);
		void generate(int seed);
		void generateNoiseImage();

		double calculateNoiseValue(double x, double y);
		glm::ivec2 getDimension(){ return glm::ivec2(mWidth, mHeight);}
	private:
		glm::dvec2 interpolationPolynomial2D(glm::dvec2 vec);
		double interpolationPolynomial2D(double t);
		void initPermutationTable();
		void initGradientTable();
	//		double clamp(double x, double min, double max); not needed here
		double gradient();
		double lerp(double alpha, double x0, double x1);

		int mSampleSize = 256;
		int mOctaves;
		bool mStart = true;
		double mFrequency, mAmplitude, mMax = 0.5, mMin = 0.5;

		std::vector<int> mPermutationTable;
		std::vector<glm::dvec2> mGradientTable2d;
		double** mGradientNoise;

		double noise(double x, double y);
};

#endif /* SRC_PERLINNOISE_H_ */

