#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FAIL(fmt, ...) { \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    return 1; \
}

int is_dir(const char *path);

int main(int argc, char **argv)
{
    const char *dest = argv[argc - 1];
    
    if (argc == 1) {
        FAIL("cp: missing file operand\n");
    }
    else if (argc == 2) {
        FAIL("cp: missing destination operand after '%s'\n", argv[argc - 1]);
    }
    else if (!(is_dir(dest)) && argc > 3) {
        FAIL("cp: destination is not a folder\n");
    }
    
    int i;
    for (i = 1; i < argc - 1; i++) {
        printf("%s\n", argv[i]);
    }
    printf("Destination: %s\n", dest);
    
	return 0;
}

int is_dir(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}
