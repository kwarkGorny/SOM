#include <iostream>
#include "SOM.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>

inline sf::Color WeightsToColor(const std::vector<double>& weights)noexcept
{
    assert(weights.size() >= 3 && "weights need to have size of 3 or more");
    return  { static_cast<unsigned char>(weights[0] * 255)
            , static_cast<unsigned char>(weights[1] * 255)
            , static_cast<unsigned char>(weights[2] * 255)};
}

struct SomInfo
{
    double learningRate = 0.1;
    int iterationCount = 1000;
    int startingRadius = 25;
    int networkSize = 50;
    int inputNumber = 5;
};


int main(int argc, char *argv[])
{
    SomInfo info;
    std::cout << "network Size\n";
    std::cin >> info.networkSize;
    std::cout << "iteration count\n";
    std::cin >> info.iterationCount;
    std::cout << "input number\n";
    std::cin >> info.inputNumber;
    std::cout << "starting Radius\n";
    std::cin >> info.startingRadius;
    std::cout << "learning Rate\n";
    std::cin >> info.learningRate;

    std::cout << "Press R to reset network and generate new input\n";
    std::cout << "Press L to update network by randomly selecting from data\n";
    std::cout << "Press K to update network by sequential selecting from data\n";

    som::Network network = som::Network::CreateRandomNetwork(info.networkSize);
    auto data = som::CreateRandomData(info.inputNumber);

    const int windowWidth = 500;
    const int windowHeight = 500;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SOM");

    const float neuronWidth = windowWidth / static_cast<float>(info.networkSize);
    const float neuronHeight = windowHeight / static_cast<float>(info.networkSize);
    sf::RectangleShape neuronRect(sf::Vector2f(neuronWidth, neuronHeight));




    const auto drawNetwork = [&](){
        window.clear();
        for(int ii =0; ii < info.networkSize; ++ii )
        {
            for(int jj =0; jj < info.networkSize; ++jj )
            {
                neuronRect.setFillColor(WeightsToColor(network.GetNeuron(ii,jj).GetWeights()));
                neuronRect.setPosition(ii * neuronWidth, jj * neuronHeight);
                window.draw(neuronRect);
            }
        }
        window.display();
    };

    drawNetwork();
    window.display();

    while (window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            switch(e.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyReleased:
                if(e.key.code == sf::Keyboard::K)
                {
                    const auto t1 = std::chrono::high_resolution_clock::now();
                    network.ProcessData(data, info.iterationCount, info.startingRadius, info.learningRate);
                    const auto t2 = std::chrono::high_resolution_clock::now();
                    std::cout<< std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " ms\n";

                    drawNetwork();
                }
                if(e.key.code == sf::Keyboard::L)
                {
                    const auto t1 = std::chrono::high_resolution_clock::now();
                    network.ProcessDataRandomly(data, info.iterationCount, info.startingRadius, info.learningRate);
                    const auto t2 = std::chrono::high_resolution_clock::now();
                    std::cout<< std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << " ms\n";

                    drawNetwork();
                }
                else if(e.key.code == sf::Keyboard::R) // Reset network
                {
                    network = som::Network::CreateRandomNetwork(info.networkSize);
                    data = som::CreateRandomData(info.inputNumber);
                    drawNetwork();
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}
