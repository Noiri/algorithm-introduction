#include <stdio.h>
#include <stdlib.h>

#define t 4

typedef struct node{
    int n;
    int leaf;
    int keyValue[2*t+1];
    struct node* pChild[2*t+2];
} Node;

Node* Root;


Node* BtreeSearch(Node* x, int k, int* index){
    int i = 1;
    while(i <= x->n && k > x->keyValue[i]){
        i++;
    }
    if(i <= x->n && k == x->keyValue[i]){
        *index = i;
        return x;
    }
    else if(x->leaf == 1){
        return NULL;
    }
    else{
        return BtreeSearch(x->pChild[i], k, index);
    }
}



int Search(int k){
    printf("Target is %d.:", k);
    int KeyIndex = 0;
    Node* Pointer = BtreeSearch(Root, k, &KeyIndex);
    if(Pointer == NULL){
        printf("Key is not found\n");
        return 0;
    }
    else{
        printf("[");
        for(int i=1; i<=Pointer->n; i++){
            if(i == Pointer->n) printf("%d", Pointer->keyValue[i]);
            else printf("%d, ",Pointer->keyValue[i]);
        }
        printf("]\n");
    }
    return 1;
}


Node* makeNewNode(){
    Node* pNewNode;
    pNewNode = (Node*)malloc(sizeof(Node));
    if(pNewNode != NULL){
        //initialize structure's atribute
        pNewNode->n = 0;
        for(int i=0; i<2*t-1; i++) pNewNode->keyValue[i] = 0;
        for(int i=0; i<2*t; i++) pNewNode->pChild[i] = NULL;
        pNewNode->leaf = 1;
    }
    return pNewNode;
}


void splitChild(Node* x, int i, Node* y){
    Node* z = makeNewNode();
    y = x->pChild[i];
    z->leaf = y->leaf;
    z->n = t - 1;
    for(int j=1; j<=t-1; j++){
        z->keyValue[j] = y->keyValue[j+t];
    }
    if(y->leaf == 0){
        for(int j=1; j<=t; j++){
            z->pChild[j] = y->pChild[j+t];
        }
    }
    y->n = t - 1;

    for(int j=x->n+1; j>=i+1; j--){
        x->pChild[j+1] = x->pChild[j];
    }
    x->pChild[i+1] = z;
    for(int j=x->n; j>=i; j--){
        x->keyValue[j+1] = x->keyValue[j];
    }
    x->keyValue[i] = y->keyValue[t];
    x->n++;
    return;
}


void insertNoFull(Node* x, int k){
    int i = x->n;
    if(x->leaf == 1){
        while(i >= 1 && k < x->keyValue[i]){
            x->keyValue[i+1] = x->keyValue[i];
            i--;
        }
        x->keyValue[i+1] = k;
        x->n++;
    }
    else{
        while(i >= 1 && k < x->keyValue[i]){
            i--;
        }
        i++;
        if((x->pChild[i])->n == 2*t -1){
            splitChild(x, i, x->pChild[i]);
            if(k > x->keyValue[i]) i++;
        }
        insertNoFull(x->pChild[i], k);
    }
    return;
}

Node* Queue[100000];
int head = 0;
int tail = 0;
int size = 0;

int indexQue[100000];
int idxHead = 0;
int idxTail = 0;

void push(Node* p){
    Queue[tail] = p;
    tail++;
    size++;
    return;
}

Node* pop(){
    head++;
    size--;
    return Queue[head-1];
}

void PrintBtree(){
    push(Root);
    while(size != 0){
        printf("[");
        Node* CurrentNode = pop();
        for(int i=1; i<=CurrentNode->n; i++){
            if(i == CurrentNode->n) printf("%d", CurrentNode->keyValue[i]);
            else printf("%d, ", CurrentNode->keyValue[i]);
        }
        printf("]\n");

        if(CurrentNode->leaf == 0){
            for(int i=1; i<=CurrentNode->n+1; i++){
                push(CurrentNode->pChild[i]);
            }
        }
    }

    return;
}


void BtreeInsert(int k){
    Node* r = Root;
    if(r->n == 2*t-1){
        Node* s = makeNewNode();
        Root = s;
        s->leaf = 0;
        s->pChild[1] = r;
        splitChild(s, 1, r);
        insertNoFull(s, k);
    }
    else{
        insertNoFull(r, k);
    }
    return;
}



int main(void){
    Root = makeNewNode();

    int data;
    FILE* file;
    file = fopen("/Users/noiri/Desktop/data.txt", "r");

    while(fscanf(file, "%d", &data) != EOF){
        BtreeInsert(data);
    }
    fclose(file);

    Search(98);
    Search(35);

    printf("\nB-Tree\n");
    PrintBtree();

    return 0;
}

/*RESULT

Target is 98.:[90, 92, 93, 98]
Target is 35.:Key is not found

B-Tree(t=4)
[15, 36, 49, 64, 76, 88]
[2, 4, 5, 8, 10, 13, 14]
[17, 20, 23, 25, 29, 31, 32]
[38, 40, 46, 48]
[52, 53, 56, 60, 62]
[66, 72, 73]
[80, 82, 83, 86]
[90, 92, 93, 98]
*/