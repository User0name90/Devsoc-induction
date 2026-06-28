#include <SFML/Graphics.hpp>
#include <random>
#include <mutex>
#include<shared_mutex>
class particle_splasher: public sf::Drawable,public sf::Transformable
{
private:
    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime; 
    };
    
    sf::Vector2f& gravity;
    std::mt19937  randomEngine;
    std::uniform_real_distribution<float> distribution;
    std::uniform_int_distribution<int> distribution_life;
    mutable std::shared_mutex            m_mutex;
    std::vector<Particle> m_particles;
    sf::VertexArray       m_vertices;
    sf::Vector2f          m_emitter;
    sf::Time              m_lifetime{sf::seconds(3)};
    sf::Vector2f          emmiter_posi;
    std::atomic_flag stop_flag=false;
    std::atomic_flag terminate_flag=false;
    std::atomic_flag finished_flag=true;
    /* data */
public:
    particle_splasher(sf::Vector2f& gravity);
    particle_splasher(const particle_splasher& to_be_copied);
    particle_splasher(particle_splasher&&);
    void start(sf::Color color,unsigned int count,sf::Vector2f posi,sf::Vector2f initial_vel,float av_speed,sf::Angle ang11,sf::Angle ang12, sf::Angle ang21,sf::Angle ang22,sf::FloatRect rect);
    void stop();
    void resume();
    void terminate();
    void draw(sf::RenderTarget& target , sf::RenderStates states)const override;
    bool isfinished();
    ~particle_splasher();
};


