#ifndef ARP_DECEIVE_MYLIB_H
#define ARP_DECEIVE_MYLIB_H
#endif //ARP_DECEIVE_MYLIB_H
#pragma once // 防止重复导入

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <libnet.h> // 构造数据包的库

#define MAC_ADDR_LEN 6 // MAC地址长度
#define IP_ADDR_LEN 4 // IP地址长度
#define KEYVALLEN 256

extern char *dev; // 网络接口名称（即网卡名）
extern unsigned char targetIP[4]; // 目标IP地址
extern unsigned char targetMAC[6]; // 目标MAC地址
extern unsigned char broadcastMAC[6]; // 以太网MAC广播地址
extern int speed; // 数据包发送速度
extern int times; // 一次发送多少个数据包

char *l_trim(char *, const char *); /* 删除左边的空格 */
char *r_trim(char *, const char *); /* 删除右边的空格 */
char *a_trim(char *, const char *); /* 删除两边的空格 */

/*
 *  获取配置文件中的字符串
 *  profile: 配置文件的路径（相对路径）
 *  appName: 节点的名字
 *  keyName: 键的名字
 *  keyVal:  值的存储区
 *
 */
extern int getProfileString(const char *profile, const char *appName, const char *keyName, char *keyVal);

extern void init(const char *, const char *); /* 初始化参数 */

extern int
gratuitousArpReply(unsigned char *, unsigned char *, unsigned char *, unsigned char *, uint16_t, unsigned int);
