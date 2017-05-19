#include <iostream>
#include "intro/IntroState.h"
#include "connect/ConnectState.h"
#include "core/RessourceManager.h"

IntroState::IntroState(sf::RenderWindow& window, Network& net_driver,
  Context& ctx) : State(window, net_driver, ctx) {
  // Load background image
  bg_sprite_.setTexture(RessourceManager::get().get_intro_bg_texture());
  // Blinking intro text
  intro_text_.SetAll("Press space to continue...",
    38, sf::Color(95, 120, 230), 50, 90);
  // Start playing the intro music
  RessourceManager::get().play_intro_music();
}

IntroState::~IntroState() {
}

void IntroState::Events() {
  sf::Event event;
  bool goto_connect_state = false;

  while (window_.pollEvent(event)) {
    // Closing window
    if (event.type == sf::Event::Closed)
      window_.close();
    // Must go to the next state
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Space) {
        RessourceManager::get().play_space_pressed();
        goto_connect_state = true;
      }
    }
  }
  if (goto_connect_state)
    StateManager::get().replace(new ConnectState(window_, net_driver_, ctx_));
}

void IntroState::Update() {
  // Blinking intro text
  if (alpha_ == alpha_max_)
    inc_ = false;
  else if (alpha_ <= alpha_min_)
    inc_ = true;
  if (inc_)
    alpha_ += blink_speed_;
  else
    alpha_ -= blink_speed_;
  intro_text_.SetFillColor(sf::Color(45, 45, 210, alpha_));
}

void IntroState::Render() {
  window_.clear();
  window_.draw(bg_sprite_);
  intro_text_.Render(window_);
  window_.display();
}
