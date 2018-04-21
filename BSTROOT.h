//
//  BSTRAOT.h
//  COP3530
//
//  Created by Nicholas Alvarez on 12/1/17.
//  Copyright © 2017 Nicholas Alvarez. All rights reserved.
//
#ifndef _BSTROOT_H_
#define _BSTROOT_H_

#include<iostream>
#include <stdexcept>
#include <exception>
#include <stdlib.h>

// Template and Class SSLL
namespace cop3530
{
    
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    class BSTROOT
    {
        
    public:
        // type aliases for prettier code
        using self_type = BSTROOT;
        using self_reference = BSTROOT&;
        
        // constructor
        BSTROOT();
        
        // copy constructor/assignment
        BSTROOT( const BSTROOT<K,V,Comp,Equal>& src );
        BSTROOT<K,V,Comp,Equal>& operator=( const BSTROOT<K,V,Comp,Equal>& src );
        
        // move constructor/assignment
        BSTROOT( const BSTROOT<K,V,Comp,Equal>&& src );
        BSTROOT<K,V,Comp,Equal>& operator=( BSTROOT<K,V,Comp,Equal>&& src );
        
        // destructor
        ~BSTROOT();
        
        // Required Methods for project
        void insert(const K& key, const V& value);
        void remove(const K& key);
        V& lookup(const K& key);
        
        // Optional - Bonus Methods
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
                std::cout<<"PreOrder ="<<root->key<<std::endl;
                preOrder(root->left);
                preOrder(root->right);
            }
        }
        
        // A utility function to do inorder traversal of BST
        void inOrder(Node* root)
        {
            if (root != NULL)
            {
                inOrder(root->left);
                std::cout<<"InOrder = "<<root->key<<std::endl;
                inOrder(root->right);
            }
        }
        
        
        // Makes a copy of the BST
        Node* CopyTree(Node* OrigNode ){
            
            if ( OrigNode == NULL )
                return NULL;
            
            // create a new node and assign to Orig
            Node* NewNode  = new Node();
            NewNode->key   = OrigNode->key;   // write key   to root
            NewNode->value = OrigNode->value; // write value to root
            NewNode->left  = CopyTree(OrigNode->left);
            NewNode->right = CopyTree(OrigNode->right);
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
        
        // Method to insert new nodes at leaf, and then
        // apply proper rotatation so new Node is root
        Node* InsertRoot(Node* node, const K& key, const V& value) {
            
            if ( node==NULL )
            {
                // create a new node and assign to root
                node        = new Node(); // Create New Node
                node->left  = NULL;       // Set left    to NULL
                node->right = NULL;       // Set right   to NULL
                node->key   = key;        // write key   to root
                node->value = value;      // write value to root
                return node;
            }
            else if ( Comp(key,node->key) )
            {
                // key < node->key
                node->left = InsertRoot(node->left, key, value);
                node = RotR(node);
            }
            else  // key > node->key )
            {
                node->right = InsertRoot(node->right, key, value);
                node = RotL(node);
            }
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
                if ( Equal(key,current->key) )
                    return current;
                else if ( Comp(key,current->key) )
                    // [ key < current->key ]
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
                // Node to Left since [ key < node->key ]
                return getParentNode(node->left,key);
            else
                // Node to Right since [ key < node->key ]
                return getParentNode(node->right,key);
            
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
                
                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
            return root;
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
    BSTROOT<K,V,Comp,Equal>::BSTROOT() {
        root=NULL;
    }
    
    // Copy Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTROOT<K,V,Comp,Equal>::BSTROOT( const BSTROOT<K,V,Comp,Equal>& src ) {
        // Make a deep copy of the BST
        root=CopyTree(src.root);
    }
    
    // Copy Assignment
    //      ....do a bunch of stuff with dst...
    //      ....now I don't need dst anymore, so reuse it...
    //  dst= src;
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTROOT<K,V,Comp,Equal>& BSTROOT<K,V,Comp,Equal>::operator=( const BSTROOT<K,V,Comp,Equal>& src ) {
        
        // Need to deallocate all of current nodes from BST
        clear();
        // Now Make a deep copy using the Src BST
        root=CopyTree(src.root);
        
        // Return Reference to destination
        return *this;
    }
    
    // Move Constructor - using source nodes
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTROOT<K,V,Comp,Equal>::BSTROOT( const BSTROOT<K,V,Comp,Equal>&& src ) {
        root = src.root;
    }
    
    
    // Move Assignment  - HiJack source Binary Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTROOT<K,V,Comp,Equal>& BSTROOT<K,V,Comp,Equal>::operator=( BSTROOT<K,V,Comp,Equal>&& src ) {
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
    BSTROOT<K,V,Comp,Equal>::~BSTROOT() {
        clear();
    }
    
    
    //------------ CLEAR () ------------------
    // clear() — removes all key-value pairs from the BS Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTROOT<K,V,Comp,Equal>::clear() {
        
        // call delete Tree to remove all nodes
        deleteTree(root);
        root=NULL;
        return;
        
    }
    
    
    //------------ IS_FULL() ------------------
    //is_full() — returns true IFF the no more key-value pairs can be added to the map.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTROOT<K,V,Comp,Equal>::is_full() const {
        return false;
    }
    
    
    // ----------- IS_EMPTY() -----------------
    //is_empty() — returns true IFF the map contains no key-value pairs.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTROOT<K,V,Comp,Equal>::is_empty() const {
        
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
    void BSTROOT<K,V,Comp,Equal>::insert(const K& key, const V& value) {
        // Call method to insert node at the root versus leaf nodes
        root = InsertRoot(root, key, value);
        return;
    }
    
    
    // ----------- size_t size( ) -----------------
    //size() — returns the number of key-value pairs in the map
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTROOT<K,V,Comp,Equal>::size() {
        // count number of nodes in tree starting from root
        return getSize(root);
        
    }
    
    // ----------- size_t height( ) -----------------
    //height() — [for tree-based maps only] returns the tree's height.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTROOT<K,V,Comp,Equal>::height() {
        
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
    size_t BSTROOT<K,V,Comp,Equal>::balance() {
        
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
    bool BSTROOT<K,V,Comp,Equal>::contains( const K& key ) const {
        
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
    V& BSTROOT<K,V,Comp,Equal>::lookup(const K& key) {
        
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
    void BSTROOT<K,V,Comp,Equal>::remove( const K& key ) {
        
        root = deleteNode(root,key);
        return;
        
    }
    
    /*
     // ----------- void remove( const K& key ) -----------------
     // remove( key ) — removes the node associated with the key provided
     template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
     void BSTROOT<K,V,Comp,Equal>::remove( const K& key ) {
     
     preOrder(root);
     
     // call private method SearchTree to see if key is in BSTree
     // if return = NULL then key not found
     Node* keyptr;
     keyptr = SearchTree(root,key);
     if ( keyptr==NULL ){
     throw std::runtime_error("BSTROOT<>.remove(): Key not found in tree ");
     }
     
     std::cout<<"Remove Keyptr key = "<<keyptr->key<<std::endl;
     
     // So Key Node is found...proceed to remove
     if ( (keyptr->left==NULL) && (keyptr->right==NULL) )
     {
     // -----------------------------------------------------
     // Case 1 - Node to be removed has no children
     //          (1) set parent to NULL and delete Node
     //          (2) - delete the node associated with key
     // -----------------------------------------------------
     
     std::cout<<"Remove Keyptr Case #1 = "<<std::endl;
     // Find the parent node
     Node* parentNode;
     parentNode = getParentNode(root,key);
     
     // Set either the left or right child nodes of the
     // parent node to Null based on key
     if ( (parentNode->left != NULL) && parentNode->left->key == key )
     parentNode->left=NULL;
     else // Right side is child node of the key
     parentNode->right=NULL;
     
     // Delete the Key Node
     delete keyptr;
     
     }
     else if ( ((keyptr->left == NULL) && (keyptr->right != NULL)) ||
     ((keyptr->left != NULL) && (keyptr->right == NULL))  )
     {
     // -----------------------------------------------------
     // Case 2 - Node to be removed has only one child
     //          (1) - Cut node from the tree and link in
     //                single child (with its subtree)
     //          (2) - delete the node associated with key
     // -----------------------------------------------------
     //
     std::cout<<"Remove Keyptr Case #2 = "<<std::endl;
     // Find the parent node
     Node* parentNode;
     parentNode = getParentNode(root,key);
     
     // Now need to find out if it is the left or right
     // node of the parent node that matches the key
     if ( (parentNode->left != NULL) && parentNode->left->key == key )
     {
     // Check with side of key node it is
     if ( keyptr->left != NULL )
     {
     // So set parent left node to the Key Node Left
     parentNode->left = keyptr->left;
     }
     else
     {
     // else.. set parent left node to the Key Node Right
     parentNode->left = keyptr->right;
     }
     }
     else
     {
     // Parent node right side matches the key
     // Check with side of key node it is
     if ( keyptr->left !=NULL )
     {
     // So set parent right node to the Key Node Left
     parentNode->right = keyptr->left;
     }
     else
     {
     // else...set parent right node to the Key Node Right
     parentNode->right = keyptr->right;
     }
     }
     delete keyptr;
     
     }
     else {
     
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
     Node* MinNode;
     MinNode = FindMinNode(keyptr->right);
     
     std::cout<<"Remove Keyptr Case #3 = "<<std::endl;
     std::cout<<"MinNode Key = "<<MinNode->key<<std::endl;
     
     // Need to Find Parent of MinNode
     Node* parentNode;
     parentNode = getParentNode(keyptr,MinNode->key); 
     std::cout<<"Parent Node Key = "<<parentNode->key<<std::endl;
     
     // Replace the (key,value) of KeyNode with MinNode
     keyptr->key   = MinNode->key;
     keyptr->value = MinNode->value;
     
     if ( parentNode == keyptr )
     {
     // ------------------------
     // Parent Node = KeyNode 
     // ------------------------
     // So just need to set the left or right 
     // of the KeyNode to NULL
     if ( keyptr->left->key == MinNode->key ) 
     keyptr->left = NULL;
     else
     keyptr->right = NULL;
     }
     else {
     
     // ------------------------
     // Parent Node != KeyNode 
     // ------------------------
     
     // Now need to find out if it is the left or right
     // node of the parent node that matches the MinNode key
     if ( parentNode->left->key == MinNode->key ) 
     {
     // So set parent left node to NULL
     parentNode->left = NULL;
     }
     else 
     {
     // So set parent left node to NULL
     parentNode->right = NULL;
     }
     
     }
     // Now go ahead and delete the MinNode
     delete MinNode;
     
     } 
     return;
     
     }
     
     */
    
    
}
#endif 
