#ifndef LOBBIESLIST_LOBBIES_LIST_STATE_H
# define LOBBIESLIST_LOBBIES_LIST_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "gui/Button.h"
# include "gui/TextField.h"
# include "gui/List.h"

class LobbiesListState : public State {
 private:
  TextField online_status_text_;
  TextField player_name_text_;
  sf::Sprite bg_sprite_;
  sf::Sprite lobbies_list_bg_sprite_;
  sf::Sprite missions_logo_sprite_;

  Button back_btn_;
  Button refresh_btn_;
  Button create_btn_;

  List lobbies_list_;

 public:
  LobbiesListState() = delete;
  LobbiesListState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  LobbiesListState(const LobbiesListState&) = delete;
  ~LobbiesListState();
  LobbiesListState operator=(const LobbiesListState&) = delete;

  void Events();
  void Update();
  void Render();
};

#endif  // LOBBIESLIST_LOBBIES_LIST_STATE_H
