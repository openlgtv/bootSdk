#define NULL 0
typedef unsigned int size_t;

#define ENTRY_FUNC  __attribute__((__section__(".entry")))

#define MICOM_I2C_INVERTER 0x36
#define MICOM_I2C_PANEL    0x3A

typedef struct {
	char *cmdName;
	int (*cmdFunc)(int, char **);
	char *cmdDescr;
	char *cmdDescrArgs;
	char *defaultArg;
} LXCommand;

LXCommand *gCmdTableFixed = (LXCommand *)0x78154980;
unsigned int *gNumberOfCommands = (unsigned int *)0x7815F67C;
LXCommand *gCmdTableDynamic = (LXCommand *)0x7815F680;

int (*Micom_I2C_Write)(void *packet, unsigned int packetSize) = (void *)0x7810CB2C;
int (*printf)(const char *fmt, ...) = (void *)0x7812043C;
int (*strcmp)(const char *str1, const char *str2) = (void *)0x78135708;
void *(*malloc)(size_t size) = (void *)0x78120974;
//void (*malloc_align)()
void *(*dmalloc)(size_t size) = (void *)0x78120EF4;
void (*free)(void *pMem) = (void *)0x78120C38;
void *(*calloc)(size_t nmemb, size_t size) = (void *)0x781209FC;
void *(*realloc)(void *ptr, size_t size) = (void *)0x78120A54;
void (*dfree)(void *pMem) = (void *)0x78120F88;