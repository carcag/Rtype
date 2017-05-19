#ifndef OPTIONS_OPTIONS_STATE_H
# define OPTIONS_OPTIONS_STATE_H

# include <SFML/Graphics.hpp>
# include "core/State.h"
# include "core/StateManager.h"
# include "net/Network.h"
# include "core/Context.h"
# include "gui/Button.h"
# include "gui/TextField.h"

class OptionsState : public State {
 private:
  TextField online_status_text_;
  TextField title_text_;
  sf::Sprite bg_sprite_;

  Button back_btn_;

 public:
  OptionsState() = delete;
  OptionsState(sf::RenderWindow& window, Network& net_driver, Context& ctx);
  OptionsState(const OptionsState&) = delete;
  ~OptionsState();
  OptionsState operator=(const OptionsState&) = delete;

  void Events();
  void Update();
  void Render();
};

#endif  // OPTIONS_OPTIONS_STATE_H
