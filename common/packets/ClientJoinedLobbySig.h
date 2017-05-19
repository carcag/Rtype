#ifndef PACKETS_CLIENT_JOINED_LOBBY_SIG_H
#define PACKETS_CLIENT_JOINED_LOBBY_SIG_H

# include <packets/Packet.h>

# include <cstdint>

class ClientJoinedLobbySig : public Packet {
 public:
  ~ClientJoinedLobbySig() {}
  ClientJoinedLobbySig() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kClientJoinedLobbySig;
  }

# pragma pack(push, 1)
  struct Body {
    int32_t client_id = 0;
    char client_name[kMaxClientNameSize] = {0};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_client_infos(int32_t client_id, const std::string& client_name) {
    cmd_.body.client_id = client_id;
    const char *tmp = client_name.c_str();
    for (int i = 0; tmp[i] && i < kMaxClientNameSize; ++i)
      cmd_.body.client_name[i] = tmp[i];
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

#endif  // PACKETS_CLIENT_JOINED_LOBBY_SIG_H
