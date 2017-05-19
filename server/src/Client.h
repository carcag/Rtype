#ifndef CLIENT_H
# define CLIENT_H

# include <string>
# include <vector>

# include "net/TcpSocket.h"
# include "net/UdpSocket.h"
# include "net/IpAddress.h"
# include "Entity.h"
# include "Spaceship.h"
# include "ClientEvent.h"

class Client {
 public:
  enum Location {
    kHome = 0,
    kGameLobby,
    kInGame
  };

 private:
  // Network related stuff
  TcpSocket *tcp_sock_;
  UdpSocket *udp_sock_;
  IpAddress ip_;
  // Client related stuff
  int id_;
  std::string name_;
  // Location/Status related stuff
  Location location_;
  int location_id_;  // lobby or game ID
  bool ready_;  // ready to start the game
  // If set to true, will be removed on the next loop tick
  bool remove_flag_;
  // Game stuff
  uint8_t player_id_;

  void remove();
  // Commands handling
  void HandleConnectCmd();
  void HandleCreateLobbyCmd();
  void HandleJoinLobbyCmd();
  void HandleLeaveLobbyCmd();
  void HandleListLobbiesCmd();
  void HandleSetReadyCmd();
  // Send Responses to the client
  void SendStatusResp(int8_t ok,
                      uint8_t cmd,
                      uint8_t code = 0,
                      const std::string& msg = "") const;
  void SendJoinLobbyResp(int32_t id,
                         const std::string& name,
                         std::vector<Client *> clients) const;
  void SendListLobbiesResp() const;

 public:
  Client() = delete;
  Client(TcpSocket *s, const IpAddress& ip, int id);
  ~Client();
  Client(const Client&) = delete;
  Client operator=(const Client&) = delete;
  // Network
  void set_udp_socket(UdpSocket *udp_sock) { udp_sock_ = udp_sock; }
  const IpAddress& get_ip() const { return ip_; }
  // Client
  int get_id() const { return id_; }
  const std::string& get_name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }
  // Location/Status
  Location get_location() const { return location_; }
  void set_location(Location loc) { location_ = loc; }
  int get_location_id() const { return location_id_; }
  void set_location_id(int location_id) { location_id_ = location_id; }
  bool is_ready() const { return ready_; }
  void set_ready(bool ready) { ready_ = ready; }
  // Game stuff
  void set_player_id(uint8_t player_id) { player_id_ = player_id; }
  uint8_t get_player_id() { return player_id_; }

  bool to_remove() const { return remove_flag_; }
  // Send Signals to the client
  void SendGameStartedSig(uint16_t port, uint8_t player_nb) const;
  void SendReadyUpdateSig(int client_id, bool ready) const;
  void SendClientJoinedLobbySig(int client_id,
                                const std::string& client_name) const;
  void SendClientLeftLobbySig(int client_id) const;
  void SendClientDisconSig(int client_id) const;
  // Game packet
  void SendServerPack(const std::vector<Entity *>& entities);
  // Client's API
  void Update();
  // void PollEvents(Spaceship *spaceship);
  bool PollEvents(ClientEvent& event);
};

#endif  // CLIENT_H
