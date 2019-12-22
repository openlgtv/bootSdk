
set(target_definitions "")
set(compile_opts "")
set(include_dirs "")

set(sources "preloader.c")

if("${TARGET}" STREQUAL "bcm63138")
	set(CONFIG_TARGET_BCM63138 ON)

	if(NOT DEFINED SHARED_DIR)
		message(FATAL_ERROR "SHARED_DIR is not defined.")
	endif()

	set(INC_SHARED_PUB_PATH ${SHARED_DIR}/opensource/include/bcm963xx)

	list(APPEND include_dirs "${INC_SHARED_PUB_PATH}")
	list(APPEND compile_opts "-mcpu=cortex-a7")
endif()

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_binary(
	PIC
	TARGET preloader
	LDSCRIPT preloader.lds
	SOURCES ${sources}
	OUT_TARGET target
)

target_include_directories(${target} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${include_dirs}
)

target_compile_definitions(${target} PUBLIC ${target_definitions})
target_compile_options(${target} PUBLIC ${compile_opts})