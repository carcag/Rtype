#ifndef NET_NETWORK_H
# define NET_NETWORK_H

# include <thread>
# include <mutex>
# include <stack>

# include <SFML/Network.hpp>

# include "packets/Packets.h"
# include "net/NetEvent.h"

class Network {
 private:
  static constexpr const char* kServerIp = "127.0.0.1";

  // TCP
  std::thread *tcp_listen_;
  std::mutex tcp_sock_mutex_;
  sf::TcpSocket tcp_sock_;
  // UDP
  std::thread *udp_listen_;
  std::mutex udp_listening_mutex_;
  bool udp_listening_;
  std::mutex udp_sock_mutex_;
  sf::UdpSocket udp_sock_;
  uint16_t udp_port_;

  std::mutex events_mutex_;
  std::stack<NetEvent> events_;

  // Handling Responses and Signals
  void HandleStatusResp();
  void HandleListLobbiesResp();
  void HandleJoinLobbyResp();
  void HandleClientJoinedSig();
  void HandleClientLeftSig();
  void HandleReadyUpdateSig();
  void HandleGameStartedSig();

  void HandleServerPack();

 public:
  Network();
  Network(const Network&) = delete;
  ~Network();
  Network operator=(const Network&) = delete;

  void set_udp_port(uint16_t port) { udp_port_ = port; }
  // Network's API
  void reconnect(const std::string& name);
  void NetLoop();
  bool PollNetEvent(NetEvent& event);

  void GameNetLoop();
  // Send Commands
  void SendConnectCmd(const std::string& name);
  void SendListLobbiesCmd();
  void SendJoinLobbyCmd(int32_t id);
  void SendLeaveLobbyCmd();
  void SendCreateLobbyCmd(const std::string& name);
  void SendSetReadyCmd(bool ready);

  void SendClientPack(int8_t keys[Packet::kMaxKeys]);
};

#endif  // NET_NETWORK_H
