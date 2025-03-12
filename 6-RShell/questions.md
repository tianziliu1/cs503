1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_answer here_ The remote client can use the following to determine when a command's output is fully received from the server: fixed-length messages, delimiter-based protocol, length-prefixed protocol, chunked transfer encoding, and timeouts and keep-alive mechanisms, etc. 

Here are some techniques can be used to handle partial reads or ensure complete message transmission: buffering, non-blocking I/O, asynchronous I/O, Message reassembly, etc.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_answer here_ Since TCP does not preserve message booundaries, a networked shell protocol should use delimiter-based protocol (i.e. use a newline (\n)); use length-prefixed protocol (i.e. use a fixed-size header that specifies the length of the command); use message framing (i.e. a header with message type and length helping in identifying and parsing the message boundaries). 

If this is not handled correctly, there could be some challenges including data corruption, synchronizatin issues, buffer overflow, increased latency, etc.

3. Describe the general differences between stateful and stateless protocols.

_answer here_ The stateful protocols require more overhead (more memory and processing power) on the server than the stateless protocols. For the stateful protocols, the server maintains the state of each client session and keeps track of previous interations, while the stateless protocols' server does not.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_answer here_ Although UDP is "unreliable," the reasons for using it are due to its efficiency for small messages, low latency (minimal overhead compared to TCP), Simpler and faster. It's ideal for applications that require real-time communication and stateless communication. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_ Socket is provided by the operating system to enable applications to use network communications. A socket is an endpoint for communication between two machines. It can be used for both connection-oriented (TCP) and connectionless (UDP) communication.