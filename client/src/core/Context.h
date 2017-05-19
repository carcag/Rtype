#ifndef CORE_CONTEXT_H
# define CORE_CONTEXT_H

# include <string>

class Context {
 private:
  bool is_online_ = false;
  std::string player_name_ = "";
  std::string lobby_name_ = "";
  NetEvent event_;

 public:
  Context() {}
  Context(const Context&) = delete;
  ~Context() {}
  Context operator=(const Context&) = delete;

  void set_player_name(const std::string& name) { player_name_ = name; }
  const std::string& get_player_name() { return player_name_; }
  bool is_online() { return is_online_; }
  void set_online(bool is_online) { is_online_ = is_online; }
  void set_lobby_name(const std::string& name) { lobby_name_ = name; }
  const std::string& get_lobby_name() { return lobby_name_; }
  void set_net_event(const NetEvent& event) { event_ = event; }
  const NetEvent& get_net_event() { return event_; }
};

#endif  // CORE_CONTEXT_H
