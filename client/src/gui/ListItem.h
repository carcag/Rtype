#ifndef GUI_LIST_ITEM_H
# define GUI_LIST_ITEM_H

# include <vector>

# include <SFML/Graphics.hpp>
# include "core/RessourceManager.h"
# include "gui/TextField.h"

class ListItem {
 private:
  TextField text_;
  std::size_t item_height_;
  int32_t id_;
  std::size_t idx_;
  bool inside_list_item_;

 public:
  ListItem(const std::string& text,
           int32_t id,
           std::size_t idx,
           std::size_t item_height);
  ListItem(const ListItem&) = delete;
  ~ListItem() {}

  int32_t get_id() { return id_; }

  int ProcessEvent(sf::Event& event);
  void Render(sf::RenderWindow& window, sf::Vector2f origine, bool center = false);
};

#endif  // GUI_LIST_ITEM_H
