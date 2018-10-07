#ifndef SOM_H
#define SOM_H
#include <vector>
#include <random>
#include <limits>
#include <iostream>
namespace Math
{
    std::mt19937 g_RandomEngine;

    inline double Decay(const double sima0, const int timeItr, const double lambda)noexcept
    {
        return sima0 * std::exp(- timeItr / lambda);
    }
    template<class T> struct Point
    {
        T x;
        T y;
    };
    typedef Point<int> PointI;
}
namespace SOM
{
    class Neuron
    {
    public:
        inline void UpdateWeights(std::vector<double> const& input, const double influence)noexcept
        {
            for(int ii = 0; ii < input.size(); ++ii)
            {
                m_Weights[ii] += influence * (input[ii] - m_Weights[ii]);
            }
        }

        inline double CalcSqrDistance(std::vector<double> const& input)const noexcept
        {
            double sum = 0;
            for(int ii=0 ; ii < input.size(); ++ii)
            {
                const double diff = input[ii] -  m_Weights[ii];
                sum += diff*diff;
            }
            return sum;
        }
        std::vector<double>& GetWeights(){
            return m_Weights;
        }
        std::vector<double>const& GetWeights()const{
            return m_Weights;
        }
    protected:
        std::vector<double> m_Weights;
    };

    class Network
    {
    public:
        static inline Network CreateRandomNetwork(const int size, const int weightNumber)noexcept
        {
            std::uniform_real_distribution<double> uniform(0,1);
            Network network;
            network.m_Neurons.reserve(size);
            for(int ii =0; ii < size; ++ii)
            {
                network.m_Neurons.emplace_back();
                network.m_Neurons[ii].reserve(size);
                for(int jj =0; jj < size; ++jj)
                {
                    network.m_Neurons[ii].emplace_back();
                    network.m_Neurons[ii][jj].GetWeights().reserve(weightNumber);
                    for(int kk = 0; kk < weightNumber; ++kk)
                    {
                        network.m_Neurons[ii][jj].GetWeights().push_back(uniform(Math::g_RandomEngine));
                    }
                }
            }
            return network;
        }
        inline Math::PointI FindBMU(std::vector<double> const& input)const noexcept
        {
            Math::PointI bmu;
            double minDistance =  std::numeric_limits<double>::max();
            for(int ii = 0; ii < m_Neurons.size(); ++ii)
            {
                for(int jj = 0; jj < m_Neurons[ii].size(); ++jj)
                {
                    const double distance = m_Neurons[ii][jj].CalcSqrDistance(input);
                    if(distance < minDistance)
                    {
                        minDistance = distance;
                        bmu = {ii,jj};
                    }
                }
            }
            return bmu;
        }

        void UpdateNetwork( std::vector<double> const& input, const int radius, const double learningRate) noexcept
        {
           const int radiusSqr = radius * radius;
           const double sigma = 2 * learningRate * learningRate;

           const Math::PointI bmu = FindBMU(input);
           const Math::PointI xRange{ bmu.x - radius < 0 ? 0 : - radius, bmu.x + radius >= m_Neurons.size() ? static_cast<int>(m_Neurons.size()- bmu.x - 1) : radius};
           const Math::PointI yRange{ bmu.y - radius < 0 ? 0 : - radius, bmu.y + radius >= m_Neurons.size() ? static_cast<int>(m_Neurons.size()- bmu.y - 1) : radius};

           for(int ii = xRange.x ; ii <= xRange.y; ++ii)
           {
               const int iiSqr = ii*ii;
               for(int jj = yRange.x ; jj <= yRange.y; ++jj)
               {
                   const int distanceSqr = iiSqr + jj*jj;
                   if( radiusSqr > distanceSqr )
                   {
                        const double influence = Math::Decay(learningRate, distanceSqr, sigma);
                        m_Neurons[bmu.x + ii][bmu.y + jj].UpdateWeights(input, influence);
                   }
               }
           }
        }

        void ProcessNetwork(std::vector<std::vector<double>> const& data,const int startingRadius, const double startingLearningRate, const int numOfIterations)noexcept
        {
            std::uniform_int_distribution<int> dataRandomizer(0,data.size() - 1);
            const double timeConstant = numOfIterations/std::log(startingRadius);
            for(int timeIter = 0; timeIter < numOfIterations; ++timeIter)
            {
                const int radius = Math::Decay(startingRadius, timeIter, timeConstant);
                const double learningRate = Math::Decay(startingLearningRate, timeIter, numOfIterations);
                const std::vector<double>& randomData = data[dataRandomizer(Math::g_RandomEngine)];

                UpdateNetwork(randomData,radius,learningRate);
            }
        }
        const Neuron& GetNeuron(const int x, const int y)const{
            return m_Neurons[x][y];
        }
    protected:
        std::vector<std::vector<Neuron>> m_Neurons;
    };

    inline std::vector<double> CreateRandomInput(const int inputNumber)noexcept
    {
        std::uniform_real_distribution<double> uniform(0,1);
        std::vector<double> input;
        input.reserve(inputNumber);
        for(int ii = 0; ii < inputNumber; ++ii)
        {
            input.emplace_back(uniform(Math::g_RandomEngine));
        }
        return input;
    }
    inline std::vector<std::vector<double>> CreateRandomData(const int inputNumber, const int size)noexcept
    {
        std::uniform_real_distribution<double> uniform(0,1);
        std::vector<std::vector<double>> data ;
        data.reserve(size);
        for(int ii = 0; ii < size; ++ii)
        {
            data.emplace_back(CreateRandomInput(inputNumber));
        }
        return data;
    }
}
#endif // SOM_H
