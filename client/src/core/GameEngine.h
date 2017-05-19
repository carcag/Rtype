#ifndef GAME_ENGINE
# define GAME_ENGINE

# include <SFML/Graphics.hpp>
# include "core/StateManager.h"

# define WINDOW_WIDTH (1600)
# define WINDOW_HEIGHT (900)

class GameEngine {
 private:
  sf::RenderWindow window_;

 public:
  GameEngine();
  ~GameEngine();

  sf::RenderWindow& get_window() { return window_; }

  bool running() { return window_.isOpen(); }
  void Events();
  void Update();
  void Render();
};

#endif  // GAME_ENGINE
