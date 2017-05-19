#ifndef GUI_LIST_H
# define GUI_LIST_H

# include <vector>

# include <SFML/Graphics.hpp>
# include "core/RessourceManager.h"
# include "gui/ListItem.h"

class List {
 private:
  sf::Vector2f origine_;
  std::size_t item_height_;
  std::size_t spacing_;
  std::vector<ListItem *> items_;
  bool items_centered_;

 public:
  List() : item_height_(100), spacing_(0), items_centered_(false) {}
  List(const sf::Vector2f& origine) : origine_(origine), item_height_(100),
    spacing_(0), items_centered_(false) {}
  List(const List&) = delete;
  ~List() {}
  List operator=(const List&) = delete;

  std::size_t size() { return items_.size(); }

  void set_origin(const sf::Vector2f& origine) { origine_ = origine; }
  void set_item_height(std::size_t height) { item_height_ = height; }
  void set_spacing(std::size_t space) { spacing_ = space; }
  void set_items_centered(bool centered) { items_centered_ = centered; }
  void add_item(const std::string& text, std::size_t id);
  void remove_item(int32_t id);
  void clear();
  int32_t ProcessEvent(sf::Event& event);
  void Render(sf::RenderWindow& window);
};

#endif  // GUI_LIST_H
