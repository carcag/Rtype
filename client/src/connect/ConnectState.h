#ifndef CONNECT_CONNECT_STATE_H
# define CONNECT_CONNECT_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "menu/MenuState.h"
# include "gui/TextField.h"

class ConnectState : public State {
 private:
  // UI
  sf::Sprite bg_sprite_;
  sf::RectangleShape line_;
  TextField connect_text_;
  TextField player_name_text_;

  sf::String player_name_;
  bool goto_menu_state_;

  bool filter_displayable(uint32_t keycode) {
    if ((keycode >= '0' && keycode <= '9')
        || (keycode >= 'A' && keycode <= 'Z')
        || (keycode >= 'a' && keycode <= 'z'))
        return true;
    return false;
  }
  void process_text_entered(sf::Event& event);

 public:
  ConnectState() = delete;
  ConnectState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  ConnectState(const ConnectState&) = delete;
  ~ConnectState();

  void Events();
  void Update();
  void Render();
};

#endif  // CONNECT_CONNECT_STATE_H
