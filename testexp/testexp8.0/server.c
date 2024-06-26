#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

sqlite3 *db;

// 初始化数据库
void init_db() {
    char *err_msg = 0;
    int rc = sqlite3_open("users.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    char *sql = "CREATE TABLE IF NOT EXISTS Users("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                "Username TEXT NOT NULL,"
                "Password TEXT NOT NULL,"
                "Status TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
}

// 插入用户
void insert_user(const char *username, const char *password) {
    char *err_msg = 0;
    char sql[BUFFER_SIZE];
    snprintf(sql, sizeof(sql), "INSERT INTO Users(Username, Password, Status) VALUES('%s', '%s', 'offline');", username, password);
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// 用户登录验证
int authenticate_user(const char *username, const char *password) {
    char *err_msg = 0;
    char sql[BUFFER_SIZE];
    char **results = 0;
    int rows, columns;
    
    snprintf(sql, sizeof(sql), "SELECT * FROM Users WHERE Username='%s' AND Password='%s';", username, password);
    int rc = sqlite3_get_table(db, sql, &results, &rows, &columns, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }

    sqlite3_free_table(results);
    return rows > 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    init_db();
    insert_user("user1", "password1"); // Example user for testing

    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // 接受客户端连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Client: %s\n", buffer);

        if (strncmp(buffer, "login", 5) == 0) {
            char username[50], password[50];
            sscanf(buffer, "login %s %s", username, password);

            if (authenticate_user(username, password)) {
                send(new_socket, "Login successful", strlen("Login successful"), 0);
            } else {
                send(new_socket, "Login failed", strlen("Login failed"), 0);
            }
        } else if (strcmp(buffer, "quit") == 0) {
            printf("Client requested to quit. Shutting down server.\n");
            break;
        } else {
            send(new_socket, buffer, valread, 0);
        }
    }

    close(new_socket);
    close(server_fd);
    sqlite3_close(db);
    return 0;
}
