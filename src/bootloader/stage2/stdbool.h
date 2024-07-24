#ifndef STDBOOL_H_
#define STDBOOL_H_


#ifndef _Bool
typedef unsigned char _Bool;
#endif // _Bool 

#ifndef __bool_true_false_are_defined
#define bool _Bool
#define false 0 
#define true 1
#define __bool_true_false_are_defined 1
#endif 

#endif // STDBOOL_H_