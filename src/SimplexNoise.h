/*
 * SimplexNoise.h
 *
 *  Created on: Jan 26, 2016
 *      Author: adrian
 */

#ifndef SRC_SIMPLEXNOISE_H_
#define SRC_SIMPLEXNOISE_H_
#include "Noise.h"
#include "PerlinNoise.h"
#include <random>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>

class SimplexNoise : public PerlinNoise
{
public:
	SimplexNoise();
	SimplexNoise(int x, int y, int seed);
	SimplexNoise(int x, int y, int z, int seed);
	SimplexNoise(int x, int y, int z, int u, int seed);
	SimplexNoise(int x, int y, int z, int seed, int octaves, double frequency, double amplitude);
	SimplexNoise(int x, int y, int z, int u, int seed, int octaves, double frequency, double amplitude);
	virtual ~SimplexNoise();
	void generateNoiseImage();
	double calculateNoiseValue(double x, double y);
	double calculateNoiseValue(double x, double y, double z);
	double calculateNoiseValue(double x, double y, double z, double w);
private:

	double fbm(double x, double y, double z);
	double fbm(double x, double y, double z, double u);
	void initPermutationTable();
	int mSampleSize = 256;
	int N = 4096;
	std::vector<std::vector<int>>mGradientTable3d{std::vector<int>{1,1,0},std::vector<int>{-1,1,0},std::vector<int>{1,-1,0},std::vector<int>{-1,-1,0},
		std::vector<int>{1,0,1},std::vector<int>{-1,0,1},std::vector<int>{1,0,-1},std::vector<int>{-1,0,-1},
		std::vector<int>{0,1,1},std::vector<int>{0,-1,1},std::vector<int>{0,1,-1},std::vector<int>{0,-1,-1}};
	std::vector<std::vector<int>>mGradientTable4d{std::vector<int>{0,1,1,1},std::vector<int>{0,1,1,-1},std::vector<int>{0,1,-1,1},std::vector<int>{0,1,-1,-1},
		std::vector<int>{0,-1,1,1},std::vector<int>{0,-1,1,-1},std::vector<int>{0,-1,-1,1},std::vector<int>{0,-1,-1,-1},
		std::vector<int>{1,0,1,1},std::vector<int>{1,0,1,-1},std::vector<int>{1,0,-1,1},std::vector<int>{1,0,-1,-1},
		std::vector<int>{-1,0,1,1},std::vector<int>{-1,0,1,-1},std::vector<int>{-1,0,-1,1},std::vector<int>{-1,0,-1,-1},
		std::vector<int>{1,1,0,1},std::vector<int>{1,1,0,-1},std::vector<int>{1,-1,0,1},std::vector<int>{1,-1,0,-1},
		std::vector<int>{-1,1,0,1},std::vector<int>{-1,1,0,-1},std::vector<int>{-1,-1,0,1},std::vector<int>{-1,-1,0,-1},
		std::vector<int>{1,1,1,0},std::vector<int>{1,1,-1,0},std::vector<int>{1,-1,1,0},std::vector<int>{1,-1,-1,0},
		std::vector<int>{-1,1,1,0},std::vector<int>{-1,1,-1,0},std::vector<int>{-1,-1,1,0},std::vector<int>{-1,-1,-1,0}};
	//Skewing Factors
	double F2 = (sqrt(3) - 1) / 2.0;
	double G2 = (3 - sqrt(3))/ 6.0;
	double F3 = 1 / 3.0;
	double G3 = 1 / 6.0;
	double F4 = (sqrt(5) - 1) / 4.0;
	double G4 = (5 - sqrt(5)) / 20.0;
	double F5 = (sqrt(6) - 1) / 5.0;
	double G5 = (6 - sqrt(6)) / 30.0;
	int cnt = 0;
};

#endif /* SRC_SIMPLEXNOISE_H_ */
