#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "localhost" // 这里假设服务器IP，实际中请替换为真实IP
#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    // 创建套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "创建套接字失败" << std::endl;
        return -1;
    }

    // 配置服务器地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // 连接服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "连接服务器失败" << std::endl;
        close(client_socket);
        return -1;
    }

    // 准备要发送的数据，模拟粘包和半包情况
    char msg1[] = "Hello";
    char msg2[] = "World";
    char buffer[BUFFER_SIZE];
    // 先模拟粘包情况，将两个消息连续放入缓冲区发送
    std::strcpy(buffer, msg1);
    std::strcat(buffer, msg2);
    ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
    if (bytes_sent == -1) {
        std::cerr << "发送数据失败" << std::endl;
        close(client_socket);
        return -1;
    }
    std::cout << "已发送粘包数据，大小: " << bytes_sent << " 字节" << std::endl;

    // 模拟半包情况，发送部分数据
    std::memset(buffer, 0, sizeof(buffer));
    std::strcpy(buffer, msg1);
    bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
    if (bytes_sent == -1) {
        std::cerr << "发送数据失败" << std::endl;
        close(client_socket);
        return -1;
    }
    std::cout << "已发送半包数据，大小: " << bytes_sent << " 字节" << std::endl;

    close(client_socket);
    return 0;
}
