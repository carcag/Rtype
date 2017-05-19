#ifndef NET_NET_EVENT_H
# define NET_NET_EVENT_H

# include <cstdint>
# include "packets/Packets.h"

class NetEvent {
 public:
  enum Type {
    // Responses
    kStatusResp = 0,
    kListLobbiesResp,
    kJoinLobbyResp,
    // Signals
    kClientJoinedLobbySig,
    kClientLeftLobbySig,
    kReadyUpdateSig,
    kGameStartedSig,
    // Server packet
    kServerPack
  };
  // Status
  struct StatusResp {
    bool ok = 0;
    uint8_t cmd = Packet::kInvalid;
    uint8_t err_code = 0;
    char err_msg[Packet::kMaxMessageSize] = {0};
  };
  // List lobbies
  struct ListLobbiesResp {
    struct Lobby {
      int32_t id = 0;
      char name[Packet::kMaxLobbyNameSize] = {0};
      uint8_t clients_nb = 0;
    };

    uint8_t lobbies_nb = 0;
    Lobby lobbies[Packet::kMaxLobbiesNb] = {Lobby()};
  };
  // Join lobby
  struct JoinLobbyResp {
    struct Client {
      int32_t id = 0;
      char name[Packet::kMaxClientNameSize] = {0};
      uint8_t ready = 0;
    };

    struct Lobby {
      int32_t id = 0;
      char name[Packet::kMaxLobbyNameSize] = {0};
      uint8_t clients_nb = 0;
    };

    Lobby lobby = Lobby();
    Client clients[Packet::kMaxClientsPerLobby] = {Client()};
  };
  // Client joined Lobby
  struct ClientJoinedLobbySig {
    int32_t client_id = 0;
    char client_name[Packet::kMaxClientNameSize] = {0};
  };
  // Client left Lobby
  struct ClientLeftLobbySig {
    int32_t client_id = 0;
  };
  // Ready update
  struct ReadyUpdateSig {
    int32_t client_id = 0;
    int8_t ready = 0;
  };
  // Game started
  struct GameStartedSig {
    uint16_t port = 0;
    uint8_t player_nb = 0;
  };
  // Server Pack
  struct ServerPack {
    struct Entity {
      int16_t x = 0;
      int16_t y = 0;
      uint8_t type = 0;
    };

    uint8_t entities_nb = 0;
    Entity entities[Packet::kMaxEntities] = {Entity()};
  };

  Type type;
  union {
    StatusResp status;
    ListLobbiesResp lobbies_list;
    JoinLobbyResp join_lobby;
    ClientJoinedLobbySig client_joined;
    ClientLeftLobbySig client_left;
    ReadyUpdateSig ready_update;
    GameStartedSig game_started;
    ServerPack server_pack;
  };

  NetEvent() {}
  ~NetEvent() {}
};

#endif  // NET_NET_EVENT_H
