#include <stdio.h>

// http://www.cplusplus.com/reference/cstdio/scanf
int main() {
    char tmp[100] = {0};
    char tmp1[100] = {0};
    int ret;

    while ((ret = scanf("%s %s", tmp, tmp1)) != EOF) {
        printf("ret:[%d]\t%s %s\n", ret, tmp, tmp1);
        tmp[0] = '\0';
        tmp1[0] = '\0';
    }

    printf("ret:[%d], EOF(%d)\n", ret, EOF);
    return 0;
}

/*
root@ubuntu:/media/sf_VMshare/test# echo "a hi abc abcd hello" | ./a.out
ret:[2]	a hi
ret:[2]	abc abcd
ret:[1]	hello
ret:[-1], EOF(-1)
*/
