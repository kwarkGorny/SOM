#ifndef SOM_H
#define SOM_H

#include <vector>
#include <random>
#include <limits>

namespace math
{
std::mt19937 g_RandomEngine(std::random_device{}());

    inline double Decay(const double sima0, const int timeItr, const double lambda)noexcept
    {
        return sima0 * std::exp(- timeItr / lambda);
    }
    template<class T> struct Point
    {
        T x,y;

    };
    typedef Point<int> PointI;
    template<class T> struct Range
    {
        T start,end;
    };
    typedef Range<int> RangeI;
}
namespace som
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
        std::vector<double>const& GetWeights()const{
            return m_Weights;
        }
        std::vector<double>& GetWeights(){
            return m_Weights;
        }
    protected:
        std::vector<double> m_Weights;
    };

    class Network
    {
    public:
        static inline Network CreateRandomNetwork(const int size, const int weightsNumber = 3)noexcept
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
                    network.m_Neurons[ii][jj].GetWeights().reserve(weightsNumber);
                    for(int kk = 0; kk < weightsNumber; ++kk)
                    {
                        network.m_Neurons[ii][jj].GetWeights().push_back(uniform(math::g_RandomEngine));
                    }
                }
            }
            return network;
        }
        inline math::PointI FindBMU(std::vector<double> const& input)const noexcept
        {
            math::PointI bmu;
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

        void UpdateWeights( std::vector<double> const& input, const int radius, const double learningRate) noexcept
        {
           const int radiusSqr = radius * radius;
           const double sigma = 2 * radiusSqr;

           const math::PointI bmu = FindBMU(input);
           const math::RangeI xRange{ bmu.x - radius < 0 ? -bmu.x : -radius, bmu.x + radius > m_Neurons.size() ? static_cast<int>(m_Neurons.size() - bmu.x) : radius};
           const math::RangeI yRange{ bmu.y - radius < 0 ? -bmu.y : -radius, bmu.y + radius > m_Neurons.size() ? static_cast<int>(m_Neurons.size() - bmu.y) : radius};
           for(int ii = xRange.start ; ii < xRange.end; ++ii)
           {
               const int iiSqr = ii*ii;
               for(int jj = yRange.start ; jj < yRange.end; ++jj)
               {
                   const int distanceSqr = iiSqr + jj*jj;
                   if( radiusSqr >= distanceSqr )
                   {
                        const double influence = math::Decay(learningRate, distanceSqr, sigma);
                        m_Neurons[bmu.x + ii][bmu.y + jj].UpdateWeights(input, influence);
                   }
               }
           }
        }

        void ProcessData(std::vector<std::vector<double>> const& data, int numberOfIteration,const int startingRadius, const double startingLearningRate)noexcept
        {
            const double timeConstant = numberOfIteration/std::log(startingRadius);
            for(int timeIter = 0; timeIter < numberOfIteration; ++timeIter)
            {
                const int radius = math::Decay(startingRadius, timeIter, timeConstant);
                const double learningRate = math::Decay(startingLearningRate, timeIter, numberOfIteration);

                UpdateWeights(data[timeIter%data.size()],radius,learningRate);
            }
        }

       void ProcessDataRandomly(std::vector<std::vector<double>> const& data, int numberOfIteration, const int startingRadius, const double startingLearningRate)noexcept
       {
           std::uniform_int_distribution<int> randomizer(0, data.size() - 1);
           const double timeConstant = numberOfIteration/std::log(startingRadius);
           for(int timeIter = 0; timeIter < numberOfIteration; ++timeIter)
           {
               const int radius = math::Decay(startingRadius, timeIter, timeConstant);
               const double learningRate = math::Decay(startingLearningRate, timeIter, numberOfIteration);

               UpdateWeights(data[randomizer(math::g_RandomEngine)],radius,learningRate);
           }
       }
        const Neuron& GetNeuron(const int x, const int y)const{
            return m_Neurons[x][y];
        }
    protected:
        std::vector<std::vector<Neuron>> m_Neurons;
    };

    inline std::vector<std::vector<double>> CreateRandomData( const int size, const int weightsNumber = 3)noexcept
    {
        std::uniform_real_distribution<double> uniform(0,1);
        std::vector<std::vector<double>> data ;
        data.reserve(size);
        for(int ii = 0; ii < size; ++ii)
        {
            std::vector<double> input;
            input.reserve(weightsNumber);
            for(int ii = 0; ii < weightsNumber; ++ii)
            {
                input.emplace_back(uniform(math::g_RandomEngine));
            }
            data.push_back(std::move(input));
        }
        return data;
    }
}
#endif // SOM_H
