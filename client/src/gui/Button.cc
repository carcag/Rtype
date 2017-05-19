#include "gui/Button.h"
#include "core/RessourceManager.h"

Button::Button() : inside_btn_(false) {
}

Button::~Button() {
}

void Button::SetType(Type type) {
  switch (type) {
    case kMenuOptions:
      texture_.loadFromFile("ressource/image/options_btn.png");
      hover_texture_.loadFromFile("ressource/image/options_btn_hover.png");
      break;
    case kMenuLaunch:
      texture_.loadFromFile("ressource/image/launch_btn.png");
      hover_texture_.loadFromFile("ressource/image/launch_btn_hover.png");
      break;
    case kMenuAbort:
      texture_.loadFromFile("ressource/image/abort_btn.png");
      hover_texture_.loadFromFile("ressource/image/abort_btn_hover.png");
      break;
    case kLobbiesListBack:
      texture_.loadFromFile("ressource/image/back_btn.png");
      hover_texture_.loadFromFile("ressource/image/back_btn_hover.png");
      break;
    case kLobbiesListRefresh:
      texture_.loadFromFile("ressource/image/refresh_btn.png");
      hover_texture_.loadFromFile("ressource/image/refresh_btn_hover.png");
      break;
    case kLobbiesListCreate:
      texture_.loadFromFile("ressource/image/create_btn.png");
      hover_texture_.loadFromFile("ressource/image/create_btn_hover.png");
      break;
    case kLobbyLeave:
      texture_.loadFromFile("ressource/image/leave_btn.png");
      hover_texture_.loadFromFile("ressource/image/leave_btn_hover.png");
      break;
    case kLobbyReady:
      texture_.loadFromFile("ressource/image/ready_btn.png");
      hover_texture_.loadFromFile("ressource/image/ready_btn_hover.png");
      break;
  }
  sprite_.setTexture(texture_);
}

void Button::SetPosition(const sf::Vector2f& pos) {
  sprite_.setPosition(
    pos.x - (sprite_.getGlobalBounds().width / 2),
    pos.y - (sprite_.getGlobalBounds().height / 2));
}

void Button::SetPosition(uint8_t wpercent, uint8_t hpercent, bool flag) {
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


void Button::SetTypeAndPosition(Type type, const sf::Vector2f& pos) {
  SetType(type);
  SetPosition(pos);
}

void Button::SetTypeAndPosition(Type type,
                                uint8_t wpercent,
                                uint8_t hpercent,
                                bool flag) {
    SetType(type);
    SetPosition(wpercent, hpercent, flag);
}


sf::FloatRect Button::getGlobalBounds() {
  return sprite_.getGlobalBounds();
}

bool Button::ProcessEvent(const sf::Event& event) {
  if (event.type == sf::Event::MouseMoved) {
    if (sprite_.getGlobalBounds().contains(
      event.mouseMove.x, event.mouseMove.y)) {
      if (inside_btn_ == false)
        RessourceManager::get().play_hover_btn();
      inside_btn_ = true;
      sprite_.setTexture(hover_texture_);
    } else {
      sprite_.setTexture(texture_);
      inside_btn_ = false;
    }
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (sprite_.getGlobalBounds().contains(
        event.mouseButton.x, event.mouseButton.y)) {
        RessourceManager::get().play_click_btn();
        return true;
      }
    }
  }
  return false;
}

void Button::Render(sf::RenderWindow& window) {
  window.draw(sprite_);
}
