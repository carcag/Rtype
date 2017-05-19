#ifndef CREATE_LOBBY_CREATE_LOBBY_STATE_H
# define CREATE_LOBBY_CREATE_LOBBY_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "menu/MenuState.h"
# include "gui/TextField.h"
# include "gui/Button.h"

class CreateLobbyState : public State {
 private:
  // UI
  sf::Sprite bg_sprite_;
  sf::RectangleShape line_;
  TextField online_status_text_;
  TextField create_lobby_text_;
  TextField lobby_name_text_;

  Button back_btn_;
  Button create_btn_;

  sf::String lobby_name_;
  bool goto_menu_state_;

  bool filter_displayable(uint32_t keycode) {
    if ((keycode >= '0' && keycode <= '9')
        || (keycode >= 'A' && keycode <= 'Z')
        || (keycode >= 'a' && keycode <= 'z')
        || keycode == ' ')
        return true;
    return false;
  }
  bool process_text_entered(sf::Event& event);

 public:
  CreateLobbyState() = delete;
  CreateLobbyState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  CreateLobbyState(const CreateLobbyState&) = delete;
  ~CreateLobbyState();

  void Events();
  void Update();
  void Render();
};

#endif  // CREATE_LOBBY_CREATE_LOBBY_STATE_H
