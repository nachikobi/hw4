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
int distance[N];

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
  fclose(fp);
}


int name_to_num() {
  int num=-1, id;
  char name[10], nickname[10];

  printf("name: ");
  scanf("%s", name);

  FILE *fp;
  if ((fp=fopen("nicknames.txt","r"))==NULL) {
    fprintf(stderr, "nicknames.txtのオープンに失敗しました\n");
    exit(2);
  }
  while (fscanf(fp, "%d	%s", &id, nickname)!=EOF) {
    if (strcmp(name, nickname)==0) {
      num = id;
      break;
    }
  }
  fclose(fp);
  if (num==-1) {
    fprintf(stderr, "no find %s\n", name);
    exit(2);
  } else return num;
}

void num_to_name(int num) {
  int id;
  char name[10], nickname[10];
  FILE *fp;
  if ((fp=fopen("nicknames.txt","r"))==NULL) {
    fprintf(stderr, "nicknames.txtのオープンに失敗しました\n");
    exit(2);
  }
  while (fscanf(fp, "%d	%s", &id, nickname)!=EOF) {
    if (id==num) {
      strcpy(name, nickname);
      break;
    }
  }
  fclose(fp);
  printf("%s ", name);
  return;
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
    if (ffrelation[from][i]==1 && distance[i]==-1) {
      distance[i]=depth+1;
      QUEUEenqueue(i);
    }
  }
  int num = QUEUEdequeue();
  if (num==-1) return;
  path_search(num, distance[num]);
}

void farthest_node_search(int from) {
  int max = distance[0];
  char name[10];

  /* 最長距離探索 */
  for (int i = 0; i < N; i++) {
    if (distance[i]>max) {
      max=distance[i];
    }
  }

  /* 出力 */
  num_to_name(from);
  printf("から最も遠いノードは");
  for (int i = 0; i < N; i++) {
    if (distance[i]==max) {
      num_to_name(i);
    }
  }
  printf("で、%dステップかかります\n", max);
}

int main() {
  int from=0;
  /* 配列distanceを-1に初期化 */
  for (int i = 0; i < N; i++) {
    distance[i]=-1;
  }
  distance[from]=0;

  relation_check(); //ff関係を2次元配列に格納
  from = name_to_num(); //名前を入力&名前を番号に変換
  path_search(from, 0);  //幅優先探索
  farthest_node_search(from); //最も遠い頂点を探し、出力
  return 0;
}
