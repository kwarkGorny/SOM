#ifndef NEURON_H
#define NEURON_H
#include "Point2D.h"
#include <vector>
#include <algorithm>
#include <random>
#include "RandomSystem.h"
#include <limits>
inline double decayFunction(const double sima0, const int timeItr, const double lambda)noexcept
{
    return sima0 * std::exp(- timeItr / lambda);
}

struct Neuron
{
    inline void UpdateWeights(std::vector<double> const& input,  const double influence)noexcept
    {
        for(int ii = 0; ii < input.size(); ++ii)
        {
            weights[ii] +=influence * (input[ii] - weights[ii]);
        }
    }
    inline double CalcSqrDistance(std::vector<double> const& input)const noexcept
    {
        double sum = 0;
        for(int ii=0 ; ii < input.size(); ++ii)
        {
            const double diff = input[ii] -  weights[ii];
            sum += diff*diff;
        }
        return sum;
    }
    std::vector<double> weights;
};
class Network
{
public:

    inline PointI FindBMU(std::vector<double> const& input)const noexcept
    {
        PointI bmu;
        double minDistance =  std::numeric_limits<double>::max();
        for(int ii = 0; ii < m_NeuronMatrix.size(); ++ii)
        {
            for(int jj = 0; jj < m_NeuronMatrix[ii].size(); ++jj)
            {
                const double distance = m_NeuronMatrix[ii][jj].CalcSqrDistance(input);
                if(distance < minDistance)
                {
                    minDistance = distance;
                    bmu = {ii,jj};
                }
            }
        }
        return bmu;
    }
    inline void Initialize(int size, int inputSize)noexcept
    {
        std::uniform_real_distribution<double> uniform(0,1);
        m_NeuronMatrix.reserve(size);
        for(int ii =0; ii < size; ++ii)
        {
            m_NeuronMatrix[ii].reserve(size);
            for(int jj =0; jj < size; ++jj)
            {
                m_NeuronMatrix[ii][jj].weights.reserve(inputSize);
                for(int kk = 0; kk <inputSize; ++kk)
                {
                    m_NeuronMatrix[ii][jj].weights.push_back(uniform(RANDOMSYSTEM));
                }
            }
        }
    }

    inline void Update(std::vector<std::vector<double>> const& data, const int numOfIterations) noexcept
    {
        const double timeConstant = numOfIterations/std::log(m_StartingRadius);
        std::uniform_int_distribution<int> dataRandomizer(0,data.size() - 1);
        for ( int timeIter = 0; timeIter < numOfIterations; ++timeIter)
        {
           const int radius = decayFunction(m_StartLearningRate, timeIter, timeConstant);
           if(radius < 1) return;
           const int radiusSqr = radius *radius;

           std::vector<double> const& input = data[dataRandomizer(RANDOMSYSTEM)];
           const PointI bmu = FindBMU(input);
           const int startPos = std::max(bmu.x - radius, 0);
           const int endPos = std::min(bmu.x + radius,(int)m_NeuronMatrix.size());

           const double learningRate = decayFunction(m_StartLearningRate, timeIter, numOfIterations);
           const double sigma = 2 * learningRate * learningRate;

           for(int ii = startPos ; ii < endPos; ++ii)
           {
               const int iiSqr = ii*ii;
               for(int jj = startPos ; jj < endPos; ++jj)
               {
                   const int distanceSqr =iiSqr + jj*jj;
                   if(distanceSqr > radiusSqr)
                   {
                        const double influence = learningRate * std::exp(- distanceSqr / sigma);
                        m_NeuronMatrix[ii][jj].UpdateWeights(input,influence);
                   }
               }
           }

        }
    }

private:
    std::vector<std::vector<Neuron>> m_NeuronMatrix;
    double m_StartLearningRate;
    int m_StartingRadius;

};
struct SOM
{
    SOM()noexcept{}

    Network neurons;
};
#endif // NEURON_H
