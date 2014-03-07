/*
 * =========================================================================
 *
 *       Filename:  isearch.cpp
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

#include "isearch.h"


/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  isearch(std::string usHead)
 *  Description:  The construction dunction of isearch
 * =========================================================================
*/
isearch::isearch(std::string usHead)
{
    flags = false;
    productNum = 0;
    errorNum = 0;
    send_buf_size = MAX_BUFFER;
    recv_buf_size = MAX_WEBPAGE;
    uRequest = usHead;

    //create a log directory to storage the log information
    if (!mkdir(LOG, 0775)) {
        write_to_log("Create log directory failed, it can cause download failed");
    }
    buildReqHead();
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  buildReqHead()
 *  Description:  init the request head for connect
 * =========================================================================
 */
int isearch::buildReqHead()
{
    hostName = "search.suning.com";

    httpHead += "GET /emall/search.do?keyword=";
    httpHead += uRequest;
    httpHead += " HTTP/1.1\r\nHost: ";
    httpHead += hostName;
    httpHead += "\r\nAccept:*/*\r\nConnection:keep-alive\r\nUser-Agent:*/*\r\n\r\n";

    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  connectToHost()
 *  Description:  init the http head and connect to host
 * =========================================================================
 */
int isearch::connectToHost()
{
    int res = 0;

    short port;
    int flag = 1;
    struct in_addr ip;
    struct hostent *hpoint;
    struct servent *spoint;
    struct sockaddr_in addr;

    hpoint = gethostbyname(hostName.c_str());
    if (hpoint) {
        ip = *(struct in_addr*)hpoint->h_addr;
        //more information reference linux /etc/services
        spoint = getservbyname("http", "tcp");
        if (spoint) {
            port = spoint->s_port;
        } else {
            write_to_log("%s refused service",hostName.c_str());
            errorNum = 1;
            return 1;
        }
    } else {
        write_to_log("%s not found", hostName.c_str());
        errorNum = 2;
        return 1;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //change the system buffer size and set no delay
//    res = setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
//    res |= setsockopt( sockfd, SOL_SOCKET, SO_SNDBUF,
//                       (char *)&send_buf_size, sizeof(send_buf_size) );
//    res |= setsockopt( sockfd, SOL_SOCKET, SO_RCVBUF,
//                       (char *)&recv_buf_size, sizeof(recv_buf_size) );
    if(sockfd == -1 || res) {
        write_to_log("Create socket desscription failed.");
        errorNum = 3;
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr = ip;

    res = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        write_to_log("Connect host:<%s> failed.", hostName.c_str());
        errorNum = 4;
        return 1;
    }

    return 0;
}

bool isearch::isConnSucc() const
{
    if (errorNum == 0 || sockfd >= 0)
        return true;
    else
        return false;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  sendRequestToHost()
 *  Description:  send request head to the host
 * =========================================================================
 */
void isearch::sendRequestToHost()
{
    int count = 0;
    int nsend = 0;

    //we allow three times to try to send request if failed
    while (count < 3) {
          nsend = write(sockfd, httpHead.c_str(), httpHead.size());
          if (nsend > 0)
               break;
          count++;
    }
    if (count > 3) {
        write_to_log("Connect server failed!.");
        return;
    }
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  receiveMsgFromHost()
 *  Description:  open the sockfd and read charaters into buffer
 * =========================================================================
 */
int isearch::receiveMsgFromHost()
{
    int nread = 0;
    char buffer[MAX_WEBPAGE] = {'\0'};

    if (!isConnSucc())
        return 1;
    sleep(1);
    nread = read(sockfd, buffer, MAX_WEBPAGE);
#ifdef DEBUG
    std::cout << "Read main page size: " << nread << std::endl;
#endif
    if (nread > 0)
        parseMsgFromHost(buffer);
    flags = true;

    return 0;
}

/*
 * We get a web page link like this.
 * <a href="http://www.baidu.com/gaoji/preferences.html"
 * name="tj_setting">
 * We found that the link contained by a "a" label, then there is
 * a "href" attribute. And next simble is "=" and '"', this link ends
 * with a '"'.
 *
 * speacial characters table, they represent different means.
 * 1. + the space in URL %2B	2. space space of URL or %20
 * 3. / parent /child dir %2F	4. ? URL?parameters %3F
 * 5. % speacial character %25	6. # bookmark %23
 * 7. & URL&paramters %26	8. = paramter=value %3D
 * 9. ! URL values %21
 *
 * Ok, we sholu have a look at the web escape characters.
 * ^ %5E	` %60	{ %7B	} %7D	| %7C	] %5D
 * [ %5B	" %22	< %3C	> %3E	\ %5
 *
 * In this table, we can ensure that this characters must not appear in
 * our URL string.
 *
 * How can we judge the string is link?
 * 1. The links must contained by "a" label.
 * 2. The links are the values of "href" attributes.
 * 3. Begin with '"' and end with '"'.
 */
/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  parseMsgFromHost(char * buff)
 *  Description:  parse the buffer, and get usefull information
 * =========================================================================
 */
int isearch::parseMsgFromHost(char * buff)
{
    char *point = buff;
    int result  = 0;
    int flags = 1;

    //the main search key is "<div class="proLists" id="proShow">"
    //but we use "<div class="proLists"" to reduce the compare time
    //use a fine-grained string can enhance efficiency
    std::string mainSearchKey = "<div class=\"proListTile\"";
    std::string notFoundKey = "<i class=\"thirdBg\">";
    std::string secondKey("<li class");
    std::string endKey = "</ul>";

    if (point == NULL)
        return 1;
    //after we analysis the webpage, we found that front 2k bytes aren't
    // useless. we jump about 2k to improve the efficent.
    point += 2048;

    while (*point && flags) {
        if (*point == '<') {
            if ((result = strncmp(point, notFoundKey.c_str(),  notFoundKey.size())) == 0) {
                //Not dound the products
                productNum = 0;
                goto END;
            }
            if ((result = strncmp(point, "<div", 4)) == 0) {
                point += 4;
                JUMP_SPACE(point);
                mainSearchKey = "class=\"proList";
                if ((result = strncmp(point, mainSearchKey.c_str(),  mainSearchKey.size())) == 0) {
                    point += mainSearchKey.size();
                    JUMP_SPACE(point);
                    while (*point) {
                        if ((result = strncmp(point, secondKey.c_str(), secondKey.size())) == 0) {
                            point += secondKey.size();
                            JUMP_SPACE(point);
                            searchKeywords(&point, 0);   //product description
                            sprintf(fileName, "propic%d", productNum + 1);
                            searchKeywords(&point, 1);   //image link
//                            sprintf(fileName, "propage%d", productNum + 1);
//                            searchKeywords(&point, 2);   //man page link
                            sprintf(fileName, "propri%d", productNum + 1);
                            searchKeywords(&point, 3);   //price image link
                            searchKeywords(&point, 4);   //product status
                            searchKeywords(&point, 5);   //product comments
                            FILE *fp = fopen(PROFILE, "a");
                            if (fp) {
                                fputc('\n', fp);
                                fclose(fp);
                            }
                            productNum++;
                        }
                        if ((result = strncmp(point, endKey.c_str(),  endKey.size())) == 0) {
                            flags = 0;
                            break;
                        }
                        point++;
                    }
                }
            }
        }
        point++;
    }
END:
    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  searchKeywords(char **point, int value)
 *  Description:  search the keywords in the buffer
 * =========================================================================
 */
int isearch::searchKeywords(char **point, int value)
{
    int flags = 1;
    int result = 0;
    std::string url;
    std::string fkey;
    std::string skey;

    char *lp, *rp;

    lp = rp = NULL;
    if (value == 0) {
        fkey = "<a";
        skey = "title";
    } else if (value == 1) {
        fkey = "<img";
        skey = "src";
    } else if (value == 2) {
        fkey = "<a";
        skey = "href";
    } else if (value == 3) {
        fkey = "<img";
        skey = "src2";
    } else if (value == 4) {
        fkey = "<i class=\"\">";
        skey = "";
    } else if (value == 5) {
        fkey = "<i>";
        skey = "";
    }

    do {
        if ((result = strncmp(*point, fkey.c_str(), fkey.size())) == 0) {
            *point += fkey.size();
            lp = *point + 1;
            JUMP_SPACE(*point);
            //deal like this:<i class=\"\">南京有货</i>
            if (skey.empty()) {
                lp -= 1;
                while (*((*point)++) != '<');
                rp = *point - 1;
                url.append(lp, rp - lp);
                if (value == 1)
                    downloadInfo(url, true);
                downloadInfo(url, false);
                (*point)++;
                break;
            }
            while (1) {
                if ((result = strncmp(*point, skey.c_str(), skey.size())) == 0) {
                    *point += skey.size();
                    JUMP_SPACE(*point);
                    if (**point == '2')
                        (*point)++;
                    if ((*point)[0] == '=') {
                        (*point)++;
                        JUMP_SPACE(*point);
                        if (**point == '"') {
                            lp = *point = *point + 1;
                            while (*((*point)++) != '"');
                            rp = *point - 1;
                            url.append(lp, rp - lp);
                            if (value <= 3)
                                downloadInfo(url, value);
                            else
                                downloadInfo(url, false);
                            flags = 0;
                            break;
                        }
                    }
                }
                (*point)++;
            }
        }
        (*point)++;
    } while ((**point) && flags);
    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  downloadInfo(std::string url, int isHttpLink)
 *  Description:  download the information base on url
 * =========================================================================
 */
int isearch::downloadInfo(std::string url, int isHttpLink)
{
    FILE *fp;
    int nread = 0;
    int nwrite = 0;

    int httpsize = 0;
    int offsethead = 0;

    std::string rhead;
    std::string filename;
    std::string hostname;

    if (isHttpLink) {
        char httphead[1024];
        unsigned char ubuffer[MAX_WEBPAGE] = {'\0'};
        //we ensure that url is a llink, then check it is
        //a image or a html page
        std::string head = "HEAD /";
        int offsetlen = strlen("http://");
        int offset = url.find('/', offsetlen);

        rhead += (url.c_str() + offset + 1);
        hostname.append(url.c_str() + offsetlen, offset - offsetlen);
        head += rhead;
        head += " HTTP/1.1\r\nHost: ";
        head += hostname;
        head += "\r\nAccept:*/*\r\nConnection:keep-alive\r\nUser-Agent:*/*\r\n\r\n";

        //get the http head size
        httpsize = connectForDownload(hostname, head, httphead);
        //if it is a image, then get the image size
        if (isHttpLink & 1)
            offsethead = getContentSize(httphead);

        //change the HEAD method to GET method and download this page
        memset(ubuffer, '\0', sizeof(ubuffer));
        head.replace(0, 4, "GET");
        nread = connectForDownload(hostname, head, ubuffer);
        //build the file name
        offset = url.find_last_of('.');
        filename.append("./log/");
        filename.append(fileName);
        filename += (url.c_str() + offset);

        fp = fopen(filename.c_str(), "w");
        if (fp && nread > offsethead) {
            if (isHttpLink & 1)
                httpsize = nread - offsethead;  //image--httpsize characters are useless
            else
                httpsize += 20;                 //html--plus the "\r\n\r\n008000\r\n"
            nwrite = fwrite(ubuffer + httpsize, nread - httpsize, 1, fp);
            fclose(fp);
        } else {
            write_to_log("Download file: %s failed.", filename.c_str());
        }
    } else {
        fp = fopen(PROFILE, "a");
        if (fp) {
            url.append("\t");
            nwrite = fwrite(url.c_str(), url.size(), 1, fp);
            fclose(fp);
        } else {
            write_to_log("Write: %s  to product.info failed.", url.c_str());
        }
    }

    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  getContentSize(char *buffer)
 *  Description:  get the content size of the webpage
 * =========================================================================
 */
int isearch::getContentSize(char *buffer)
{
    char *lp;
    char *rp;
    char size[10] = {'\0'};
    char *point = buffer;
    char *str = "Content-Length";

    while (*point) {
        if ((strncmp(point, str, strlen(str))) == 0) {
            point += strlen(str);
            point += 2;
            lp = point;
            rp = strchr(lp, '\n');
            strncpy(size, lp, rp - lp);
            return atoi(size);
        }
        point++;
    }
    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  connectForDownload(std::string hostname, std::string head, 
 *					TYPE *buffer)
 *  Description:  base on the hostname,head and download webpage into buffer
 * =========================================================================
 */
template <class TYPE>
int isearch::connectForDownload(std::string hostname, std::string head, TYPE *buffer)
{
    int fd;
    int res = 0;
    int flag = 1;
    int nread = 0;
    int nwrite = 0;

    struct hostent *hostinfo;
    struct sockaddr_in addr;
    struct servent *servinfo;

    fd = socket(AF_INET, SOCK_STREAM, 0);
//    res = setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
//    res |= setsockopt( fd, SOL_SOCKET, SO_SNDBUF,
//                       (char *)&send_buf_size, sizeof(send_buf_size) );
//    res |= setsockopt( fd, SOL_SOCKET, SO_RCVBUF,
//                       (char *)&recv_buf_size, sizeof(recv_buf_size) );
    hostinfo = gethostbyname(hostname.c_str());
    servinfo = getservbyname("http", "tcp");
    if (fd != -1 && hostinfo && servinfo && !res) {
        addr.sin_family = hostinfo->h_addrtype;
        addr.sin_port = servinfo->s_port;
        memcpy(&addr.sin_addr.s_addr, (struct in_addr *)hostinfo->h_addr_list[0],
                sizeof(struct in_addr));
        res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
        if (res == 0) {
            nwrite = write(fd, head.c_str(), head.size());
            if (nwrite > 0) {
                if (head.compare(0, 3, "GET") == 0)
                    sleep(1);
                nread = read(fd, buffer, MAX_WEBPAGE);
#ifdef DEBUG
                std::cout << "Download file size: " << nread << std::endl;
#endif
                return nread;
            }
        }
    } else {
        write_to_log("download file failed.");
        errorNum = 6;
        return 1;
    }

    return 0;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  set_noblocking(int *sockfd)
 *  Description:  set read and write no blocking
 * =========================================================================
 */
int isearch::set_noblocking(int *sockfd)
{
    int opts = fcntl(*sockfd, F_GETFL);

    if (opts < 0)
    {
        return 1;
    }

    // set nonblocking
    opts |= O_NONBLOCK;

    if (fcntl(*sockfd, F_SETFL, opts) < 0)
    {
        return 1;
    }
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  timeOut(int fd)
 *  Description:  Checking it is timeout
 * =========================================================================
 */
int isearch::timeOut(int fd)
{
    int res = 0;
    fd_set read_set;
    struct timeval timeout;

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    res = select(1, &read_set, NULL, NULL, &timeout);

    return res;
}

int isearch::getProductNum()
{
    return productNum;
}

void isearch::closeConnection()
{
    close(sockfd);
}

int isearch::isErrorOccur() const
{
    return errorNum;
}

bool isearch::searchFinished() const
{
    if (flags)
        return true;
}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  write_to_log(char *fmt, ...)
 *  Description:  write the log file to logfile
 * =========================================================================
 */
void isearch::write_to_log(char *fmt, ...)
{
    va_list args;
    struct tm *tm;

    time_t curr_time;
    char timebuf[128];
    char buffer[1024];
    FILE *log_fp;

    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);
    va_end(args);

    time(&curr_time);
    tm = localtime(&curr_time);
    sprintf(timebuf, "%04d/%02d/%02d %02d:%02d:%02d ",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour,
        tm->tm_min, tm->tm_sec);
    log_fp = fopen(LOGFILE,  "a");
#ifdef DEBUG
    perror("Log file: ");
#endif
    if (log_fp) {
        fwrite(timebuf, strlen(timebuf), 1, log_fp);
        fwrite(buffer, strlen(buffer), 1, log_fp);
        fputc('\n', log_fp);
        fclose(log_fp);
    }
}
