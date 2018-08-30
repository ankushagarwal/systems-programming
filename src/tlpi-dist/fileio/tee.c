/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Solution to Exercise 4-1 */

/* tee.c

   Copy stdin to argv[1] and stdout
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int
main(int argc, char *argv[])
{
    int inputFd, outputFd, outputFileFd,  openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    /* Open input and output files */

    inputFd = STDIN_FILENO;
    outputFd = STDOUT_FILENO;

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFileFd = open(argv[1], openFlags, filePerms);
    if (outputFileFd == -1)
        errExit("opening file %s", argv[2]);

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead)
            errExit("couldn't write whole buffer to STDOUT");
        if (write(outputFileFd, buf, numRead) != numRead)
            errExit("couldn't write whole buffer to %s", argv[1]);
    }
    if (numRead == -1)
        errExit("read");

    if (close(outputFileFd) == -1)
        errExit("close output file");

    exit(EXIT_SUCCESS);
}
