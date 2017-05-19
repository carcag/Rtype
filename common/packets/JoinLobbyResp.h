#ifndef PACKETS_JOIN_LOBBY_RESP_H
#define PACKETS_JOIN_LOBBY_RESP_H

# include <packets/Packet.h>

# include <string>
# include <cstdint>

class JoinLobbyResp : public Packet {
 public:
  ~JoinLobbyResp() {}
  JoinLobbyResp() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kJoinLobbyResp;
  }

# pragma pack(push, 1)
  struct Client {
    int32_t id = 0;
    char name[kMaxClientNameSize] = {0};
    int8_t ready = 0;
  };

  struct Lobby {
    int32_t id = 0;
    char name[kMaxLobbyNameSize] = {0};
    uint8_t clients_nb = 0;
  };

  struct Body {
    Lobby lobby;
    Client clients[kMaxClientsPerLobby] = {Client()};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_lobby(int32_t id, const std::string& name) {
    cmd_.body.lobby.id = id;
    const char *tmp = name.c_str();
    for (int j = 0; tmp[j] && j < kMaxLobbyNameSize; ++j) {
      cmd_.body.lobby.name[j] = tmp[j];
    }
  }

  void add_client(int32_t id, const std::string& name, bool ready) {
    int i = cmd_.body.lobby.clients_nb;
    cmd_.body.clients[i].id = id;
    const char *tmp = name.c_str();
    for (int j = 0; tmp[j] && j < kMaxClientNameSize; ++j) {
      cmd_.body.clients[i].name[j] = tmp[j];
    }
    cmd_.body.clients[i].ready = ready ? 1 : 0;
    cmd_.body.lobby.clients_nb++;
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

#endif  // PACKETS_JOIN_LOBBY_RESP_H
