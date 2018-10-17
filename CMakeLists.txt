cmake_minimum_required(VERSION 3.12)
project(arp_deceive C)

set(CMAKE_C_STANDARD 99)

add_executable(arp_deceive main.c mylib.h arp.c kvconf.c)

INCLUDE_DIRECTORIES(BEFORE /usr/include/)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)
SET(Libnet_LIBRARIES libnet.so)
TARGET_LINK_LIBRARIES(arp_deceive ${Libnet_LIBRARIES})
