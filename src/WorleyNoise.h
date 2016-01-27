#ifndef SRC_WORLEYNOISE_H_
#define SRC_WORLEYNOISE_H_
#define _USE_MATH_DEFINES

#include "Noise.h"
#include <random>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <functional>
#include <queue>
#include <glm/glm.hpp>

class WorleyNoise : public Noise
{
    public:
        enum NoiseDistanceFunction
        {
            EULIDEAN = 0,
            MANHATTEN,
        };
        WorleyNoise();
        /* WorleyNoise(int xDim, int yDim, int zDim = 1); */

        /* void setParams() {}; */
        void init();
        void generateNoiseImage();
        bool saveToFile(const char* filename);

        int getXDim();
        void setXDim(int xDim){ mXDim = xDim; };
        int getYDim();
        void setYDim(int yDim){ mYDim = yDim; };
        int getZDim();
        void setZDim(int zDim){ mZDim = zDim; };
        
        float* getNoiseValues() {return mNoiseValues.data(); };

        /* void makeSomeNoise(Array_2D<float>& map, int zz); */

        /* //getters/setters */
        /* void setDistanceFunction(std::string dis); */
        /* void setEquation(std::string func); */
        /* void setMinkowski(float coefficient); */

    private:
        const uint OFFSET_BASIS = 2166136261;
        const uint FNV_PRIME = 16777619;
        /* std::vector<float> mNoiseValues; */
        std::function< float(glm::vec3, glm::vec3) > mDistFunc;
        unsigned int mDistSeed;
        double mAvgNumFeaturePoints;
        int mNumDistF;


        float combine_function_1( float arr[] );
        unsigned int lookup( unsigned int value );
        void insert( float arr[] , float value );
        unsigned int lcg_random( unsigned int last );
        unsigned int hash( unsigned int i, unsigned int j, unsigned int k );
        float cellNoise( glm::vec3 input );

        glm::vec3 generatePoint(int x, int y, int z);
        std::minstd_rand0 getRandDist;
        unsigned int probLookup(unsigned int value);
        unsigned int fnvHash(unsigned int i, unsigned int j, unsigned int k);
        unsigned int lcgRandom(unsigned int lastValue);

        float euclidean_distance( glm::vec3 p1, glm::vec3 p2 );
        float euclidean(glm::vec3 p1, glm::vec3 p2);
        float euclideanSquared(glm::vec3 p1, glm::vec3 p2);
        float manhattan(glm::vec3 p1, glm::vec3 p2);
        float chebychev(glm::vec3 p1, glm::vec3 p2);
        float quadratic(glm::vec3 p1, glm::vec3 p2);
        float minkowski(glm::vec3 p1, glm::vec3 p2);


        /* bool wrapping; */
        
        /* unsigned xgrid,ygrid,zgrid; */
        /* unsigned num_points; */
        
        /* std::vector<location> ***buckets; */
        
        /* int bucket_size; */
        /* unsigned buckets_x,buckets_y,buckets_z; */
        
        /* float (CellNoise::*distanceFunction)(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* equation* eq; */
        /* float minkowski_coefficient; */
        
        /* float euclidean(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* float euclideanSquared(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* float manhattan(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* float chebychev(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* float quadratic(float x1, float y1, float z1, float x2, float y2, float z2); */
        /* float minkowski(float x1, float y1, float z1, float x2, float y2, float z2); */
        
        /* void construct(unsigned Xgrid, unsigned Ygrid, unsigned Zgrid, unsigned Num_points, unsigned Bucket_size, bool Wrapping); */

};

#endif /* SRC_WORLEYNOISE_H_ */
