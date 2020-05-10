#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

struct coord {
    int x, y;
};

char ** splitString(char *str) {
    int len = 0;
    char **cmd_list = (char **)malloc(BUFF_SIZE * sizeof(char *));
    char *cmd;

    cmd = strtok(str, " ");
    while(cmd != NULL) {
        cmd_list[len++] = cmd;
        cmd_list = (char **)realloc(cmd_list, (len + 1)*sizeof(char *));
        cmd = strtok(NULL, " ");
    }
    cmd_list[len] = NULL;
    printf("%d\n", len);
    return cmd_list;
}

struct coord getCoordinates(char *s) {
    struct coord p;
    int i;
    for (i = 1; i < s[i] != ','; i++) {
        p.x = p.x * 10 + (s[i] - '0');
    }
    for (; i < strlen(s)-1; i++) {
        p.y = p.y * 10 + (s[i] - '0');
    }
    return p;
}

void printBoard(char **board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%c ", board[i][j]);
        printf("\n");
    }
}

int main() {
    int array[] = {'%', '#', '&', '@'};
    int n; scanf("%d", &n);
    char **board = (char **)malloc(n * sizeof(char *));

    for (int i = 0; i < n; i++) {
        board[i] = (char *)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++)
            board[i][j] = array[rand() % 4];
    }

    while(1) {
        char user_input[BUFF_SIZE];

        printf("Enter command: ");

        scanf("%s", user_input);
        char **args = splitString(user_input);
        char *cmd = args[0];
        printf("%s\n", args[1]);

        if (strcmp(cmd, "swap") == 0) {
            // if (sizeof(args) / sizeof(char *) != 3) {
            //     printf("swap <coordinates> <coordinates>\n");
            //     continue;
            // }
            printf("%s %s\n", args[1], args[2]);
            struct coord p1 = getCoordinates(args[1]);
            struct coord p2 = getCoordinates(args[2]);
            printf("%d %d\n", p1.x, p1.y);

            // swap(board, p1, p2);
        }
        else if (strcmp(cmd, "showboard") == 0) {
            printBoard(board, n);
        }
    }
    return 0;
}