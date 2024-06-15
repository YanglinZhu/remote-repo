#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_KEY 1234
#define MAX_MSG_SIZE 1024
#define FILE_REQUEST 1
#define FILE_DATA 2

struct message {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

void receive_file(int msgid, const char *output_filename) {
    FILE *file = fopen(output_filename, "wb");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    struct message msg;
    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), FILE_DATA, 0) == -1) {
            perror("msgrcv");
            fclose(file);
            return;
        }
        if (msg.msg_text[0] == '\0') {
            break;  // End of file
        }
        fwrite(msg.msg_text, sizeof(char), strlen(msg.msg_text), file);
    }

    fclose(file);
    printf("File received and saved as %s\n", output_filename);
}

int main() {
    int msgid = msgget(MSG_KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message msg;
    msg.msg_type = FILE_REQUEST;

    printf("Enter the filename to request: ");
    scanf("%s", msg.msg_text);

    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    receive_file(msgid, "received_file");

    return 0;
}
