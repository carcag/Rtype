#ifndef STATE_H
# define STATE_H

# include <SFML/Graphics.hpp>
# include "net/Network.h"
# include "core/Context.h"

class State {
 protected:
  sf::RenderWindow& window_;
  Network& net_driver_;
  Context& ctx_;

 public:
  State() = delete;
  State(sf::RenderWindow& window,
        Network& net_driver,
        Context& ctx) : window_(window), net_driver_(net_driver), ctx_(ctx) {}
  State(const State&) = delete;
  virtual ~State() {}

  virtual void Events() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
};

#endif  // STATE_H
