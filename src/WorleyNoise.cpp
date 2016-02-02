#include "WorleyNoise.h"
/* 
 * Params:
 * x,y position
 * seed
 * distance function
 * distance vector
 * combine distance functions
 *
 */

WorleyNoise::WorleyNoise()
{
    // Params
    mDistSeed = 24793197;

    mAvgNumFeaturePoints = 2.5;
    mNumDistF = 3;
}

void WorleyNoise::generateNoiseImage()
{
    float min = 10.0, max = 0.0;
    mNoiseValues.resize(mXDim * mYDim * mZDim);
    for(int z = 0; z < mZDim; z++){
        for(int y = 0; y < mYDim; y++){
            for(int x = 0; x < mXDim; x++){
                /* int index = ((mXDim * mYDim * z) + (y * mXDim + x)); */
                /* mNoiseValues.at(index) = (cellNoise(glm::vec3( */
                /* float value = static_cast<float>(cellNoise(static_cast<double>(x), static_cast<double>(y))); */
                float value = cellNoise(glm::vec3(
                            static_cast<float>(x) / 16.0f, 
                            static_cast<float>(y) / 16.0f, 
                            static_cast<float>(z) / 16.0f
                            ));
                mNoiseValues[y * mXDim + x] = value;
                if (value < min)
                    min = value;
                if (value > max)
                    max = value;
            }
        }
    }

    std::cout << std::endl << "cellNoise: min = " << min << ", max = " << max << std::endl;
    max = max - min;
    for(int x = 0; x < mXDim; x++){
        for(int y = 0; y < mYDim; y++){
            mNoiseValues[y * mXDim + x] = (mNoiseValues[y * mXDim + x] - min) / max;
            /* mNoiseValues[y * mXDim + x] = 2.5f*(mNoiseValues[y * mXDim + x]); */
        }
    }

}

void WorleyNoise::init()
{

    // Params
    mDistSeed = 24793197;
    /* mDistFunc = std::bind(&WorleyNoise::euclidean_distance, this, std::placeholders::_1, std::placeholders::_2); */
    /* if((mSeed % 6) == 0) mDistFunc = WorleyNoise::euclidean; */
    /* if((mSeed % 6) == 1) mDistFunc = WorleyNoise::euclideanSquared; */
    /* if((mSeed % 6) == 2) mDistFunc = WorleyNoise::manhattan; */
    /* if((mSeed % 6) == 3) mDistFunc = WorleyNoise::chebychev; */
    /* if((mSeed % 6) == 4) mDistFunc = WorleyNoise::quadratic; */
    /* if((mSeed % 6) == 5) mDistFunc = WorleyNoise::minkowski; */

    mAvgNumFeaturePoints = 2.5;
    mNumDistF = 3;

    /* mDistFunc = manhattan; */

}

bool WorleyNoise::saveToFile(const char* filename){
    char* bmp = new char[mXDim*mYDim*mYDim];
    char* it = bmp;
    for (int x = 0; x < this-> mXDim * this->mYDim * this->mZDim; x++){
        *it = (char)(mNoiseValues[x] * 255);
        it++;
    }

    uint16_t header[9] = {0, 3, 0, 0 ,0 ,0, (uint16_t)mXDim, (uint16_t)mYDim, 8};
    std::ofstream file(filename, std::ios::binary);
    if(file.is_open()){
        file.write((char*) (&header), sizeof(header));
        file.write(bmp, sizeof(char)*mXDim*mYDim);
        file.close();
    }

    else{
        std::cout << "Error TGA Save: File not found" << std::endl;
        return false;
    }

    return true;

}

unsigned int WorleyNoise::probLookup(unsigned int value)
{
    if (value < 393325350) return 1;
    if (value < 1022645910) return 2;
    if (value < 1861739990) return 3;
    if (value < 2700834071) return 4;
    if (value < 3372109335) return 5;
    if (value < 3819626178) return 6;
    if (value < 4075350088) return 7;
    if (value < 4203212043) return 8;
    return 9;
}

unsigned int WorleyNoise::fnvHash(unsigned int i, unsigned int j, unsigned int k)
{
    const unsigned int OFFSET_BASIS = 2166136261;
    const unsigned int FNV_PRIME = 16777619;
    return (unsigned int)((((((OFFSET_BASIS ^ (unsigned int)i) * FNV_PRIME) ^ (unsigned int)j) * FNV_PRIME) ^ (unsigned int)k) * FNV_PRIME);
}

unsigned int WorleyNoise::lcgRandom(unsigned int lastValue)
{
    return (unsigned int)((1103515245u * lastValue + 12345u) % 0x100000000u);
}

float WorleyNoise::cellNoise( glm::vec3 input )
{

    unsigned coord_seed = 24793197;

    /* std::default_random_engine generator; */
    /* std::poisson_distribution<int> distribution(mAvgNumFeaturePoints); */
    /* auto getNumFeaturePoints = std::bind ( distribution, generator ); */

    /* std::default_random_engine generator; */
    /* std::linear_congruential_engine<int> distribution(mDistSeed); */
    /* auto getRandDist = std::bind ( distribution, generator ); */
    // TODO: limit priority queue
    std::priority_queue<float, std::vector<float>, std::greater<float> > distF;
    
    glm::vec3 featurePoint;

    for ( int i = 0; i < mNumDistF; i++ ) {
        distF.push(6666.0f);
    }

    int evalX = ( int ) floorf( input.x );
    int evalY = ( int ) floorf( input.y );
    int evalZ = ( int ) floorf( input.z );


    for ( int i = -1; i < 2; ++i ) {
        for ( int j = -1; j < 2; ++j ) {
            for ( int k = -1; k < 2; ++k ) {
    
                mDistSeed = lcgRandom(fnvHash((unsigned int)(evalX + i + coord_seed), (unsigned int)(evalY + j), (unsigned int)(evalZ + k)));

                /* int mNumFeaturePoints = 3; */

                for( unsigned int l = 0; l < probLookup(mDistSeed); ++l ) {

                    featurePoint = generatePoint(evalX + i, evalY + j, evalZ + k);

                    /* if((mSeed % 6) == 0) distF.push( euclidean( input, featurePoint)); */
                    /* if((mSeed % 6) == 1) distF.push( euclideanSquared( input, featurePoint)); */
                    /* if((mSeed % 6) == 2) distF.push( manhattan( input, featurePoint)); */
                    /* if((mSeed % 6) == 3) distF.push( chebychev( input, featurePoint)); */
                    /* if((mSeed % 6) == 4) distF.push( quadratic( input, featurePoint)); */
                    /* if((mSeed % 6) == 5) distF.push( minkowski( input, featurePoint)); */
                    /* distF.push( mDistFunc( input, featurePoint ) ); */
                    distF.push( euclidean( input, featurePoint));
                }
            }
        }
    }

    /* std::cout << "Dist = " << distF.top() << std::endl; */

    return distF.top();
    /* return 0.0; */
}

glm::vec3 WorleyNoise::generatePoint(int x, int y, int z)
{

    std::default_random_engine generator;
    std::linear_congruential_engine<std::uint_fast32_t, 1103515245, 12345, 0x100000000> getRandDist2(mDistSeed);
    /* auto getRandDist2 = std::bind ( distribution, generator ); */
    /* std::minstd_rand getRandDist(mDistSeed); */
    //1103515245u * lastValue + 12345u) % 0x100000000u
    

    /* glm::vec3 randomDiff(0.0, 0.0, 0.0); */
    glm::vec3 randomDiff;
    glm::vec3 featurePoint;

    mDistSeed = getRandDist2();
    randomDiff.x = ( float ) mDistSeed / 0x100000000;

    /* mDistSeed = lcgRandom(mDistSeed); */
    mDistSeed = getRandDist2();
    randomDiff.y = ( float ) mDistSeed / 0x100000000;

    mDistSeed = getRandDist2();
    randomDiff.z = ( float ) mDistSeed / 0x100000000;

    featurePoint.x = randomDiff.x + ( float ) x;
    featurePoint.y = randomDiff.y + ( float ) y;
    featurePoint.z = randomDiff.z + ( float ) z;

    return featurePoint;
}

float WorleyNoise::euclidean_distance( glm::vec3 p1, glm::vec3 p2 ) {
    return ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) + ( p1.z - p2.z ) *  ( p1.z - p2.z );
}

float WorleyNoise::euclidean(glm::vec3 p1, glm::vec3 p2)
{
        float dif_x = p1.x - p2.x,
                  dif_y = p1.y - p2.y,
                  dif_z = p1.z - p2.z;
        
        return (pow(dif_x * dif_x + dif_y * dif_y + dif_z * dif_z,0.5f));
}

float WorleyNoise::euclideanSquared(glm::vec3 p1, glm::vec3 p2)
{
        float   dif_x = p1.x-p2.x,
                        dif_y = p1.y-p2.y,
                        dif_z = p1.z-p2.z;

        return (dif_x * dif_x + dif_y * dif_y + dif_z * dif_z);
}

float WorleyNoise::manhattan(glm::vec3 p1, glm::vec3 p2)
{
        float   dif_x = fabs(p1.x - p2.x),
                        dif_y = fabs(p1.y - p2.y),
                        dif_z = fabs(p1.z - p2.z);

        return (dif_x + dif_y + dif_z);
}

float WorleyNoise::chebychev(glm::vec3 p1, glm::vec3 p2)
{
        float   dif_x = fabs(p1.x - p2.x),
                        dif_y = fabs(p1.y - p2.y),
                        dif_z = fabs(p1.z - p2.z);

        if (dif_x > dif_y && dif_x > dif_z)
                return (dif_x);
        else if (dif_y > dif_x && dif_y > dif_z)
                return (dif_y);
        else
                return (dif_z);
}

float WorleyNoise::quadratic(glm::vec3 p1, glm::vec3 p2)
{
        float   dif_x = p1.x - p2.x,
                        dif_y = p1.y - p2.y,
                        dif_z = p1.z - p2.z;
                        
        return (dif_x * dif_x + dif_y * dif_y + dif_z * dif_z + dif_x * dif_y + dif_x * dif_z + dif_y * dif_z);
}

float WorleyNoise::minkowski(glm::vec3 p1, glm::vec3 p2)
{
        float minkowski_coefficient = 4.0f;

        float ddx = pow(fabs(p1.x - p2.x), minkowski_coefficient);
        float ddy = pow(fabs(p1.y - p2.y), minkowski_coefficient);
        float ddz = pow(fabs(p1.z - p2.z), minkowski_coefficient);
        
        return (pow(ddx + ddy + ddz,1.0f/minkowski_coefficient));
}

