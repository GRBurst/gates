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
		~PerlinNoise();
        void printParams();
		void generate(int seed);
		void generateNoiseImage();

		double calculateNoiseValue(double x, double y);
		double calculateNoiseValue(double x, double y, double z);
		glm::ivec2 getDimension(){ return glm::ivec2(mXDim, mYDim);}
	private:
		glm::dvec4 interpolationPolynomial(glm::dvec4 vec);
		glm::dvec3 interpolationPolynomial(glm::dvec3 vec);
		glm::dvec2 interpolationPolynomial(glm::dvec2 vec);
		double interpolationPolynomial(double t);
		void initPermutationTable();
		void init2DGradientTable();
		void init3DGradientTable();
		void init4DGradientTable();
		void setGridPoints(double i, int &b0, int &b1, double &r0, double &r1);

	//		double clamp(double x, double min, double max); not needed here
		double gradient();
		double lerp(double alpha, double x0, double x1);

		int mSampleSize = 256;
		int N = 4096;
		bool mStart = true;
		double mMax = 0.0, mMin = 1.0;

		std::vector<int> mPermutationTable;
		std::vector<glm::dvec2> mGradientTable2d;
		std::vector<glm::dvec3> mGradientTable3d;
		std::vector<glm::dvec4> mGradientTable4d;

		double noise(double x, double y);
		double noise(double x, double y, double z);
};
#endif
