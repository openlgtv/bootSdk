#ifndef __UBOOT_EXPORTS_H
#define __UBOOT_EXPORTS_H

#include "uboot.h"

enum {
#define EXPORT_FUNC(x) XF_ ## x ,
#include "_exports.h"
#undef EXPORT_FUNC
	XF_MAX
};

#define DECL_FUNC(addr, name, ret, ...) \
    ret(*name)(__VA_ARGS__) = ((void *)(addr))

#define EXPORT_FUNC(sym) \
	asm volatile (			\
"	.globl " #sym "\n"		\
"	.section .text\n"	\
#sym ":\n"				\
"	ldr	ip, =%0\n"		\
"   ldr ip, [ip]\n"		\
"	ldr	pc, [ip, %1]\n"		\
	: : "i"(GD_ADDR + offsetof(gd_t, jt)), "i"(XF_##sym * sizeof(void *)) : "ip");

void __attribute__((unused)) dummy(void)
{
	#include <_exports.h>
}

unsigned long get_version(void);
int  getc(void);
int  tstc(void);
void putc(const char);
void puts(const char*);
int printf(const char* fmt, ...);
void install_hdlr(int, interrupt_handler_t*, void*);
void free_hdlr(int);
void *malloc(size_t);
void free(void*);
void udelay(unsigned long);
unsigned long get_timer(unsigned long);
int vprintf(const char *, va_list);
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
int strict_strtoul(const char *cp, unsigned int base, unsigned long *res);
char *getenv (char *name);
int setenv (char *varname, char *varvalue);

long simple_strtol(const char *cp,char **endp,unsigned int base);
int strcmp(const char * cs,const char * ct);
int ustrtoul(const char *cp, char **endp, unsigned int base);

int i2c_write (uchar, uint, int , uchar* , int);
int i2c_read (uchar, uint, int , uchar* , int);

int spi_init(void);
struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode);

void spi_free_slave(struct spi_slave *slave);
int spi_claim_bus(struct spi_slave *slave);
void spi_release_bus(struct spi_slave *slave);
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
		void *din, unsigned long flags);

//DECL_FUNC(0x3F6826EC, printf, void, const char *, ...);

#endif