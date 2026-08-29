# 🐍 Snaky

A classic Snake game built from scratch in **C++ and SFML**.

This project started as my first 2D game development project in C++. Instead of keeping the gameplay completely traditional, I added features such as **smooth movement interpolation, wall wrapping, a temporary magnet power-up, keyboard and mouse controls, pause functionality, and a simple menu system**.

The goal of the project was not only to make Snake playable, but also to learn how a real-time game loop, input handling, game logic, rendering, timers, and game states work together.

---

## 🎮 Features

### Classic Snake Gameplay

* Control the snake using **WASD** or **Arrow Keys**.
* Eat food to increase your score and snake length.
* The snake gradually becomes faster as food is collected.
* Collision with the snake's body ends the game.
* Collision with the walls ends the game when wrapping is disabled.

### 🧲 Magnet Power-Up

A special magnet power-up periodically appears on the board.

When the snake collects it:

* The magnet becomes active.
* Food is attracted toward the snake.
* Food moves smoothly toward the snake instead of waiting for the snake to reach it normally.
* The magnet remains active for a limited duration.
* After the effect expires, the magnet enters a waiting period before another one can spawn.

The magnet uses distance-based attraction rather than simply teleporting the food toward the snake.

### 🌀 Wall Wrapping

Wall wrapping can be toggled during gameplay.

When enabled:

```text
        ┌───────────────┐
        │               │
        │           🐍 →│
        │               │
        └───────────────┘
                     ↓
        ┌───────────────┐
        │← 🐍            │
        │               │
        └───────────────┘
```

The snake can leave one side of the playable area and reappear on the opposite side.

The renderer also handles the visual transition so the snake doesn't appear to jump awkwardly across the screen.

### 📐 Optional Grid

Press **G** to toggle the grid.

The grid makes the underlying game board easier to see and is useful for understanding how the snake moves from cell to cell.

### ⏸️ Pause System

Press **P** to pause and resume the game.

The game uses separate clocks for:

* Snake movement
* Frame interpolation
* Magnet timing

These clocks are stopped while the game is paused so that game events do not continue progressing in the background.

### 🖱️ Mouse Menu

The game supports mouse interaction with menu buttons.

Buttons are used for:

* Starting the game
* Restarting after Game Over
* Resuming after Pause
* Quitting the game

Keyboard controls are also available for the main menu.

### 👀 Directional Snake Eyes

The snake's head has directional eyes that change position depending on its current direction.

The eyes point toward:

* ⬆️ Up
* ⬇️ Down
* ⬅️ Left
* ➡️ Right

This gives the snake a little more personality than a simple collection of green squares.

### ✨ Smooth Movement

Although the snake moves logically from cell to cell, the renderer interpolates between the previous and current positions.

This allows the snake to visually move smoothly rather than appearing to instantly teleport from one grid cell to another.

---

## 🕹️ Controls

| Key       | Action                     |
| --------- | -------------------------- |
| `W` / `↑` | Move Up                    |
| `S` / `↓` | Move Down                  |
| `A` / `←` | Move Left                  |
| `D` / `→` | Move Right                 |
| `G`       | Toggle Grid                |
| `T`       | Toggle Wall Wrapping       |
| `P`       | Pause / Resume             |
| `R`       | Restart                    |
| `Enter`   | Start                      |
| `X`       | Exit                       |
| `Mouse`   | Interact with menu buttons |

---

## 🏗️ Project Structure

The project is separated into three main responsibilities:

```text
Snaky/
│
├── Snake
│   ├── snaky.hpp
│   └── snaky.cpp
│
├── Input
│   ├── Input.hpp
│   └── Input.cpp
│
├── Renderer
│   ├── Renderer.hpp
│   └── Renderer.cpp
│
├── Assets/
│   └── Fonts/
│       └── PressStart2P-Regular.ttf
│
└── main.cpp
```

### `Snake`

Responsible for the game's underlying state and gameplay logic.

This includes:

* Snake movement
* Direction handling
* Collision detection
* Food generation
* Snake growth
* Score
* Game states
* Magnet behaviour
* Wall wrapping
* Timers

The `Snake` class acts as the main game model.

### `Input`

Responsible for converting SFML events into a simpler `InputState`.

Instead of having the game logic directly process SFML keyboard events, input is collected and represented through commands such as:

```cpp
state.requestedDir = Snake::Up;
state.hasDirRequest = true;
```

This keeps input processing separate from gameplay logic.

### `Renderer`

Responsible for everything that appears on screen.

It handles:

* Snake rendering
* Food rendering
* Magnet rendering
* Walls
* Grid
* Text
* Buttons
* Menu
* Pause screen
* Game Over screen

The renderer reads the state of the `Snake` object without being responsible for changing the game's underlying logic.

### `main.cpp`

Coordinates the major game states and runs the main loop.

The game moves between states such as:

```text
             ┌─────────┐
             │  MENU   │
             └────┬────┘
                  │
                  ▼
             ┌─────────┐
        ┌───►│ GAME ON │
        │    └────┬────┘
        │         │
        │         ├──────────► PAUSE
        │         │               │
        │         │               │ Resume
        │         │               │
        │         │◄──────────────┘
        │         │
        │         ▼
        │    ┌─────────┐
        └────│   END   │
             └────┬────┘
                  │
             Restart / Quit
```

---

## ⚙️ Game Loop

The core game loop continuously:

1. Polls for input.
2. Applies player input.
3. Updates the game state.
4. Renders the current state.
5. Displays the frame.

Conceptually:

```cpp
while (window.isOpen())
{
    InputState input = inputSystem.poll(window);

    snake.applyInput(input);
    snake.update();

    renderer.draw(window, snake);

    window.display();
}
```

The actual implementation separates the menu, gameplay, pause, and game-over states into their own loops.

---

## 🧠 Movement System

Snake movement is based on a grid.

The logical position of the snake is stored using integer coordinates:

```cpp
snakeX[i]
snakeY[i]
```

Before moving, the next head position is calculated.

The game then checks:

```text
Next Head Position
        │
        ▼
 ┌───────────────┐
 │ Wall Collision│
 └───────┬───────┘
         │
         ▼
 ┌───────────────┐
 │ Body Collision│
 └───────┬───────┘
         │
         ▼
     Move Snake
         │
         ▼
    Check Food
         │
         ▼
   Check Magnet
```

This ensures collisions are detected before the snake's logical position is updated.

---

## 🎯 Food System

Food is randomly generated inside the playable area.

The game prevents food from spawning:

* Inside the snake.
* On top of an available magnet.

When the snake reaches the food:

```text
Food collected
      │
      ▼
Increase score
      │
      ▼
Increase snake length
      │
      ▼
Increase movement speed
      │
      ▼
Generate new food
```

The food also has a small collection radius, which allows the magnet system to interact naturally with it.

---

## 🧲 Magnet System

The magnet has three states:

```cpp
Waiting
Available
Active
```

### Waiting

The magnet is not visible and waits for its spawn timer.

### Available

The magnet appears somewhere on the board.

It cannot spawn:

* Inside the snake.
* On top of the food.

### Active

After the snake collects it, the magnet disappears from the board and begins attracting food.

The attraction is calculated using the distance between the snake and food.

The pull speed increases as the food gets closer to the snake:

```cpp
speed = minimumPullSpeed
      + (attractionRange - distance) * pullStrength;
```

This creates a simple physics-like attraction effect.

---

## 🌀 Wrap Rendering

One of the more interesting parts of the project is handling visual interpolation when wall wrapping is enabled.

For example, logically the snake might move from:

```text
X = 19 → X = 2
```

A normal interpolation would make the snake move across the entire screen.

Instead, the renderer detects the large coordinate difference and renders the snake as crossing the boundary:

```text
19 ────────────────→ 2
   boundary crossing
```

This allows the movement interpolation system and wrapping system to work together visually.

---

## ⏱️ Timing

The game uses SFML clocks to control different aspects of the game.

### Game Clock

Controls how often the snake's logical movement occurs.

### Frame Clock

Used to calculate the frame delta time (`dt`).

This is particularly important for the magnet because its movement is based on elapsed time rather than the number of frames rendered.

### Magnet Clock

Controls:

* Magnet spawn delay.
* Magnet active duration.

This separation prevents different game systems from becoming unnecessarily dependent on one another.

---

## 🎨 Rendering

The game uses simple SFML shapes rather than sprite sheets for most of the game.

The visual style consists primarily of:

* Rectangles for the snake and walls.
* Circles for food.
* Circles and text for the magnet.
* SFML text for menus and UI.

The project also uses the **Press Start 2P** font to give the game a retro arcade aesthetic.

---

## 🛠️ Technologies

* **C++**
* **SFML**
* Object-Oriented Programming
* Real-time game loop
* Event-based input handling
* Basic 2D collision detection
* Time-based movement
* Linear interpolation
* State-based game flow

---

## 🚀 Building and Running

### Requirements

You will need:

* A C++ compiler supporting modern C++.
* **SFML 3.x**
* CMake or your preferred build system/IDE.
* The required font asset inside the `Assets/Fonts` directory.

### Build

Clone the repository:

```bash
git clone <your-repository-url>
cd Snaky
```

Configure and build the project using your preferred C++ development environment.

Make sure the executable can access:

```text
Assets/Fonts/PressStart2P-Regular.ttf
```

The game expects the font using the relative path:

```text
Assets/Fonts/PressStart2P-Regular.ttf
```

---

## 📸 Screenshots

Add screenshots of your game here once you have them.

For example:

```markdown
## Screenshots

![Main Menu](screenshots/menu.png)

![Gameplay](screenshots/gameplay.png)

![Magnet Power-Up](screenshots/magnet.png)

![Game Over](screenshots/gameover.png)
```

A short gameplay GIF would also be a great addition to the repository.

---

## 📚 What I Learned

This project helped me learn several important concepts in C++ game development:

* Designing classes for different game responsibilities.
* Building a real-time game loop.
* Processing keyboard and mouse input with SFML.
* Managing different game states.
* Working with SFML clocks and delta time.
* Implementing collision detection.
* Managing dynamic gameplay systems such as power-ups.
* Separating game logic from rendering.
* Using interpolation to create smoother movement.
* Handling edge cases created by screen wrapping.
* Creating interactive UI buttons.

One of the main goals of this project was learning by actually building something rather than following a tutorial from beginning to end.

---

## 🔮 Future Improvements

Some features I would like to explore in future versions include:

* [ ] High-score system
* [ ] Persistent save data
* [ ] Multiple levels
* [ ] Different food types
* [ ] More power-ups
* [ ] Sound effects
* [ ] Background music
* [ ] Animated menus
* [ ] Better visual effects
* [ ] Difficulty settings
* [ ] Configurable game speed
* [ ] Improved UI scaling
* [ ] More polished game-over and pause screens
* [ ] Refactor the game-state handling into a dedicated state system
* [ ] Add automated tests for core game logic

---

## 👨‍💻 About the Project

**Snaky** is a personal C++ game-development project created to learn the fundamentals of 2D game programming and SFML.

It is intentionally built without a large game engine. The goal was to understand what is happening underneath the game rather than relying on an engine to handle the core systems automatically.

From input handling to movement, collision detection, power-ups, timing, and rendering, the majority of the game's systems were implemented directly in C++.

---

