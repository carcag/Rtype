#ifndef LOBBY_LOBBY_STATE_H
# define LOBBY_LOBBY_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "gui/Button.h"
# include "gui/ToggleButton.h"
# include "gui/TextField.h"
# include "gui/List.h"

class LobbyState : public State {
 private:
  TextField online_status_text_;
  TextField player_name_text_;
  sf::Sprite bg_sprite_;
  sf::Sprite lobby_bg_sprite_;

  List players_list_;

  Button leave_btn_;
  ToggleButton ready_btn_;

 public:
  LobbyState() = delete;
  LobbyState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  LobbyState(const LobbyState&) = delete;
  ~LobbyState();
  LobbyState operator=(const LobbyState&) = delete;

  void Events();
  void Update();
  void Render();
};

#endif  // LOBBY_LOBBY_STATE_H
