/* walkup is a trivial C program to quickly look for a magic file up the directory tree from a starting directory  */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


int
main(int argc, char* argv[])
{
    struct stat root;
    struct stat cwd_stat;
    struct stat magic_stat;
    int found = 0;
    char *cwd_name;
    char *search_for;

    if (argc != 2) {
        fprintf(stderr, "usage: %s MAGIC\n", argv[0]);
        exit(3);
    }

    search_for = argv[1];

    if (0 > stat("/", &root)) {
        perror("stat root");
        exit(2);
    }

    if (0 > stat(".", &cwd_stat)) {
        perror("stat cwd");
        exit(2);
    }

    while (!((root.st_dev == cwd_stat.st_dev) && (root.st_ino == cwd_stat.st_ino))) {
        if (0 > stat(search_for, &magic_stat)) {
            if (errno != ENOENT) {
                perror("stat magic");
                exit(2);
            }
        }
        else {
            found++;
            break;
        }

        if (0 > chdir("..")) {
            perror("chdir upwards");
            exit(2);
        }

        if (0 > stat(".", &cwd_stat)) {
            perror("stat cwd");
            exit(2);
        }
    }

    if (!found) {
        exit(1);
    }

    if (NULL == (cwd_name = getcwd(NULL, 0))) {
        perror("getwd");
        exit(2);
    };

    printf("%s\n", cwd_name);

    free(cwd_name);

    return 0;
}
