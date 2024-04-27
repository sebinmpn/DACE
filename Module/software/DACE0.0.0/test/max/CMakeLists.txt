add_library(max30102
	max30102.c
)

target_include_directories(max30102 PUBLIC
	.
)

target_link_libraries(max30102 pico_stdlib hardware_i2c)   