/*
 * Elevator
 * Copyright (c) 2010 - 2011 Luke Carrier
 * Licensing information available at
 *   http://labs.cloudflux.net/go/elevator/license
 */

#include <stdio.h>
#include <unistd.h>
#include "elevator.h"

#define E_USERCREDENTIALVALIDATIONFAILURE 1
#define E_TOOFEWARGUMENTSSUPPLIED         2

int main(int argc, char **argv) {

    if (  getuid()  != allow_uid
       || geteuid() != allow_uid
       || getgid()  != allow_gid
       || getegid() != allow_gid)
       
        printf("Failed user credential validation; aborting%s", "\n");
        return E_USERCREDENTIALVALIDATIONFAILURE;
    }

    if (argc < 2) {
        printf("Too few arguments supplied; aborting%s", "\n");
	return E_TOOFEWARGUMENTSSUPPLIED;
    }

    setuid(target_uid);
    setgid(target_gid);

    argv++;
    execv(argv[0], argv);

}
