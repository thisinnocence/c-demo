#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct vector {
    size_t item_size;
    unsigned int item_num;
    unsigned int item_max_num;
    void *head;
};

void ve_push_back(struct vector *ve, void *item) {
    if (ve->item_num == ve->item_max_num) {
        unsigned int old_len = ve->item_size * ve->item_num;
        ve->item_max_num *= 1.5;
        void *new_array = malloc(ve->item_size * ve->item_max_num);
        memcpy(new_array, ve->head, old_len);
        free(ve->head);
        ve->head = new_array;
    }
    void *new_item = malloc(ve->item_size);
    if (new_item == NULL) {
        printf("malloc failed");
        return;
    }
    void *pos = (char *) ve->head + ve->item_num * ve->item_size;
    memcpy(pos, item, ve->item_size);
    ve->item_num++;
}

void ve_erase(struct vector *ve, void *item) {
    if (item == NULL) {
        printf("erase nullptr\n");
        return;
    }

    char *last = (char *) ve->head + ve->item_size * (ve->item_num - 1);
    if (last < (char*)item) {
        printf("erase item out of range\n");
        return;
    }
    if (item < ve->head) {
        printf("erase item addr invalid, < first item\n");
        return;
    }

    ve->item_num--;
    if (last > (char*)item) {
        memmove(item, (char *) item + ve->item_size, last - (char *) item);
    }
}

void ve_init(struct vector *ve, size_t item_size) {
    ve->item_max_num = 5;
    ve->head = malloc(ve->item_max_num * item_size);
    ve->item_size = item_size;
    ve->item_num = 0;
}

void ve_clear(struct vector *ve) {
    free(ve->head);
    memset(ve, 0, sizeof(*ve));
}

void show_re(struct vector *ve) {
    int i;
    int *data = (int *) ve->head;
    for (i = 0; i < ve->item_num; i++) {
        printf("%d ", data[i]);
    }
}

int main() {
    int n = 0;
    struct vector ve;
    ve_init(&ve, sizeof(int));
    ve.head = malloc(ve.item_max_num * ve.item_size);

    while (n != -1) {
        scanf("%d", &n);
        ve_push_back(&ve, &n);
        show_re(&ve);
        printf("\n");
    }

    while (n != -2) {
        scanf("%d", &n);
        int *item = NULL;
        unsigned int i = 0;
        for (i = 0; i < ve.item_num; i++) {
            if (((int *) ve.head)[i] == n) {
                item = &((int *) ve.head)[i];
                break;
            }
        }
        ve_erase(&ve, item);
        show_re(&ve);
        printf("\n");
    }
    ve_clear(&ve);
    return 0;
}
