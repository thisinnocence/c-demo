#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    uint32_t ip;
    char *val = (char*)&ip;
    // clang-tidy suggest: use strtoul
    sscanf("192.168.1.1", "%hhu.%hhu.%hhu.%hhu", &val[3], &val[2], &val[1], &val[0]);
    printf("%u\n", ip);

    uint32_t ip2;
    char *val2 = (char*)&ip2;

    char *next = "192.168.1.1";
    int n = 0, i = 3;
    while (next && *next) {
        // Reference to an object of type char*, whose value is set by the function to 
        //   the next character in str after the numerical value.
        // well, not ued often
        val2[i] = strtoul(next, &next, 0);
        printf("%hhu [%d %p %c]\n", val[i], i, next, *next);
        i--;
        if (*next == '.') next++;
    }
    printf("%u\n", ip2);
    return 0;
}

/*
3232235777
192 [3 0x106781f81 .]
168 [2 0x106781f85 .]
1 [1 0x106781f87 .]
1 [0 0x106781f89 ]
3232235777
*/
