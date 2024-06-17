#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080
#define BUFFER_SIZE 1024

int sock = 0;
GtkWidget *message_entry;
GtkWidget *chat_view;
GtkTextBuffer *chat_buffer;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }
        
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(chat_buffer, &end);
        gtk_text_buffer_insert(chat_buffer, &end, buffer, -1);
        gtk_text_buffer_insert(chat_buffer, &end, "\n", -1);
    }
    return NULL;
}

void on_send_button_clicked(GtkWidget *widget, gpointer data) {
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_entry));
    send(sock, message, strlen(message), 0);
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");
}

void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(data));
    const char *password = gtk_entry_get_text(GTK_ENTRY(message_entry));
    
    char login_message[BUFFER_SIZE];
    snprintf(login_message, sizeof(login_message), "login %s %s", username, password);
    send(sock, login_message, strlen(login_message), 0);
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *login_button;
    GtkWidget *send_button;
    GtkWidget *username_entry;
    GtkWidget *scrolled_window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), username_entry, 0, 0, 1, 1);

    login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), username_entry);
    gtk_grid_attach(GTK_GRID(grid), login_button, 1, 0, 1, 1);

    message_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), message_entry, 0, 1, 2, 1);

    send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(on_send_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), send_button, 2, 1, 1, 1);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 3, 1);

    chat_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), chat_view);
    chat_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_view));

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    struct sockaddr_in serv_addr;

    // 创建socket文件描述符
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IPv4和IPv6地址从文本转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0) {
        perror("pthread_create");
        return -1;
    }

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.ChatClient", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    close(sock);
    return status;
}
