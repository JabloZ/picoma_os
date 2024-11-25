#ifndef JMP_H
#define JMP_H
void __attribute__((cdecl)) prepare_paging();
void __attribute__((cdecl)) change_stack();
void __attribute__((cdecl)) unmap();
#endif