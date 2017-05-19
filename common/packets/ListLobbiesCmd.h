#ifndef PACKETS_LOBBIES_LIST_H
#define PACKETS_LOBBIES_LIST_H

# include <packets/Packet.h>

# include <cstdint>

class ListLobbiesCmd : public Packet {
 public:
  ~ListLobbiesCmd() {}
  ListLobbiesCmd() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kListLobbiesCmd;
  }

# pragma pack(push, 1)
  struct Cmd {
    Packet::Header header;
  };
# pragma pack(pop)

  const char *pack() const {
    char *data = new char[sizeof(Cmd)];
    std::memcpy(data, &cmd_, sizeof(Cmd));
    return data;
  }

 private:
  Cmd cmd_;
};

#endif  // PACKETS_LOBBIES_LIST_H
