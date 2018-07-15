/* 
CSCI 260 Assignment 2 
Submitted by: Dylan Suveges
S#: 546853797
*/

#ifndef AVLTREE_H
#define AVLTREE 1

#include <string>
#include <iostream>
using namespace std;

class avltree {
   private:
      struct node {
          node *right, *left;
          node *lastvisit; //last visited node during iterative insert
          string      key, data;
          int         height, balance;
      };
      // we maintain a pointer to the root of the tree
      node *root;

      // private, recursive routines
      // (used by the public methods)
      node *search(string k, node *n);
      void deallocate(node* &n);
      bool insert(string k, string d, node* &n);
      bool remove(string k, node* &n);
      void print(node *n);
      void debugprint(node *n); 
      void rotateleft(node* &n);
      void rotateright(node* &n);
      void checkrotation(node* &n);
      node *findsmallest(node *n);
      void updateheight(node *n);
      bool treetest(node *n, string low, string high);


   public:

      bool insert(string k, string d);
      avltree() { root = NULL; }
      ~avltree() { deallocate(root); }
      void display() { print(root); }
      void debug() { debugprint(root); }

      bool remove(string k) {
           if (remove(k, root)) checkrotation(root);
           else return false;
           return true;
      }
      bool search(string k, string &d) {
          node *n = search(k, root);
          if (!n) return false;
          d = n->data;
          return true;
     }
     bool checkvalid();
};

#endif
