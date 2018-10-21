#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FAIL(fmt, ...) { \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    exit(1); \
}

void validate(int argc, char **argv);
void parse_args(int argc, char **argv);
int is_file(const char *path);
int is_dir(const char *path);
size_t fsize(FILE *fp);
char *add_str(const char *str1, const char *str2);
void copy(FILE *src, FILE *dest);

unsigned char print_progress = 1;

int main(int argc, char **argv) {
    validate(argc, argv);
    //parse_args(argc, argv);

    const char *dest = argv[argc - 1];
    const char dest_last = dest[strlen(dest)];
    const int dest_is_dir = is_dir(dest);

    FILE *src_file, *dest_file;
    size_t file_index;
    for (file_index = 1; file_index < argc - 1; file_index++) {
        printf("%s\n", argv[file_index]);

        char *dest_fname;
        if (dest_is_dir) {
            printf("last character of dest is %c\n", dest[strlen(dest) - 1]);
            size_t dest_fname_size = strlen(dest_fname) + strlen(argv[file_index]);
            if (dest_last != '/') {
                dest_fname_size++;
            }

            if ((dest_fname = malloc(dest_fname_size)) == NULL) {
                FAIL("cp: an error occurred (failed to allocate memory for destination path)\n");
            }
            dest_fname[0] = '\0';
            strcat(dest_fname, dest);
            if (dest_last != '/') {
                strcat(dest_fname, "/");
            }
            strcat(dest_fname, argv[file_index]);
            
        }
        else {
            dest_fname = strdup(dest);
            printf("%s\n", dest_fname);
        }

        if ((src_file = fopen(argv[file_index], "r")) == NULL) {
            FAIL("cp: failed to open source file %s\n", argv[file_index]);
        }
        if ((dest_file = fopen(dest_fname, "w")) == NULL) {
            fclose(src_file);
            FAIL("cp: failed to open destination file %s\n", dest_fname);
        }
        
        copy(src_file, dest_file);

        fclose(src_file);
        fclose(dest_file);
        free(dest_fname);
    }
    //printf("Destination: %s\n", dest);
    
	return 0;
}

void validate(int argc, char **argv) {
    if (argc == 1) {
        FAIL("cp: missing file operand\n");
    }
    else if (argc == 2) {
        FAIL("cp: missing destination operand after '%s'\n", argv[argc - 1]);
    }
    else if (!(is_dir(argv[argc - 1])) && argc > 3) {
        FAIL("cp: destination is not a folder\n");
    }

    size_t file_index;
    for (file_index = 1; file_index < argc - 1; file_index++) {
        if (!is_file(argv[file_index])) {
            FAIL("cp: %s is not a file or does not exist\n", argv[file_index]);
        }
    }
}

void parse_args(int argc, char **argv) {
    char c;
    while ((c = getopt(argc, argv, "nd")) != -1) {
        switch (c) {
        case 'n':
        case 'd':
            print_progress = 0;
            break;
        default:
            FAIL("cp: unknown argument -%c\n", c);
            break;
        }
    }
}

int is_file(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        return 0;
    }
    return S_ISREG(statbuf.st_mode);
}

int is_dir(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0) {
       return 0;
   }
   return S_ISDIR(statbuf.st_mode);
}

size_t fsize(FILE *fp) {
    size_t size;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    return size;
}

char *add_str(const char *str1, const char *str2) {
    char *new_str;
    if ((new_str = malloc(strlen(str1) + strlen(str2) + sizeof('\0'))) == NULL) {
        return NULL;
    }

    new_str[0] = '\0';
    strcat(new_str, str1);
    strcat(new_str, str2);

    return new_str;
}

void copy(FILE *src, FILE *dest) {
    char progress_bar[] = "----------------";
    int progress_index = 0;
    double progress = 0;
    unsigned char byte;
    size_t src_size = fsize(src);
    printf("source file is %ld bytes long\n", src_size);
    size_t counter = 0;
    while (fread(&byte, sizeof(unsigned char), 1, src)) {
        if (print_progress && (!counter || (progress_index != (int)(progress * 15)))) {
            printf("[%s]\r", progress_bar);
            fflush(stdout);
            progress_index = (int)(progress * 15);
            progress_bar[progress_index] = '#';
        }
        
        // Perform the copy here
        
        progress = (double)counter/(double)src_size;
        counter++;
    }
}
