#pragma once
#include <SFML/Graphics.hpp>

class Sphere : public sf::Drawable
{
    double radius;
    double mass;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;
    public:
    std::shared_ptr<std::mutex> mutex;
    std::chrono::steady_clock::time_point last_update; 
    Sphere(double mass, double radius, sf::Vector2f position,sf::Vector2f velocity);
    void setMass(double);
    void setRadius(double);
    void setPosition(sf::Vector2f);
    void setVelocity(sf::Vector2f);
    void setColor(sf::Color);
    
    double getMass() const;
    double getRadius() const;
    sf::Color getColor() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    bool isDead()const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
};