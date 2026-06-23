#include "sphere.hpp"

Sphere::Sphere(double mass, double radius, sf::Vector2f position,sf::Vector2f velocity): mass{mass}, shape{(float)radius} , position{position}, velocity{velocity},radius{radius}
{

    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition(position);
    mutex=std::make_shared<std::mutex>();
};

double Sphere::getMass() const
{
    return mass;
}

double Sphere::getRadius() const
{
    return radius;
}
sf::Vector2f Sphere::getVelocity() const
{
    return velocity;
}
sf::Vector2f Sphere::getPosition()const
{
    return position;
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
    shape.setPosition(pos);
    this->position=pos;
}
void Sphere::setVelocity(sf::Vector2f velocity)
{
    this->velocity=velocity;
}
void Sphere::setColor(sf::Color c)
{
    shape.setFillColor(c);
}
void Sphere::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}
