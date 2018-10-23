cmake_minimum_required(VERSION 3.5)
#cmake_minimum_required(VERSION 3.12)
project(arp_deceive C)

set(CMAKE_C_STANDARD 99)

add_executable(arp_deceive main.c mylib.h arp.c kvconf.c)

# 指定头文件 libnet.h 在当前文件系统中的位置
INCLUDE_DIRECTORIES(BEFORE /usr/include/)
# 指定动态链接库 libnet.so 在当前文件系统中的位置
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)
SET(Libnet_LIBRARIES libnet.so)
TARGET_LINK_LIBRARIES(arp_deceive ${Libnet_LIBRARIES})
