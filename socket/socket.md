# socket demo

例子是chatgpt生成的，对于这种标准API的使用举例，非常准确方便。

## TCP stream

SOCK_STREAM: Indicates a TCP socket (stream-based communication).

- Receiving data with `read` or `recv` or `recvmsg`;
- Sending data with `write` or `send` or 'sendmsg`;

API按照顺序支持的场景依次变的复杂，可以看官方API说明。

是阻塞还是非阻塞，通过 `fcntl` 来设置对应 socket fd即可。
同步阻塞接口代码会相对好些，阻塞后，OS会调度对应thread让出来CPU，直到有数据重新调度唤醒执行。
