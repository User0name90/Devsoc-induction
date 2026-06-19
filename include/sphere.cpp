#include "sphere.hpp"

Sphere::Sphere(double mass, double radius, sf::Vector2f position,sf::Vector2f velocity): mass{mass}, radius{radius} , position{position}, velocity{velocity}
{};

double Sphere::getMass() const
{
    return mass;
}

double Sphere::getRadius() const
{
    return radius;
}
//setter functions
void Sphere::setMass(double mass)
{
    if(mass<=0) return;
    this->mass=mass;
}
void Sphere::setRadius(double radius)
{
    if(radius<=0) return;
    this->radius=radius;
}
void Sphere::setPosition(sf::Vector2f pos)
{
    this->position=pos;
}
void Sphere::setVelocity(sf::Vector2f velocity)
{
    this->velocity=velocity;
}