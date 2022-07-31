#ifndef  LIBRA_CORE_H
#define  LIBRA_CORE_H
//#include <string.h>
//#include <unistd.h>

#define l_strlen(S) strlen((char *)S)
#define l_strncpy(S1,S2,L) strncpy((char *)S1,(char *)S2,L)
#define l_open(S,B) open((const char *)S,B)

#endif
