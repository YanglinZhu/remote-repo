#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6666
#define BUFFER_SIZE 1024
const char* Username;
int sock = 0;
GtkWidget *message_entry, *chat_view, *username_entry, *password_entry;
GtkTextBuffer *chat_buffer;
pthread_t recv_thread;

void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_send_button_clicked(GtkWidget *widget, gpointer data);
void *receive_messages(void *arg);

void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    Username = username;
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Username or Password cannot be empty.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    char login_message[BUFFER_SIZE];
    snprintf(login_message, sizeof(login_message), "login %s %s", username, password);
    send(sock, login_message, strlen(login_message), 0);
}

void on_send_button_clicked(GtkWidget *widget, gpointer data) {
    char tempMessage[1024];
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_entry));
    
    if (strlen(message) == 0) {
        return;
    }
    sprintf(tempMessage , "%s: %s" , Username ,message);
    send(sock, tempMessage, strlen(tempMessage), 0);
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }
        
        // 将接收到的消息显示在聊天窗口中
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(chat_buffer, &end);
        gtk_text_buffer_insert(chat_buffer, &end, buffer, -1);
        gtk_text_buffer_insert(chat_buffer, &end, "\n", -1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;

    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将服务器IP地址转换为二进制形式并填充到结构体中
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // 初始化GTK
    gtk_init(&argc, &argv);

    // 创建主窗口
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 创建网格布局
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 创建用户名输入框
    username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), username_entry, 0, 0, 1, 1);

    // 创建密码输入框
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(password_entry), '*');
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 0, 1, 1);

    // 创建登录按钮
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), login_button, 2, 0, 1, 1);

    // 创建消息输入框
    message_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), message_entry, 0, 1, 2, 1);

    // 创建发送按钮
    GtkWidget *send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), send_button, 2, 1, 1, 1);

    // 创建聊天显示区域
    chat_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_view), FALSE);
    chat_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_view));
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), chat_view);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 3, 3);

    // 创建并启动接收消息的线程
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // 显示所有窗口和控件
    gtk_widget_show_all(window);

    // 运行GTK主循环
    gtk_main();

    // 等待接收消息线程结束
    pthread_join(recv_thread, NULL);

    // 关闭socket
    close(sock);

    return 0;
}
