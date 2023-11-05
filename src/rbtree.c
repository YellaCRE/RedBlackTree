#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  printf("init start");
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode->color = RBTREE_BLACK;

  p->nil = nilNode;
  p->root = nilNode;
  printf("init end");
  return p;
}

node_t *rbtree_rotate(rbtree *t, node_t *x, const int d){
  printf("rotate start");
  node_t *y;
  
  if (d == 0){
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
      if (x == x->parent->left){
        x->parent->left = y;
      }
      else{
        x->parent->right = y;
      }
    }

    y->left = x;
    x->parent = y;
  }

  else{
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
      if (x == x->parent->left){
        x->parent->right = y;
      }
      else{
        x->parent->left = y;
      }
    }

    y->right = x;
    x->parent = y;
  }
  printf("rotate end");
  return t->root;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *z){
  printf("fixup start");
  node_t *y;

  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->right){
          z = z->parent;
          rbtree_rotate(t, z, 0);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent = RBTREE_RED;
        rbtree_rotate(t, z, 1);
      }
    }
    else if(z->parent == z->parent->parent->right){
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->left){
          z = z->parent;
          rbtree_rotate(t, z, 1);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent = RBTREE_RED;
        rbtree_rotate(t, z->parent->parent, 0);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
  printf("fixup end");
  return t->root;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  printf("insert start");
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->color = RBTREE_RED;
  z->key = key;

  node_t *y = t->nil;
  node_t *x = t->root;

  while(x != t->nil){
    y = x;
    if(key < x->key){
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

  printf("insert end");
  return t->root;
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
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
