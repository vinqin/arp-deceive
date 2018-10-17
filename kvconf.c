#include "mylib.h"

/* 删除szInput中左边的空格，并将结果复制到szOutput中 */
char *l_trim(char *szOutput, const char *szInput) {
    assert(szInput != NULL);
    assert(szOutput != NULL);
    while (*szInput != '\0' && isspace(*szInput)) {
        ++szInput;
    }

    return strcpy(szOutput, szInput);
}

/* 删除szInput中右边的空格，并将结果复制到szOutput中 */
char *r_trim(char *szOutput, const char *szInput) {
    assert(szInput != NULL);
    assert(szOutput != NULL);

    strcpy(szOutput, szInput);
    char *p = NULL;
    p = szOutput + strlen(szOutput) - 1;
    while (p >= szOutput && isspace(*p)) {
        --p;
    }
    *(++p) = '\0';
    return szOutput;
}


/* 删除szInput中左右两边的空格，并将结果复制到szOutput中 */
char *a_trim(char *szOutput, const char *szInput) {
    assert(szInput != NULL);
    assert(szOutput != NULL);

    l_trim(szOutput, szInput);
    r_trim(szOutput, szOutput);

    return szOutput;
}

/*
 *  获取配置文件中的字符串
 *  profile: 配置文件的路径（相对路径）
 *  appName: 节点的名字
 *  keyName: 键的名字
 *  keyVal:  值的存储区
 *
 */
int getProfileString(const char *profile, const char *appName, const char *keyName, char *keyVal) {
    int exitStatus = -1;
    char appname[KEYVALLEN], keyname[KEYVALLEN];
    char bufInput[KEYVALLEN], bufOutput[KEYVALLEN];
    char *buf, *c;
    int found = 0; /* 1 appName 2 keyName */
    FILE *fp;

    if ((fp = fopen(profile, "r")) == NULL) {
        printf("Error open file '%s'\n%s\n", profile, strerror(errno));
        exitStatus = -1;
        exit(exitStatus);
    }
    fseek(fp, 0, SEEK_SET); // 将文件指针置为文件最开始处
    memset(appname, 0, sizeof(appname));
    sprintf(appname, "[%s]", appName);

    while (!feof(fp) && fgets(bufInput, KEYVALLEN, fp) != NULL) {
        a_trim(bufOutput, bufInput);
        if (strlen(bufOutput) <= 0 || bufOutput[0] == '#') {
            continue;
        }
        buf = bufOutput;

        if (0 == found) {
            if (buf[0] != '[') {
                continue;
            } else if (!strncmp(buf, appname, strlen(appname))) {
                found = 1; // 找到指定节点
                continue;
            }
        } else if (1 == found) {
            if ('[' == buf[0]) {
                break; // 检测到了下一个节点，故直接退出
            }
            c = (char *) strchr(buf, '='); // 第一次出现'=' 的位置
            if (NULL == c) {
                continue;
            }
            memset(keyname, 0, sizeof(keyname));
            sscanf(buf, "%[^=|^ |^\t]", keyname); // 将buf按照指定格式输出到keyname

            if (!strcmp(keyname, keyName)) {
                sscanf(++c, "%[^\n]", keyVal);
                char *keyValOutput = (char *) malloc(strlen(keyVal) + 1);
                if (keyValOutput != NULL) {
                    memset(keyValOutput, 0, sizeof(keyValOutput));
                    a_trim(keyValOutput, keyVal);

                    if (keyValOutput && strlen(keyValOutput) > 0) {
                        strcpy(keyVal, keyValOutput);
                    }
                }
                free(keyValOutput);
                keyValOutput = NULL;
                found = 2;
                break;
            } else {
                continue;
            }
        }

    }

    fclose(fp);
    if (2 == found) {
        return 0;
    }

    return exitStatus;
}


