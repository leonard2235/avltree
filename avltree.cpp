/* 
AVL Tree Function Implementations
By Dylan Suveges
*/

#include "avltree.h"

void avltree::updateheight(node *n)
// compute the height of node n, assuming the heights
//    of n's left and right children are correct
// n's height is one greater than the height of the
//    taller of its two children
{
   // make sure n isn't null
   if (n == NULL) return;

   // remember one or both of n's children might be null
   if ((!n->left) && (!n->right)) {
      n->height = 0;
      n->balance = 0;
   } else if (!n->left) {
      n->height = n->right->height + 1;
      n->balance = 1;
   } else if (!n->right) {
      n->height = n->left->height + 1;
      n->balance = -1;
   } 
   // general case: both children exist,
   else {
      n->height = n->left->height + 1;
      if (n->height <= n->right->height)
         n->height = n->right->height + 1;
      n->balance = n->right->height - n->left->height;
   }
}

avltree::node *avltree::search(string k, node *n)
// search the subtree rooted at n,
//    looking for the topmost node whose key matches k
// if a match is found return a pointer to the node,
// otherwise return null
{
   if (!n) return NULL;
   if (n->key == k) return n;
   else if (n->key > k) return search(k, n->left);
   else return search(k, n->right);
}

void avltree::deallocate(node* &n)
// delete all nodes in the subtree rooted at n,
//    and set n to null
{
   if (!n) return;
   deallocate(n->left);
   deallocate(n->right);
   delete n;
   n = NULL;
}

avltree::node *avltree::findsmallest(node *n)
// in the subtree rooted at n,
//    find the node with the smallest key value
// (i.e. go as far left as possible)
{
   if (!n) return NULL;
   while (n->left) n = n->left;
   return n;
}

bool avltree::insert(string k, string d, node* &n)
// insert a new node in the binary search tree rooted at n,
// returning true if successful, false otherwise
//
// after a successful insertion below n,
//    checkrotation is called to determine if the
//    subtree rooted at n is unbalanced,
//    and to perform any reconstruction necessary
{
   // if we've found the end of a chain,
   //    insert the node here
   if (!n) {
      n = new node;
      if (!n) return false;
      n->data = d;
      n->key = k;
      n->left = NULL;
      n->right = NULL;
      n->height = 0;
      n->balance = 0;
      return true;
   }

   // call the insert routine recursively on either
   //      the left or right subtree,
   // checking for and performing rotations if it
   //      was successful
   if (n->key > k) {
      if (insert(k, d, n->left)) {
         checkrotation(n->left);
         return true;
      }
   } else {
      if (insert(k, d, n->right)) {
         checkrotation(n->right);
         return true;
      }
   }

   // if we get here then the recursive insert
   //    was unsuccessful
   return false;
}

bool avltree::remove(string k, node* &n)
// if the subtree rooted at n contains a node whose key
//    matches k then remove it from the subtree,
//    then check for any necessary reconstruction of the tree
// return true if an element is successfully removed,
//     or false otherwise
{
   // if n is an empty tree then give up
   if (!n) return false;

   // if the matching node must be somewhere in the left subtree
   //    then make a recursive call and check for any needed rotation
   if (n->key > k) {
      if (remove(k, n->left)) {
         checkrotation(n->left);
         return true;
      } 
   } 

   // if the matching node must be somewhere in the right subtree
   //    then make a recursive call and check for any needed rotation
   else if (n->key < k) {
      if (remove(k, n->right)) {
         checkrotation(n->right);
         return true;
      }
   } 

   // if the current node is the one that must be removed,
   //    base the handling on how many children the node has
   else {
      // remember which node we'll actually delete
      node *victim = n;

      // if the node has no children we can simply delete it
      if ((!n->left) && (!n->right)) {
         delete n;
         n = NULL;
         return true;
      } 

      // if the node has just a right child then we can
      //    bypass n (i.e. make the pointer to n point
      //    to its right child instead)
      else if (!n->left) {
         n = n->right;
         delete victim;
         return true;
      }

      // if the node has just a left child then we can
      //    bypass n (i.e. make the pointer to n point
      //    to its left child instead)
      else if (!n->right) {
         n = n->left;
         delete victim;
         return true;
      } 

      // if the node has two children then we'll replace the
      //    node with the smallest node from the right subtree
      //    (basically copying the other node's key/data values
      //     over top of n's)
      // then make a recursive call to remove the duplicate
      //    element from the right subtree,
      //    remembering to check for necessary rotation
      //    once we've altered n's subtrees
      else {
         victim = findsmallest(n->right);
         if (!victim) return false;
         string vkey = victim->key;
         string vdata = victim->data;
         if (!remove(victim->key, n->right)) return false;
         n->key = vkey;
         n->data = vdata;
         checkrotation(n);
         return true;
      }
   }
   return false;
}

void avltree::print(node *n)
// display the key/data contents of the subtree rooted at n,
// sorted (ascending) by key value
{
   if (!n) return;
   print(n->left);
   cout << n->key << ":" << n->data << endl;
   print(n->right);
}

void avltree::debugprint(node *n)
// display the contents and structure of the subtree rooted at n,
// performed via preorder traversal
{
   if (!n) return;
   cout << n->key << ":" << n->data << " (";
   if (n->left) cout << n->left->key;
   else cout << "NULL";
   cout << "<-left,right->";
   if (n->right) cout << n->right->key;
   else cout << "NULL";
   cout << ")(height:" << n->height << ", balance: ";
   cout << n->balance << ")" << endl;
   debugprint(n->left);
   debugprint(n->right);
}

void avltree::rotateleft(node* &n)
// rotates n's right child up, and n down to the left
//   BEFORE               AFTER
//      N                   Y
//     / \                 / \
//    /   \               /   \
//   X     Y             N     D
//  / \   / \           / \
// A   B C   D         X   C
//                    / \
//                   A   B
{
   node *tmp = n;         // remember N
   n = n->right;          // make Y the root of the subtree
   tmp->right = n->left; // make C into N's right child
   n->left = tmp;         // make N into Y's left child
   updateheight(tmp);     // N's height has probably changed
   updateheight(n);       // Y's height has probably changed
}

void avltree::rotateright(node* &n)
// rotates n's left child up, and n down to the right
//   BEFORE               AFTER
//      N                   X
//     / \                 / \
//    /   \               /   \
//   X     Y             A     N
//  / \   / \                 / \     
// A   B C   D               B   Y
//                              / \                
//                             C   D
{
   node *tmp = n;         // remember N
   n = n->left;           // make X the root of the subtree
   tmp->left = n->right;   // make B into N's left child
   n->right = tmp;        // make N into X's right child
   updateheight(tmp);     // N's height has probably changed
   updateheight(n);       // X's height has probably changed
}

void avltree::checkrotation(node* &n)
// determine if the subtree rooted at n has become unbalanced
// (i.e. the height difference between the left and right 
//   subtrees of n is more than 1)
// and perform any rotations necessary to reconstruct the tree
//   in a balanced form.
{
   if (n == NULL) return;

   // update the height and balance fields for n
   updateheight(n);


   // quit if n is balanced (i.e. if the left and right
   //      subtree heights are exactly the same or there are no children
   if (n->balance == 0)
      return;

   // if n and it's right subtree are both positively
   //    balanced then do one left rotation on n
   if ((n->balance > 0) && (n->right->balance > 0))
      rotateleft(n);

   // if n and it's left subtree are both negatively
   //    balanced then do one right rotation on n
   else if ((n->balance < 0) && (n->left->balance < 0))
      rotateright(n);

   // if n is negatively balanced 
   //    and it's left child is positively balanced
   // then do two rotations:
   //    do one left rotation on n's left child
   //    then one right rotation on n
   else if ((n->balance < 0) && (n->left->balance > 0)){
      rotateleft(n->left);
      rotateright(n);
   }

   // if n is positively balanced
   //    and it's right child is negatively balanced
   // then do two rotations
   //    do one right rotation on n's right child
   //    then one left rotation on n
   else if ((n->balance > 0) && (n->right->balance < 0)) {
      rotateright(n->right);
      rotateleft(n);
   }
}

bool avltree::checkvalid() 
//helper function which calls the treetest method if there is
//at least one node in the tree.
{
   if (root == NULL){
      cout << "No nodes in tree...\n";
      return false;
   }
   return treetest(root,"","");
}

bool avltree::treetest(node *n, string low, string high) 
//Checks to see if the avltree is currently a valid binary search 
//tree and no node has a subtree whose height differs by more than 1.
//returns true if the structure of the tree is correct otherwise returns 
//false.
//NOTE: assumes there is at least one node in the tree
//if there is no low or high value, the empty string is passed.
{
   //check that keys adhere to the bounds set by low and high
   if ( (low != "") && (n->key < low) ) { 
      //if the current nodes key is too small
      cerr << "Error at node with key: " << n->key 
           << ", key smaller than lowest allowable (" << low << ").\n";
      return false;
   } else if ( (high != "") && (n->key > high) ) { 
      //if the currentnodes key is too big
      cerr << "Error at node with key: " << n->key 
           << ", key bigger than biggest allowable (" << high << ").\n";
      return false;
   }//done checking key bounds

   //if heights are ok then call recursively as appropriate
   if (n->left != NULL && n->right != NULL) {
      if ((n->left->height - n->right->height) > 1) {  
         //left subtree is too big
         cerr << "Error at node with key " << n->key 
              << ", subtree starting with the node with key " 
              << n->left->key 
              << " is >1 node taller than the subtree starting"
              << " with the node with key " 
              << n->right->key << ".\n";
         return false;

      } else if ((n->right->height - n->left->height) > 1) { 
         //right subtree too big
         cerr << "Error at node with key " << n->key 
              << ", subtree starting with the node with key " 
              << n->right->key 
              << " is >1 node taller than the subtree starting"
              << " with the node with key " 
              << n->left->key << ".\n";         
         return false;

      } else //heights are ok, check both children
         return ( treetest(n->left,low,n->key) && 
                  treetest(n->right,n->key,high) );

   } else if (n->left != NULL) {
      if (n->left->height  > 0) { //left subtree too big
         cerr << "Error at node with key " << n->key 
              << ", subtree starting with the node with key " 
              << n->left->key 
              << "'s height is >0 when there is no right subtree.\n";         
         return false;
      } else //heights are ok, check left child      
         return treetest(n->left,low,n->key);
   } else if (n->right != NULL) {
      if (n->right->height  > 0) { //left subtree too big
         cerr << "Error at node with key " << n->key 
              << ", subtree starting with the node with key " 
              << n->right->key 
              << "'s height is >0 when there is no left subtree.\n";         
         return false;
      } else //heights are ok, check left child      
          return treetest(n->right,n->key,high);
   }
   //otherwise the node n does not have a subtree we need to check
   return true; 
}

bool avltree::insert(string k, string d)
//Iteratively goes through the list to find the correct place to insert
//a new node with the passed string k as its key and the passed string 
//d as its data.  Once the correct place is found, it inserts the node
//and traverses back up updating the height of any node affected and
//checking if a rotation is needed on any of the affected nodes.
{
   
   node *itrnode = root;
   node *prevnode = NULL;

   //find the correct place to insert the new node
   while (itrnode != NULL){
      //if the new key is smaller than itrnode's key go left
      if (itrnode->key > k){ 
         prevnode = itrnode;
         itrnode = itrnode->left;
         if (itrnode)
            itrnode->lastvisit = prevnode; //update last visited node
      } else { 
         //go right
         prevnode = itrnode;
         itrnode = itrnode->right;
         if (itrnode)
            itrnode->lastvisit = prevnode; //update last visited node
      }      
   } //found the correct place to insert a new node
   itrnode = prevnode;
  
   //create the new node
   node *n = new node;
   if (!n) return false;
   n->data = d;
   n->key = k;
   n->left = NULL;
   n->right = NULL;
   n->height = 0;
   n->balance = 0;
   n->lastvisit = NULL;

   //place the new node in tree
   if (root == NULL){
      root = n;
      return true;
   }
   if (itrnode->key > k)
      itrnode->left = n;
   else
      itrnode->right = n;

   node *tmp = NULL;

   //iterate back up through tree and check if rotations are needed   
   while (itrnode->lastvisit != NULL){
      tmp = itrnode->lastvisit; //store last visited node
      itrnode->lastvisit = NULL; //scrub record if last visited node
      itrnode = tmp; //iterate up to last visited node

      //update height and check rotation for the node we came from
      //note: updatehight and checkrotation take a pointer by reference so
      //we need to use the previous node's children pointers when calling.
      if (itrnode->key > k){ //we came from the left node
         updateheight(itrnode->left);
         checkrotation(itrnode->left);
      } else { //we came from the right node
         updateheight(itrnode->right);
         checkrotation(itrnode->right); 
      }
   }
   //now we take care of updating/checking the root
   updateheight(root);
   checkrotation(root);

   return true;   
}


