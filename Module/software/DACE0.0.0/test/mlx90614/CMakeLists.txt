add_library(mlx90614
	mlx90614.c
)

target_include_directories(mlx90614 PUBLIC
	.
)

target_link_libraries(mlx90614 pico_stdlib hardware_i2c)   