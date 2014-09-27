#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "xml-parser.h"
#include "connect.h"


void build_url(char buf[], int buf_size, const char *ip_addr) {
    snprintf(buf, buf_size, "%s%s%s%s%s%s", "http://", ip_addr, ":", TR064_PORT, "/", TR064_FILE);
}

void tr064_cleanup(SessionHandle *handle) {

    /* libcurl cleanup */
    curl_easy_cleanup(handle->curl);

    free(handle->password);
    free(handle->ip_addr);
    free(handle);

}

size_t curl_data_callback(void *actual_buffer, size_t size, size_t nmemb, void *xml_buffer_p) {
    size_t realsize = size * nmemb;
    XmlBuffer *buf = (XmlBuffer *) xml_buffer_p;

    buf->buffer = realloc(buf->buffer, buf->size + realsize + 1);
    if(buf->buffer == NULL) {
        fprintf(stderr, "Error on allocating memory\n");
        return 0;
    }

    memcpy(&(buf->buffer[buf->size]), actual_buffer, realsize);
    buf->size += realsize;
    buf->buffer[buf->size] = 0;

    return realsize;
}

/**
 * Request tr64desc.xml file
 */
char *tr064_request_description(SessionHandle *handle) {
    CURLcode res;

    char url[256];
    build_url(url, sizeof(url), handle->ip_addr);

    XmlBuffer xml_buf;
    xml_buf.buffer = malloc(1);
    xml_buf.size = 0;
    
    /* specify URL */
    curl_easy_setopt(handle->curl, CURLOPT_URL, url);
    /* specify Port */
    curl_easy_setopt(handle->curl, CURLOPT_PORT, atol(TR064_PORT));
    /* specify callback function, default: STDOUT*/
    curl_easy_setopt(handle->curl, CURLOPT_WRITEFUNCTION, curl_data_callback);
    /* specify 4th argument of callback function */
    curl_easy_setopt(handle->curl, CURLOPT_WRITEDATA, (void *)&xml_buf);

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(handle->curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } else {
        return xml_buf.buffer;
    }
}

SessionHandle *tr064_init(const char *l_password, const char *l_ip_addr) {
    SessionHandle *handle = (SessionHandle *) malloc(sizeof(SessionHandle));

    /* init libcurl */
    handle->curl = curl_easy_init();

    handle->password = strdup(l_password);
    handle->ip_addr = strdup(l_ip_addr);

    const char *xml_data = tr064_request_description(handle);   
    //printf("RESULT: %s\n", xml_data);

    get_services(handle, xml_data);

    return handle;
}
