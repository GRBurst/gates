/*
 * Simplex.h
 *
 *  Created on: Jan 25, 2016
 *      Author: adrian
 */

#ifndef SRC_SIMPLEXNOISE_H_
#define SRC_SIMPLEXNOISE_H_
#include <vector>
#include <algorithm>
#include <random>
#include "PerlinNoise.h"
#include "Noise.h"
class SimplexNoise : public PerlinNoise
{
public:
	SimplexNoise();
	virtual ~SimplexNoise();
	double calculateNoiseValue(double x) { return calculateNoiseValue(x, 0, 0); }
	double calculateNoiseValue(double x, double y) { return calculateNoiseValue(x, y, 0); }
	double calculateNoiseValue(double x, double y, double z);
	void generateNoiseImage();
private:
	std::vector<int> mPermutationTable;
	double grad(int hash, double x, double y, double z);
	double interpolationPolynomial(double t);
	void initPermutationTable();
	double fbm(double x, double y, double z = 0.0);

};

#endif /* SRC_SIMPLEXNOISE_H_ */
