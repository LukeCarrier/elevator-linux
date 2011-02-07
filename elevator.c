/*
 * Elevator
 * Copyright (c) 2010 - 2011 Luke Carrier
 * Licensing information available at
 *   http://labs.cloudflux.net/go/elevator/license
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "elevator.h"

int is_executable(const char *file_path) {
    struct stat stat_data;
    int result = stat(file_path, &stat_data);

    if (result < 0) {
        #ifdef DEBUG
            perror("Could not stat file\n");
        #endif
        return 0;
    }

    if (!S_ISREG(stat_data.st_mode)) {
        #ifdef DEBUG
            perror("Not a regular file\n");
        #endif
        return 0;
    }

    if (stat_data.st_uid == geteuid()) {
            if(stat_data.st_mode & S_IXUSR){
                return 1;
            }

            #ifdef DEBUG
                perror("Owner but have no execute permissions\n");
            #endif
            return 0;
    }

    if (stat_data.st_gid == getegid()){
            if(stat_data.st_mode & S_IXGRP){
                return 1;        
            }

            #ifdef DEBUG
                perror("Group but no execute permission\n");
            #endif
            return 0;
    }

    if(stat_data.st_mode & S_IXOTH) {
        return 1;
    }

    #ifdef DEBUG
        perror("No global execute permissions\n");
    #endif
    return 0;
}



int main(int argc, char **argv) {

    if (getuid()  != allow_uid || getgid()  != allow_gid) {
        #ifdef DEBUG
            perror("Failed uid check; aborting\n");
        #endif
        return E_USERCREDENTIALVALIDATIONFAILURE;
    }

    if (argc < 2) {
        #ifdef DEBUG
            perror("Too few arguments supplied; aborting\n");
        #endif
        return E_TOOFEWARGUMENTSSUPPLIED;
    }

    if(setuid(target_uid) != 0) {
        #ifdef DEBUG
            perror("SetUID failed\n");
        #endif
        return E_SETUIDFAILED;
    }
    
    if(setgid(target_gid) != 0){
        #ifdef DEBUG
            perror("SetGID failed\n");
        #endif
        return E_SETGUIDFAILED;
    }

    /* Increment argv to remove this binary from the call */
    argv++;

    /* Check this after setuid in cause permissions dictate that the initial user cannot see it */
    if(!is_executable(argv[0])) {
        printf("%s\n", argv[0]);
        #ifdef DEBUG
            perror("File requested is not executable\n");
        #endif
        return E_INVALIDEXECUTABLE;
    }

    execv(argv[0], argv);
    #ifdef DEBUG
        perror("Execution failed\n");
    #endif
    return E_EXECUTIONERROR;
}
