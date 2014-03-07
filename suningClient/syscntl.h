#ifndef SYSCNTL_H
#define SYSCNTL_H

/*
 *Define the configuration files and array size, we must obey this define.
 *If the this file you changed, there something exceptions will appear.
 *When you change this file you must be carefully.
*/

//The information log file.
#define LOGFILE "./system.log"
#define LOG "./log"
//The product information file
#define PROFILE "./log/products.info"

//switch of debugging
/*######################################################################*/
//#define DEBUG 1         //open debugger
/*######################################################################*/

/*
 *Define the URL length, but the length is not identified in the http
 *protocol, so we just define a effecient one.
 *
 * browser limits for the URL
 * ie	  length:2083b		firefox	   length:65536b
 * safari  length:80000b		opera	   length:190000b
 * chrome  length:8182b
 *
 * server limits for the URL
 * apache -> length:8192		IIS -> length:16384
 *Here we found that the server defined serval kb to accept the URL.
 *But actually, the user may not use all this bytes. So we can define
 *a small one to storge the URL. Of course, this is dangerous.
*/
#define URL_LEN 1024

/*
 *Define the max hosts to limit the resources, why we do this?
 *The problem is the servers' resource are limited. So we sholud
 *try our best to reduce the usage of resources. Here we define
 *the max hosts number is 5. You can change it!
*/
#define MAX_HOSTS 1

//#define the max htt head size
#define MAX_HTTP_HEAD 1024

//define the max domain length
#define MAX_DOMAIN_LENGTH 67

//define the max link number
#define MAX_LINKS 2048

//define the max web page buffer
#define MAX_BUFFER 4096

//define max webpage size
#define MAX_WEBPAGE 1024000

/*
 *All this definitions may affect the servers' deal abilities. If
 * you want your server get more faster, small them.
*/

#endif // SYSCNTL_H
