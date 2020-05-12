#include<stdio.h>
#include<string.h>

struct places {
    const char *lysing;
    const char *tagg;
}

stadir[2] = {
    {"the vast expanse", "expanse"},
    {"an empty dog house", "dog house"}
};

#define fjoldiStada (sizeof(stadir) / sizeof(*stadir))

static unsigned whereIsMe = 0;

void goLook(const char *noun){
    if (noun != NULL && strcmp(noun, "around") == 0) {
        printf("You be at %s\n", stadir[whereIsMe].lysing);
    } else {
        printf("You are lost and confused.\n");
    }
}

void goGo(const char *noun){
    unsigned i;
    for (i = 0; i <  fjoldiStada; i++) {
        if (noun != NULL && strcmp(noun, stadir[i].tagg) == 0) {
            if (i == whereIsMe) {
                printf("useless, this is where you are now.\n");
            } else {
                printf("alright.\n");
                whereIsMe = i;
                goLook("around");
            }
            return;
        }
    }
    printf("there is mystery in your requests.\n");
}
