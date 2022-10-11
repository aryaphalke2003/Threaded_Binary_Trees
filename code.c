#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// structure of a node of a doubly threaded tree
struct node
{
    int data;           // stores data in the node
    struct node *left;  // stores address of left child
    struct node *right; // stores address of right child
    bool leftthread;    // 0-indicates node has left child otherwise indicates left child is null
    bool rightthread;   // 0-indicates node has right child otherwise indicates right child is null
};

// function to create a node
struct node* dummy;

struct node *createnode(int val)
{
    struct node *temp = malloc(sizeof(struct node));
    temp->data = val;
    temp->left = dummy;
    temp->right = dummy;
    temp->leftthread = true;  // initially left child is null
    temp->rightthread = true; // initially left child is null
    return temp;
}
// function to search any node in the tree
// searching without recursion
void search(struct node *root, int key)
{
    struct node *temp = root;
    int flag = 0; // 1 if element is present
    // searching in the tree similar to binary search
    while (1)
    {
        if (temp->data == key)
        {

            printf("key present\n");
            flag = 1;
            break;
        }
        else if (temp->data < key)
        {   if(temp->rightthread==false)
            temp = temp->right;
            else break;
        }
        else{
        if(temp->leftthread==false)
            temp = temp->left;
            else break;
        }
    }
    if (flag == 0)
        printf("key is absent\n");
}

// function to insert a node in the tree
void insert(struct node **root, int val)
{
    struct node *temp = *root;
    struct node *par = NULL; // parent node
    if(!temp){
        struct node* p= createnode(val);
        p->left=dummy;
        p->right=dummy;
        *root=p;
        return;
    }
    while (temp != dummy)
    {
        if (val == temp->data)
        { // if the element is already present
            return;
        }
        par = temp; // insert at proper place

        if (val < temp->data)
        {
            if (temp->leftthread == false)
            {
                temp = temp->left;
            }
            else
                {
                    par=temp;
                    break;
                }
        }
        if (val > temp->data){
    
            if (temp->rightthread == false)
            {
                temp = temp->right;
            }
            else
                {
                    par=temp;
                    break;
                }
        }
    }
    // if element is not present
    struct node *ptr = createnode(val); // create node
    if (par == NULL)   // if tree is empty
       { ptr->left=dummy;
       ptr->right=dummy;
        (*root) = ptr;
       }
        // node becomes root else enter at proper place
    else if (val < par->data)
    {

        ptr->left = par->left;
        ptr->right = par;
        par->leftthread = false;
        par->left = ptr;
    }
    else
    {

        ptr->left = par;
        ptr->right = par->right;
        par->rightthread = false;
        par->right = ptr;
    }
}

// return inorder successor using righttread
struct node *inorder_successor(struct node *ptr)
{
    // If rthread is set, we can quickly find
    if (ptr->rightthread == true)
        return ptr->right;

    // Else return leftmost child of right subtree
    ptr = ptr->right;
    while (ptr->leftthread == false)
        ptr = ptr->left;
    return ptr;
}

// function to print inorder traversal
void inorder(struct node *root)
{
    if (root == NULL)
        return;
    struct node *ptr = root;
    while (ptr->leftthread == false)
    {
    // printf("%d",ptr->data);

        ptr = ptr->left;

    }
    while (ptr != dummy)
    {
        printf("%d ", ptr->data);
        ptr = inorder_successor(ptr);
    }
    printf("\n");
}
// Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
// if the node has no child
struct node *case1(struct node *root, struct node *par, struct node *ptr)
{
    // If Node to be deleted is root
    if (par == NULL)
        root = NULL;

    // If Node to be deleted is left of its parent
    else if (ptr == par->left)
    {
        par->leftthread = true;
        par->left = ptr->left;
    }
    else
    {
        par->rightthread = true;
        par->right = ptr->right;
    }

    // Free memory and return new root
    free(ptr);
    return root;
}

// returns inorder successor using left and right children
struct node *insucc(struct node *ptr)
{
    if (ptr->rightthread == true)
        return ptr->right;

    ptr = ptr->right;
    while (ptr->leftthread == false)
        ptr = ptr->left;

    return ptr;
}

// returns inorder predecessor
struct node *inpred(struct node *ptr)
{
    if (ptr->leftthread == true)
        return ptr->left;

    ptr = ptr->left;
    while (ptr->rightthread == false)
        ptr = ptr->right;
    return ptr;
}

// if node has only one child
struct node *case2(struct node *root, struct node *par, struct node *ptr)
{
    struct node *child;
    // if node has left child
    if (ptr->leftthread == false)
        child = ptr->left;
    else
        child = ptr->right; // if node has right child
    if (par == NULL)        // if node is root
        root = child;
    else if (ptr == par->left) // if node is left child
        par->left = child;
    else
        par->right = child;       // if node is right child
    struct node *s = insucc(ptr); // inorder successor
    struct node *p = inpred(ptr); // inorder predecessor
    if (ptr->leftthread == false) // node is left child
        p->right = s;
    else
    { // if node is right child
        if (ptr->rightthread == false)
            s->left = p;
    }
    free(ptr);
    return root;
}

// if node has two child
struct node *case3(struct node *root, struct node *par,
                   struct node *ptr)
{

    struct node *parsucc = ptr;     // parent of inorder successor
    struct node *succ = ptr->right; // inorder successor

    // Find leftmost child of successor
    while (succ->leftthread == false)
    {
        parsucc = succ;
        succ = succ->left;
    }

    ptr->data = succ->data;

    if (succ->leftthread == true && succ->rightthread == true)
        root = case1(root, parsucc, succ);
    else
        root = case2(root, parsucc, succ);

    return root;
}

// function to delete a node
struct node *delete (struct node *root, int key)
{
    struct node *par = NULL, *ptr = root;
    int flag = 0; // indicates presence of the element
    // check if the element is absent
    while (ptr != NULL)
    {
        if (key == ptr->data)
        {
            flag = 1; // element present
            break;
        }
        par = ptr;
        if (key < ptr->data)
        {
            if (ptr->leftthread == false)
                ptr = ptr->left;
            else
                break;
        }
        else
        {
            if (ptr->rightthread == false)
                ptr = ptr->right;
            else
                break;
        }
    }
    if (flag == 0)
        printf("key not found\n");
    // two children
    else if (ptr->leftthread == false && ptr->rightthread == false)
        root = case3(root, par, ptr);
    // left child
    else if (ptr->leftthread == false && ptr->rightthread == true)
        root = case2(root, par, ptr);
    // right child
    else if (ptr->rightthread == false && ptr->leftthread == true)
        root = case2(root, par, ptr);
    else // no child
        root = case1(root, par, ptr);
    return root;
}
int main()
{
    dummy=createnode(INT_MIN);
    struct node *root = NULL;
    while (1)
    {
        char c;
        printf("enter I for insert \n enter D to delete \n enter S to search \n ente Q to quit \n enter T print inorder traversal\n");
        scanf(" %c", &c);
        if (c == 'I')
        {
            int n;
            printf("enter a number to insert in the tree\n");
            scanf("%d", &n);
            insert(&root, n);
        }
        if (c == 'D')
        {
            int d;
            printf("enter a number to delete\n");
            scanf("%d", &d);
            root = delete (root, d);
        }
        if (c == 'S')
        {
            int s;
            printf("enter a number to search in the tree\n");
            scanf("%d", &s);
            search(root, s);
        }
        if (c == 'T')
        {
            inorder(root);
            printf("\n");
        }
        if (c == 'Q')
            return 0;
    }
}
