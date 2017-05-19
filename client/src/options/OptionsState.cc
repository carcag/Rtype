#include "options/OptionsState.h"
#include "menu/MenuState.h"
#include "lobby/LobbyState.h"
#include "core/RessourceManager.h"

OptionsState::OptionsState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  // Buttons
  back_btn_.SetTypeAndPosition(Button::kLobbiesListBack, 25, 90);
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
    ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75), 2, 1);
  // Player name text
  title_text_.SetAll("OPTIONS MENU", 30, sf::Color(95, 120, 230), 50, 3);
}

OptionsState::~OptionsState() {
}

void OptionsState::Events() {
  sf::Event event;
  bool goto_menu_state = false;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window_.close();
    // Go back to main menu on Escape
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        goto_menu_state = true;
      }
    }
    // Buttons
    if (back_btn_.ProcessEvent(event))
      goto_menu_state = true;
  }

  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      // Server close the connection
      if (ne.status.ok != 0 && ne.status.cmd == Packet::kInvalid
            && ne.status.err_code == 50) {
        ctx_.set_online(false);
        goto_menu_state = true;
      }
    }
  }
  if (goto_menu_state)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
}

void OptionsState::Update() {
  // Server status
  online_status_text_.SetText(ctx_.is_online() ? "ONLINE" : "OFFLINE");
  online_status_text_.SetFillColor(
      ctx_.is_online() ? sf::Color(95, 120, 230) : sf::Color(255, 75, 75));
}

void OptionsState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  online_status_text_.Render(window_);
  title_text_.Render(window_);
  back_btn_.Render(window_);
  window_.display();
}
