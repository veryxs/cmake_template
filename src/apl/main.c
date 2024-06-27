#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../config.h"

void usage()
{
    printf("USAGE Help\n");
    printf("OPTIONS: -h help\n");
    printf("         -t test code\n");
    printf("         -v software version\n");
}

void version()
{
    printf("%s  Ver:%d.%d.%d  compile time:%s %s\n",PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_LEVEL3, __DATE__, __TIME__);
}

void test(void)
{
    printf("hello cmake template\n");
}

int main(int argc, char *argv[])
{
    printf("%s  Ver:%d.%d.%d  compile time:%s %s\n",PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_LEVEL3, __DATE__, __TIME__);
    // 判断参数数量，如果小于2就退出
    if (argc < 2)
    {
        printf("no such param\n");
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (*argv[i] == '-')
        {
            switch (argv[i++][1])
            {
            case 'h': // 帮助
                usage();
                break;
            case 't': // 测试
                test();
                break;
            case 'v': // 版本
                version();
                break;
            default:
                printf("no such param\n");
                break;
            }
        }
        else
        {
            printf("no such param\n");
        }
    }

    while (1)
    {
        sleep(1);
    }

    return 0;
}
