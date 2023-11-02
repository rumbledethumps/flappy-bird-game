cmake_minimum_required(VERSION 3.13)

project(MY-RP6502-PROJECT C ASM)

add_subdirectory(rp6502-sdk)

add_executable(hello)
rp6502_asset(hello 0x10000 assets/bin/midflap_spr16.bin)
rp6502_asset(hello 0x11000 assets/bin/bg_day_144x256.pal.bin)
rp6502_asset(hello 0x12000 assets/bin/bg_day_144x256.dat.bin)
rp6502_executable(hello 0x200 0x200
    midflap_spr16.rp6502
    bg_day_144x256.pal.rp6502
    bg_day_144x256.dat.rp6502
)
target_sources(hello PRIVATE
    src/hello.c
)
target_link_libraries(hello PRIVATE
    rp6502
)
