# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sebin/RP/pico/pico-sdk/tools/pioasm"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pioasm"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sebin/RP/pico/workspace/DACE0.0/test/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
