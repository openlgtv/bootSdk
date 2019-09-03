add_binary(
	TARGET hello
	LDSCRIPT hello.lds
	LOADADDR 0x2000000
	ENTRYPOINT 0x2000000
	SOURCES
		hello.c
)
