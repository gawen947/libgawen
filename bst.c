/* Copyright (c) 2013, David Hauweele <david@hauweele.net>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "bst.h"

struct node {
  void *data;

  struct node *left;
  struct node *right;
};

struct bst {
  int  (*compare)(const void *, const void *);
  void (*destroy)(void *);

  struct node *root;
};

bst_t bst_create(int  (*compare)(const void *, const void *),
                 void (*destroy)(void *))
{
  struct bst *bst = malloc(sizeof(struct bst));

  if(!bst)
    return NULL;

  bst->compare = compare;
  bst->destroy = destroy;
  bst->root    = 0;

  return bst;
}

void * bst_insert(bst_t bst, void *data)
{
  struct node **ref = &bst->root;
  struct node *node = bst->root;

  while(node) {
    int comparison = bst->compare(data, node->data);

    if(comparison < 0) {
      ref  = &node->left;
      node = node->left;
    }
    else if(comparison > 0) {
      ref  = &node->right;
      node = node->right;
    }
    else {
      bst->destroy(node->data);
      node->data = data;
      return data;
    }
  }

  node = malloc(sizeof(struct node));

  if(!node)
    return NULL;

  node->left  = NULL;
  node->right = NULL;
  node->data  = data;

  *ref = node;

  return data;
}

void * bst_search(bst_t bst, void *data)
{
  struct node *node = bst->root;

  while(node) {
    int comparison = bst->compare(data, node->data);

    if(comparison < 0)
      node = node->left;
    else if(comparison > 0)
      node = node->right;
    else
      return node->data;
  }

  return NULL;
}

void bst_delete(bst_t bst, void *data)
{
  struct node **ref = &bst->root;
  struct node *node = bst->root;

  while(node) {
    int comparison = bst->compare(data, node->data);

    if(comparison < 0) {
      ref  = &node->left;
      node = node->left;
    }
    else if(comparison > 0) {
      ref  = &node->right;
      node = node->right;
    }
    else
      goto found;
  }

  return;

found:
  bst->destroy(node->data);

  if(node->left && node->right) {
    struct node *root = node;
    node = node->right;
    while(node->left) {
      ref  = &node->left;
      node = node->left;
    }

    root->data = node->data;
    *ref       = node->right;
    free(node);
  }
  else if(node->left) {
    *ref = node->left;
    free(node);
  }
  else {
    *ref = node->right;
    free(node);
  }
}

static void inorder(struct node *node, void (*action)(void *data))
{
  if(!node)
    return;

  inorder(node->left, action);
  action(node->data);
  inorder(node->right, action);
}

void bst_walk(bst_t bst, void (*action)(void *data))
{
  inorder(bst->root, action);
}

static void destroy(bst_t bst, struct node *node)
{
  if(!node)
    return;

  destroy(bst, node->left);
  destroy(bst, node->right);

  bst->destroy(node->data);
  free(node);
}

void bst_destroy(bst_t bst)
{
  destroy(bst, bst->root);
  free(bst);
}
