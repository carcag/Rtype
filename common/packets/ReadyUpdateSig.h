#ifndef PACKETS_READY_UPDATE_COMMMAND_H
#define PACKETS_READY_UPDATE_COMMMAND_H

# include <packets/Packet.h>

# include <cstdint>

class ReadyUpdateSig : public Packet {
 public:
  ~ReadyUpdateSig() {}
  ReadyUpdateSig() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kReadyUpdateSig;
  }

# pragma pack(push, 1)
  struct Body {
    int32_t client_id = 0;
    int8_t ready = 0;
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_status(int32_t client_id, bool ready) {
    cmd_.body.client_id = client_id;
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

#endif  // PACKETS_READY_UPDATE_COMMMAND_H
