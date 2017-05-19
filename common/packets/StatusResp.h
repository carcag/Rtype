#ifndef PACKETS_STATUS_COMMMAND_H
#define PACKETS_STATUS_COMMMAND_H

# include <packets/Packet.h>

# include <string>
# include <cstdint>

class StatusResp : public Packet {
 public:
  ~StatusResp() {}
  StatusResp() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kStatusResp;
  }

# pragma pack(push, 1)
  struct Body {
    int8_t ok = 0;
    uint8_t cmd = Packet::kInvalid;
    uint8_t err_code = 0;
    char err_msg[kMaxMessageSize] = {0};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_status(int8_t ok,
                  uint8_t cmd,
                  uint8_t code = 0,
                  const std::string& err_msg = "") {
    cmd_.body.ok = ok;
    cmd_.body.cmd = cmd;
    cmd_.body.err_code = code;
    const char *tmp = err_msg.c_str();
    int i = 0;
    while (tmp[i] && i < kMaxMessageSize) {
      cmd_.body.err_msg[i] = tmp[i];
      i++;
    }
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

#endif  // PACKETS_STATUS_COMMMAND_H
