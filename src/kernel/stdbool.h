#ifndef STDBOOL_H_
#define STDBOOL_H_

#ifndef __cplusplus

// Definicja typu _Bool tylko wtedy, gdy nie jest już zdefiniowany
#ifndef _Bool
typedef unsigned char bool;
#endif // _Bool

// Definicje bool, true, false oraz makro __bool_true_false_are_defined
#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

#endif // __cplusplus

#endif // STDBOOL_H_
