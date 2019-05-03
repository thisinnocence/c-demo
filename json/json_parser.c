#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define X64_LONG_MAX 9223372036854775807L

typedef enum json_type_e {
    JSON_NUM,
    JSON_STRING,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL,
    JSON_OBJ,
    JSON_ARRAY,
} json_type_t;

typedef struct json_value_s json_value_t;
typedef struct json_obj_s json_obj_t;
typedef struct json_array_s json_array_t;

struct json_value_s {
    json_type_t type;
    union {
        json_obj_t *obj;
        json_array_t *array;
        long num;
        char *str;
    } val;
};

struct json_obj_s {
    char *key;
    json_value_t value;
    json_obj_t *next;
};

struct json_array_s {
    json_value_t value;
    json_array_t *next;
};

typedef struct json_ctx_s {
    const char *cur;
} json_ctx_t;

int parse_obj(json_ctx_t *ctx, json_value_t *v);

int parse_value(json_ctx_t *ctx, json_value_t *v);

int is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

void skip_whitespace(json_ctx_t *ctx) {
    while (is_whitespace(*ctx->cur)) {
        ctx->cur++;
    }
}

int parse_string(json_ctx_t *ctx, char **out) {
    ctx->cur++;
    const char *begin = ctx->cur;
    size_t num = 0;
    while (*ctx->cur != '"') {
        ctx->cur++;
        num++;
    }
    char *tmp = (char *) malloc(num + 1);
    if (tmp == NULL)
        return -1;
    strncpy(tmp, begin, num);
    tmp[num] = '\0';
    *out = tmp;
    ctx->cur++;
    return 0;
}

// x64 long
int parse_number(json_ctx_t *ctx, json_value_t *v) {
    long n = 0;
    // https://tools.ietf.org/html/rfc7159#page-6
    if (ctx->cur[0] == '0' && ctx->cur[1] != '0' && isdigit(ctx->cur[1]))
        return -1;

    int sign_flag = 1;
    if (*ctx->cur == '-') {
        sign_flag = -1;
        ctx->cur++;
    }

    while (isdigit(*ctx->cur)) {
        if (n > X64_LONG_MAX / 10)
            return -1;
        if (n == X64_LONG_MAX / 10 && *ctx->cur - '0' > X64_LONG_MAX % 10)
            return -1;
        n = n * 10 + (*ctx->cur) - '0';
        ctx->cur++;
    }
    v->type = JSON_NUM;
    v->val.num = n * sign_flag;
    return 0;
}

int parse_array(json_ctx_t *ctx, json_value_t *v) {
    if (*ctx->cur != '[')
        return -1;

    ctx->cur++;
    skip_whitespace(ctx);
    if (*ctx->cur == ']')
        return 0;

    v->type = JSON_ARRAY;
    while (1) {
        json_array_t *a = malloc(sizeof(json_array_t));
        if (a == NULL)
            return -1;
        a->next = v->val.array;
        v->val.array = a;

        if (parse_value(ctx, &a->value) != 0)
            return -1;
        skip_whitespace(ctx);

        if (*ctx->cur == ']') {
            ctx->cur++;
            return 0;
        }

        if (*ctx->cur == ',') {
            ctx->cur++;
        } else {
            return -1;
        }
        skip_whitespace(ctx);
    }
}

int parse_obj(json_ctx_t *ctx, json_value_t *v) {
    if (*ctx->cur != '{')
        return -1;

    ctx->cur++;
    skip_whitespace(ctx);
    if (*ctx->cur == '}')
        return 0;

    v->type = JSON_OBJ;
    while (1) {
        json_obj_t *m = malloc(sizeof(json_obj_t));
        if (m == NULL)
            return -1;
        m->next = v->val.obj;
        v->val.obj = m;

        // key
        if (*ctx->cur == '"') {
            parse_string(ctx, &m->key);
        } else {
            return -1;
        }
        skip_whitespace(ctx);

        // colon
        if (*ctx->cur++ != ':')
            return -1;
        skip_whitespace(ctx);

        // value
        if (parse_value(ctx, &m->value) == -1)
            return -1;

        // next element
        skip_whitespace(ctx);
        if (*ctx->cur == ',') {
            ctx->cur++;
            skip_whitespace(ctx);
        } else if (*ctx->cur == '}') {
            ctx->cur++;
            return 0;
        } else {
            return -1;
        }
    }
}

int parse_string_word(json_ctx_t *ctx, const char *word, json_value_t *v, json_type_t type) {
    while (*word) {
        if (*ctx->cur != *word)
            return -1;
        ctx->cur++;
        word++;
    }
    v->type = type;
    return 0;
}

int parse_value(json_ctx_t *ctx, json_value_t *v) {
    switch (*ctx->cur) {
        case '"':
            v->type = JSON_STRING;
            return parse_string(ctx, &v->val.str);
        case '{':
            v->type = JSON_OBJ;
            v->val.obj = NULL;
            return parse_obj(ctx, v);
        case 't':
            v->type = JSON_TRUE;
            return parse_string_word(ctx, "true", v, JSON_TRUE);
        case 'f':
            v->type = JSON_FALSE;
            return parse_string_word(ctx, "false", v, JSON_FALSE);
        case 'n':
            v->type = JSON_NULL;
            return parse_string_word(ctx, "null", v, JSON_NULL);
        case '[':
            v->type = JSON_ARRAY;
            v->val.array = NULL;
            return parse_array(ctx, v);
        default:
            return parse_number(ctx, v);
    }
}

json_value_t *parse(const char *json) {
    json_ctx_t ctx;
    ctx.cur = json;
    skip_whitespace(&ctx);
    if (*ctx.cur != '{')  // first must an object
        return NULL;
    json_value_t *v = malloc(sizeof(json_value_t));
    if (v == NULL)
        return NULL;
    int ret = parse_value(&ctx, v);
    if (ret != 0) {
        printf("parse ret %d\n", ret);
        //TODO: free memory
    }
    return v;
}

void show(json_value_t *value) {
    switch (value->type) {
        case JSON_STRING:
            printf("[str]%s\n", value->val.str);
            break;
        case JSON_NUM:
            printf("[num]%ld\n", value->val.num);
            break;
        case JSON_OBJ:
            printf("[obj]-----start\n");
            json_obj_t *head = value->val.obj;
            while (head) {
                printf("[key]%s:\t", head->key);
                printf("[value]");
                show(&head->value);
                head = head->next;
            }
            printf("[obj]-----end\n");
            break;
        case JSON_ARRAY:
            printf("[array]-----start\n");
            json_array_t *arr = value->val.array;
            while (arr) {
                printf("[value]");
                show(&arr->value);
                arr = arr->next;
            }
            printf("[array]-----end\n");
            break;
        case JSON_TRUE:
            printf("true\n");
            break;
        case JSON_FALSE:
            printf("false\n");
            break;
        case JSON_NULL:
            printf("null\n");
            break;
        default:
            return;
    }
}

int main() {
    char *json = "{\"hi\":[1,\"hi\",{\"hello\":22}],\n"
                 "\"isNull  \":null,\n"
                 "\"isTrue\":  true,\n"
                 "\"hello2\":-2,\n"
                 "\"arr2\":[\"hi\",3]}";
    json_value_t *value = parse(json);
    if (value == NULL) {
        printf("parse error!\n");
        return -1;
    }
    show(value);
    return 0;
}
