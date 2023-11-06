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

// void delete_rbtree(rbtree *t) {
//   // TODO: reclaim the tree nodes's memory
//   node_t *y;
//   node_t *z = t->root;

//   while (z != t->nil){
//     if (z->left != t->nil){
//       z = z->left;
//     }
//     else if (z->right != t->nil){
//       z = z->right;
//     }
//     else{
//       y = z->parent;
//       free(z);
//       z = NULL;
//       z = y;
//     }
//   }
  
//   free(t->nil);
//   free(t);
// }

void free_node(rbtree *t, node_t *x) {
  // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *z = t->root;
  while (z->key != key){
    if (z == t->nil){
      return NULL;
    }

    if (z->key < key){
      z = z->right;
    }
    else{
      z = z->left;
    }
  }

  return z;
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
      else { // x의 형제 w가 BLACK이고 w의 왼쪽이 RED, 오른쪽이 BLACK일 때
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
  x->color = RBTREE_BLACK;
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
    rbtree_transplant(t, z, y);  // y->parent = z->parent
    y->left = z->left; // y와 z를 하나로 만든다
    y->left->parent = y;
    y->color = z->color;
  }

  if (original == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);  // x는 이동하는 값의 자식
  }
  return 0;
}

void inorder(const rbtree *t, node_t *now, key_t *arr, size_t n, size_t *count){
  if (now != t->nil)
    inorder(t, now->left, arr, n, count);
  else
    return;  // now가 트리 밖이면 return

  // 왼쪽으로 쭉 가고 범위를 벗어나면 기록
  if (*count < n)
    arr[(*count)++] = now->key;  // 중위 순회는 가운데서 기록을 한다
  else
    return;
  // 한 번 오른쪽으로 가고 다시 왼쪽으로 쭉 간다
  inorder(t, now->right, arr, n, count);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil) {
    return 0;
  }
  size_t cnt = 0;  // 전역변수 선언
  inorder(t, t->root, arr, n, &cnt); 
  return 0;
}
