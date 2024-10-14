#include <stdio.h>

void wypisz_binarna(unsigned int n) {
    if (n==0){
        printf("0");
        return 0;
    }
    int bez_zer_wiodacych=0;
    unsigned int maska=1<<31; //1 i 31 zer
    
    while (maska!=0){
       if (maska & n){
           printf("1");
           bez_zer_wiodacych=1;
       }
       else{
           if (bez_zer_wiodacych==1){
               printf("0");
           }
       }
       maska=maska>>1; //przesuniecie jedynki o jeden w prawo
    }
    return 0;
}

int main() {
    unsigned int liczba;
    scanf("%d",&liczba);
    wypisz_binarna(liczba);
    return 0;
}