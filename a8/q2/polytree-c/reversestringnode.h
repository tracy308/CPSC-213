#ifndef __REVERSESTRINGNODE_H__
#define __REVERSESTRINGNODE_H__

/**
 * struct definition of class and external definition of class table
 */
struct ReverseStringNode_class {
  // TODO add function pointers
  struct StringNode_class* super;
  int  (*compareTo) (void*, void*);
  void (*printNode) (void*);
  void (*insert)    (void*, void*);
  void (*print)     (void*);
  void (*delete)    (void*);
};
extern struct ReverseStringNode_class ReverseStringNode_class_table;

/**
 * struct definition of object
 */
struct ReverseStringNode;
struct ReverseStringNode {
  // TODO add class pointer and 
  // variables that are stored in instances of this class (including those introduced by super class)
};

/**
 * definition of methods implemented by this class
 */
void ReverseStringNode_ctor(void*, char*);
int ReverseStringNode_compareTo(void*, void*);

/**
 * definition of new for class
 */
void* new_ReverseStringNode(char*);

#endif /*__REVERSESTRINGNODE_H__*/
