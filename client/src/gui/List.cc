#include "gui/List.h"

void List::add_item(const std::string& text, std::size_t id) {
  items_.push_back(new ListItem(text, id, items_.size(), item_height_));
}

void List::remove_item(int32_t id) {
  std::vector<ListItem *>::iterator it = items_.begin();
  while (it != items_.end()) {
    if ((*it)->get_id() == id) {
      it = items_.erase(it);
    } else {
      ++it;
    }
  }
}

void List::clear() {
  for (auto item : items_) {
    delete item;
  }
  items_.clear();
}

int32_t List::ProcessEvent(sf::Event& event) {
  for (auto item : items_) {
    int id = -1;
    if ((id = item->ProcessEvent(event)) != -1)
      return id;
  }
  return -1;
}

void List::Render(sf::RenderWindow& window) {
  sf::Vector2f copy_origine = origine_;
  for (auto item : items_) {
    item->Render(window, copy_origine, items_centered_);
    copy_origine.y += item_height_ + spacing_;
  }
}
