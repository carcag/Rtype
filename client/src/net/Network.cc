#include <iostream>
#include "Network.h"

Network::Network() : udp_port_(0), tcp_listen_(nullptr), udp_listen_(nullptr),
udp_listening_(false) {
  tcp_sock_.connect(kServerIp, 2020);
  udp_sock_.setBlocking(false);
  udp_listening_mutex_.lock();
  udp_listening_ = false;
  udp_listening_mutex_.unlock();
  tcp_listen_ = new std::thread(&Network::NetLoop, this);
  tcp_listen_->detach();
}

Network::~Network() {
  if (tcp_listen_) {
    std::cout << "~tcp_listen_" << std::endl;
    delete tcp_listen_;
  }
  if (udp_listen_) {
    udp_listening_mutex_.lock();
    udp_listening_ = false;
    udp_listening_mutex_.unlock();
    udp_listen_->join();
    std::cout << "~udp_listen_" << std::endl;
    delete udp_listen_;
  }
}

/*
** Handling Responses and Signals
*/

void Network::HandleStatusResp() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, StatusResp::body_size(), received);
  tcp_sock_mutex_.unlock();
  const StatusResp::Body& s = StatusResp::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kStatusResp;
  ne.status.ok = s.ok == 0 ? false : true;
  ne.status.cmd = s.cmd;
  ne.status.err_code = s.err_code;
  for (int i = 0; i < StatusResp::kMaxMessageSize; ++i)
    ne.status.err_msg[i] = s.err_msg[i];
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleListLobbiesResp() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, ListLobbiesResp::body_size(), received);
  tcp_sock_mutex_.unlock();
  const ListLobbiesResp::Body& ll = ListLobbiesResp::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kListLobbiesResp;
  ne.lobbies_list.lobbies_nb = ll.lobbies_nb;
  for (int i = 0; i < ll.lobbies_nb; ++i) {
    ne.lobbies_list.lobbies[i].id = ll.lobbies[i].id;
    for (int j = 0; j < ::ListLobbiesResp::kMaxLobbyNameSize; ++j)
      ne.lobbies_list.lobbies[i].name[j] = ll.lobbies[i].name[j];
    ne.lobbies_list.lobbies[i].clients_nb = ll.lobbies[i].clients_nb;
  }
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleJoinLobbyResp() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, JoinLobbyResp::body_size(), received);
  tcp_sock_mutex_.unlock();
  const JoinLobbyResp::Body& jl = JoinLobbyResp::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kJoinLobbyResp;
  ne.join_lobby.lobby.id = jl.lobby.id;
  for (int i = 0; i < ::JoinLobbyResp::kMaxLobbyNameSize; ++i)
  ne.join_lobby.lobby.name[i] = jl.lobby.name[i];
  ne.join_lobby.lobby.clients_nb = jl.lobby.clients_nb;
  for (int i = 0; i < ::JoinLobbyResp::kMaxClientsPerLobby; ++i) {
    ne.join_lobby.clients[i].id = jl.clients[i].id;
    for (int j = 0; j < ::JoinLobbyResp::kMaxClientNameSize; ++j)
      ne.join_lobby.clients[i].name[j] = jl.clients[i].name[j];
    ne.join_lobby.clients[i].ready = jl.clients[i].ready;
  }
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleClientJoinedSig() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, ClientJoinedLobbySig::body_size(), received);
  tcp_sock_mutex_.unlock();
  const ClientJoinedLobbySig::Body& jl = ClientJoinedLobbySig::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kClientJoinedLobbySig;
  ne.client_joined.client_id = jl.client_id;
  for (int i = 0; i < Packet::kMaxClientNameSize; ++i)
    ne.client_joined.client_name[i] = jl.client_name[i];
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleClientLeftSig() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, ClientLeftLobbySig::body_size(), received);
  tcp_sock_mutex_.unlock();
  const ClientLeftLobbySig::Body& lls = ClientLeftLobbySig::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kClientLeftLobbySig;
  ne.client_joined.client_id = lls.client_id;
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleReadyUpdateSig() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, ReadyUpdateSig::body_size(), received);
  tcp_sock_mutex_.unlock();
  const ReadyUpdateSig::Body& rus = ReadyUpdateSig::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kReadyUpdateSig;
  ne.ready_update.client_id = rus.client_id;
  ne.ready_update.ready = rus.ready;
  events_mutex_.lock();
  events_.push(ne);
  events_mutex_.unlock();
}

void Network::HandleGameStartedSig() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_mutex_.lock();
  tcp_sock_.receive(buf, ReadyUpdateSig::body_size(), received);
  tcp_sock_mutex_.unlock();
  const GameStartedSig::Body& gss = GameStartedSig::unpack(buf);
  NetEvent ne;
  ne.type = NetEvent::kGameStartedSig;
  ne.game_started.port = gss.port;
  ne.game_started.player_nb = gss.player_nb;
  events_mutex_.lock();
  events_.push(ne);
  set_udp_port(gss.port);
  events_mutex_.unlock();
  // Start udp listening thread
  udp_listening_mutex_.lock();
  udp_listening_ = true;
  udp_listening_mutex_.unlock();
  udp_listen_ = new std::thread(&Network::GameNetLoop, this);
  // udp_listen_->detach();
}

/*
** Network's API
*/

void Network::reconnect(const std::string& name) {
  // End previous connection
  delete tcp_listen_;
  tcp_sock_.disconnect();
  // Start a new connection, a new listening thread
  tcp_sock_.connect(kServerIp, 2020);
  tcp_listen_ = new std::thread(&Network::NetLoop, this);
  tcp_listen_->detach();
  // Send the initial Command
  SendConnectCmd(name);
}

void Network::NetLoop() {
  char buf[4096] = {0};
  std::size_t received = 0;

  while (true) {
    std::cout << "waiting for a packet..." << std::endl;
    // tcp_sock_mutex_.lock();
    tcp_sock_.receive(buf, Packet::kHeaderSize, received);
    // tcp_sock_mutex_.unlock();
    // Server closed the connection
    if (received == 0) {
      NetEvent ne;
      ne.type = NetEvent::kStatusResp;
      ne.status.ok = true;
      ne.status.cmd = Packet::kInvalid;
      ne.status.err_code = 50;
      events_mutex_.lock();
      events_.push(ne);
      events_mutex_.unlock();
      break;
    }
    const auto& h = Packet::unpack(buf);
    std::cout << "code: " << +h.code << std::endl;
    switch (h.code) {
      case Packet::kStatusResp: HandleStatusResp(); break;
      case Packet::kListLobbiesResp: HandleListLobbiesResp(); break;
      case Packet::kJoinLobbyResp: HandleJoinLobbyResp(); break;
      case Packet::kClientJoinedLobbySig: HandleClientJoinedSig(); break;
      case Packet::kClientLeftLobbySig: HandleClientLeftSig(); break;
      case Packet::kReadyUpdateSig: HandleReadyUpdateSig(); break;
      case Packet::kGameStartedSig: HandleGameStartedSig(); break;
      default: std::cout << "unknown packet, there is a mystic error somewhere" << std::endl;
    }
  }
  std::cout << "stopped game net loop" << std::endl;
}

bool Network::PollNetEvent(NetEvent& event) {
  if (!events_.empty()) {
    event = events_.top();
    events_.pop();
    return true;
  }
  return false;
}

void Network::GameNetLoop() {
  sf::IpAddress ip;
  uint16_t port = 0;
  std::size_t received = 0;
  char buf[4096] = {0};

  udp_sock_.bind(2040);
  while (true) {
    udp_listening_mutex_.lock();
    if (!udp_listening_)
      break;
    udp_listening_mutex_.unlock();
    if (udp_sock_.receive(buf, ServerPack::body_size(), received, ip, port) == sf::Socket::Status::Done) {
      const ServerPack::Body& sp = ServerPack::unpack(buf);
      NetEvent ne;
      ne.type = NetEvent::kServerPack;
      ne.server_pack.entities_nb = sp.entities_nb;
      for (int i = 0; i < sp.entities_nb; ++i) {
        ne.server_pack.entities[i].x = sp.entities[i].x;
        ne.server_pack.entities[i].y = sp.entities[i].y;
        ne.server_pack.entities[i].type = sp.entities[i].type;
      }
      events_mutex_.lock();
      events_.push(ne);
      events_mutex_.unlock();
    }
  }
  udp_listening_mutex_.unlock();
}

/*
** Send Commands
*/

void Network::SendConnectCmd(const std::string& name) {
  ConnectCmd cc;
  cc.set_name(name);
  const char *spack = cc.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize + cc.body_size());
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendListLobbiesCmd() {
  ListLobbiesCmd ll;
  const char *spack = ll.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize);
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendJoinLobbyCmd(int32_t id) {
  JoinLobbyCmd jl;
  jl.set_id(id);
  const char *spack = jl.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize + jl.body_size());
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendLeaveLobbyCmd() {
  LeaveLobbyCmd ll;
  const char *spack = ll.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize);
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendCreateLobbyCmd(const std::string& name) {
  CreateLobbyCmd cl;
  cl.set_name(name);
  const char *spack = cl.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize + cl.body_size());
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendSetReadyCmd(bool ready) {
  SetReadyCmd sr;
  sr.set_ready(ready);
  const char *spack = sr.pack();
  tcp_sock_mutex_.lock();
  tcp_sock_.send(spack, Packet::kHeaderSize + sr.body_size());
  tcp_sock_mutex_.unlock();
  delete []spack;
}

void Network::SendClientPack(int8_t keys[Packet::kMaxKeys]) {
  ClientPack cp;
  cp.set_keys(keys);
  const char *spack = cp.pack();
  udp_sock_.send(spack, cp.body_size(),
    kServerIp, udp_port_);
  delete []spack;
}
