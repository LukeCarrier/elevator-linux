/*
 * Elevator
 * Copyright (c) 2010 - 2011 Luke Carrier
 * Licensing information available at
 *   http://github.com/cloudflux/elevator
 */

/* Required header files */
#include <stdio.h>
#include <sys/stat.h>
#include <sysexits.h>
#include <unistd.h>
#include "elevator.h"

/**
 * Elevator.
 *
 * <h2>
 * Usage
 * </h2>
 *
 * <p>
 * elevator <executable> [options]
 * </p>
 *
 * <p>
 * Run <executable> with the supplied list of [options]. The full path to the
 * executable must be specified.
 * </p>
 *
 * @param string executable
 *        The full path to the executable we're calling. Elevator does not
 *        search the PATH environment variable for this executable, so be sure
 *        to use only fully-qualified paths.
 * @param string options
 *        A list of options, relevant only to the called executable. Elevator
 *        itself does not have any run-time parameters.
 */
int main(int argc, char **argv) {

    /* Check we have enough arguments
     *   We require at least 2 arguments: the elevator binary and the target
     *   binary. Any number greater than 2 is also okay, because these are
     *   passed to the target binary as arguments. */
    if (argc < 2) {

        #ifdef DEBUG
            perror("Too few arguments supplied; aborting\n");
        #endif
        return EX_USAGE;

    }

    /* Check the calling user's credentials
     *   Abort if they don't match those specified at configure time; probably
     *   some script kiddie. */
    if (getuid() != allow_uid || getgid() != allow_gid) {

        #ifdef DEBUG
            perror("Failed uid check; aborting\n");
        #endif
        return EX_NOPERM;

    }

    /* Set uid to that of the target user
     *   Abort if this fails, as we obviously cannot execute the binary with
     *   the desired privileges. */
    if (setuid(target_uid) != 0) {

        #ifdef DEBUG
            perror("setuid() failed; aborting\n");
        #endif
        return EX_TEMPFAIL;

    }

    /* Set gid to that of the target user's group
     *   Again, we handle errors from setgid() that prevent us from adopting
     *   the necessary privileges. */
    if (setgid(target_gid) != 0) {

        #ifdef DEBUG
            perror("setgid() failed; aborting\n");
        #endif
        return EX_TEMPFAIL;

    }

    /* Gather information about the target binary
     *   Use stat() to gather information about the binary from the disk, since
     *   we carry out basic sanity checks on permissions. */
    struct stat stat_data;
    int result = stat(argv[1], &stat_data);

    /* Check if an error occurred
     *   stat() returns negative values on error, in the name of inconsistency
     *   and confusion. */
    if (result < 0) {

        #ifdef DEBUG
            perror("stat() failed; aborting\n");
        #endif
        return EX_NOINPUT;

    }

    /* Check if the type is a regular file
     *   Error if it is not a regular file and we can't use it. This usually
     *   means it's likely to be a directory, symbolic link or other oddity. */
    if (!S_ISREG(stat_data.st_mode)) {

        #ifdef DEBUG
            perror("Not a regular file; aborting\n");
        #endif
        return EX_DATAERR;

    }

    /* Check we have some form of executable rights
     *   We must have execute rights either through owner, group or world
     *   ownership. */
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
        return EX_NOPERM;

    }

    /* Increment the argument vector
     *   We don't need the path to the elevator executable any more. */
    argv++;

    /* Execute the executable */
    execv(argv[0], argv);

    /* Something went drastically wrong
     *   execv() will only return if any error has occurred, as it otherwise
     *   replaces the executable in memory, thus taking control of its process.
     *   We really need the caller to acknowledge this, so we should return an
     *   evil exit status. */
    #ifdef DEBUG
        perror("Execution failed\n");
    #endif
    return EX_OSERR;

}
