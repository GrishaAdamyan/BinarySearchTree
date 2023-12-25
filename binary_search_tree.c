#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    struct Node * left;
    struct Node * right;
    int value;
    int quantity;
};

void add(struct Node **head, int value, int * most_frequent_value, int * most_frequent_quantity){
    struct Node *copy_head = *head;
    struct Node **new_head = head;
    int val = *most_frequent_value;
    int q = *most_frequent_quantity;
    if (copy_head->value == 0 && copy_head->quantity == 0){
        copy_head->quantity = 1;
        copy_head->value = value;
        if (copy_head->quantity > q){
            q = copy_head->quantity;
            val = copy_head->value;
        }
    }else{
        int f = 0;
        while (f == 0){
            if (copy_head->left != NULL && copy_head->value > value){
                copy_head = copy_head->left;
            }else if (copy_head->left == NULL && copy_head->value > value){
                struct Node *new = malloc(sizeof(struct Node));
                new->quantity = 1;
                new->value = value;
                new->left = NULL;
                new->right = NULL;
                copy_head->left = new;
                f = 1;
            }else if (copy_head->value == value){
                copy_head->quantity += 1;
                f = 1;
            }else if (copy_head->right != NULL && copy_head->value < value){
                copy_head = copy_head->right;
            }else if (copy_head->right == NULL && copy_head->value < value){
                struct Node *new = malloc(sizeof(struct Node));
                new->quantity = 1;
                new->value = value;
                new->right = NULL;
                copy_head->right = new;
                f = 1;
            }else if (copy_head->value == value){
                copy_head->quantity += 1;
                f = 1;
            }
            if (copy_head->quantity > q){
                    q = copy_head->quantity;
                    val = copy_head->value;
            }
        }
    }
    *most_frequent_quantity = q;
    *most_frequent_value = val;
    head = new_head;
    return;
}

void freeing(struct Node ** head) {
    if ((*head)->left != NULL){
        freeing(&(*head)->left);
    }if ((*head)->right != NULL){
        freeing(&(*head)->right);
    }
    free((*head));
}

int main(){
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1){
        perror("Cannot open file");
        return errno;
    }
    int flag = 0;
    int count = 0;
    char buf[1];
    while (flag == 0){
        ssize_t bytes = read(fd, buf, 1);
        if (bytes == -1){
            perror("Cannot open file");
            return errno;
        }else if (bytes == 0){
            flag = 1;
        }else{
            count += 1;
        }
    }
    close(fd);

    char buf_final[count];
    int fd_final = open("test.txt", O_RDONLY);
    if (fd_final == -1){
        perror("Cannot open file");
        return errno;
    }
    ssize_t bytes = read(fd_final, buf_final, count);
    if (bytes == -1){
        perror("Cannot open file");
        return errno;
    }
    buf_final[bytes] = '\0'; // buf[bytes] = 0;
    close(fd_final);

    struct Node *Head = malloc(sizeof(struct Node));
    Head->value = 0;
    Head->quantity = 0;
    Head->left = NULL;
    Head->right = NULL;
    char str[count];
    int point = 0;
    int most_frequent_value = 0;
    int most_frequent_quantity = 0;
    for (int i = 0; i < count + 1; i++){
        if (buf_final[i] == ' ' || i == count){
            if (point != 0){
                int sum = 0;
                int result = 1;
                for (int j = 0; j < point; j++){
                    for (int k = 0; k < point - 1 - j; k++){
                        result *= 10;
                    }
                    sum += str[i - point + j] * result;
                    result = 1;
                }
                add(&Head, sum, &most_frequent_value, &most_frequent_quantity);
                point = 0;
            }
        }else{
            str[i] = buf_final[i] - '0';
            point += 1;
        }
    }
    freeing(&Head);
    printf("%d: Quantity\n", most_frequent_quantity);
    printf("%d: Value\n", most_frequent_value);
    return 0;
}