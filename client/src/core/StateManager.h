#ifndef STATE_MANAGER
# define STATE_MANAGER

# include <stack>
# include "core/State.h"

class StateManager {
 private:
  std::stack<State *> states_;

  StateManager() {}

 public:
   StateManager(const StateManager&) = delete;
   StateManager operator=(const StateManager&) = delete;
  ~StateManager() { while(has_active_state()) pop(); }
  // StateManager is a singleton
  static StateManager& get() {
    static StateManager instance;
    return instance;
  }
  // StateManager's API
  bool has_active_state() { return !states_.empty(); }
  State *get_active_state() { return states_.top(); }
  void push(State *state) { states_.push(state); }
  void pop() { if (has_active_state()) { delete states_.top(); states_.pop(); } }
  void replace(State *state) { pop(); push(state); }
};

#endif  // STATE_MANAGER
