#include <SFML/Graphics.hpp>
#include "sphere.hpp"
#include <list>
#include <chrono>
#include <thread>
#include <random>
class Engine
{

    std::list<Sphere> spheres;
    std::chrono::time_point<std::chrono::steady_clock> prev,current;
    sf::RectangleShape& space;
;
    std::mt19937  randomEngine;
    std::uniform_real_distribution<double> distribution;
    Engine(sf::RectangleShape& rect);
    void checkCollision();
    void checkBounds();
    void deleteSphere(int index);
    void createSphere();
    void start();
    void stop();
    void terminate();

};