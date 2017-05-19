#ifndef MENU_MENU_STATE_H
# define MENU_MENU_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "gui/Button.h"
# include "gui/TextField.h"

class MenuState : public State {
 private:
  TextField online_status_text_;
  TextField player_name_text_;
  sf::Sprite bg_sprite_;
  sf::Sprite logo_sprite_;

  Button options_btn_;
  Button launch_btn_;
  Button abort_btn_;

 public:
  MenuState() = delete;
  MenuState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  MenuState(const MenuState&) = delete;
  ~MenuState();
  MenuState operator=(const MenuState&) = delete;

  void Events();
  void Update();
  void Render();
};

#endif  // MENU_MENU_STATE_H
