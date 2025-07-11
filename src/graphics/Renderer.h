#pragma once
#include "../engine/SystemContext.h"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <vector>

class Renderer {
  public:
    sf::RenderWindow& window;
    std::vector<uint32_t> circle;
    std::vector<uint32_t> convex;
    std::vector<sf::ConvexShape> convex_shapes;
    Bodies& bodies;
    SystemContext* cntx;
    bool fullscreen;
    sf::Clock clock;
    sf::Time frameTime;

    sf::RenderTexture texture;
    int framenumber = 0;

    static sf::Vector2f translate(glm::vec2 pos) {
        return sf::Vector2f((pos.x + 500) * 2, (-pos.y + 250) * 2);
    }

    bool active() {
        return window.isOpen();
    }

    Renderer(SystemContext* _cntx, bool Fullscreen = false, uint32_t fps = 60)
        : window(_cntx->window), cntx(_cntx), bodies(_cntx->entity_manager.bodies), texture({2000, 1000}), fullscreen(Fullscreen), frameTime(sf::seconds(1.0f / fps)) {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;
        if (Fullscreen)
            window.create(sf::VideoMode({1000, 500}), "Debug View", sf::Style::None, sf::State::Fullscreen, settings);
        else
            window.create(sf::VideoMode({2000, 1000}), "Debug View", sf::Style::None, sf::State::Windowed, settings);
        window.setPosition({0, 0});
        window.display();
        texture.clear(sf::Color::Black);

        // Initialize frame texture for video recording
    }

    static void debugCircle(SystemContext* cntx, glm::vec2 position) {
        sf::CircleShape circleshape;
        circleshape.setRadius(10);
        circleshape.setOrigin({circleshape.getRadius(), circleshape.getRadius()});
        circleshape.setFillColor(sf::Color::Transparent);
        circleshape.setOutlineColor(sf::Color::Green);
        circleshape.setOutlineThickness(2);
        circleshape.setPosition(translate(position));
        cntx->window.draw(circleshape);
    }

    void addCircle(uint32_t id) {
        circle.push_back(id);
    }
    void addConvex(uint32_t id) {
        uint32_t verticy_count = bodies.collider[id].convex.end - bodies.collider[id].convex.begin;
        sf::ConvexShape shape(bodies.collider[id].convex.end - bodies.collider[id].convex.begin);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setFillColor(sf::Color::Transparent);
        for (int i = 0; i < verticy_count; i++)
            shape.setPoint(i, sf::Vector2f{cntx->vertex_pool[bodies.collider[id].convex.begin + i].x * 2,
                                           -cntx->vertex_pool[bodies.collider[id].convex.begin + i].y * 2});
        convex_shapes.push_back(shape);
        convex.push_back(id);
    }

    void refresh(int green = -1) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            window.close();

        sf::Time elapsed = clock.restart(); // Only restart once per frame

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
            circleshape.setRadius(bodies.collider[circle[i]].circle.radius * 2);
            circleshape.setOrigin({circleshape.getRadius(), circleshape.getRadius()});
            circleshape.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setPosition(translate(bodies.position[circle[i]]));
            line.setSize({circleshape.getRadius(), 1});
            line.setRotation(-sf::radians(bodies.rotation[circle[i]] + 3.14 / 2));
            window.draw(line);
            texture.draw(line);
            window.draw(circleshape);
            texture.draw(circleshape);
        }

        // Draw convex shapes - fixed to use the correct data
        for (int i = 0; i < convex.size(); i++) {
            if (convex[i] == green)
                convex_shapes[i].setOutlineColor(sf::Color::Green);
            convex_shapes[i].setPosition(translate(bodies.position[convex[i]]));
            convex_shapes[i].setRotation(sf::radians(-bodies.rotation[convex[i]]));
            window.draw(convex_shapes[i]);
            texture.draw(convex_shapes[i]);
        }

        window.display();

        sf::Image frame = texture.getTexture().copyToImage();
        std::stringstream ss;
        ss << "frames/frame_" << std::setw(3) << std::setfill('0') << framenumber++ << ".png";
        std::string filename = ss.str();
        frame.saveToFile(filename);

        // window.clear(sf::Color::White);
        // window.display();
        window.clear();
        texture.clear(sf::Color::Black);

        // Sleep to maintain 60 FPS
        sf::Time sleepTime = frameTime - clock.getElapsedTime();
        if (sleepTime > sf::Time::Zero) {
            sf::sleep(sleepTime);
        }
    }
};
