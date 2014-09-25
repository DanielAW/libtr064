#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

#include "../lib/connect.h"

#ifdef __cplusplus
}
#endif

int main(void) {

  SessionHandle *tr064;
 
  //set password and ip address
  tr064 = tr064_init("abc", "192.168.178.1");

  if(tr064) {
    tr064_request_description(tr064);

    /* always cleanup */ 
    tr064_cleanup(tr064);
  }
  return 0;
}
