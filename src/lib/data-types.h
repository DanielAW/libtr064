#ifndef TR064_DATA_TYPES_H_
#define TR064_DATA_TYPES_H_

#include <curl/curl.h>

struct Action {
    char *action_name;
    struct Action *next;
};

struct Service {
    char *service_type;
    char *control_url;
    char *scpd_url;
    struct Action *action_list;
    struct Service *next;
};

struct SessionHandle{
    char *password;
    char *ip_addr;
    CURL *curl;
    struct Service *service_list;
};

struct XmlBuffer{
    char *buffer;
    size_t size;
};

typedef struct Action Action;
typedef struct Service Service;
typedef struct SessionHandle SessionHandle;
typedef struct XmlBuffer XmlBuffer;

#endif /* TR064_DATA_TYPES_H_ */
