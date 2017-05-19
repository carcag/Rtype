#ifndef PACKETS_LOBBIES_LIST_RESP_H
#define PACKETS_LOBBIES_LIST_RESP_H

# include <packets/Packet.h>

# include <string>
# include <cstdint>

class ListLobbiesResp : public Packet {
 public:
  ~ListLobbiesResp() {}
  ListLobbiesResp() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kListLobbiesResp;
  }

# pragma pack(push, 1)
  struct Lobby {
    int16_t id = 0;
    char name[kMaxLobbyNameSize] = {0};
    uint8_t clients_nb = 0;
  };

  struct Body {
    uint8_t lobbies_nb = 0;
    Lobby lobbies[kMaxLobbiesNb] = {Lobby()};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

void add_lobby(int32_t id, const std::string& name, uint8_t clients_nb) {
    int i = cmd_.body.lobbies_nb;
    cmd_.body.lobbies[i].id = id;
    const char *tmp = name.c_str();
    for (int j = 0; tmp[j] && j < kMaxLobbyNameSize; ++j) {
      cmd_.body.lobbies[i].name[j] = tmp[j];
    }
    cmd_.body.lobbies[i].clients_nb = clients_nb;
    cmd_.body.lobbies_nb++;
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

#endif  // PACKETS_LOBBIES_LIST_RESP_H
