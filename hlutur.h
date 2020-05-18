typedef struct hlutur {
    const char *lysing;
    const char *tagg;
    struct hlutur *stadur;
} HLUTUR;

extern HLUTUR hlutir[];

#define field (hlutir + 0)
#define cave (hlutir + 1)
#define silver (hlutir + 2)
#define gold (hlutir + 3)
#define guard (hlutir + 4)
#define player (hlutir + 5)

#define ekkiFleiriHlutir (hlutir + 6)
