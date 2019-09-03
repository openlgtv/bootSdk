cmake_minimum_required(VERSION 3.13)
project(bootSdk)


function(add_binary)
	cmake_parse_arguments(BIN "" "TARGET;LDSCRIPT;LOADADDR;ENTRYPOINT" "SOURCES" ${ARGN})
	
	get_filename_component(BIN_LDSCRIPT "${BIN_LDSCRIPT}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

	# prepend directory basename to target
	get_filename_component(dirname "${CMAKE_CURRENT_LIST_DIR}" NAME)
	set(BIN_TARGET "${dirname}_${BIN_TARGET}")

	set(deps "")
	set(sources "")
	foreach(src ${BIN_SOURCES})
		# convert to absolute
		get_filename_component(src "${src}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

		## FIXME: use CMake 3.14 LAST_EXT/NAME_WLE (when CMake 3.14 is widely available)
		
		get_filename_component(ext "${src}" EXT)
		
		# LAST_EXT
		string(REGEX MATCH  "\.([a-zA-Z0-9]+)$" lastExt "${ext}")

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
			set_property(SOURCE "${output}" PROPERTY LANGUAGE C)
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

	set(cflags -fno-exceptions -nostartfiles -nostdlib -ffreestanding -fPIC -Os)
	set(ldflags ${cflags} -T"${BIN_LDSCRIPT}")
	
	string (REPLACE ";" " " cflags_str "${cflags}")
	string (REPLACE ";" " " ldflags_str "${ldflags}")
	
	set_target_properties(${BIN_TARGET} PROPERTIES COMPILE_FLAGS "${cflags_str}")
	set_target_properties(${BIN_TARGET} PROPERTIES LINK_FLAGS "${ldflags_str}")
	set_target_properties(${BIN_TARGET} PROPERTIES LINK_DEPENDS ${BIN_LDSCRIPT})

	# objcopy target: creates .bin file
	get_filename_component(outputBin "${BIN_TARGET}.bin" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	add_custom_command(
		OUTPUT "${outputBin}"
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${BIN_TARGET}> "${outputBin}"
	)
	add_custom_target(${BIN_TARGET}.bin ALL DEPENDS "${outputBin}")
	add_dependencies(${BIN_TARGET}.bin ${BIN_TARGET})

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
				-d ${outputBin} ${outputImage})
	
	add_custom_target(${BIN_TARGET}.uimg ALL DEPENDS "${outputImage}")
	add_dependencies(${BIN_TARGET}.uimg ${BIN_TARGET}.bin)
endfunction()

add_subdirectory(lxboot)