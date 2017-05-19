#include "gui/ToggleButton.h"
#include "core/RessourceManager.h"

ToggleButton::ToggleButton() : inside_btn_(false), on_(false) {
}

ToggleButton::~ToggleButton() {
}

void ToggleButton::SetType(Type type) {
  switch (type) {
    case kLobbyReady:
      texture_on_.loadFromFile("ressource/image/ready_on_btn.png");
      texture_off_.loadFromFile("ressource/image/ready_btn.png");
      hover_texture_.loadFromFile("ressource/image/ready_btn_hover.png");
      break;
  }
  sprite_.setTexture(texture_off_);
}

void ToggleButton::SetPosition(const sf::Vector2f& pos) {
  sprite_.setPosition(
    pos.x - (sprite_.getGlobalBounds().width / 2),
    pos.y - (sprite_.getGlobalBounds().height / 2));
}

void ToggleButton::SetPosition(uint8_t wpercent, uint8_t hpercent, bool flag) {
  if (flag) {
    sprite_.setPosition(
      1600 * wpercent / 100 - (sprite_.getGlobalBounds().width / 2),
      900 * hpercent / 100 - (sprite_.getGlobalBounds().height / 2));
  } else {
    sprite_.setPosition(
      1600 * wpercent / 100,
      900 * hpercent / 100);
  }
}


void ToggleButton::SetTypeAndPosition(Type type, const sf::Vector2f& pos) {
  SetType(type);
  SetPosition(pos);
}

void ToggleButton::SetTypeAndPosition(Type type,
                                uint8_t wpercent,
                                uint8_t hpercent,
                                bool flag) {
    SetType(type);
    SetPosition(wpercent, hpercent, flag);
}


sf::FloatRect ToggleButton::getGlobalBounds() {
  return sprite_.getGlobalBounds();
}

bool ToggleButton::ProcessEvent(const sf::Event& event) {
  if (event.type == sf::Event::MouseMoved) {
    if (sprite_.getGlobalBounds().contains(
      event.mouseMove.x, event.mouseMove.y)) {
      if (inside_btn_ == false)
        RessourceManager::get().play_hover_btn();
      inside_btn_ = true;
      sprite_.setTexture(hover_texture_);
    } else {
      if (on_ == false)
        sprite_.setTexture(texture_off_);
      else
        sprite_.setTexture(texture_on_);
      inside_btn_ = false;
    }
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (sprite_.getGlobalBounds().contains(
        event.mouseButton.x, event.mouseButton.y)) {
        RessourceManager::get().play_click_btn();
        if (on_ == false) {
          sprite_.setTexture(texture_on_);
          on_ = true;
        } else {
          sprite_.setTexture(texture_off_);
          on_ = false;
        }
        return true;
      }
    }
  }
  return false;
}

void ToggleButton::Render(sf::RenderWindow& window) {
  window.draw(sprite_);
}
