#include "menu/MenuState.h"
#include "core/RessourceManager.h"
#include "game/GameState.h" //
#include "lobbies_list/LobbiesListState.h"
#include "options/OptionsState.h"

MenuState::MenuState(sf::RenderWindow &window, Network &net_driver,
                     Context &ctx)
    : State(window, net_driver, ctx) {
  // Load background image and logo
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  logo_sprite_.setTexture(RessourceManager::get().get_game_logo_texture());
  logo_sprite_.setPosition(sf::Vector2f(
      1600 * 50 / 100 - (logo_sprite_.getGlobalBounds().width / 2),
      900 * 20 / 100 - (logo_sprite_.getGlobalBounds().height / 2)));
  // Buttons
  options_btn_.SetTypeAndPosition(Button::kMenuOptions, 22, 75);
  launch_btn_.SetTypeAndPosition(Button::kMenuLaunch, 50, 57);
  abort_btn_.SetTypeAndPosition(Button::kMenuAbort, 77, 75);
  // Online status text
  online_status_text_.SetAll(ctx_.is_online() ? "ONLINE" : "OFFLINE", 14,
                             ctx_.is_online() ? sf::Color(95, 120, 230)
                                              : sf::Color(255, 75, 75),
                             2, 1);
  // Player name text
  player_name_text_.SetAll("Welcome back, Commander " + ctx_.get_player_name(),
                           24, sf::Color(95, 120, 230), 50, 90);
}

MenuState::~MenuState() {}

void MenuState::Events() {
  sf::Event event;
  bool go_to_options_state = false;
  bool go_to_lobbies_state = false;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed || abort_btn_.ProcessEvent(event))
      window_.close();
    // Buttons
    if (options_btn_.ProcessEvent(event))
      go_to_options_state = true;
    if (launch_btn_.ProcessEvent(event))
      go_to_lobbies_state = true;
    // Keys
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Space) {
        go_to_lobbies_state = true;
      }
    }
    // Try a reconnection on status click
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (online_status_text_.getGlobalBounds().contains(
                event.mouseButton.x, event.mouseButton.y) &&
            !ctx_.is_online()) {
          net_driver_.reconnect(ctx_.get_player_name());
        }
      }
    }
  }
  // Network events
  NetEvent ne;
  while (net_driver_.PollNetEvent(ne)) {
    if (ne.type == NetEvent::kStatusResp) {
      ctx_.set_online(ne.status.ok ? false : true);
    }
  }

  if (go_to_lobbies_state)
    StateManager::get().replace(
        new LobbiesListState(window_, net_driver_, ctx_));
  else if (go_to_options_state)
    StateManager::get().replace(new OptionsState(window_, net_driver_, ctx_));
}

void MenuState::Update() {
  // Server status
  online_status_text_.SetText(ctx_.is_online() ? "ONLINE" : "OFFLINE");
  online_status_text_.SetFillColor(ctx_.is_online() ? sf::Color(95, 120, 230)
                                                    : sf::Color(255, 75, 75));
}

void MenuState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  window_.draw(logo_sprite_);
  online_status_text_.Render(window_);
  player_name_text_.Render(window_);
  options_btn_.Render(window_);
  launch_btn_.Render(window_);
  abort_btn_.Render(window_);
  window_.display();
}
