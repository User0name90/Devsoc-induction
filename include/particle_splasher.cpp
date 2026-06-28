#include"particle_splasher.hpp"

particle_splasher::particle_splasher(sf::Vector2f& gravity):randomEngine{std::random_device{}()}, gravity{gravity}
{
        distribution_life.param(std::uniform_int_distribution<int>::param_type(1000,2000));
}
particle_splasher::particle_splasher(const particle_splasher& to_be_copied): gravity{to_be_copied.gravity}, randomEngine{std::random_device{}()}, m_particles{to_be_copied.m_particles},
      m_vertices{to_be_copied.m_vertices},
      m_lifetime{to_be_copied.m_lifetime},
      emmiter_posi{to_be_copied.emmiter_posi},
      distribution{to_be_copied.distribution},
    distribution_life{to_be_copied.distribution_life}
{
}
particle_splasher::particle_splasher(particle_splasher&& to_be_moved): gravity{to_be_moved.gravity}, randomEngine{std::move(to_be_moved.randomEngine)}, m_particles{std::move(to_be_moved.m_particles)},
      m_vertices{std::move(to_be_moved.m_vertices)},
      m_lifetime{std::move(to_be_moved.m_lifetime)},
      emmiter_posi{std::move(to_be_moved.emmiter_posi)},
      distribution{std::move(to_be_moved.distribution)},
      distribution_life{std::move(to_be_moved.distribution_life)}
{
}
void particle_splasher::draw(sf::RenderTarget& target , sf::RenderStates states)const
{
    std::shared_lock<std::shared_mutex> sl (m_mutex);
    states.transform *= getTransform();
    states.texture = nullptr;

        // draw the vertex array
        target.draw(m_vertices, states);
}
void particle_splasher::start(sf::Color color,unsigned int count,sf::Vector2f m_emitter,sf::Vector2f initial_velocity,float av_speed,sf::Angle ang11,sf::Angle ang12, sf::Angle ang21,sf::Angle ang22,sf::FloatRect space)
{
    finished_flag.clear();
    float angle1,angle2,ran_vel;
    m_particles.resize(count);
    m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
    m_vertices.resize(count);

    for(int i=0; i<count/2;i++)
        {
            distribution.param(std::uniform_real_distribution<float>::param_type(ang11.asDegrees(),ang12.asDegrees()));
            angle1=distribution(this->randomEngine);
            distribution.param(std::uniform_real_distribution<float>::param_type(0.f,av_speed*2));
            ran_vel=distribution(this->randomEngine);

            m_particles[i].velocity = sf::Vector2f(ran_vel, sf::degrees(angle1))+initial_velocity;
            m_particles[i].lifetime = sf::milliseconds(distribution_life(this->randomEngine));
            // reset the position of the corresponding vertex
            m_vertices[i].position = m_emitter;
            m_vertices[i].color=color;
        }
        for(int i=count/2 ; i<count;i++)
        {
            distribution.param(std::uniform_real_distribution<float>::param_type(ang21.asDegrees(),ang22.asDegrees()));
            angle2=distribution(this->randomEngine);
            distribution.param(std::uniform_real_distribution<float>::param_type(0.f,av_speed*2));
            ran_vel=distribution(this->randomEngine);

            m_particles[i].velocity = sf::Vector2f(ran_vel,sf::degrees(angle2))+initial_velocity;
            m_particles[i].lifetime = sf::milliseconds(distribution_life(this->randomEngine));
            // reset the position of the corresponding vertex
            m_vertices[i].position = m_emitter;
            m_vertices[i].color=color;
        }
    
    sf::Clock total_time;
    sf::Clock clock;
    sf::Time elapsed;
    bool just_paused=false;
    while(1)
    {
        if(total_time.getElapsedTime().asMilliseconds()>2000) break;
        while(stop_flag.test())
        {
            clock.stop();
            total_time.stop();

            stop_flag.wait(true);
            just_paused=true;
        }
        if(just_paused)
        {
            clock.start();
            total_time.start();
            just_paused=false;
        }
        if(terminate_flag.test())
        {
            finished_flag.test_and_set();
            return;
        }
        elapsed=clock.restart();
        
        std::unique_lock<std::shared_mutex>ul(m_mutex);
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // if the particle is dead, leave it
            if (p.lifetime <= sf::Time::Zero) continue;
            
            if(m_vertices[i].position.y>=space.position.y+space.size.y)
            {
                p.velocity=sf::Vector2f(p.velocity.x,-p.velocity.y);
            }
            else p.velocity+=sf::Vector2f(gravity)*elapsed.asSeconds();

            m_vertices[i].position += p.velocity * elapsed.asSeconds();
            // update the alpha (transparency) of the particle according to its lifetime
            float ratio           = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
        }
    }
    finished_flag.test_and_set();
}
void particle_splasher::stop()
{
    stop_flag.test_and_set();
}
void particle_splasher::resume()
{
    stop_flag.clear();
    stop_flag.notify_all();
}
void particle_splasher::terminate()
{
    terminate_flag.test_and_set();
}
bool particle_splasher::isfinished()
{
    return finished_flag.test();
}
particle_splasher::~particle_splasher()
{
    terminate_flag.test_and_set();
    stop_flag.clear();
    stop_flag.notify_all();
}