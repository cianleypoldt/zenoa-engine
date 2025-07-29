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
    sf::RenderWindow &           window;
    std::vector<uint32_t>        circle;
    std::vector<uint32_t>        convex;
    std::vector<sf::ConvexShape> convex_shapes;
    bodies_ref &                 bodies;
    system_context *             cntx;
    bool                         fullscreen;
    sf::Clock                    clock;
    sf::Time                     frame_time;

    static sf::Vector2f translate(glm::vec2 pos) { return sf::Vector2f((pos.x + 500), (-pos.y + 250)); }

    bool active() { return window.isOpen(); }

    Renderer(system_context * sys_context, bool fullscreen = false, uint32_t fps = 60) :
        window(sys_context->window),
        bodies(sys_context->entity_manager.bodies),
        cntx(sys_context),
        fullscreen(fullscreen),
        frame_time(sf::seconds(1.0f / fps)) {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;
        if (fullscreen) {
            window.create(sf::VideoMode({ 1000, 500 }), "Debug View", sf::Style::None, sf::State::Fullscreen, settings);
        } else {
            window.create(sf::VideoMode({ 1000, 500 }), "Debug View", sf::Style::None, sf::State::Windowed, settings);
        }
        window.setPosition({ 0, 0 });
        window.display();
    }

    static void debug_circle(system_context * cntx, glm::vec2 position) {
        sf::CircleShape circleshape;
        circleshape.setRadius(10);
        circleshape.setOrigin({ circleshape.getRadius(), circleshape.getRadius() });
        circleshape.setFillColor(sf::Color::Transparent);
        circleshape.setOutlineColor(sf::Color::Green);
        circleshape.setOutlineThickness(1);
        circleshape.setPosition(translate(position));
        cntx->window.draw(circleshape);
    }

    void add_circle(uint32_t id) { circle.push_back(id); }

    void add_convex(uint32_t id) {
        uint32_t verticy_count = bodies.collider_shape[id].convex_col.end - bodies.collider_shape[id].convex_col.begin;
        sf::ConvexShape shape(bodies.collider_shape[id].convex_col.end - bodies.collider_shape[id].convex_col.begin);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setFillColor(sf::Color::Transparent);
        for (int i = 0; i < verticy_count; i++) {
            shape.setPoint(i,
                           sf::Vector2f{ cntx->vertex_pool[bodies.collider_shape[id].convex_col.begin + i].x,
                                         -cntx->vertex_pool[bodies.collider_shape[id].convex_col.begin + i].y });
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

        sf::CircleShape circleshape;
        circleshape.setFillColor(sf::Color::Transparent);
        circleshape.setOutlineColor(sf::Color::White);
        circleshape.setOutlineThickness(1);

        // Draw circles
        for (int i = 0; i < circle.size(); i++) {
            if (circle[i] == green) {
                circleshape.setOutlineColor(sf::Color::Green);
                line.setFillColor(sf::Color::Green);
            } else {
                circleshape.setOutlineColor(sf::Color::White);
                line.setFillColor(sf::Color::White);
            }
            circleshape.setRadius(bodies.collider_shape[circle[i]].circle_col.radius);
            circleshape.setOrigin({ circleshape.getRadius(), circleshape.getRadius() });
            circleshape.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setSize({ circleshape.getRadius(), 1 });
            line.setRotation(-sf::radians(bodies.rotation[circle[i]] + 3.14 / 2));
            window.draw(line);
            window.draw(circleshape);
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
