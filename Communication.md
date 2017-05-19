# R-Type Client Server Communication

## PART I : GENERAL COMMUNICATION (TCP)

All the communication is done via a custom binary protocol. 'Packets' are build
by a sender (that can be a client or the server) and are interpreted by the
receiver. There are three kinds of packets that can transit either from the
client to the server, or the other way around, from the server to the client.
Each packet is composed by a `header` and a `body`. The header has a fixed size
and the body has a variable size. Under the hood, those packets are just C
structs with no padding, like for example :

```cpp
# pragma pack(push, 1)
struct Cmd {
  Packet::Header header;
  Body body;
};
# pragma pack(pop)
```

The header is described as follow in the `Packet` base class :

```cpp
# pragma pack(push, 1)
  struct Header {
    int16_t magic_nb;
    int8_t code;
  };
# pragma pack(pop)
```

|Type|Variable|Comment|
|----|--------|-------|
|int16_t|magic_nb|Used to check `packet` integrity|
|int8_t|code|Used to determine witch command will follow|

Thus, the header is and always will be 3 bytes long. The body of those `packets`
is the variable part, and will be set accordingly to the `code` field of the
`header`. Here is a list of the possible values for `code` :
* Commands
  * kConnectCmd
  * kCreateLobbyCmd
  * kJoinLobbyCmd
  * kLeaveLobbyCmd
  * kListLobbiesCmd
  * kSetReadyCmd
* Responses
  * kStatusResp
  * kJoinLobbyResp
  * kListLobbiesResp
* Signals
  * kGameStartedSig
  * kReadyUpdateSig
  * kClientDisconSig

Those `packets` can be grouped in three categories : **Commands**, **Responses**
and **Signals**.

#### How to send a `packet` ?

To send a `packet`, you need to create an instance of it (1). Then, if its
`Body` contains fields, you can set them with the `packet` methods (2). Finally,
call the `pack()` method (3) that will return a `const char *` that you can
send over the network (4).

e.g.:

```cpp
ConnectCmd cc;  // (1)
cc.set_name("Player name");  // (2)
const char *spack = cc.pack();  // (3)
tcp_sock_->Send(spack, Packet::kHeaderSize + cc.body_size());  // (4)
delete []spack;  // free the memory allocated by pack()
```

#### How to receive a `packet` ?

To receive a `packet`, you first have to `receive()` its `Header` (1), then call
the `Packet::unpack()` method on the buffer received from the network (2). You
will be returned a `Packet::Header` instance, from witch you will be able to get
the `code` field, allowing you to detect witch `packet` was sent (3). A second
`receive()` of the command's `Body` size (4) will fetch the rest of the buffer
and finally, a call to `unpack()` will return an instance of the `Pack`'s `
Body` (5).

e.g.:

```cpp

char buf[4096] = {0};
std::size_t received = 0;
Socket::Status status;

status = tcp_sock_->Receive(buf, Packet::kHeaderSize, &received);  // (1)
const auto& h = Packet::unpack(buf);  // (2)
if (h.code == Packet::kConnectCmd) {  // (3)
  tcp_sock_->Receive(buf, ConnectCmd::body_size(), &received);  // (4)
  const ConnectCmd::Body& c = ConnectCmd::unpack(buf);  // (5)
}
```

### Commands (client->server)

    Commands are suffixed by "Cmd" and are always sent by a client to the server.

> kConnectCmd

```cpp
# pragma pack(push, 1)
  struct Body {
    char name[kMaxNameSize] = {0};
  };
# pragma pack(pop)
```
This commands is sent when a new client connects to the host. Its contains only
one field, the name of the client. It will trigger a `kStatusResp` response from
the server.

###### Methods :
- ```void set_name(const std::string& name);```

> kCreateLobbyCmd

```cpp
# pragma pack(push, 1)
  struct Body {
    char name[kMaxNameSize] = {0};
  };
# pragma pack(pop)
```

Like the `kConnectCmd`, this command has only one field, a name witch is the
name of the lobby to create. Once the lobby is created, the client who sent this
command will automatically be moved into the newly created lobby. Two answers
are possible :
  - `kJoinLobbyResp` if the lobby has successfully been created
  - `kStatusResp` if there has been an error

###### Methods :
  - ```void set_name(const std::string& name);```

> kJoinLobbyCmd

```cpp
# pragma pack(push, 1)
  struct Body {
    int32_t lobby_id = 0;
  };
# pragma pack(pop)
```

This command has one field, the ID of the lobby to join. Two answers are
possible :
  - `kJoinLobbyResp` if the client has successfully been moved to the lobby
  - `kStatusResp` if there has been an error

###### Methods :
  - ```void set_id(int32_t id);```

> kLeaveLobbyCmd

This command has no `Body` member. It will remove the client that sent it from
its current lobby if he is currently in one. This will trigger a `kStatusResp`
from the server.

> kListLobbiesCmd

Like the `kLeaveLobbyCmd` command, this one has no `Body`. It will list the
lobbies available on the server. It will trigger a `kListLobbiesResp` from the
server.

> kSetReadyCmd

```cpp
# pragma pack(push, 1)
  struct Body {
    int8_t ready = 0;
  };
# pragma pack(pop)
```

This command has one field, that can have two values 0 or 1. It simulates a
boolean and sets the client's ready status that sent it like the parameter
passed. The server will answer with a `kStatusCmd`. If successful, it will also
trigger a `kReadyUpdateSig` from the server to all the clients in to the same
lobby as the sender of this command (the sender will not receive the Signal).

###### Methods :
- ```void set_ready(bool ready);```

### Responses (server->client)

    Responses are suffixed by "Resp" and are always sent by the server after the
    reception of a "Cmd" sent by a client.

> kStatusResp

```cpp
# pragma pack(push, 1)
  struct Body {
    int8_t ok = 0;
    uint8_t err_code = 0;
    char err_msg[kMaxMessageSize] = {0};
  };
# pragma pack(pop)
```

This response is the basic response to a client command. It is composed of three
fields :

|Type|Variable|Comment|
|----|--------|-------|
|int8_t|ok|Used as a boolean, 0 means no error, 1 otherwise|
|uint8_t|err_code|Will be set to an error code if `ok` is 1|
|char[]|err_msg|Will be set to an error message if `ok` is 1|

###### Methods :
- ```void set_status(char ok, int32_t code = 0, const std::string& err_msg =
"");```

> kJoinLobbyResp

```cpp
# pragma pack(push, 1)
  struct Client {
    int32_t id = 0;
    char name[kMaxClientNameSize] = {0};
    int8_t ready = 0;
  };

  struct Lobby {
    int32_t id = 0;
    char name[kMaxLobbyNameSize] = {0};
    uint8_t clients_nb = 0;
  };

  struct Body {
    Lobby lobby;
    Client clients[kMaxClientsPerLobby] = {Client()};
  };
# pragma pack(pop)
```

The client will receive this response after sending a `kCreateLobbyCmd` or a
`kJoinLobbyCmd` command if it succeed. It is composed of two substructs, one
representing a `Client` and another one representing a `Lobby`. The `Body`
itself has a `Lobby` instance, witch represent the lobby that was
created/joined, and an array of `Clients` wich is the list of clients inside
this lobby.

###### Methods :
- ```void set_lobby(int32_t id, const std::string& name);```
- ```void add_client(int32_t id, const std::string& name, bool ready);```

> kListLobbiesResp

```cpp
# pragma pack(push, 1)
  struct Lobby {
    int16_t id = 0;
    char name[kMaxLobbyNameSize] = {0};
    uint8_t clients_nb = 0;
  };

  struct Body {
    uint8_t lobbies_nb = 0;
    Lobby lobbies[kMaxLobbiesNb] = {Lobby()};
  };
# pragma pack(pop)
```

The client will receive this response after sending a `kListLobbiesCmd`. The
`Lobby` substruct represents a `Lobby` on the server. The `Body` itself contains
the number of lobbies in the list, and an array of `Lobbies` containing details
about each `Lobby`.

###### Methods :
- ```void add_lobby(int32_t id, const std::string& name, uint8_t clients_nb);```

### Signals (server->client)

    Finally, Signals are suffixed by *Sig* and are sent by the server to one or many
    clients, without the need of a *Cmd* reception beforehand.

> kGameStartedSig

```cpp
# pragma pack(push, 1)
  struct Body {
    uint16_t port = 0;
  };
# pragma pack(pop)
```

This signal is sent to all clients in a same `lobby` when they are all ready. It
contains the UDP port each client shoud send Game Packets to.

###### Methods :
- ```void set_port(uint16_t port);```

> kReadyUpdateSig

```cpp
# pragma pack(push, 1)
  struct Body {
    int32_t client_id = 0;
    int8_t ready = 0;
  };
# pragma pack(pop)
```

This signal is sent by the server to all clients in a `lobby` (except to the
client that changed its status) when a client changes its status.

###### Methods :
- ```void set_status(int32_t client_id, bool ready);```

> kClientDisconSig

```cpp
# pragma pack(push, 1)
  struct Body {
    int32_t client_id = 0;
  };
# pragma pack(pop)
```

This signal is sent by the server to all clients in a `lobby` or in `game` when
a client disconnects from the server.

###### Methods :
- ```void set_id(int32_t client_id);```

---

## PART II : IN GAME COMMUNICATION (UDP)

In development...
