
//  BSTRAND.h
//  COP3530
//
//  Created by Nicholas Alvarez on 12/1/17.
//  Copyright © 2017 Nicholas Alvarez. All rights reserved.
//
#ifndef _BSTRAND_H_
#define _BSTRAND_H_

#include<iostream>
#include <stdexcept>
#include <exception>
#include <stdlib.h>

// Template and Class SSLL
namespace cop3530
{
    
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    class BSTRAND
    {
        
    public:
        // type aliases for prettier code
        using self_type = BSTRAND;
        using self_reference = BSTRAND&;
        
        // constructor
        BSTRAND();
        
        // copy constructor/assignment
        BSTRAND( const BSTRAND<K,V,Comp,Equal>& src );
        BSTRAND<K,V,Comp,Equal>& operator=( const BSTRAND<K,V,Comp,Equal>& src );
        
        // move constructor/assignment
        BSTRAND( const BSTRAND<K,V,Comp,Equal>&& src );
        BSTRAND<K,V,Comp,Equal>& operator=( BSTRAND<K,V,Comp,Equal>&& src );
        
        // destructor
        ~BSTRAND();
        
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
            size_t size;
            Node* left;
            Node* right;
        };
        
        
    private:
        Node* root;
        
        
        // Method used by Copy Constructor/assignement
        // to Make a deep copy of the BST
        Node* CopyTree(Node* OrigNode ){
            
            if ( OrigNode == NULL )
                return NULL;
            
            // create a new node and assign to Orig
            Node* NewNode  = new Node();
            NewNode->key   = OrigNode->key;             // write key
            NewNode->value = OrigNode->value;           // write value
            NewNode->size  = OrigNode->size;            // write node size
            NewNode->left  = CopyTree(OrigNode->left);  // left  node ptr
            NewNode->right = CopyTree(OrigNode->right); // right node ptr
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
        
        
        // Rotate Node-T Left
        // Class lectures on 11/7
        Node* RotL(Node* T) {
            
            Node* root = T->right;
            T->right   = root->left;
            root->left = T;
            return root;
            
        }
        
        // Rotate Node-T Right
        // Class lectures on 11/7
        Node* RotR(Node* T) {
            
            Node* root = T->left;
            T->left    = root->right;
            root->right= T;
            return root;
            
        }
        
        // Method returns size of the node
        size_t getNodeSize(Node* node)
        {
            if ( node == NULL )
                return 0;
            else
                return node->size;
        }
        
        // Method computes the proper size for the node
        void UpateNodeSize (Node* node)
        {
            //std::cout<<" -> Node Key = "<<node->key<<" Node Size = "<<node->size<<std::endl;
            // update the size for the node
            node->size = getNodeSize(node->left) + getNodeSize(node->right) + 1;
        }
        
        
        // Method to insert new nodes at leaf, and then
        // apply proper rotation so new Node is root
        Node* InsertRoot(Node* node, const K& key, const V& value) {
            
            if ( node==NULL )
            {
                // create a new node and assign to root
                node        = new Node(); // Create New Node
                node->left  = NULL;       // Set left    to NULL
                node->right = NULL;       // Set right   to NULL
                node->key   = key;        // write key   to root
                node->value = value;      // write value to root
                node->size  = 1;          // Number of nodes part of this node
                return node;
            }
            else if ( Equal(key, node->key) )
            {
                // Check if Key provided matches current Node Key
                // Key matches so update root node value
                node->value = value;
                return node;
            }
            else if ( Comp(key,node->key) )
            {
                // Left Side .. [ key > node->key ]
                node->left = InsertRoot(node->left, key, value);
                node = RotR(node);
            }
            else
            {
                // Right Side .. [ key > node->key ]
                node->right = InsertRoot(node->right, key, value);
                node = RotL(node);
            }
            return node;
        }
        
        
        // Method to insert new nodes Randomly to tree
        Node* InsertRand(Node* node, const K& key, const V& value) {
            
            //std::cout<<"Entering InsertRand key="<<key<<std::endl;
            if ( node==NULL )
            {
                // create a new node and assign to root
                node        = new Node(); // Create New Node
                node->left  = NULL;       // Set left    to NULL
                node->right = NULL;       // Set right   to NULL
                node->key   = key;        // write key   to root
                node->value = value;      // write value to root
                node->size  = 1;          // size of nodes for subtree
                return node;
            }
            else if ( Equal(key, node->key) )
            {
                // Check if Key provided matches Root Node Key (Duplicate)
                // Root Key matches so just update current node value
                node->value = value;
                return node;
            }
            
            // Check if insertion needs to be at Root
            // Random Number modulo of 1+n where n=number of nodes in subtree
            //std::cout<<".... RANDOM NUM= "<<rand()<<"Rand % = "<<rand()%(node->size+1)<<std::endl;
            if ( rand()%(node->size+1)==0 )
            {
                //std::cout<<"..Calling Insert Root node size="<<node->size<<std::endl;
                return InsertRoot(node,key,value);
            }
            else if ( Comp(key,node->key) )
            {
                //std::cout<<"..Calling Insert Leaf Left"<<std::endl;
                node->left = InsertRand(node->left, key, value);
            }
            else  // key > node->key
            {
                //std::cout<<"..Calling Insert Leaf Right"<<std::endl;
                node->right = InsertRand(node->right, key, value);
            }
            
            // Update Size for Node
            UpateNodeSize (node);
            return node;
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
                //          if ( key == current->key )
                if ( Equal(key,current->key) )
                    return current;
                //          else if ( key < current->key )
                else if ( Comp(key,current->key) )
                    current = current->left;
                else
                    // key > current->key )
                    current = current->right;
            }
            // key not found
            return NULL;
        }
        
        
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
        
        
        // Given a binary search tree and a key, this method
        // deletes the node and returns the new root
        Node* deleteNode(Node* root, const K& key)
        {
            // base case
            if (root == NULL) return root;
            
            // Search for the node containing the key
            // If key < node->key then key is smaller than current node key
            // so proceed to left subtree
            if ( Comp(key,root->key)  )
                root->left = deleteNode(root->left, key);
            else if ( !Comp(key,root->key) && !Equal(key,root->key) )
                // If the key is greater than node key
                // proceed to right subtree
                root->right = deleteNode(root->right, key);
            else
            {
                // This nodes key matches the key provided
                // this is the node to be deleted
                
                // node with only one child or no child
                if (root->left == NULL)
                {
                    Node* temp = root->right;
                    delete root;
                    return temp;
                }
                else if (root->right == NULL)
                {
                    Node* temp = root->left;
                    delete root;
                    return temp;
                }
                
                // since no return then -> current node has two children:
                // now need to Get the inorder successor (smallest in the right subtree)
                Node* temp = FindMinNode(root->right);
                
                // Copy the inorder successor's content to this node
                root->key   = temp->key;
                root->value = temp->value;
                // For now decided to not update the size of the node
                //root->size  = temp->size;
                
                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
            return root;
        }
        
    };
    
    
    // Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTRAND<K,V,Comp,Equal>::BSTRAND() {
        root=NULL;
    }
    
    
    // Copy Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTRAND<K,V,Comp,Equal>::BSTRAND( const BSTRAND<K,V,Comp,Equal>& src ) {
        // Make a deep copy of the BST
        root=CopyTree(src.root);
    }
    
    
    // Copy Assignment
    //      ....do a bunch of stuff with dst...
    //      ....now I don't need dst anymore, so reuse it...
    //  dst= src;
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTRAND<K,V,Comp,Equal>& BSTRAND<K,V,Comp,Equal>::operator=( const BSTRAND<K,V,Comp,Equal>& src ) {
        
        // Need to deallocate all of current nodes from BST
        clear();
        // Now Make a deep copy using the Src BST
        root=CopyTree(src.root);
        
        // Return Reference to destination
        return *this;
    }
    
    
    // Move Constructor - using source nodes
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTRAND<K,V,Comp,Equal>::BSTRAND( const BSTRAND<K,V,Comp,Equal>&& src ) {
        root = src.root;
    }
    
    
    // Move Assignment  - HiJack source Binary Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTRAND<K,V,Comp,Equal>& BSTRAND<K,V,Comp,Equal>::operator=( BSTRAND<K,V,Comp,Equal>&& src ) {
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
    BSTRAND<K,V,Comp,Equal>::~BSTRAND() {
        clear();
    }
    
    
    //------------ CLEAR () ------------------
    // clear() — removes all key-value pairs from the BS Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTRAND<K,V,Comp,Equal>::clear() {
        
        // call delete Tree to remove all nodes
        deleteTree(root);
        root=NULL;
        return;
        
    }
    
    
    //------------ IS_FULL() ------------------
    //is_full() — returns true IFF the no more key-value pairs can be added to the map.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTRAND<K,V,Comp,Equal>::is_full() const {
        return false;
    }
    
    
    // ----------- IS_EMPTY() -----------------
    //is_empty() — returns true IFF the map contains no key-value pairs.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTRAND<K,V,Comp,Equal>::is_empty() const {
        
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
    void BSTRAND<K,V,Comp,Equal>::insert(const K& key, const V& value) {
        // Call method to insert node Randomly
        root = InsertRand(root, key, value);
        return;
    }
    
    
    // ----------- size_t size( ) -----------------
    //size() — returns the number of key-value pairs in the map
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTRAND<K,V,Comp,Equal>::size() {
        // count number of nodes in tree starting from root
        return getSize(root);
        
    }
    
    // ----------- size_t height( ) -----------------
    //height() — [for tree-based maps only] returns the tree's height.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTRAND<K,V,Comp,Equal>::height() {
        size_t height = getHeight(root);
        if ( height > 1 )
            height = height - 1;
        else
            height = height;
        
        return height;
    }
    
    
    // ----------- size_t balance( ) -----------------
    //balance() — [for tree-based maps only] returns the tree's balance factor.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTRAND<K,V,Comp,Equal>::balance() {
        
        // Check if Root == NULL
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
    bool BSTRAND<K,V,Comp,Equal>::contains( const K& key ) const {
        
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
    V& BSTRAND<K,V,Comp,Equal>::lookup(const K& key) {
        
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
    void BSTRAND<K,V,Comp,Equal>::remove( const K& key ) {
        
        // -----------------------------------------------------
        // Case 1 - Node to be removed has no children 
        //          (1) set parent to NULL and delete Node
        //          (2) - delete the node associated with key
        // -----------------------------------------------------
        
        // -----------------------------------------------------
        // Case 2 - Node to be removed has only one child  
        //          (1) - Cut node from the tree and link in
        //                single child (with its subtree)
        //          (2) - delete the node associated with key
        // -----------------------------------------------------
        //
        // ---------------------------------------------------------
        // Case 3 - Node to be removed has two childs (Complicated)
        //     (1) - Find the minimum element in the right 
        //           subtree of the node to be removed. 
        //     (2) - Replace KeyNode with the MinNode (key,value) 
        //              KeyNode->key   = MinNode->key
        //              KeyNode->value = MinNode->value
        //     (3) - Find Parent Node of MinNode  
        //     (4) - Set Left or Right of Parent of MinNode to Null
        //     (3) - Now remove MinNode from BST
        // ---------------------------------------------------------
        root = deleteNode(root,key);
        return;
        
    }
    
    
}
#endif 
