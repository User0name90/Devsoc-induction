#pragma once
#include <SFML/Graphics.hpp>

class Sphere
{
    double radius;
    double mass;
    sf::Vector2f position;
    sf::Vector2f velocity;
    public:
    Sphere(double mass, double radius, sf::Vector2f position,sf::Vector2f velocity);
    void setMass(double);
    void setRadius(double);
    void setPosition(sf::Vector2f);
    void setVelocity(sf::Vector2f);
    double getMass() const;
    double getRadius() const;
};