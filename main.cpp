#include "snaky.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include <iostream>
int main()
{
    Snake s;
    Renderer renderer;
    Input inputSystem;
    s.setUp();
    renderer.setUp();
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(s.getCols() * s.getCellSize()), static_cast<unsigned int>(s.getRows() * s.getCellSize())}), "Snake");

    while (window.isOpen()&&s.getGameState()!=Snake::exit)
    {
        while(s.getGameState()==Snake::menu&&window.isOpen()){
            renderer.menueScreen(window);
            InputState menue=inputSystem.poll(window);
            if (menue.mouseClicked)
            {
                auto action = renderer.getClickedAction(menue.clickPosition);
                if (action.has_value() && action.value() == ButtonAction::Start)
                {
                    s.gameState = Snake::gameOn;
                }
                else if (action.has_value() && action.value() == ButtonAction::Exit)
                {
                    window.close();
                }
            }
            s.menue(menue);
        }
        
        while (s.getGameState() == Snake::gameOn)
        {
            InputState in = inputSystem.poll(window);
            s.applyInput(in);
            s.update();
            renderer.draw(window,s);
        }
        while(s.getGameState() == Snake::end && window.isOpen())
        {
            renderer.gameOverScreen(window, s);
            InputState gameOverInput = inputSystem.poll(window);
            if (gameOverInput.mouseClicked)
            {
                auto action = renderer.getClickedAction(gameOverInput.clickPosition);
                if (action.has_value() && action.value() == ButtonAction::Start)
                {
                    std::cout<<"REstat\n";
                    s.restart();
                }
                else if (action.has_value() && action.value() == ButtonAction::Exit)
                {
                    window.close();
                }
            }
            s.gameOver(gameOverInput);
        }
        while(s.gameState==Snake::pause&&window.isOpen()){
            renderer.pauseScreen(window,s);
            InputState pause=inputSystem.poll(window);
            if (pause.mouseClicked)
            {
                auto action = renderer.getClickedAction(pause.clickPosition);
                if (action.has_value() && action.value() == ButtonAction::Start)
                {
                    s.gameState=s.gameOn;
                }
                else if (action.has_value() && action.value() == ButtonAction::Exit)
                {
                    window.close();
                }
            }
            s.pauseUnpause(pause);
        }
    }
    window.close();
}