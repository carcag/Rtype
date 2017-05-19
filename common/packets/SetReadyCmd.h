#ifndef PACKETS_SET_READY_COMMMAND_H
#define PACKETS_SET_READY_COMMMAND_H

# include <packets/Packet.h>

# include <cstdint>

class SetReadyCmd : public Packet {
 public:
  ~SetReadyCmd() {}
  SetReadyCmd() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kSetReadyCmd;
  }

# pragma pack(push, 1)
  struct Body {
    int8_t ready = 0;
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_ready(bool ready) {
    cmd_.body.ready = (ready ? 1 : 0);
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

#endif  // PACKETS_SET_READY_COMMMAND_H
