#include <stdio.h>

int main(){
    char buf[1024*1024];
    int len = 0;
    FILE* f = fopen("liso_header.png", "rb");
    len = fread(buf, 1, 1024*1024, f);
    fclose(f);
    write(1, buf, 100);
    return 0;
};