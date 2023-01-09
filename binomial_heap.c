
#include <stdio.h>
#include <stdlib.h>
typedef struct node* node;
struct node
{
int v; /* The v to store */
int rank; /* Number of childen */
node child; /* First child if it has any */
node brother; /* In heap list points to larger rank.
Inside a tree points to smaller rank. */
node father; /* Points up */
};


node A;

int ptr2loc(node v, node A)
{
int r;

r = -1;
if(NULL != v)
r = ((size_t) v - (size_t) A) / sizeof(struct node);
return (int)r;
}

/*Prints one node*/
void showNode(node v)
{
if(NULL == v)
printf("NULL\n");
else {
printf("node: %d ", ptr2loc(v, A));
printf("v: %d ", v->v);
printf("rank: %d ", v->rank);
printf("child: %d ", ptr2loc(v->child, A));
printf("brother: %d ", ptr2loc(v->brother, A));
printf("father: %d ", ptr2loc(v->father, A));
printf("\n");
}
}


/*Prints all nodes*/
void showList( node v )
{
    showNode(v);
    node s = v->brother;
    while(s != NULL){
    showNode(s);
    s = s->brother;
}
}

/*Changes v field of the node if node is heap by itself*/
void Set(node v, int val){
    if(v->brother== NULL && v->child == NULL && v->father == NULL ){

    v->v = val;
    }
}

/*Linking function for the nodes*/
void link(node h1, node h2){

    h2->brother = h1->child;
    h2->father = h1;
    h1->rank = h1->rank+1;
    h1->child = h2;
    printf("link %d as child of %d\n", ptr2loc(h2, A), ptr2loc(h1, A));
}


node mergeall(node h1, node h2){

node new;
node next;
node eka = h1;
node toka = h2;

if(eka != NULL){
    if(toka != NULL && eka->rank <= toka->rank){
        new = eka;
        eka = eka->brother;
    }
    else if(toka != NULL && eka->rank > toka->rank){
        new = toka;
        toka = toka->brother;
    }

    else{
        new = eka;
        
    }
}
else{
    new = toka;

}
next = new;
while(eka != NULL && toka != NULL){

if(eka->rank <= toka->rank){
    next->brother = eka;
    eka = eka->brother;
}

else{
    next->brother = toka;
    toka = toka->brother;
}
next = next->brother;
}

if(eka != NULL){
    next->brother = eka;
}

if(toka != NULL){
    next->brother = toka;
}

return(new);
}

/*Join two binomial heaps*/
int Unite(node h1, node h2){

node new = mergeall(h1,h2);
node cur = new;
node tmp = NULL;
node next = new->brother;


while(next != NULL){
    if(  (cur->rank != next->rank ) || (next->brother != NULL && next->brother->rank == cur->rank)){
        tmp = cur;
        cur = next;
    }
    else{
        if(next->v >= cur->v){
            cur->brother = next->brother;
            link(cur, next);
        }
        else{
            if(tmp != NULL){
                tmp->brother =next;
            }
            else{
                new = next;
            }
            link(next, cur);
            cur = next;
        }
    }

    next = cur->brother;

}

return ptr2loc(new, A);

}

/*Returns minimum value that exists in the binomial heap h*/
int Min(node h){
    int min = h->v;
    node n = h->brother;
    while(n != NULL ){
        if(n->v<min){
            min = n->v;
        }

        n=n->brother;

    }
    /*printf("mini2: %d",min);*/
    return min;
}

/*Reurns node that contains the minimum value*/
int ArgMin(node h){
    int min = h->v;
    node m = h;
    node n = h->brother;
    while(n != NULL ){
        if(n->v<min){
            min = n->v;
            m = n;
        }

        n= n->brother;

    }

    return (ptr2loc(m, A));
}

/*Deletes node from given heap and returns the resulting heap*/
void del_node(node n){
   /* if(n->father != NULL){
        n->father->child = NULL;
    }*/
    n->child = NULL;
    n->father = NULL;
    n->brother = NULL;
    n->v = 0;
    n->rank = 0;
    n  = NULL;

}

/*Removes node that contains the minimum value and returns the resulting heap*/
int ExtractMin(node h){
    int mini = h->v;
    node min = h;
    node prev = NULL;
    node n = h;
    while(n->brother != NULL ){
        if(n->brother->v<mini){
            prev = n;
            mini = n->brother->v;
            min = n->brother;
        }

        n= n->brother;

    }
    int ret;
    node h1;
    node h2;

    void reverse(node child){
        child->father = NULL;
        if(child->brother == NULL){
            h2 = child;
        }
        else{
            reverse(child->brother);
            child->brother->brother = child;
        }
    }
    
    if(prev != NULL){
        if(min->brother == NULL && min->child == NULL){
       
            prev->brother = NULL;
            del_node(min);
            return(ptr2loc(h, A));

        }
        else if(min->brother == NULL){
            prev->brother = NULL;
            reverse(min->child);
            min->child->brother = NULL;
            del_node(min);
            ret = Unite(h,h2);
            return(ret);
        }
        else if(min->child==NULL){
             prev->brother = min->brother;
             return (ptr2loc(h, A));
        }
        else{
   
            prev->brother = min->brother;
            h1 = min->brother;
            reverse(min->child);
            min->child->brother = NULL;
            del_node(min);
            ret = Unite(h,h2);
            return (ret);
            }
    }
    else{
        if(min->brother == NULL && min->child == NULL){
       
            del_node(min);
            return(ptr2loc(min, A));
        }
        else if(min->brother == NULL){
            reverse(min->child);
            min->child->brother = NULL;
            del_node(min);
            return (ptr2loc(h2, A));
            
        }
        else if(min->child==NULL){
     
            h1 = min->brother;
            del_node(min);
            return (ptr2loc(h1, A));
        }
    
        else{
            h1 = min->brother;
            reverse(min->child);
            min->child->brother = NULL;
            del_node(min);
            ret = Unite(h1,h2);
            return (ret);
    
    }
    }
}

/*Decreases v field of node*/
int DecreaseKey(node h, int v){
    node dad;
    node n = h;
    int tmp;
    if(v<n->v){
        dad = n->father;
        n->v = v;
        while(dad != NULL && n->v < dad->v){
            tmp = n->v;
            n->v = dad->v;
            dad ->v = tmp;
            n = dad;
            dad = dad->father;
        }

    return (ptr2loc(n,A));

    }
    else{
       return (ptr2loc(h,A));

    }


}

int Delete(node h, node n){
int sin = Min(h);
DecreaseKey(n, sin-1);
/*printf("d: %d", dec);*/
int ret = ExtractMin(h);

return(ret);

}







int main(){
int u;
int n;
int n2 = 0;
char in = 's';
scanf("%d\n", &n);
A = (node)calloc(n, sizeof(struct node));
int z = n;
while(in != 'X'){
    scanf("%c", &in);
    if(in == 'S'){
        scanf("%d\n", &n);
        showNode(&A[n]);
    }
    if(in == 'P'){
        scanf("%d\n", &n);
        showList(&A[n]);
    }
    if(in == 'V'){
        scanf("%d %d\n", &n, &n2);
        Set(&A[n], n2);
    }
    if(in == 'U'){
        scanf("%d %d\n", &n, &n2);
        u = Unite(&A[n], &A[n2]);
        printf("%d\n", u);
    }
    if(in == 'R'){
        scanf("%d %d\n", &n, &n2);
        u = DecreaseKey(&A[n],n2);
        printf("%d\n", u);
    }
    if(in == 'M'){
        scanf("%d\n", &n);
        u = Min(&A[n]);
        printf("%d\n", u);
    }
    if(in == 'A'){
        scanf("%d\n", &n);
         u = ArgMin(&A[n]);
          printf("%d\n", u);
    }
    if(in == 'E'){
        scanf("%d\n", &n);
         u =ExtractMin(&A[n]);
          printf("%d\n", u);
    }
    if(in == 'D'){
        scanf("%d %d\n", &n, &n2);
        u = Delete(&A[n], &A[n2]);
         printf("%d\n", u);
    }
    
    }
printf("Final configuration:\n");
int i = 0;
while(i<z){
    showNode(&A[i]);
    i+= 1;
}

free(A);
return(0);
}
