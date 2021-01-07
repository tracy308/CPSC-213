struct it_tree;
struct it_iterator;

struct it_tree* it_new             ();
void            it_delete          (struct it_tree* tree);
void            it_add             (struct it_tree* tree, int value);
void*           it_iterator        (void* tree);
int             it_has_next        (void* iterator);
void*           it_get_next        (void* iterator);
void            it_delete_iterator (void* iterator);