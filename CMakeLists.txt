cmake_minimum_required(VERSION 3.13)
project(bootSdk)

macro(handle_asm_file file)
	set_property(SOURCE "${file}" PROPERTY LANGUAGE C)
	set_property(SOURCE "${file}" PROPERTY COMPILE_DEFINITIONS __ASSEMBLY__)
endmacro()

function(add_binary)
	cmake_parse_arguments(BIN "PIC" "TARGET;LDSCRIPT;LOADADDR;ENTRYPOINT;OUT_TARGET" "SOURCES" ${ARGN})
	
	if(BIN_LDSCRIPT)
		get_filename_component(BIN_LDSCRIPT "${BIN_LDSCRIPT}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()

	# prepend directory basename to target
	get_filename_component(dirname "${CMAKE_CURRENT_LIST_DIR}" NAME)
	set(BIN_TARGET "${dirname}_${BIN_TARGET}")

	if(BIN_OUT_TARGET)
		set(${BIN_OUT_TARGET} "${BIN_TARGET}" PARENT_SCOPE)
	endif()

	set(deps "")
	set(sources "")
	foreach(src ${BIN_SOURCES})
		# convert to absolute
		get_filename_component(src "${src}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

		## FIXME: use CMake 3.14 LAST_EXT/NAME_WLE (when CMake 3.14 is widely available)
		
		get_filename_component(ext "${src}" EXT)
		
		# LAST_EXT
		string(REGEX MATCH  "\.([a-zA-Z0-9]+)$" lastExt "${ext}")

		if(lastExt STREQUAL ".S")
			handle_asm_file("${src}")
		endif()

		# Skip non-m4 files
		if(NOT lastExt STREQUAL ".m4")
			list(APPEND sources "${src}")
			continue()
		endif()

		# get relative path
		file(RELATIVE_PATH relPath "${CMAKE_CURRENT_SOURCE_DIR}" "${src}")

		get_filename_component(relDir "${relPath}" DIRECTORY)
		# NAME_WLE
		string(REGEX REPLACE "\.([a-zA-Z0-9]+)$" "" output "${relPath}") 

		set(output "${CMAKE_CURRENT_BINARY_DIR}/${output}")
		if(relDir)
			file(MAKE_DIRECTORY "${relDir}")
			# append reldir
			set(output "${output}/${relDir}")
		endif()
		
		message(STATUS ${src})
		add_custom_command(
			OUTPUT "${output}"
			COMMAND sh -c \"m4 -P ${src} > ${output}\"
			DEPENDS "${src}"
		)

		# .S files must be passed to gcc just like C files
		get_filename_component(output_ext "${output}" EXT)
		if(output_ext STREQUAL ".S")
			handle_asm_file("${output}")
		endif()

		get_filename_component(src_name "${src}" NAME)
		set(targetName ${dirname}_${src_name})
		add_custom_target(${targetName} DEPENDS ${output})

		list(APPEND deps "${targetName}")
		list(APPEND sources "${output}")
	endforeach()

	add_executable(${BIN_TARGET} ${sources})
	foreach(tdep ${deps})
		add_dependencies(${BIN_TARGET} ${tdep})
	endforeach()

	message(STATUS "${BIN_TARGET} ${sources}")

	set(cflags -fno-exceptions -nostartfiles -nostdlib -ffreestanding -Os -s -Wl,--build-id=none -fomit-frame-pointer)

	if(BIN_PIC)
		set(cflags "${cflags} -pie -fPIE")
	else()
		set(cflags "${cflags} -no-pie -fno-plt")
	endif()

	#set(cflags -fno-exceptions -nostartfiles -nostdlib -ffreestanding -Os -s -Wl,--build-id=none)
	set(ldflags ${cflags})

	if(BIN_LDSCRIPT)
		set(ldflags ${ldflags} -T"${BIN_LDSCRIPT}")
		set_target_properties(${BIN_TARGET} PROPERTIES LINK_DEPENDS ${BIN_LDSCRIPT})
	endif()
	
	string (REPLACE ";" " " cflags_str "${cflags}")
	string (REPLACE ";" " " ldflags_str "${ldflags}")

	set_target_properties(${BIN_TARGET} PROPERTIES COMPILE_FLAGS "${cflags_str}")
	set_target_properties(${BIN_TARGET} PROPERTIES LINK_FLAGS "${ldflags_str}")	
	target_include_directories(${BIN_TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

	# objcopy target: creates .bin file
	get_filename_component(outputBin "${BIN_TARGET}.bin" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	add_custom_command(
		OUTPUT "${outputBin}"
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${BIN_TARGET}> "${outputBin}"
		DEPENDS $<TARGET_FILE:${BIN_TARGET}>
	)
	add_custom_target(${BIN_TARGET}.bin ALL DEPENDS "${outputBin}")
	add_dependencies(${BIN_TARGET}.bin ${BIN_TARGET})

	if(BIN_LOADADDR AND BIN_ENTRYPOINT)
		# mkimage target: creates .uimg file
		get_filename_component(outputImage "${BIN_TARGET}.uimg" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
		add_custom_command(
			OUTPUT "${outputImage}"
			COMMAND mkimage
					-A arm
					-T standalone
					-O u-boot
					-C none
					-n "${BIN_TARGET}"
					-a ${BIN_LOADADDR}
					-e ${BIN_ENTRYPOINT}
					-d ${outputBin} ${outputImage}
			DEPENDS ${outputBin}
		)
		
		add_custom_target(${BIN_TARGET}.uimg ALL DEPENDS "${outputImage}")
		add_dependencies(${BIN_TARGET}.uimg ${BIN_TARGET}.bin)
	endif()
endfunction()

add_subdirectory(lxboot)
add_subdirectory(uboot-mstar)
add_subdirectory(preloader)
