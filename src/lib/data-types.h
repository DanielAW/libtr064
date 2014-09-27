#ifndef TR064_DATA_TYPES_H_
#define TR064_DATA_TYPES_H_

#include <curl/curl.h>

typedef struct {
    char *action_name;
    char *service_type;
} Service;

typedef struct {
    char *password;
    char *ip_addr;
    CURL *curl;
    Service services[];
} SessionHandle;

typedef struct {
    char *buffer;
    size_t size;
} XmlBuffer;

#endif /* TR064_DATA_TYPES_H_ */
