#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    // 创建套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "创建套接字失败" << std::endl;
        return -1;
    }

    // 绑定地址和端口
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "绑定地址失败" << std::endl;
        close(server_socket);
        return -1;
    }

    // 监听端口
    if (listen(server_socket, 5) == -1) {
        std::cerr << "监听端口失败" << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "服务器正在监听端口 " << PORT << "..." << std::endl;

    // 接受客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1) {
        std::cerr << "接受客户端连接失败" << std::endl;
        close(server_socket);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    // 模拟接收数据，可能出现粘包和半包情况
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "接收到客户端数据: " << buffer << std::endl;
    } else if (bytes_received == 0) {
        std::cout << "客户端关闭连接" << std::endl;
    } else {
        std::cerr << "接收数据出错" << std::endl;
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
