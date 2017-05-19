#include "game/ScrollingBackground.h"

ScrollingBackground::ScrollingBackground() : speed_(2.f) {
  scroll_bg_texture_.loadFromFile("ressource/image/infinit_bg.jpg");
  scroll_bg_sprite1_.setTexture(scroll_bg_texture_);
  scroll_bg_sprite2_.setTexture(scroll_bg_texture_);
  scroll_bg_sprite2_.setPosition(3200, 0);
}

ScrollingBackground::~ScrollingBackground() {
}

void ScrollingBackground::Update() {
  auto bg1x = scroll_bg_sprite1_.getPosition().x;
  auto bg2x = scroll_bg_sprite2_.getPosition().x;

  if (bg1x <= -3200)
    bg1x = 3200;
  if (bg2x <= -3200)
    bg2x = 3200;
  if (bg1x <= 3200) {
    bg1x -= speed_;
    bg2x -= speed_;
  }

  scroll_bg_sprite1_.setPosition(bg1x, 0);
  scroll_bg_sprite2_.setPosition(bg2x, 0);
}

void ScrollingBackground::Render(sf::RenderWindow& window) const {
  window.draw(scroll_bg_sprite1_);
  window.draw(scroll_bg_sprite2_);
}
