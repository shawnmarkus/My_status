#include "battery.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define POWER_SUPPLY_DIR    "/sys/class/power_supply"
#define BATTERY_NAME_PATTERN    "BAT"
#define BATTERY_CAPACITY_FILE_NAME  "/capacity"
#define BATTERY_STATUS_FILE_NAME    "/status"

void get_battery_name(char *battery_name) {

    DIR *dirp;
    struct dirent *dir;

    if((dirp = opendir(POWER_SUPPLY_DIR)) == NULL) {
        perror("opendir() failed!");
        exit(1);
    }

    while(1) {

        if((dir = readdir(dirp)) == NULL) {
            if(errno) {
                perror("readdir() failed!");
                exit(1);
            } else {
                break;
            }
        } else {
            if(dir->d_name[0] == '.') {
                continue;
            } else {
                if(!(strncmp(dir->d_name, BATTERY_NAME_PATTERN, strlen(BATTERY_NAME_PATTERN)))) {
                    strcpy(battery_name, dir->d_name);
                    break;
                }
            }
        }
    }

    if((closedir(dirp)) == -1) {
        perror("closedir() failed!");
        exit(1);
    }
}

short get_battery_capacity(char *battery_name) {

    FILE *fp;
    char battery_path[PATH_MAX] = POWER_SUPPLY_DIR;
    short capacity;

    strcat(battery_path, "/");
    strcat(battery_path, battery_name);
    strcat(battery_path, BATTERY_CAPACITY_FILE_NAME);

    if((fp = fopen(battery_path, "r")) == NULL) {
        perror("fopen() error!");
        exit(1);
    }

    fscanf(fp, "%hd", &capacity);

    if((fclose(fp)) == EOF) {
        perror("fclose() error!");
        exit(1);
    }

    return capacity;
}

void get_battery_status(char *battery_name, char *battery_status) {

    char battery_path[PATH_MAX] = POWER_SUPPLY_DIR;
    FILE *fp;

    strcat(battery_path, "/");
    strcat(battery_path, battery_name);
    strcat(battery_path, BATTERY_STATUS_FILE_NAME);

    if((fp = fopen(battery_path, "r")) == NULL) {
        perror("fopen() error!");
        exit(1);
    }

    fscanf(fp, "%s", battery_status);

    if((fclose(fp)) == EOF) {
        perror("fclose() error!");
        exit(1);
    }
}
