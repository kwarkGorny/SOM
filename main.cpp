#include <iostream>
#include "SOM.h"
#include <SFML/Graphics.hpp>
#include <cassert>
inline sf::Color WeightsToColor(const std::vector<double>& weights)noexcept
{
    assert(weights.size() >= 3 && "weights need to be have size of 3 or more");
    return  { static_cast<unsigned char>(weights[0] * 255)
            , static_cast<unsigned char>(weights[1] * 255)
            , static_cast<unsigned char>(weights[2] * 255)};
}



int main(int argc, char *argv[])
{
    const double learningRate = 0.5;
    const double startingRadius = 5;
    const int networkSize = 50;
    SOM::Network network = SOM::Network::CreateRandomNetwork(networkSize,3);
    std::vector<double> input = SOM::CreateRandomInput(3);

    const int FPS = 60;
    const int windowWidth = 600;
    const int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), "SOM");
    window.setFramerateLimit(FPS);

    const int neuronWidth = windowWidth / networkSize;
    const int neuronHeight = windowHeight / networkSize;
    sf::RectangleShape neuronRect(sf::Vector2f(neuronWidth, neuronHeight));

    sf::RectangleShape bmuRect(sf::Vector2f(neuronWidth/2, neuronHeight/2));
    bmuRect.setFillColor(sf::Color::Yellow);


    const auto& drawNetwork = [&](){
        window.clear();
        for(int ii =0; ii < networkSize; ++ii )
        {
            for(int jj =0; jj < networkSize; ++jj )
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
        {
            sf::Event e;
            while(window.pollEvent(e))
            {
                switch(e.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    break;
                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Space) // update network by input
                    {
                        network.UpdateNetwork(input, startingRadius, learningRate);
                        drawNetwork();
                    }
                    else if(e.key.code == sf::Keyboard::B) // show BMU
                    {
                        drawNetwork();
                        const Math::PointI bmu = network.FindBMU(input);
                        bmuRect.setPosition(bmu.x * neuronWidth + neuronWidth/4, bmu.y * neuronHeight + neuronHeight/4);
                        window.draw(bmuRect);
                        window.display();
                    }
                    else if(e.key.code == sf::Keyboard::R) // Reset network
                    {
                        network = SOM::Network::CreateRandomNetwork(networkSize,3);
                        drawNetwork();
                    }
                    else if(e.key.code == sf::Keyboard::N) // new input
                    {
                        input = SOM::CreateRandomInput(3);
                    }
                    else if(e.key.code == sf::Keyboard::K) // update network by 20 iteration from 100 random inputs
                    {
                        auto data = SOM::CreateRandomData(3,100);
                        network.ProcessNetwork(data, startingRadius, learningRate, 20);
                        drawNetwork();
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    return 0;
}
