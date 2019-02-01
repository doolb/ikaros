#include <stdio.h>
#include <string.h>

int main(int argc, char**argv){
    FILE* f = popen("nm -D a.out","r");
    char buff[256] = {0};
    while(!feof(f)){
        if(fgets(buff,255,f)>0) printf("%s",buff);
    }
    pclose(f);
}