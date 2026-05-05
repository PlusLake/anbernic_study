#include <stdio.h>

int main(void) {
    FILE *file = fopen("hello_world.txt", "w");
    if (file != NULL) {
        fprintf(file, "Hello from the croess-compiler.\n");
        fprintf(file, "If you are reading this, the native ARM64 execution worked perfectly\n");
        fclose(file);
    }
    return 0;
}
