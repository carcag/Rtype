#include "core/GameEngine.h"
#include <iostream>

GameEngine::GameEngine() {
  std::cout << "Starting game engine..." << std::endl;
  window_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                                 "R-Type");
  window_.setFramerateLimit(60);
}

GameEngine::~GameEngine() {
  std::cout << "Stopping game engine..." << std::endl;
}

void GameEngine::Events() {
  if (StateManager::get().has_active_state()) {
    StateManager::get().get_active_state()->Events();
  } else {
    std::cerr << "GameEngine critical error : no active state" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GameEngine::Update() {
  if (StateManager::get().has_active_state()) {
    StateManager::get().get_active_state()->Update();
  } else {
    std::cerr << "GameEngine critical error : no active state" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GameEngine::Render() {
  if (StateManager::get().has_active_state()) {
    StateManager::get().get_active_state()->Render();
  } else {
    std::cerr << "GameEngine critical error : no active state" << std::endl;
    exit(EXIT_FAILURE);
  }
}
