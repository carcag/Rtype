#include "core/GameEngine.h"
#include "intro/IntroState.h"
#include "core/StateManager.h"
#include "net/Network.h"
#include "core/Context.h"

int main() {
  GameEngine ge;
  Network net_driver;
  Context ctx;
  StateManager::get().push(new IntroState(ge.get_window(), net_driver, ctx));

  sf::Clock clock;
  while (ge.running()) {
    clock.restart();
    ge.Events();
    ge.Update();
    ge.Render();
    sf::Time t =  clock.getElapsedTime();
    // printf("elapsed: %d\n", t.asMicroseconds());
  }
}
