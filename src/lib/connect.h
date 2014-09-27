#ifndef TR064_CONNECT_H_
#define TR064_CONNECT_H_

#include <curl/curl.h>
#include "data-types.h"

#define TR064_PORT "49000"
#define TR064_FILE "tr64desc.xml"


SessionHandle *tr064_init(const char *l_password, const char *l_ip_addr);

void tr064_cleanup(SessionHandle *handle);

char *tr064_request_description(SessionHandle *handle);

#endif /* TR064_CONNECT_H_ */

