#include "snaky.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include <iostream>

void handleMenueInput(InputState &input,Renderer &renderer,Snake &s,sf::RenderWindow &window){
    if (input.mouseClicked)
    {
        auto action = renderer.getClickedAction(input.clickPosition);
        if (action.has_value() && action.value() == ButtonAction::Start)
        {
           s.restart();
        }
        else if (action.has_value() && action.value() == ButtonAction::Exit)
       {
            window.close();
       }
    }
}

void handleGameOverInput(InputState &input,Renderer &renderer,Snake &s,sf::RenderWindow &window){
    if (input.mouseClicked)
    {
        auto action = renderer.getClickedAction(input.clickPosition);
        if (action.has_value() && action.value() == ButtonAction::Start)
        {
            s.restart();
        }
        else if (action.has_value() && action.value() == ButtonAction::Exit)
        {
            window.close();
        }
     }
}

void handlePauseInput(InputState &input,Renderer &renderer,Snake &s,sf::RenderWindow &window){
    if (input.mouseClicked)
    {
       auto action = renderer.getClickedAction(input.clickPosition);
         if (action.has_value() && action.value() == ButtonAction::Start)
         {
             s.pauseUnpause(input,true);
         }
         else if (action.has_value() && action.value() == ButtonAction::Exit)
         {
             window.close();
       }
    }
}

int main()
{
    Snake s;
    Renderer renderer;
    Input inputSystem;
    s.setUp();
    renderer.setUp();
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(s.getCols() * s.getCellSize()), static_cast<unsigned int>(s.getRows() * s.getCellSize())}), "Snake");

    while (window.isOpen()&&s.getGameState()!=Snake::Exit)
    {
        switch (s.getGameState())
        {
        case Snake::Menu :{
            renderer.menuScreen(window);
            InputState menue=inputSystem.poll(window);
            handleMenueInput(menue,renderer,s,window);
            s.menu(menue);
            break;
        }
        case Snake::GameOn :{
            InputState in = inputSystem.poll(window);
            s.applyInput(in);
            s.update();
            renderer.draw(window,s);
            break;
        }
        case Snake::End :{
            renderer.gameOverScreen(window, s);
            InputState gameOverInput = inputSystem.poll(window);
            handleGameOverInput(gameOverInput,renderer,s,window);
            s.gameOver(gameOverInput); 
            break;
        }
        case Snake::Pause :{
            renderer.pauseScreen(window,s);
            InputState pauseInput=inputSystem.poll(window);
            handlePauseInput(pauseInput,renderer,s,window);
            s.pauseUnpause(pauseInput,false);
            break;
        }
        }
        window.display();
    }
    window.close();
}
