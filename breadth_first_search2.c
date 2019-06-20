/*
23 jacob → 6 carlos
幅優先探索で何ステップか調べる
全部の頂点を探索して、最も遠い頂点を調べる
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int N = 49;

int ffrelation[N][N];
int flag[N];

void relation_check() {
  int from, to;
  FILE *fp;
  if ((fp=fopen("links.txt","r"))==NULL) {
    fprintf(stderr, "links.txtのオープンに失敗しました\n");
    exit(2);
  }
  while (fscanf(fp, "%d %d", &from, &to)!=EOF) {
    ffrelation[from][to]=1;
  }
}


typedef struct QUEUEnode *link;
struct QUEUEnode { //幅優先探索で用いるキューの構造体
  int num;
  link next;
};

static link head;
static link tail;

void QUEUEerror() {
  fprintf(stderr, "queue error\n");
  printf("求めているパスはありません\n");
  exit(2);
}

link NEW(int num, link next) {
  link x = malloc(sizeof *x);
  if (x==NULL) QUEUEerror();
  x->num = num;
  x->next = next;
  return x;
}

void QUEUEenqueue(int num) {
  if (head==NULL) {
    head = (tail = NEW(num,head));
    return;
  }
  tail->next = NEW(num,tail->next);
  tail = tail->next;
}

int QUEUEdequeue() {
  if (head==NULL) return -1;
  int num = head->num;
  link t = head->next;
  free(head);
  head = t;
  return num;
}


void path_search(int from, int depth) {
  for (int i = 0; i < N; i++) {
    if (ffrelation[from][i]==1 && flag[i]==-1) {
      flag[i]=depth+1;
      QUEUEenqueue(i);
    }
  }
  int num = QUEUEdequeue();
  if (num==-1) return;
  path_search(num, flag[num]);
}

void farthest_node_search(int from) {
  int max = flag[0];
  int farthest_node[N];
  for (int i = 0; i < N; i++) {
    farthest_node[i]=-1;
  }
  int j=0;
  for (int i = 0; i < N; i++) {
    if (flag[i]>=max) {
      max=flag[i];
      farthest_node[j]=i;
      j++;
    }
  }
  printf("%dから最も遠いノードは", from);
  for (int i = 0; farthest_node[i]!=-1; i++) {
    printf("%d ", farthest_node[i]);
  }
  printf("で、%dステップかかります\n", max);
}

int main() {
  int from=23;
  /* 2次元配列ffrelationの初期化 */
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      ffrelation[i][j]=0;
    }
  }
  /* 配列flagの初期化 */
  for (int i = 0; i < N; i++) {
    flag[i]=-1;
  }
  flag[from]=0;

  relation_check(); //ff関係を2次元配列に格納
  path_search(from, 0);  //幅優先探索
  farthest_node_search(from); //最も遠い頂点を探す
  return 0;
}
