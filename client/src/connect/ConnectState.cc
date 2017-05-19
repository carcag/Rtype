#include "connect/ConnectState.h"
#include "menu/MenuState.h"
#include "core/RessourceManager.h"

ConnectState::ConnectState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx), goto_menu_state_(false) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_menu_bg_texture());
  // Line
  line_.setSize(sf::Vector2f(600, 1));
  line_.setPosition(sf::Vector2f(500, 220));
  line_.setFillColor(sf::Color(215, 213, 244, 175));
  // Connect text
  connect_text_.SetAll("Enter a commander name :",
    38, sf::Color(95, 120, 230), 50, 10);
  // Player's name text
  player_name_text_.SetAll("", 50, sf::Color(95, 120, 230), 50, 20);
}

ConnectState::~ConnectState() {
}

void ConnectState::process_text_entered(sf::Event& event) {
  if (event.text.unicode == 8) {
    RessourceManager::get().play_key_pressed3();
    player_name_ = player_name_.substring(0, player_name_.getSize() - 1);
  } else if (event.text.unicode == 13) {
    RessourceManager::get().play_key_pressed2();
    ctx_.set_player_name(player_name_.toAnsiString());
    net_driver_.SendConnectCmd(ctx_.get_player_name());
    goto_menu_state_ = true;
  } else if (filter_displayable(event.text.unicode) && player_name_.getSize() < 16) {
    RessourceManager::get().play_key_pressed1();
    player_name_ += static_cast<char>(event.text.unicode);
  }
}

void ConnectState::Events() {
  sf::Event event;

  while (window_.pollEvent(event)) {
    // Closing window
    if (event.type == sf::Event::Closed)
      window_.close();
    // Text input
    if (event.type == sf::Event::TextEntered) {
      process_text_entered(event);
    }
  }
  if (goto_menu_state_)
    StateManager::get().replace(new MenuState(window_, net_driver_, ctx_));
}

void ConnectState::Update() {
  player_name_text_.SetText(player_name_);
  player_name_text_.SetPosition(50, 20);
}

void ConnectState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  window_.draw(line_);
  connect_text_.Render(window_);
  player_name_text_.Render(window_);
  window_.display();
}
