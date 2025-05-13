#include "engine/Simulation.h"
#include "utility/Utility.h"
#include <SFML/Graphics.hpp>

int main()
{

    utl::initLogger(utl::LogLevel::Debug, "physics_engine.log");
    UTL_INFO("2D Physics Engine Starting...");

    sf::RenderWindow window(sf::VideoMode({1000, 500}), "Debug View", sf::Style::None, sf::State::Windowed);
    window.setPosition({0, 0});
    window.display();

    auto simulation = Simulation::make_context();
    unsigned int id[101];
    float creationTime = utl::measureExecutionTime([&]() {
        for (int i = 0; i < 101; i++)
        {
            id[i] = Simulation::addEntity(simulation);
            Simulation::addCircleCollider(simulation, id[i], 10);
        }
    });
    UTL_INFO("Created 100 entities in %.2f ms", creationTime);

    sf::CircleShape circle;
    circle.setOutlineColor(sf::Color::White);
    circle.setFillColor(sf::Color::Transparent);
    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            window.close();
        window.clear();
        for (int i = 0; i < 100; i++)
        {
            float radius = Simulation::getCircleRadius(simulation, id[i]);
            auto pos = Simulation::getPosition(simulation, id[i]);
            circle.setPosition(sf::Vector2f(pos.x, pos.y));
            circle.setRadius(radius);
            window.draw(circle);
        }
        window.display();
    }

    UTL_INFO("2D Physics Engine shutting down");
    Simulation::drop(simulation);
}
