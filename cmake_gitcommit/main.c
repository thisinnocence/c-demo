#include <stdio.h>

int main()
{
    printf("build_sha1:\t%s\nbuild_time:\t%s %s\n",
           GIT_COMMIT_SHA1, __DATE__, __TIME__);
    return 0;
}
