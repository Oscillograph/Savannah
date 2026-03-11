cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(compile_flags -Wall -Werror -Wfatal-errors -DSAVANNAH_LOGGER_CLASS)

savannah_add_target(
	basic-opengl
	EXE
	${SAVANNAH_EXAMPLES_DIR}
	${SAVANNAH_EXAMPLES_DIR}/basic-opengl/source
	${SAVANNAH_BASE_SOURCE_DIR}/release
	".."
	-Wall -Werror -Wfatal-errors -DSAVANNAH_LOGGER_CLASS
	)

savannah_add_target(
	implot-test
	EXE
	${SAVANNAH_EXAMPLES_DIR}
	${SAVANNAH_EXAMPLES_DIR}/implot-test/source
	${SAVANNAH_BASE_SOURCE_DIR}/release
	".."
	-Wall -Werror -Wfatal-errors -DSAVANNAH_LOGGER_CLASS
	)

savannah_add_target(
	proforientator
	EXE
	${SAVANNAH_EXAMPLES_DIR}
	${SAVANNAH_EXAMPLES_DIR}/proforientator/src
	${SAVANNAH_BASE_SOURCE_DIR}/release
	".."
	${compile_flags}
	)