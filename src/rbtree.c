#include "rbtree.h"

#include <stdlib.h>

#include <stdio.h>  // test

rbtree *new_rbtree(void) {
  // printf("init start\n");
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode->color = RBTREE_BLACK;

  p->nil = nilNode;
  p->root = nilNode;
  // printf("init end\n");
  return p;
}

void left_rotate(rbtree *t, node_t *x){
  // printf("left rotate start\n");
  node_t *y;

  y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else{
    if (x == x->parent->left){  // 이 부분은 left와 right가 같다.
      x->parent->left = y;
    }
    else{
      x->parent->right = y;
    }
  } 

  y->left = x;
  x->parent = y;
  // printf("left rotate end\n");
}

void right_rotate(rbtree *t, node_t *x){
  // printf("right rotate start\n");
  node_t *y;

  y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else{
    if (x == x->parent->left){  // 이 부분은 left와 right가 같다.
      x->parent->left = y;
    }
    else{
      x->parent->right = y;
    }
  } 

  y->right = x;
  x->parent = y;
  // printf("right rotate end\n");
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  // printf("fixup start\n");
  node_t *y;

  while(z->parent->color == RBTREE_RED){

    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }

      else{
        if (z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else{
      y = z->parent->parent->left;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }

      else{
        if (z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
  // printf("fixup end\n");
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // printf("insert start\n");
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  node_t *y = t->nil;
  node_t *x = t->root;

  while(x != t->nil){
    y = x;
    if (z->key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }

  z->parent = y;
  if (y == t->nil){
    t->root = z;
  }
  else{
    if(z->key < y->key){
      y->left = z;
    }
    else{
      y->right = z;
    }
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rbtree_insert_fixup(t, z);

  // printf("insert end\n");
  return t->root;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil){
    t->root = v;
  }
  else{
    if (u == u->parent->left){
      u->parent->left = v;
    }
    else{
      u->parent->right = v;
    }
  }
  v->parent = u->parent;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  if (x == t->nil){
    return t->nil;
  }

  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  if (x == t->nil){
    return t->nil;
  }

  while (x->right != t->nil){
    x = x->right;
  }
  return x;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK){
    if (x == x->parent->left){
      w = x->parent->right;
      
      // x의 형제 w가 RED인 경우
      if (w->color == RBTREE_RED){  
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;  // 형제/부모 색 바꾸기
        left_rotate(t, x->parent);  // 바꾸면 회전
        w = x->parent->right;
      }
      
      // x의 형제 w가 BLACK이고 w의 두 자식이 모두 BLACK인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }  // 형제의 자식이 전부 블랙이 아닐 때까지 올라간다.

      // x의 형제 w가 BLACK이고 w의 왼쪽이 RED, 오른쪽이 BLACK일 때
      else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;  // 부모/자식 색 바꾸기
          right_rotate(t, w);  // 바꾸면 회전
          w = x->parent->right;
        }
        // x의 형제 w가 BLACK이고 w의 오른쪽이 RED일 때
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;  
        left_rotate(t, x->parent);  // 바꾸면 회전
        x = t->root;
      }
    }
    else{
      w = x->parent->left;
      // x의 형제 w가 RED인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // x의 형제 w가 BLACK이고 w의 두 자식이 모두 BLACK인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // x의 형제 w가 BLACK이고 w의 오른쪽이 RED, 왼쪽이 BLACK일 때
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // x의 형제 w가 BLACK이고 w의 왼쪽이 RED일 때
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *x, *y;
  color_t original;
  y = z;  // y는 삭제할 노드
  original = y->color;

  if (z->left == t->nil){  // 자식이 둘 다 없거나 왼쪽이 없을 때
    x = z->right;  // 지워지는 노드의 오른쪽
    rbtree_transplant(t, z, z->right);
  }
  else if (z->right == t->nil){  // 오른쪽 자식이 없을 때
      x = z->left;  // 지워지는 노드의 왼쪽
      rbtree_transplant(t, z, z->left);
  }
  else{  // 둘 다 자식이 있을 때
    y = z->right;
    while (y->left != t->nil){
      y = y->left;
    }
    original = y->color;
    x = y->right;  // z 오른쪽 서브트리의 가장 작은 수의 오른쪽(y 다음으로 큰 수)
    if (y->parent == z){
      x->parent = y;
    }
    else{
      rbtree_transplant(t, y, y->right);  // y->right->parent = y->parent
      y->right = z->right;
      y->right->parent = y;
    }
  }
  rbtree_transplant(t, z, y);  // y->parent = z->parent
  y->left = z->left; // y와 z를 하나로 만든다
  y->left->parent = y;
  y->color = z->color;
  
  if (original == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);  // x는 이동하는 값의 자식
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

// test
int main(){
  printf("test start\n\n");
  
  //insert
  rbtree *t = new_rbtree();
  node_t *i1 = rbtree_insert(t, 1024);
  node_t *i2 = rbtree_insert(t, 512);
  node_t *i3 = rbtree_insert(t, 256); 
  node_t *i4 = rbtree_insert(t, 128);
  node_t *i5 = rbtree_insert(t, 64);
  node_t *i6 = rbtree_insert(t, 32);
  node_t *i7 = rbtree_insert(t, 16);
  node_t *i8 = rbtree_insert(t, 8);
  
  // check tree
  printf("root의 값은 %d \n\n", t->root->key);

  printf("root->left의 값은 %d \n", t->root->left->key);
  printf("left->left의 값은 %d \n", t->root->left->left->key);
  printf("left->right의 값은 %d \n\n", t->root->left->right->key);

  printf("root->right의 값은 %d \n", t->root->right->key);
  printf("right->left의 값은 %d \n", t->root->right->left->key);
  printf("right->right의 값은 %d \n\n", t->root->right->right->key);

  // min max
  printf("min의 값은 %d \n", rbtree_min(t)->key);
  printf("max의 값은 %d \n\n", rbtree_max(t)->key);

  //delete
  rbtree_erase(t, t->root->left);
  
  // check tree
  printf("root의 값은 %d \n\n", t->root->key);

  printf("root->left의 값은 %d \n", t->root->left->key);
  printf("left->left의 값은 %d \n", t->root->left->left->key);
  printf("left->right의 값은 %d \n\n", t->root->left->right->key);

  printf("root->right의 값은 %d \n", t->root->right->key);
  printf("right->left의 값은 %d \n", t->root->right->left->key);
  printf("right->right의 값은 %d \n\n", t->root->right->right->key);

  printf("test end\n");
  return 0;
}