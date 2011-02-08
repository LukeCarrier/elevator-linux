/*
 * Elevator
 * Copyright (c) 2010 - 2011 Luke Carrier
 * Licensing information available at
 *   http://labs.cloudflux.net/go/elevator/license
 */

/* Required header files */
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "elevator.h"

int main(int argc, char **argv) {

    /* Get some information about the executable file */
    struct stat stat_data;
    int result = stat(file_path, &stat_data);

    /* If an error occured in stat(), the file probably doesn't exist */
    if (result < 0) {

        #ifdef DEBUG
            perror("stat() failed; aborting\n");
        #endif
        return E_NONEXISTENTEXECUTABLE;

    }

    /* Ensure it's a file; not a directory, symbolic link or other oddity */
    if (!S_ISREG(stat_data.st_mode)) {

        #ifdef DEBUG
            perror("Not a regular file; aborting\n");
        #endif
        return E_INVALIDEXECUTABLE;

    }

    /* Do we have executable rights? */
    if ((stat_data.st_uid == getuid()) && (stat_data.st_mode & S_IXUSR)) {

        /* We own the binary and have execute rights */
        #ifdef DEBUG
            perror("Have owner execute rights; continuing\n");
        #endif

    } else if ((stat_data.st_gid == getgid()) && (stat_data.st_mode & S_IXGRP)) {

        /* We're a member of the binary's group and have execute rights */
        #ifdef DEBUG
            perror("Have group execute rights; continuing\n");
        #endif

    } else if (stat_data.st_mode & S_IXOTH) {

        /* The binary has the 'world' execute bit set */
        #ifdef DEBUG
            perror("Have world execute rights; continuing");
        #endif

    } else {

        #ifdef DEBUG
            perror("Don't have any execute rights; aborting\n");
        #endif
        return E_NOEXECUTERIGHTS;

    }

    if (getuid() != allow_uid || getgid()  != allow_gid) {

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
            perror("setuid() failed; aborting\n");
        #endif
        return E_SETUIDFAILED;

    }

    if(setgid(target_gid) != 0){

        #ifdef DEBUG
            perror("setgid() failed; aborting\n");
        #endif
        return E_SETGUIDFAILED;

    }

    /* Increment the argument vector to remove the executable */
    argv++;

    /* Execute the executable */
    execv(argv[0], argv);

    /* Something went drastically wrong if we got here */
    #ifdef DEBUG
        perror("Execution failed\n");
    #endif
    return E_DOOMED;

}
