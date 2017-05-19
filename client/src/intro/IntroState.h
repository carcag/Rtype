#ifndef INTRO_INTRO_STATE_H
# define INTRO_INTRO_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "menu/MenuState.h"
# include "gui/TextField.h"

class IntroState : public State {
 private:
  sf::Sprite bg_sprite_;
  TextField intro_text_;
  // Blinking intro text configuration
  bool inc_ = false;
  uint8_t blink_speed_ = 2;
  uint8_t alpha_ = 255;
  uint8_t alpha_max_ = 255;
  uint8_t alpha_min_ = 120;

 public:
  IntroState() = delete;
  IntroState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  IntroState(const IntroState&) = delete;
  ~IntroState();

  void Events();
  void Update();
  void Render();
};

#endif  // INTRO_INTRO_STATE_H
