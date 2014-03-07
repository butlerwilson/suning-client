/*
 * =========================================================================
 *
 *       Filename:  isearch.h
 *
 *    Description:  implement the fibonacci using recusion
 *
 *        Version:  1.0
 *        Created:  11/22/2013 05:30:27 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Youngcy (yangxin), youngcy.youngcy@gmail.com
 *   Organization:  Ocreator
 *
 * =========================================================================
 */

#ifndef NETPARSE_H
#define NETPARSE_H

 #include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <vector>

#include "syscntl.h"

#define JUMP_SPACE(point) {while (*point == ' ') {++point;}}

class isearch
{
private:
    bool flags;
    int sockfd;
    int errorNum;

    int productNum;
    int send_buf_size;
    int recv_buf_size;

    char fileName[20];
    std::string uRequest;
    std::string hostName;
    std::string httpHead;

public:
    isearch(std::string usHead);

    int timeOut(int fd);
    int buildReqHead();
    int connectToHost();
    void sendRequestToHost();
    int receiveMsgFromHost();
    void closeConnection();

    int set_noblocking(int *fd);
    int getContentSize(char *buffer);
    int parseMsgFromHost(char *buff);
    int searchKeywords(char **point, int value);
    int downloadInfo(std::string url, int isHttpLink);
    template <class TYPE>
    int connectForDownload(std::string hostname, std::string head, TYPE *buffer);

    int getProductNum();
    bool isConnSucc() const;
    int isErrorOccur() const;
    bool searchFinished() const;
    void write_to_log(char *fmt, ...);
};

#endif // NETPARSE_H
