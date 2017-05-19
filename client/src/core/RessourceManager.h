#ifndef CORE_RESSOURCE_MANAGER_H
# define CORE_RESSOURCE_MANAGER_H

# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>

class RessourceManager {
 private:
  // Font
  sf::Font default_font_;
  // Musics
  sf::Music intro_music_;
  sf::Music game_music_;
  // Sound buffers
  sf::SoundBuffer click_btn;
  sf::SoundBuffer hover_btn;
  sf::SoundBuffer space_pressed_;
  sf::SoundBuffer key_pressed1_;
  sf::SoundBuffer key_pressed2_;
  sf::SoundBuffer key_pressed3_;
  // Sound players
  sf::Sound click_player_;
  sf::Sound hover_player_;
  sf::Sound space_pressed_player_;
  sf::Sound key_pressed1_player_;
  sf::Sound key_pressed2_player_;
  sf::Sound key_pressed3_player_;
  // Backgrounds
  sf::Texture intro_bg_texture_;
  sf::Texture menu_bg_texture_;
  // GUI assets
  sf::Texture game_logo_texture_;
  sf::Texture panel_texture_;
  sf::Texture missions_logo_texture_;
  // Game assets
  sf::Texture spaceship1_texture_;
  sf::Texture spaceship2_texture_;
  sf::Texture spaceship3_texture_;
  sf::Texture spaceship4_texture_;
  sf::Texture pew1_texture_;
  sf::Texture pew2_texture_;
  sf::Texture pew3_texture_;
  sf::Texture pew4_texture_;

  RessourceManager() {
    // Font
    default_font_.loadFromFile("ressource/font/OpenSans-Bold.ttf");
    // Musics
    intro_music_.openFromFile("ressource/sound/intro.ogg");
    intro_music_.setVolume(75);
    intro_music_.setLoop(true);
    game_music_.openFromFile("ressource/sound/game.ogg");
    game_music_.setVolume(100);
    game_music_.setLoop(true);
    // Sounds
    click_btn.loadFromFile("ressource/sound/on_click.wav");
    hover_btn.loadFromFile("ressource/sound/on_hover.wav");
    space_pressed_.loadFromFile("ressource/sound/on_space.wav");
    key_pressed1_.loadFromFile("ressource/sound/key_pressed1.wav");
    key_pressed2_.loadFromFile("ressource/sound/key_pressed2.wav");
    key_pressed3_.loadFromFile("ressource/sound/key_pressed3.wav");
    // Backgrounds
    intro_bg_texture_.loadFromFile("ressource/image/intro.jpg");
    menu_bg_texture_.loadFromFile("ressource/image/connect.jpg");
    // GUI assets
    game_logo_texture_.loadFromFile("ressource/image/rtype_logo.png");
    panel_texture_.loadFromFile("ressource/image/list_lobbies_bg.png");
    missions_logo_texture_.loadFromFile("ressource/image/missions_logo.png");
    // Game assets - spaceships
    spaceship1_texture_.loadFromFile("ressource/image/spaceship1.png");
    spaceship2_texture_.loadFromFile("ressource/image/spaceship2.png");
    spaceship3_texture_.loadFromFile("ressource/image/spaceship3.png");
    spaceship4_texture_.loadFromFile("ressource/image/spaceship4.png");
    pew1_texture_.loadFromFile("ressource/image/pew1.png");
    pew2_texture_.loadFromFile("ressource/image/pew2.png");
    pew3_texture_.loadFromFile("ressource/image/pew3.png");
    pew4_texture_.loadFromFile("ressource/image/pew4.png");
  }

 public:
  RessourceManager(const RessourceManager&) = delete;
  RessourceManager operator=(const RessourceManager&) = delete;
  ~RessourceManager() {}
  // RessourceManager is a singleton
  static RessourceManager& get() {
    static RessourceManager instance;
    return instance;
  }
  // Font
  const sf::Font& get_default_font() const { return default_font_; }
  // Musics
  void play_intro_music() { intro_music_.play(); }
  void pause_intro_music() { intro_music_.pause(); }
  void stop_intro_music() { intro_music_.stop(); }
  void play_game_music() { game_music_.play(); }
  void pause_game_music() { game_music_.pause(); }
  void stop_game_music() { game_music_.stop(); }
  // Sounds
  void play_click_btn() {
    click_player_.setBuffer(click_btn);
    click_player_.play();
  }
  void play_hover_btn() {
    hover_player_.setBuffer(hover_btn);
    hover_player_.setVolume(10);
    hover_player_.play();
  }
  void play_space_pressed() {
    space_pressed_player_.setBuffer(space_pressed_);
    hover_player_.setVolume(75);
    space_pressed_player_.play();
  }
  void play_key_pressed1() {
    key_pressed1_player_.setBuffer(key_pressed1_);
    key_pressed1_player_.play();
  }
  void play_key_pressed2() {
    key_pressed2_player_.setBuffer(key_pressed2_);
    key_pressed2_player_.play();
  }
  void play_key_pressed3() {
    key_pressed3_player_.setBuffer(key_pressed3_);
    key_pressed3_player_.play();
  }
  // Backgrounds
  const sf::Texture& get_intro_bg_texture() const { return intro_bg_texture_; }
  const sf::Texture& get_menu_bg_texture() const { return menu_bg_texture_; }
  // GUI assets
  const sf::Texture& get_game_logo_texture() const { return game_logo_texture_; }
  const sf::Texture& get_panel_texture() const { return panel_texture_; }
  const sf::Texture& get_missions_logo_texture() const { return missions_logo_texture_; }
  // Game assets - spaceships
  const sf::Texture& get_spaceship1_texture() const { return spaceship1_texture_; }
  const sf::Texture& get_spaceship2_texture() const { return spaceship2_texture_; }
  const sf::Texture& get_spaceship3_texture() const { return spaceship3_texture_; }
  const sf::Texture& get_spaceship4_texture() const { return spaceship4_texture_; }
  const sf::Texture& get_pew1_texture() const { return pew1_texture_; }
  const sf::Texture& get_pew2_texture() const { return pew2_texture_; }
  const sf::Texture& get_pew3_texture() const { return pew3_texture_; }
  const sf::Texture& get_pew4_texture() const { return pew4_texture_; }
};

#endif  // CORE_RESSOURCE_MANAGER_H
