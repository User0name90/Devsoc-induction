#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sphere.hpp"
#include <list>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
class Engine : public sf::Drawable
{
    sf::SoundBuffer sound_buffer;
    sf::Sound collision_sound;
    double sound_amp;
    mutable std::list<Sphere> spheres;
    mutable std::shared_mutex spheres_list_mutex;
    sf::RectangleShape& space;

    std::mt19937  randomEngine;
    std::uniform_real_distribution<double> distribution;

    std::thread uniform_thread;
    std::thread bounds_thread;
    std::thread collision_thread;

    mutable std::atomic_flag stop_flag=true;
    mutable std::atomic_flag terminate_flag=true;
    sf::Vector2f gravity={0,0};
    bool shpere_removed_flag=false;

    public:
    Engine(sf::RectangleShape& rect);
    void checkCollision();
    void checkBounds();
    void uniformMotion();
    void deleteSphere(size_t index);
    void createSphere();
    void start();
    void stop();
    void resume();
    void terminate();
    void draw(sf::RenderTarget& target, sf::RenderStates states)const  override;
    void playSound();
    void setGravity(sf::Vector2f);
    ~Engine();
};