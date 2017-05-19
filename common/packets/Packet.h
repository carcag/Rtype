#ifndef PACKETS_ICOMMAND_H
#define PACKETS_ICOMMAND_H

# include <cstdint>
# include <cstring>

class Packet {
 public:
  virtual ~Packet() {}

  enum Code {
    // Commands
    kConnectCmd = 0,
    kCreateLobbyCmd,
    kJoinLobbyCmd,
    kLeaveLobbyCmd,
    kListLobbiesCmd,
    kSetReadyCmd,
    // Responses
    kStatusResp,
    kJoinLobbyResp,
    kListLobbiesResp,
    // Signals
    kGameStartedSig,
    kReadyUpdateSig,
    kClientDisconSig,
    kClientJoinedLobbySig,
    kClientLeftLobbySig,
    // Invalid
    kInvalid
  };

# pragma pack(push, 1)
  struct Header {
    int16_t magic_nb;
    int8_t code;
  };
# pragma pack(pop)

  static const int16_t kMagicNb = 0x2020;
  static const int32_t kHeaderSize = sizeof(Header);

  static const int16_t kMaxClientNameSize = 16;
  static const int16_t kMaxLobbyNameSize = 16;
  static const int16_t kMaxClientsPerLobby = 4;
  static const int16_t kMaxLobbiesNb = 16;
  static const int16_t kMaxMessageSize = 256;
  static const int16_t kMaxKeys = 5;
  static const int16_t kMaxEntities = 64;

  virtual const char *pack() const = 0;

  static Header unpack(const char *data) {
    Header header;
    std::memcpy(&header, data, sizeof(Header));
    return header;
  }
};

#endif  // PACKETS_ICOMMAND_H
