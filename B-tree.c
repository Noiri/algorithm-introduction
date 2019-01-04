#include <stdio.h>
#include <stdlib.h>

#define t 4

//nodeの構造体
typedef struct node{
    int n;
    int key[2*t-1];
    struct node* c[2*t];
    struct node* p;
} Node;

//比較関数
int ascending_order(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

//配列の末尾に要素を追加
void push_back(Node* pNode, int num){
    int tail = pNode->n;
    pNode->key[tail] = num;
    pNode->n++;
    return;
}



int main(void){
}