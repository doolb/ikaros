#define _GNU_SOURCE
#include <stdio.h>


#if _WIN32
#include <Windows.h>
#define API __declspec(dllexport)
#else
#include <link.h>
#include <dlfcn.h>
#define API __attribute__((visibility("default")))
#endif


#if __x86_64__
#define ptr unsigned long long
#else
#define ptr unsigned int
#endif

API int main(int argc, char **argv)
{
#if _WIN32
    void *m = GetProcAddress(NULL,"main");
    printf("module %x\n", (unsigned int)m);
#else
    void *m = dlopen(NULL, RTLD_NOW);
    printf("module %llx\n", (ptr)m);

    void *f = dlsym(NULL, "main");
    printf("func main: %llx\n", (ptr)f);

    // query dlinfo
    struct link_map* info = NULL;
    if( dlinfo(m,RTLD_DI_LINKMAP,&info) == 0){
        while(info != NULL){
            printf("link map: base %llx ptr %llx  file %s\n",(ptr)info->l_addr, (ptr)info->l_ld, info->l_name);
            info = info->l_next;
        }
    }
#endif

    return 0;
}