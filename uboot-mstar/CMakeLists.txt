add_binary(
	TARGET hello
	LDSCRIPT hello.lds
	LOADADDR 0x25000000
	ENTRYPOINT 0x25000000
	SOURCES
		hello.c
)
