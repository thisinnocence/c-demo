#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    uint32_t ip;
    char *val = (char*)&ip;
    // clang-tidy suggest: use strtoul
    sscanf("192.168.1.1", "%hhu.%hhu.%hhu.%hhu", &val[3], &val[2], &val[1], &val[0]);
    printf("%u\n", ip);

    uint32_t ip2;
    char *val2 = (char*)&ip2;

    char *next = "192.168.1.1";
    int i = 3;
    while (next && *next) {
        val2[i--] = strtoul(next, &next, 0);
        while(!isdigit(*next) && *next) next++;
    }
    printf("%u\n", ip2);
    return 0;
}

/* output:
3232235777
3232235777
*/
