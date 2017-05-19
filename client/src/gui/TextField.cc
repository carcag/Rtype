#include "gui/TextField.h"
#include "core/RessourceManager.h"

TextField::TextField() {
  text_.setFont(RessourceManager::get().get_default_font());
}

TextField::~TextField() {
}

void TextField::SetText(const std::string& text) {
  text_.setString(text);
}

void TextField::SetSize(std::size_t fsize) {
  text_.setCharacterSize(fsize);
}

void TextField::SetFillColor(const sf::Color& color) {
	text_.setFillColor(color);
}

void TextField::SetPosition(const sf::Vector2f& pos, bool flag) {
  if (flag) {
    text_.setPosition(
      pos.x - (text_.getGlobalBounds().width / 2),
      pos.y - (text_.getGlobalBounds().height / 2));
  } else {
    text_.setPosition(
      pos.x,
      pos.y);
  }
}

void TextField::SetPosition(uint8_t wpercent, uint8_t hpercent, bool flag) {
  if (flag) {
    text_.setPosition(
      1600 * wpercent / 100 - (text_.getGlobalBounds().width / 2),
      900 * hpercent / 100 - (text_.getGlobalBounds().height / 2));
  } else {
    text_.setPosition(
      1600 * wpercent / 100,
      900 * hpercent / 100);
  }
}

void TextField::SetAll(const std::string& text,
                       std::size_t fsize,
                       const sf::Color& color,
                       uint8_t wpercent,
                       uint8_t hpercent,
                       bool flag) {
  SetText(text);
  SetSize(fsize);
  SetFillColor(color);
  SetPosition(wpercent, hpercent, flag);
}

sf::FloatRect TextField::getGlobalBounds() {
  return text_.getGlobalBounds();
}

void TextField::Render(sf::RenderWindow& window) {
  window.draw(text_);
}
