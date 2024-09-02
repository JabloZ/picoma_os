#ifndef STDBOOL_H_
#define STDBOOL_H_

#ifndef __cplusplus

#ifndef _Bool
typedef unsigned char bool;
#endif // _Bool


#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

#endif 

#endif 
