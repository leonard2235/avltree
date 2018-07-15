/* 
CSCI 260 Assignment 2 
Submitted by: Dylan Suveges
S#: 546853797
*/

#include "avltree.h"
#include <iostream>

char getcommand();
void printmenu();
bool processcommand(char cmd, avltree *&tree);
string numtostring(unsigned int a);
int getnumber();
void clearcinbuffer();

int main()
{
   avltree *T = new avltree;
   
   printmenu();
   char cmd = getcommand();
   while (processcommand(cmd, T)) { 
      cout << endl;
      cmd = getcommand();
   }
   delete T;
   return 0;
}

char getcommand()
{
   cout << "Enter your command choice (D, P, H, I, Q, N, G"
        << ", C, R, or S)" << endl;
   char cmd;
   cin >> cmd;
   cmd = toupper(cmd);
   switch (cmd) {
      case 'H':
      case 'I':
      case 'P':
      case 'S':
      case 'R':
      case 'Q':
      case 'N':
      case 'G':
      case 'C':
      case 'T':
      case 'D': return cmd;
      default:  cout << "You have entered an invalid command" << endl;
                return getcommand();
   }
}

void printmenu()
{
   cout << "Enter I to insert a new element," << endl;
   cout << "   or S to search for an element," << endl;
   cout << "   or P to print all elements," << endl;
   cout << "   or D to for debug data display," << endl;
   cout << "   or C to check the validity of the tree's structure," 
        << endl;
   cout << "   or R to remove an element," << endl;
   cout << "   or N to create a new empty tree," << endl;
   cout << "   or G to generate a new populated tree," << endl;
   cout << "   or H for help," << endl;
   cout << "   or Q to quit." << endl;
}

bool processcommand(char cmd, avltree *&tree)
{
   string k, d;
   int n;
   int total = 0;
   switch (cmd) {
      case 'H': printmenu(); 
                break;
      case 'I': cout << "Enter the key word" << endl;
                cin >> k;
                cout << "Enter the data word" << endl;
                cin >> d;
                cout << "(" << k << "," << d << ") ";
                if (tree->insert(k, d)) 
                   cout << "was inserted successfully" << endl;
                else cout << "was not inserted correctly" << endl;
                break;
      case 'S': cout << "Enter the key you wish to search for" << endl;
                cin >> k;
                cout << k << " ";
                if (tree->search(k, d)) cout << "was found, with data value: " << d << endl;
                else cout << "was not found" << endl;
                break;
      case 'R': cout << "Enter the key you wish to delete" << endl;
                cin >> k;
                cout << k << " ";
                if (tree->remove(k)) cout << "was successfully removed" << endl;
                else cout << "was not in the tree" << endl;
                break;
      case 'P': cout << "The tree contents are: ";
                tree->display();
                break; 
      case 'D': cout << "The tree debug data is:" << endl;
                tree->debug();
                break; 
      case 'N': cout << "Creating a new tree...\n";
                delete tree;
                tree = new avltree;
                if (tree != NULL)
                   cout << "SUCCESS!\n";
                break;
      case 'G': cout << "Enter the number of nodes you wish"
                     << " to generate." << endl;
                delete tree;
                tree = new avltree;
                if (tree == NULL) break; //problem creating a new tree
                do {
                  n = getnumber();
                  clearcinbuffer();
                  if (n < 1) cout << "Invalid input, please enter a "
                                  << "positive integer.\n";
                } while (n < 1);
                for(int i = 0; i < n;i++) {
                  tree->insert(numtostring(i),"genericdata");
                }
                cout << "Tree of " << n 
                     << " random items (with search keys 0-" << n-1 
                     << ") generated successfully" << endl;
                break;
      case 'C': if ( tree->checkvalid() ) 
                   cout << "Tree structure is valid.\n";
                break;
   
      default:  return false;
   }
   return true;
}

string numtostring(unsigned int a)
//recursively converts a integer to a string
//assumes 'int a' is a non-negative integer
{
   string g;

   //base case
   if (a < 10) { 
      g = "";
      g += (char)('0' + a); //add the first integer of the number 
      return g;           //to an empty string and return it as a string
   }

   //add each successiveinteger after the first one's been found
   g = numtostring(a/10) + (char)('0' + a % 10);  

   return g;             
}

void clearcinbuffer()
//This function simply clears in the cin buffer until a carriage 
//return is read so as to prevent any bad things happening from the 
//user entering too much input into one read in attempt.
//assumes there is at least a carriage return in the cin buffer
{
  char it = cin.get();
  while (it != '\n')
    cin.get(it);
}


int getnumber()
//This function tries to read in an integer number and if it is 
//successful, it returns that number. If unsuccessful, the function 
//returns -1 to indicate an invalid number.
//Note: -If an integer number is entered before any invalid characters, 
//the integer number that was entered before the invalid characters 
//is returned.
{
  char c;
  int number;
  cin >> c; //get first char, skip whitespaces
  if (c >= '0' && c <= '9'){
    number = c-'0';  // number is valid
    c = cin.peek();
    while (c >='0' && c <= '9'){  // read integer part of number
      c = cin.get();
      number = number * 10 + (c - '0');
      c = cin.peek();
    }
    return number;
  }
  return -1; //not a number
}
