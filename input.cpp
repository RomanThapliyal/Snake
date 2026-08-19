#include "Input.hpp"
#include <iostream>
InputState Input::poll(sf::RenderWindow& window)
{
    InputState state;

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::W ||keyPressed->scancode == sf::Keyboard::Scancode::Up)
            {
                state.requestedDir = Snake::Up;
                state.hasDirRequest = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::S ||keyPressed->scancode == sf::Keyboard::Scancode::Down)
            {
                state.requestedDir = Snake::Down;
                state.hasDirRequest = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::D ||keyPressed->scancode == sf::Keyboard::Scancode::Right)
            {
                state.requestedDir = Snake::Right;
                state.hasDirRequest = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::A ||keyPressed->scancode == sf::Keyboard::Scancode::Left)
            {
                state.requestedDir = Snake::Left;
                state.hasDirRequest = true;
            }
            // Commands
            else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
            {
                state.startPressed = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::P)
            {
                state.pausePressed = true;
                std::cout<<"jojo\n";
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::R)
            {
                state.restartPressed = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::X)
            {
                state.exitPressed = true;
            }
            else if (keyPressed->scancode == sf::Keyboard::Scancode::G)
            {
                state.gridToggle=true;
                std::cout<<"G\n";
            }
        }
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                state.mouseClicked = true;
                state.clickPosition = sf::Vector2f(mousePressed->position);
            }
        }
    }

    return state;
}