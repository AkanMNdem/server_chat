# ServerChat

## Overview
ServerChat is a simple server-client chat application written in C. It demonstrates basic networking concepts by implementing a chat system where messages can be sent between a server and a client.

## Features
- **Server-Client Communication**: Establishes a connection between a server and a client for message exchange.
- **Simple Chat Functionality**: Allows sending and receiving messages in real-time.
- **Network Programming in C**: Utilizes sockets and the TCP/IP protocol for network communication.

## How to Use
1. **Clone the Repository**:
git clone https://github.com/AkanMNdem/serverchat.git

2. **Compile the Code**:
- Use a C compiler like `gcc` to compile the `schat.c` file.
- Example command:
  ```
  gcc schat.c -o schat
  ```

3. **Run the Server and Client**:
- First, run the server:
  ```
  ./schat
  ```
- Then, run the client (on a different terminal or machine):
  ```
  ./schat [server_ip] [port]
  ```

## Contributing
Contributions to enhance or improve the ServerChat application are welcome. Feel free to fork the repository and submit pull requests.

---

[View the ServerChat Repository](https://github.com/AkanMNdem/serverchat)
