#ifndef __UBOOT_H

#define GD_ADDR 0x3E5DFF70
#define STR(x) #x
#define XSTR(x) STR(x)

#define ENTRY_FUNC  __attribute__((__section__(".entry")))

/*#define DECL_FUNC(addr, name) \
	asm(".globl " #name "\n" \
		".section .text\n"	\
		#name":\n"\
		"ldr pc, ="XSTR(addr));

DECL_FUNC(0x3F6826EC, printf)
extern void printf(const char *fmt, ...);*/

#define GET_GD() ((gd_t *)GD_ADDR) 

#define NULL 0
#include <stdint.h>
#include <stdarg.h>

#define offsetof(st, m) ((size_t)&(((st *)0)->m))

#define CONFIG_NR_DRAM_BANKS 1

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned int size_t;
typedef uint32_t phys_size_t;

typedef struct bd_info {
    int			bi_baudrate;	/* serial console baudrate */
    unsigned long	bi_ip_addr;	/* IP Address */
    uint32_t	        bi_arch_number;	/* unique id for this board */
    uint32_t	        bi_boot_params;	/* where this board expects params */
    struct				/* RAM configuration */
    {
	uint32_t start;
	uint32_t size;
    }			bi_dram[CONFIG_NR_DRAM_BANKS];
} bd_t;

typedef	struct	global_data {
	bd_t		*bd;
	unsigned long	flags;
	unsigned long	baudrate;
	unsigned long	have_console;	/* serial_init() was called */
	unsigned long	env_addr;	/* Address  of Environment struct */
	unsigned long	env_valid;	/* Checksum of Environment valid? */
	unsigned long	fb_base;	/* base address of frame buffer */

	/* "static data" needed by most of timer.c on ARM platforms */
	unsigned long	timer_rate_hz;
	unsigned long	tbl;
	unsigned long	tbu;
	unsigned long long	timer_reset_value;
	unsigned long	lastinc;

	unsigned long	relocaddr;	/* Start address of U-Boot in RAM */
	phys_size_t	ram_size;	/* RAM size */
	unsigned long	mon_len;	/* monitor len */
	unsigned long	irq_sp;		/* irq stack pointer */
	unsigned long	start_addr_sp;	/* start_addr_stackpointer */
	unsigned long	reloc_off;

	unsigned long	tlb_addr;

	void		**jt;		/* jump table */
	char		env_buf[32];	/* buffer for getenv() before reloc. */
} gd_t;

typedef void (interrupt_handler_t)(void *);
/*-----------------------------------------------------------------------
 * Representation of a SPI slave, i.e. what we're communicating with.
 *
 * Drivers are expected to extend this with controller-specific data.
 *
 *   bus:	ID of the bus that the slave is attached to.
 *   cs:	ID of the chip select connected to the slave.
 */
struct spi_slave {
	unsigned int	bus;
	unsigned int	cs;
};


/*#define JT_OFFSET(xf) \
	((uintptr_t)GD_ADDR) + \
	offsetof(gd_t, jt) + \
	(sizeof(void *) * xf)*/

//#define JT_OFFSET(sym) (GET_GD()->jt[XF_##sym])

//DECL_FUNC(printf, JT_OFFSET(printf), void, const char *, ...);

#endif