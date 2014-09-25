#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "connect.h"

SessionHandle *tr064_init(const char *l_password, const char *l_ip_addr) {
    SessionHandle *handle = (SessionHandle *) malloc(sizeof(SessionHandle));

    // init libcurl
    handle->curl = curl_easy_init();

    handle->password = strdup(l_password);
    handle->ip_addr = strdup(l_ip_addr);

    return handle;

}

void build_url(char buf[], int buf_size, const char *ip_addr) {
    snprintf(buf, buf_size, "%s%s%s%s%s%s", "http://", ip_addr, ":", TR064_PORT, "/", TR064_FILE);
}

void tr064_cleanup(SessionHandle *handle) {

    // libcurl cleanup
    curl_easy_cleanup(handle->curl);

    free(handle->password);
    free(handle->ip_addr);
    free(handle);

}

/*
 * Request tr64desc.xml file
 */
void tr064_request_description(SessionHandle *handle) {
    CURLcode res;

    char url[256];
    build_url(url, sizeof url, handle->ip_addr);
    printf("Gebaute URL: '%s'", url);

    curl_easy_setopt(handle->curl, CURLOPT_URL, url);
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle->curl, CURLOPT_PORT, atoll(TR064_PORT));

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(handle->curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

}

//int main(void) {
//
//  SessionHandle *tr064;
// 
//  //set password and ip address
//  tr064 = tr064_init("abc", "192.168.178.1");
//
//  if(tr064) {
//    tr064_request_description(tr064);
//
//    /* always cleanup */ 
//    tr064_cleanup(tr064);
//  }
//  return 0;
//}
