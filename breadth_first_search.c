/*
23 jacob → 6 carlos
幅優先探索で何ステップか調べる
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
  if (head==NULL) QUEUEerror();
  int num = head->num;
  link t = head->next;
  free(head);
  head = t;
  return num;
}


void path_search(int from, int to, int depth) {
  for (int i = 0; i < N; i++) {
    if (ffrelation[from][i]==1 && flag[i]==-1) {
      flag[i]=depth+1;
      if (i==to) return;
      QUEUEenqueue(i);
    }
  }
  int num = QUEUEdequeue();
  path_search(num, to, flag[num]);
}

int main() {
  int from=23, to=6;
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
  path_search(from, to, 0);  //幅優先探索
  printf("%dから%dへは%dステップです\n", from, to, flag[to]);
  return 0;
}
