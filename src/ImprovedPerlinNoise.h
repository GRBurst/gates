/*
 * Simplex.h
 *
 *  Created on: Jan 25, 2016
 *      Author: adrian
 */

#ifndef SRC_IMPROVEDPERLINNOISE_H_
#define SRC_IMPROVEDPERLINNOISE_H_
#include <vector>
#include <algorithm>
#include <random>
#include "PerlinNoise.h"
#include "Noise.h"
class ImprovedPerlinNoise : public PerlinNoise
{
public:
	ImprovedPerlinNoise();
	virtual ~ImprovedPerlinNoise();
	double calculateNoiseValue(double x) { return calculateNoiseValue(x, 0, 0); }
	double calculateNoiseValue(double x, double y) { return calculateNoiseValue(x, y, 0); }
	double calculateNoiseValue(double x, double y, double z);
	double calculateNoiseValue(double x, double y, double z, double u);
	double calculateNoiseValue(double x, double y, double z, double u, double v);
//	double calculateNoiseTileableValue(double x, double y, double z, int px, int py, int pz);
	void generateNoiseImage();
private:
	std::vector<int> mPermutationTable;
	double grad(int hash, double x, double y, double z);
	double interpolationPolynomial(double t);
	void initPermutationTable();
	double fbm(double x, double y, double z = 0.0);
//	double fbmTileable(double x, double y, double z = 0.0);

};

#endif /* SRC_IMPROVEDPERLINNOISE_H_ */
