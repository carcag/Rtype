#ifndef PACKETS_GAME_STARTING_COMMMAND_H
#define PACKETS_GAME_STARTING_COMMMAND_H

# include <packets/Packet.h>

# include <cstdint>

class GameStartedSig : public Packet {
 public:
  ~GameStartedSig() {}
  GameStartedSig() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kGameStartedSig;
  }

# pragma pack(push, 1)
  struct Body {
    uint16_t port = 0;
    uint8_t player_nb;
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_infos(uint16_t port, uint8_t player_nb) {
    cmd_.body.port = port;
    cmd_.body.player_nb = player_nb;
  }

  const char *pack() const {
    char *data = new char[sizeof(Cmd)];
    std::memcpy(data, &cmd_, sizeof(Cmd));
    return data;
  }

  static std::size_t body_size() { return sizeof(Body); }
  static Body unpack(const char *data) {
    Body body;
    std::memcpy(&body, data, sizeof(Body));
    return body;
  }

 private:
  Cmd cmd_;
};

#endif  // PACKETS_GAME_STARTING_COMMMAND_H
