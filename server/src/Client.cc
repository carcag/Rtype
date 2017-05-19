#include "Client.h"
#include "Server.h"
#include "ErrCodes.h"
#include "net/TcpSocket.h"
#include "packets/Packets.h"

Client::Client(TcpSocket *s, const IpAddress& ip, int id) : tcp_sock_(s),
udp_sock_(nullptr), ip_(ip), id_(id), name_(""), location_(kHome),
location_id_(-1), ready_(false), remove_flag_(false), player_id_(0) {
}

Client::~Client() {
  if (tcp_sock_ != nullptr)
    delete tcp_sock_;
  if (udp_sock_ != nullptr)
    delete udp_sock_;
}

void Client::remove() {
  if (location_ == kGameLobby) {
    Server::get().RemoveClientFromLobby(this, location_id_);
  } else if (location_ == kInGame) {
    Server::get().RemoveClientFromGame(this, location_id_);
  }
  remove_flag_ = true;
}

/*
** Commands handling
*/

void Client::HandleConnectCmd() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_->Receive(buf, ConnectCmd::body_size(), &received);
  const ConnectCmd::Body& c = ConnectCmd::unpack(buf);
  set_name(c.name);
  SendStatusResp(0, Packet::kConnectCmd);
}

void Client::HandleCreateLobbyCmd() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_->Receive(buf, CreateLobbyCmd::body_size(), &received);
  const CreateLobbyCmd::Body& c = CreateLobbyCmd::unpack(buf);
  if (location_id_ != -1) {
    Server::get().RemoveClientFromLobby(this, location_id_);
  }
  auto lobby = Server::get().CreateLobby(this, c.name);
  location_id_ = lobby->get_id();
  location_ = kGameLobby;
  SendJoinLobbyResp(lobby->get_id(), lobby->get_name(), lobby->get_clients());
}

void Client::HandleJoinLobbyCmd() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_->Receive(buf, JoinLobbyCmd::body_size(), &received);
  const JoinLobbyCmd::Body& c = JoinLobbyCmd::unpack(buf);
  auto lobby = Server::get().AddClientToLobby(this, c.lobby_id);
  if (lobby != nullptr) {
    location_id_ = c.lobby_id;
    location_ = kGameLobby;
    SendJoinLobbyResp(lobby->get_id(), lobby->get_name(), lobby->get_clients());
  } else {
    SendStatusResp(1, Packet::kJoinLobbyCmd, LOBBY_FULL, "Lobby is full");
  }
}

void Client::HandleLeaveLobbyCmd() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_->Receive(buf, LeaveLobbyCmd::body_size(), &received);
  const LeaveLobbyCmd::Body& c = LeaveLobbyCmd::unpack(buf);
  if (Server::get().RemoveClientFromLobby(this, this->location_id_)) {
    location_id_ = -1;
    location_ = kHome;
    SendStatusResp(0, Packet::kLeaveLobbyCmd);
  } else {
    SendStatusResp(1, Packet::kLeaveLobbyCmd, NOT_IN_LOBBY, "Not in this lobby");
  }
}

void Client::HandleListLobbiesCmd() {
  SendListLobbiesResp();
}

void Client::HandleSetReadyCmd() {
  char buf[4096] = {0};
  std::size_t received = 0;

  tcp_sock_->Receive(buf, SetReadyCmd::body_size(), &received);
  const SetReadyCmd::Body& c = SetReadyCmd::unpack(buf);
  ready_ = c.ready == 1 ? true : false;
  if (Server::get().SetClientReady(this, location_id_)) {
    SendStatusResp(0, Packet::kSetReadyCmd);
  } else {
    ready_ = false;
    SendStatusResp(1, Packet::kSetReadyCmd, NOT_IN_LOBBY, "Not in this lobby");
  }
}

/*
** Send Responses to the client
*/

void Client::SendStatusResp(int8_t ok,
                            uint8_t cmd,
                            uint8_t code,
                            const std::string& msg) const {
  StatusResp sc;
  sc.set_status(ok, cmd, code, msg);
  const char *spack = sc.pack();
  tcp_sock_->Send(spack, Packet::kHeaderSize + sc.body_size());
  delete []spack;
}

void Client::SendJoinLobbyResp(int32_t id,
                       const std::string& name,
                       std::vector<Client *> clients) const {
  JoinLobbyResp jlr;
  jlr.set_lobby(id, name);
  for (auto c : clients) {
    jlr.add_client(c->get_id(), c->get_name(), c->is_ready());
  }
  const char *pack = jlr.pack();
  tcp_sock_->Send(pack, Packet::kHeaderSize + jlr.body_size());
  delete []pack;
}

void Client::SendListLobbiesResp() const {
  ListLobbiesResp llr;
  auto lobbies = Server::get().get_lobbies();
  for (auto l : lobbies) {
    llr.add_lobby(l->get_id(), l->get_name(), l->get_clients_nb());
  }
  const char *llrpack = llr.pack();
  tcp_sock_->Send(llrpack, Packet::kHeaderSize + llr.body_size());
  delete []llrpack;
}

/*
** Send Signals to the client
*/

void Client::SendGameStartedSig(uint16_t port, uint8_t player_nb) const {
  GameStartedSig sg;
  sg.set_infos(port, player_nb);
  const char *sgpack = sg.pack();
  tcp_sock_->Send(sgpack, Packet::kHeaderSize + sg.body_size());
  delete []sgpack;
}

void Client::SendReadyUpdateSig(int client_id, bool ready) const {
  ReadyUpdateSig ru;
  ru.set_status(client_id, ready);
  const char *sgpack = ru.pack();
  tcp_sock_->Send(sgpack, Packet::kHeaderSize + ru.body_size());
  delete []sgpack;
}

void Client::SendClientJoinedLobbySig(int client_id,
                                      const std::string& client_name) const {
  ClientJoinedLobbySig jls;
  jls.set_client_infos(client_id, client_name);
  const char *sgpack = jls.pack();
  tcp_sock_->Send(sgpack, Packet::kHeaderSize + jls.body_size());
  delete []sgpack;
}

void Client::SendClientLeftLobbySig(int client_id) const {
  ClientLeftLobbySig lls;
  lls.set_id(client_id);
  const char *sgpack = lls.pack();
  tcp_sock_->Send(sgpack, Packet::kHeaderSize + lls.body_size());
  delete []sgpack;
}

void Client::SendClientDisconSig(int client_id) const {
  ClientDisconSig cd;
  cd.set_id(client_id);
  const char *sgpack = cd.pack();
  tcp_sock_->Send(sgpack, Packet::kHeaderSize + cd.body_size());
  delete []sgpack;
}

/*
** Game packet
*/

void Client::SendServerPack(const std::vector<Entity *>& entities) {
  ServerPack sp;
  for (auto entity : entities) {
    sp.add_entity(entity->x(), entity->y(), entity->type());
  }
  const char *sgpack = sp.pack();
  udp_sock_->Send(sgpack, sp.body_size(), ip_, 2040);
  delete []sgpack;
}

/*
** Client's API
*/

void Client::Update() {
  // Read a command, if any
  char buf[4096] = {0};
  std::size_t received = 0;
  Socket::Status status;

  status = tcp_sock_->Receive(buf, Packet::kHeaderSize, &received);
  if (status == Socket::kOk) {
    const auto& h = Packet::unpack(buf);
    std::cout << "-- command received code=" << +h.code << std::endl;  // DEBUG
    switch (h.code) {
      case Packet::kConnectCmd:
        HandleConnectCmd();
        break;
      case Packet::kCreateLobbyCmd:
        HandleCreateLobbyCmd();
        break;
      case Packet::kJoinLobbyCmd:
        HandleJoinLobbyCmd();
        break;
      case Packet::kLeaveLobbyCmd:
        HandleLeaveLobbyCmd();
        break;
      case Packet::kListLobbiesCmd:
        HandleListLobbiesCmd();
        break;
      case Packet::kSetReadyCmd:
        HandleSetReadyCmd();
        break;
      default:
        std::cerr << "unknown command" << std::endl;
    }
  } else if (status == Socket::kDisconnected) {
    remove();
  } else if (status == Socket::kError) {
    std::cerr << "could not receive on client " << id_ << std::endl;
  }
}

bool Client::PollEvents(ClientEvent& event) {
  if (udp_sock_ == nullptr) {
    return false;
  }
  IpAddress ip;
  uint16_t port = 0;
  std::size_t received = 0;
  char buf[4096] = {0};
  if (udp_sock_->Receive(buf, ClientPack::body_size(), &received, &ip, &port)
        == Socket::kOk) {
    const ClientPack::Body& cp = ClientPack::unpack(buf);
    ClientEvent ce;
    ce.type = ClientEvent::kKeysPressed;
    for (int i = 0; i < Packet::kMaxKeys; ++i)
      ce.keys.pressed[i] = cp.keys[i];
    event = ce;
    return true;
  }
  return false;
}
