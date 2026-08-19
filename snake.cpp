#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;
class Snake
{
public:
    int gameSpeed;
    DWORD lastMove;
    bool snakeFound = false;
    enum GameState
    {
        menu,
        gameOn,
        end
    };
    GameState gameState = gameOn;
    int screenlen = 30, screenbred = 30;
    int snakeLen = 3;
    enum Direction
    {
        Up,
        Down,
        Left,
        Right
    };
    Direction dir = Right;
    int snakeX[100] = {9, 8, 7};
    int snakeY[100] = {6, 6, 6};
    int oldTailX, oldTailY, nextHeadX, nextHeadY;
    int foodX, foodY;
    void start()
    {
        cout << "Enter a button";
        while (_kbhit() == false)
        {
        };
        gameState = gameOn;
    }
    void setUp()
    {
        lastMove = GetTickCount();
        gameSpeed = 10;
        srand(time(0));
        Cursor();
        food();
    }
    void update()
    {
        if (GetTickCount() - lastMove >= gameSpeed)
        {
            logic();
            lastMove += gameSpeed;
        }
    }
    void draw()
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
        for (int i = 1; i <= screenlen; i++)
        {
            for (int j = 1; j <= screenbred; j++)
            {
                snakeFound = false;
                for (int k = 0; k < snakeLen; k++)
                {
                    if (snakeX[k] == j && snakeY[k] == i)
                    {
                        snakeFound = true;
                    }
                }
                if (i == 1 || i == screenlen || j == 1 || j == screenbred)
                {
                    cout << "##";
                }
                else if (snakeFound)
                {
                    cout << "O ";
                }
                else if (foodX == j && foodY == i)
                {
                    cout << "@ ";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << '\n';
        }
    }
    void logic()
    {
        calcNextHeadPos();
        if (wallCollision() || bodyCollison())
        {
            gameState = end;
            return;
        }
        moveSnake();
        checkFood();
    }
    void input()
    {
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'w' && dir != Down)
            {
                dir = Up;
            }
            else if (ch == 's' && dir != Up)
            {
                dir = Down;
            }
            else if (ch == 'd' && dir != Left)
            {
                dir = Right;
            }
            else if (ch == 'a' && dir != Right)
            {
                dir = Left;
            }
        }
    }
    void food()
    {
        bool invalidFood = true;

        while (invalidFood)
        {
            invalidFood = false;

            foodX = rand() % screenlen + 1;
            foodY = rand() % screenbred + 1;

            for (int i = 0; i < snakeLen; i++)
            {
                if (foodX == snakeX[i] && foodY == snakeY[i])
                {
                    invalidFood = true;
                }
            }
            if (foodX == 1 || foodX == screenbred || foodY == 1 || foodY == screenlen)
            {
                invalidFood = true;
            }
        }
    }
    void Cursor()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
        system("cls");
    }
    void calcNextHeadPos()
    {
        nextHeadX = snakeX[0], nextHeadY = snakeY[0];
        if (dir == Right)
        {
            nextHeadX++;
        }
        else if (dir == Left)
        {
            nextHeadX--;
        }
        else if (dir == Up)
        {
            nextHeadY--;
        }
        else if (dir == Down)
        {
            nextHeadY++;
        }
    }
    void moveSnake()
    {
        for (int i = snakeLen - 1; i > 0; i--)
        {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0] = nextHeadX;
        snakeY[0] = nextHeadY;
    }
    bool wallCollision()
    {
        if (nextHeadX == 1 || nextHeadX == screenlen || nextHeadY == 1 || nextHeadY == screenbred)
        {
            return true;
        }
        return false;
    }
    bool bodyCollison()
    {
        for (int i = 1; i < snakeLen; i++)
        {
            if (nextHeadX == snakeX[i] && nextHeadY == snakeY[i])
            {
                return true;
            }
        }
        return false;
    }
    void checkFood()
    {
        oldTailX = snakeX[snakeLen - 1], oldTailY = snakeY[snakeLen - 1];
        if (snakeX[0] == foodX && snakeY[0] == foodY)
        {
            food();
            snakeLen++;
            snakeX[snakeLen - 1] = oldTailX;
            snakeY[snakeLen - 1] = oldTailY;
        }
    }
    void gameOver()
    {
        system("cls");
        cout << "\n\n\n\n\n-------Game Over-------\n\n\n";
    }
};
int main()
{
    Snake s;
    s.start();
    s.setUp();
    while (s.gameState == Snake::gameOn)
    {
        s.input();
        s.update();
        s.draw();
    }
    s.gameOver();
}