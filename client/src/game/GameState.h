#ifndef GAME_GAME_STATE_H
# define GAME_GAME_STATE_H

# include <chrono>
# include <vector>

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "packets/Packets.h"
# include "core/Context.h"
# include "gui/TextField.h"
# include "game/Entity.h"
# include "game/ScrollingBackground.h"

class GameState : public State {
 private:
  TextField online_status_text_;
  ScrollingBackground sbg_;

  Entity entities_[Packet::kMaxEntities];

  bool prev_events_[sf::Keyboard::KeyCount];
  bool curr_events_[sf::Keyboard::KeyCount];
  std::chrono::system_clock::time_point last_fire_;

  uint64_t send_;

 public:
  GameState() = delete;
  GameState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  GameState(const GameState&) = delete;
  ~GameState();
  GameState operator=(const GameState&) = delete;

  void Events();
  void Update();
  void Render();
};

#endif  // GAME_GAME_STATE_H
