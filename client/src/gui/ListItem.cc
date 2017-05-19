#include "ListItem.h"

ListItem::ListItem(const std::string& text, int32_t id, std::size_t idx,
  std::size_t item_height) : id_(id), idx_(idx), inside_list_item_(false),
  item_height_(item_height) {
  text_.SetText(text);
  text_.SetSize(item_height_ * 2 / 3);
  text_.SetFillColor(sf::Color(17, 42, 93));
}

int32_t ListItem::ProcessEvent(sf::Event& event) {
  if (event.type == sf::Event::MouseMoved) {
    if (text_.getGlobalBounds().contains(
      event.mouseMove.x, event.mouseMove.y)) {
      if (inside_list_item_ == false) {
        RessourceManager::get().play_hover_btn();
      }
      inside_list_item_ = true;
      text_.SetFillColor(sf::Color(161, 175, 209));
    } else {
      text_.SetFillColor(sf::Color(17, 42, 93));
      inside_list_item_ = false;
    }
  }
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (text_.getGlobalBounds().contains(
        event.mouseButton.x, event.mouseButton.y)) {
        RessourceManager::get().play_click_btn();
        return id_;
      }
    }
  }
  return -1;
}

void ListItem::Render(sf::RenderWindow& window, sf::Vector2f origin, bool center) {
  text_.SetPosition(origin, center);
  text_.Render(window);
}
