# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sebin/RP/pico/pico-sdk/tools/elf2uf2"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/tmp"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/src/ELF2UF2Build-stamp"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/src"
  "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sebin/RP/pico/workspace/DACE0.0/test/build/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
