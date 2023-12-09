/*
 * Tutorial code to demonstrace some common methods of anti-debugger. 
 * The code is initially designed for the GDB Camp in 2023 by Raymnd Zhang
 * Please access https://github.com/gedulab/ or https://nanocode.cn for more details. 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>

static bool g_no_debugger = false;
static jmp_buf env;

// static inline bool is_debugger_present() __attribute__((always_inline));

static void trap_handler(int sig)
{
    printf("antidbg: received signal %d: ", sig);
    if(sig == SIGSEGV)
    {
       printf("SEGMENT FAULT\n");
    }
    else if(sig == SIGINT)
       printf("INTERRUPTED, by CTRL +C?\n");
    else if(sig == SIGTRAP) {
       printf("got debugger trap\n");
       g_no_debugger = true;
       longjmp(env, 1);
    }
    else
       printf("Not translated signal\n");
}

static inline bool is_debugger_present()
{
    signal(SIGTRAP, trap_handler);
    int rp = setjmp(env);
    if(rp == 0) {
#ifdef __amd64__    
    asm ("int 3");
#endif
#ifdef __aarch64__
    asm ("brk 8"); // 0xf000
#endif
    }

    return !g_no_debugger;     
}

static int do_senisitive_business(){
    printf("Uncovering top secrets!!!\n"
    "If you know it, you might be killed...\n");

    return 0;
}
inline int die()
{
    // disappear by accessing null ptr
    return *(int*)(long)(0x888); 
}
int main(int argc, const char* argv[])
{
    int ret = 0;
    printf("This program contains top sensitive infomation which can't be seen by anyone.\n");
    if(is_debugger_present()) {
        ret = die();
    } else {
	ret = do_senisitive_business();
    }

    return ret;
}
