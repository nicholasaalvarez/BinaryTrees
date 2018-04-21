//
//  AVL.h
//  COP3530
//
//  Created by Nicholas Alvarez on 12/1/17.
//  Copyright © 2017 Nicholas Alvarez. All rights reserved.
//

#ifndef _BSTAVL_H_
#define _BSTAVL_H_

#include<iostream>
#include <stdexcept>
#include <exception>
#include <stdlib.h>


namespace cop3530
{
    
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    class BSTAVL
    {
        
    public:
        // type aliases for prettier code
        using self_type = BSTAVL;
        using self_reference = BSTAVL&;
        
        // constructor
        BSTAVL();
        
        // copy constructor/assignment
        BSTAVL( const BSTAVL<K,V,Comp,Equal>& src );
        BSTAVL<K,V,Comp,Equal>& operator=( const BSTAVL<K,V,Comp,Equal>& src );
        
        // move constructor/assignment
        BSTAVL( const BSTAVL<K,V,Comp,Equal>&& src );
        BSTAVL<K,V,Comp,Equal>& operator=( BSTAVL<K,V,Comp,Equal>&& src );
        
        // destructor
        ~BSTAVL();
        
        // Required
        void insert(const K& key, const V& value);
        void remove(const K& key);
        V& lookup(const K& key);
        
        // Optional
        bool contains(const K& key) const;
        bool is_empty() const;
        bool is_full() const;
        size_t size();
        void clear();
        size_t height();
        size_t balance();
        
        struct Node {
            K key;
            V value;
            int   height;
            Node* left;
            Node* right;
        };
        
        
    private:
        Node* root;
        
        // A utility method to print pre-order traversal of tree
        void preOrder(Node* root)
        {
            if ( root != NULL )
            {
                std::cout<<"PreOrder = "<<root->key<<std::endl;
                preOrder(root->left);
                preOrder(root->right);
            }
        }
        
        
        // utility method to get maximum of two size_t numbers
        int getMax( int h1, int h2) {
            return (( h1 > h2 ) ? h1 : h2);
        }
        
        // utility method to get Node height of the tree
        int getNodeHeight(struct Node* N)
        {
            if ( N == NULL )
                return 0;
            else
                return N->height;
        }
        
        
        // Method used by Copy Constructor/assignement
        // to Make a deep copy of the BST
        Node* CopyTree(Node* OrigNode ){
            
            if ( OrigNode == NULL )
                return NULL;
            
            // create a new node and assign to Orig
            Node* NewNode   = new Node();
            NewNode->key    = OrigNode->key;             // write key
            NewNode->value  = OrigNode->value;           // write value
            NewNode->height = OrigNode->height;          // write node height
            NewNode->left   = CopyTree(OrigNode->left);  // left  node ptr
            NewNode->right  = CopyTree(OrigNode->right); // right node ptr
            return NewNode;
            
        }
        
        
        // Traverse Tree to find leaf node for new node to be inserted
        // NOTE: This method will return the same node with key
        //       all ready is present in the BST.
        Node* FindInsertNode(Node* node, const K& key)
        {
            // Need to traverse tree to find insert location
            Node* current = node;
            while ( current != NULL )
            {
                if ( Equal(key,current->key) )
                {
                    // [ key == current->key ]
                    // key in BST all ready so just return node
                    return current;
                }
                else if ( Comp( key,current->key) )
                {
                    // [ key < current->key ]
                    if ( current->left == NULL )
                        return current;
                    else
                        // Keep searching
                        current = current->left;
                }
                else
                {
                    if ( current->right == NULL )
                        return current;
                    else
                        // [ key > current->key ]
                        // keep searching
                        current = current->right;
                }
            }
            return current;
            
        }
        
        
        
        // Get Balance factor of node N
        int getBalance(Node* N)
        {
            if (N == NULL)
                return 0;
            else
                return getNodeHeight(N->left) - getNodeHeight(N->right);
        }
        
        
        
        // A utility method to perform a left rotation
        // on the subtree rooted with x
        Node* RotL(Node* x)
        {
            
            Node* y = x->right;
            Node* z = y->left;
            
            // Perform rotation
            y->left  = x;
            x->right = z;
            
            //  Update heights with Max
            x->height = getMax(getNodeHeight(x->left), getNodeHeight(x->right))+1;
            y->height = getMax(getNodeHeight(y->left), getNodeHeight(y->right))+1;
            
            // Return new root
            return y;
        }
        
        // A utility method to perform a right rotate
        // on subtree rooted with y
        Node* RotR(Node* y)
        {
            Node* x = y->left;
            Node* z = x->right;
            
            // Perform rotation
            x->right = y;
            y->left  = z;
            
            // Update heights
            y->height = getMax(getNodeHeight(y->left), getNodeHeight(y->right))+1;
            x->height = getMax(getNodeHeight(x->left), getNodeHeight(x->right))+1;
            
            // Return new root
            return x;
        }
        
        
        // Recursive function to insert key in subtree rooted
        // with node and returns new root of subtree.
        // Note: If key all ready in BST then replace Nodes Value
        Node* InsertAvl(Node* node, const K& key, const V& value) {
            
            //  1.  Perform the normal BST insertion
            if ( node==NULL )
            {
                // create a new node and assign to root
                node         = new Node(); // Create New Node
                node->left   = NULL;       // Set left    to NULL
                node->right  = NULL;       // Set right   to NULL
                node->key    = key;        // write key   to root
                node->value  = value;      // write value to root
                node->height = 1;          // Height of node
                return node;
            }
            else if ( Equal(key, node->key) )
            {
                // Check if Key provided matches current Node Key
                // If Key matches so update root node value
                node->value = value;
                return node;
            }
            else if ( Comp(key,node->key) )
            {
                // Left Side .. [ key > node->key ]
                node->left = InsertAvl(node->left, key, value);
            }
            else if ( Equal(key,node->key) )
            {
                // Duplicate keys are not allowed
                return node;
            }
            else
            {
                // [ key > node->key ]
                node->right = InsertAvl(node->right, key, value);
            }
            
            //  2. Update height of this ancestor node
            node->height = 1 + getMax( getHeight(node->left), getHeight(node->right));
            
            // 3. Get the balance factor of this ancestor node to check
            //    if this node is now unbalanced and needs balancing
            int  balance = getBalance(node);
            
            // ---------------------------------------------
            // If this node becomes unbalanced, then
            // there are 4 cases that can occur:
            // ---------------------------------------------
            
            // Left Left Case
            // balance > 1 "AND" ( key < node->left->key )
            if (balance > 1 && Comp(key,node->left->key) )
                return RotR(node);
            
            // Right Right Case
            // balance < -1 and ( key > node->right->key )
            if (balance < -1 && ( !Equal(key,node->right->key) &&
                                 !Comp(key,node->right->key )  ) )
                return RotL(node);
            
            // Left Right Case
            // balance > 1 "AND" ( key > node->left->key )
            if (balance > 1 && ( !Equal(key,node->left->key) &&
                                !Comp(key,node->left->key )  ) )
            {
                node->left = RotL(node->left);
                return RotR(node);
            }
            
            // Right Left Case
            // balance < -1  "AND" ( key < node->right->key )
            if (balance < -1 && Comp(key,node->right->key) )
            {
                node->right = RotR(node->right);
                return RotL(node);
            }
            
            //...else return the (unchanged) node pointer
            return node;
        }
        
        
        
        // This method will recursively delete a node with key provided
        // from an AVL tree.  Returns the root of the modified subtree
        Node* deleteAVLNode(Node* root, const K& key)
        {
            // -------------------------------------------
            // STEP 1: Perform the standard BST Delete
            // -------------------------------------------
            
            // If the node is null then return
            if (root == NULL)
                return root;
            
            // Check if key is Equal, Less than or Greater than Root Key
            if ( Equal(key,root->key) )
            {
                // key == root->key This is the Key to remove
                // check if node has only one child or no child
                if( (root->left == NULL) || (root->right == NULL) )
                {
                    // create temp pointer
                    Node* temp;
                    if ( root->left != NULL )
                        temp = root->left;
                    else
                        // temp can still be NULL since both left and
                        // right child ptrs can be NULL
                        temp = root->right;
                    
                    
                    // No child case
                    if (temp == NULL)
                    {
                        // Assign root to temp and set root=NULL
                        temp = root;
                        root = NULL;
                    }
                    else
                    {
                        // One child case
                        // Copy contents of the non-empty child to root
                        *root = *temp;
                    }
                    
                    // Delete Temp
                    delete temp;
                }
                else
                {
                    // node with two children: so need to get the inorder
                    // successor (smallest key in the right subtree)
                    Node* temp = FindMinNode(root->right);
                    
                    // Copy the in-order successor's Key,Value Pair to temp node
                    root->key   = temp->key;
                    root->value = temp->value;
                    
                    // Delete the in-order successor
                    root->right = deleteAVLNode(root->right, temp->key);
                }
            }
            else if ( Comp(key,root->key) )
                // Key is smaller (Left Subtree)
                root->left = deleteAVLNode(root->left, key);
            else
                // Key is greater (Right Subtree)
                //[ key > root->key ]
                root->right = deleteAVLNode(root->right, key);
            
            
            // Check If the tree had only one node --> Return
            if (root == NULL)
                return root;
            
            
            // -----------------------------------------------
            // STEP 2: Update the Height of the current node
            // -----------------------------------------------
            root->height = 1 + getMax(getNodeHeight(root->left),
                                      getNodeHeight(root->right));
            
            // -------------------------------------------
            // STEP 3: Get the balance factor of this NODE
            // balance factor will be needed to check
            // if this node has become unbalanced
            // -------------------------------------------
            int balance = getBalance(root);
            
            // If this node becomes unbalanced, then there are 4 cases:
            
            // Case 1: Left Left Case
            if (balance > 1 && getBalance(root->left) >= 0)
                return RotR(root);
            
            // Case 2: Left Right Case
            if (balance > 1 && getBalance(root->left) < 0)
            {
                root->left = RotL(root->left);
                return RotR(root);
            }
            
            // Case 3: Right Right Case
            if (balance < -1 && getBalance(root->right) <= 0)
                return RotL(root);
            
            // Case 4: Right Left Case
            if (balance < -1 && getBalance(root->right) > 0)
            {
                root->right = RotR(root->right);
                return RotL(root);
            }
            
            return root;
        }
        
        
        // Traverse Tree to search if key is stored in tree
        // Note: Returns the node associated with the key
        //       if key not found, then returns NULL
        Node* SearchTree(Node* node, const K& key) const {
            
            // Need to traverse tree to find key
            Node* current = node;
            //std::cout<<"Entering SearchTree method"<<std::endl;
            
            // current node NULL so return
            if ( current == NULL )
                return NULL;
            
            // Else start searching tree
            while ( current != NULL )
            {
                if ( Equal(key,current->key) )
                    return current;
                else if ( Comp(key,current->key) )
                    current = current->left;
                else
                    // key > current->key )
                    current = current->right;
            }
            // key not found
            return NULL;
        }
        
        
        /*
         // Given a non-empty binary search tree, return the node with
         // minimum key value found in that tree.
         struct Node * minValueNode(struct Node* node)
         {
         struct Node* current = node;
         
         // loop down to find the leftmost leaf
         while (current->left != NULL)
         current = current->left;
         
         return current;
         }
         */
        
        
        // This method will Traverse a Binary Search Tree
        // staring at node provided and will return the
        // left most edge (minimum) of the tree.
        Node* FindMinNode(Node* node)
        {
            // Need to traverse tree to find key
            Node* current = node;
            
            // current node NULL so return
            if ( current == NULL )
                return NULL;
            
            // Else start searching tree
            while ( current != NULL )
            {
                if ( current->left==NULL )
                    // then found minimum
                    return current;
                else // (current->left != NULL)
                    // update current ptr and follow left edge (Min)
                    current = current->left;
            }
            return current;
            
        }
        
        
        // Traverse the tree and count number of nodes
        // starting from the root node
        size_t getSize(Node* root) {
            
            if ( root==NULL ) {
                return 0;
            }
            return 1 + getSize(root->left) + getSize(root->right);
            
        }
        
        
        // Traverse the tree and get height of the tree
        size_t getHeight(Node* root) {
            
            if ( root==NULL ) {
                return 0;
            }
            size_t lft_height = getHeight(root->left);
            size_t rht_height = getHeight(root->right);
            return (lft_height > rht_height) ? lft_height+1 : rht_height+1 ;
        }
        
        
        
        // This method sill search the Binary Search Tree to find
        // the parent node when based on the key provided
        Node* getParentNode(Node* node, const K& key)
        {
            
            // Check if node is all ready NULL
            if ( node == NULL )
                return NULL;
            
            // Check if Node has no childs
            if( node->left==NULL && node->right==NULL)
                return NULL;
            
            if ( ( (node->left  != NULL) && (node->left->key)  == key) ||
                ( (node->right != NULL) && (node->right->key) == key)  )
                return node;  // this is the parent node for the current key
            
            // Check if key to left
            if ( Comp(key,node->key)  )
                // [ key < node->key ]
                return getParentNode(node->left,key);
            else // ( key > node->key  )
                // Check if key to right
                return getParentNode(node->right,key);
            
        }
        
        
        
        // This method traverses tree in post order to
        // to delete each and every node of the tree recursively
        void deleteTree(Node* node)
        {
            
            // Node =NULL so return (NULL)
            if ( node == NULL )
                return;
            
            // Need to delete left and right subtree first (Postorder)
            // find the leaf nodes first
            deleteTree(node->left);
            deleteTree(node->right);
            
            // now after deleting left and right sub-tree
            // go ahead and the delete current node
            delete node;
            return;
            
        }
        
    };
    
    
    // Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>::BSTAVL() {
        root=NULL;
    }
    
    
    // Copy Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>::BSTAVL( const BSTAVL<K,V,Comp,Equal>& src ) {
        // Make a deep copy of the BST
        root=CopyTree(src.root);
    }
    
    
    // Copy Assignment
    //      ....do a bunch of stuff with dst...
    //      ....now I don't need dst anymore, so reuse it...
    //  dst= src;
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>& BSTAVL<K,V,Comp,Equal>::operator=( const BSTAVL<K,V,Comp,Equal>& src ) {
        
        // Need to deallocate all of current nodes from BST
        clear();
        // Now Make a deep copy using the Src BST
        root=CopyTree(src.root);
        
        // Return Reference to destination
        return *this;
    }
    
    
    // Move Constructor - using source nodes
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>::BSTAVL( const BSTAVL<K,V,Comp,Equal>&& src ) {
        root = src.root;
    }
    
    
    // Move Assignment  - HiJack source Binary Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>& BSTAVL<K,V,Comp,Equal>::operator=( BSTAVL<K,V,Comp,Equal>&& src ) {
        // Need to deaallocate all BST Nodes First...
        clear ();
        root = src.root;
        // Set source root to NULL since move assignment
        // now has taken ownership of them
        src.root = nullptr;
        return *this;
    }
    
    
    // Destructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTAVL<K,V,Comp,Equal>::~BSTAVL() {
        clear();
    }
    
    
    //------------ CLEAR () ------------------
    // clear() — removes all key-value pairs from the BS Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTAVL<K,V,Comp,Equal>::clear() {
        
        // call delete Tree to remove all nodes
        deleteTree(root);
        root=NULL;
        return;
        
    }
    
    
    //------------ IS_FULL() ------------------
    //is_full() — returns true IFF the no more key-value pairs can be added to the map.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTAVL<K,V,Comp,Equal>::is_full() const {
        return false;
    }
    
    
    // ----------- IS_EMPTY() -----------------
    //is_empty() — returns true IFF the map contains no key-value pairs.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTAVL<K,V,Comp,Equal>::is_empty() const {
        
        // Check if tree is empty (No Nodes)
        if ( root==NULL ) {
            return true;
        } else {
            return false;
        }
        
    }
    
    
    // ----------- void insert ( const K& key, const V& value) -----------------
    // insert   adds the specified key-value pair to the map
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTAVL<K,V,Comp,Equal>::insert(const K& key, const V& value) {
        // Call method to insert node into AVL Tree
        root = InsertAvl(root, key, value);
        return;
    }
    
    
    // ----------- size_t size( ) -----------------
    //size() — returns the number of key-value pairs in the map
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTAVL<K,V,Comp,Equal>::size() {
        // count number of nodes in tree starting from root
        return getSize(root);
        
    }
    
    // ----------- size_t height( ) -----------------
    //height() — [for tree-based maps only] returns the tree's height
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTAVL<K,V,Comp,Equal>::height() {
        size_t height = getHeight(root);
        if ( height > 1 )
            height = height - 1;
        else
            height = height;
        
        return height;
    }
    
    
    // ----------- size_t balance( ) -----------------
    //balance() — [for tree-based maps only] returns the tree's balance factor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTAVL<K,V,Comp,Equal>::balance() {
        
        // Check if Root == Null
        if ( root == NULL )
            return 0;
        
        // Get Height of left-side of tree
        size_t lft_height = getHeight(root->left);
        
        // Get Height of right-side of tree
        size_t rht_height = getHeight(root->right);
        
        // Compute balance factor
        if ( lft_height > rht_height )
            return ( lft_height-rht_height );
        else  // rht height > lft height
            return ( rht_height-lft_height );
        
    }
    
    
    // ----------- bool contains( const K& key ) -----------------
    // contains( key ) — returns true IFF the tree contains a value associated with the key.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTAVL<K,V,Comp,Equal>::contains( const K& key ) const {
        
        // call private method SearchTree to see if key in tree
        // if return = NULL then key not found
        Node* keyptr;
        keyptr = SearchTree(this->root, key);
        if ( keyptr == NULL )
            return false;
        else
            return true;
        
    }
    
    
    // ----------- V& lookup( const K& key ) -----------------
    // look up - returns a reference to the value associated with the key
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    V& BSTAVL<K,V,Comp,Equal>::lookup(const K& key) {
        
        // Search Tree for node associated with the key
        Node* keyptr;
        keyptr = SearchTree(root,key);
        if ( keyptr==NULL ){
            throw std::runtime_error("BSTLEAF<>.lookup(): key not found in BST");
        }
        else {
            // return the value of the key
            return keyptr->value;
        }
        
    }
    
    
    // ----------- void remove( const K& key ) -----------------
    // remove( key ) — removes the node associated with the key provided
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTAVL<K,V,Comp,Equal>::remove( const K& key ) {
        root = deleteAVLNode(root,key);
    }
    
    

    
    
}
#endif 
