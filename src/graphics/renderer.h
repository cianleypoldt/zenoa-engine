#pragma once
#include "../engine/context.h"
#include "/home/cianleypoldt/Documents/cpp/Zenoa/src/engine/bodies.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Sleep.hpp>
#include <vector>

class Renderer
{
  public:
    sf::RenderWindow&            window;
    std::vector<uint32_t>        circle;
    std::vector<uint32_t>        convex;
    std::vector<sf::ConvexShape> convex_shapes;
    bodies_container&            bodies;
    system_context*              cntx;
    bool                         fullscreen;
    sf::Clock                    clock;
    sf::Time                     frame_time;

    static sf::Vector2f translate(glm::vec2 pos) { return sf::Vector2f((pos.x + 500), (-pos.y + 250)); }

    bool active() { return window.isOpen(); }

    Renderer(system_context* cntx_param, bool fullscreen_param = false, uint32_t fps = 60) :
        window(cntx_param->window),
        bodies(cntx_param->entity_manager.bodies),
        cntx(cntx_param),
        fullscreen(fullscreen_param),
        frame_time(sf::seconds(1.0f / fps)) {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;
        if (fullscreen_param) {
            window.create(sf::VideoMode({ 1000, 500 }), "Debug View", sf::Style::None, sf::State::Fullscreen, settings);
        } else {
            window.create(sf::VideoMode({ 1000, 500 }), "Debug View", sf::Style::None, sf::State::Windowed, settings);
        }
        window.setPosition({ 0, 0 });
        window.display();
    }

    static void debug_circle(system_context* cntx, glm::vec2 position) {
        sf::CircleShape circle_shape;
        circle_shape.setRadius(10);
        circle_shape.setOrigin({ circle_shape.getRadius(), circle_shape.getRadius() });
        circle_shape.setFillColor(sf::Color::Transparent);
        circle_shape.setOutlineColor(sf::Color::Green);
        circle_shape.setOutlineThickness(1);
        circle_shape.setPosition(translate(position));
        cntx->window.draw(circle_shape);
    }

    void add_circle(uint32_t id) { circle.push_back(id); }

    void add_convex(uint32_t id) {
        uint32_t        verticy_count = bodies.collider[id].convex.end - bodies.collider[id].convex.begin;
        sf::ConvexShape shape(bodies.collider[id].convex.end - bodies.collider[id].convex.begin);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setFillColor(sf::Color::Transparent);
        for (int i = 0; i < verticy_count; i++) {
            shape.setPoint(i,
                           sf::Vector2f{ cntx->vertex_pool[bodies.collider[id].convex.begin + i].x,
                                         -cntx->vertex_pool[bodies.collider[id].convex.begin + i].y });
        }
        convex_shapes.push_back(shape);
        convex.push_back(id);
    }

    void refresh(int green = -1) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        sf::Time elapsed = clock.restart();  // Only restart once per frame

        sf::RectangleShape line;
        line.setFillColor(sf::Color::White);

        sf::CircleShape circle_shape;
        circle_shape.setFillColor(sf::Color::Transparent);
        circle_shape.setOutlineColor(sf::Color::White);
        circle_shape.setOutlineThickness(1);

        // Draw circles
        for (int i = 0; i < circle.size(); i++) {
            if (circle[i] == green) {
                circle_shape.setOutlineColor(sf::Color::Green);
                line.setFillColor(sf::Color::Green);
            } else {
                circle_shape.setOutlineColor(sf::Color::White);
                line.setFillColor(sf::Color::White);
            }
            circle_shape.setRadius(bodies.collider[circle[i]].circle.radius);
            circle_shape.setOrigin({ circle_shape.getRadius(), circle_shape.getRadius() });
            circle_shape.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setSize({ circle_shape.getRadius(), 1 });
            line.setRotation(-sf::radians(bodies.rotation[circle[i]] + 3.14 / 2));
            window.draw(line);
            window.draw(circle_shape);
        }

        // Draw convex shapes - fixed to use the correct data
        for (int i = 0; i < convex.size(); i++) {
            if (convex[i] == green) {
                convex_shapes[i].setOutlineColor(sf::Color::Green);
            }
            convex_shapes[i].setPosition(translate(bodies.position[convex[i]]));
            convex_shapes[i].setRotation(sf::radians(-bodies.rotation[convex[i]]));
            window.draw(convex_shapes[i]);
        }

        window.display();
        window.clear();

        sf::Time sleep_time = frame_time - clock.getElapsedTime();
        if (sleep_time > sf::Time::Zero) {
            sf::sleep(sleep_time);
        }
    }
};
