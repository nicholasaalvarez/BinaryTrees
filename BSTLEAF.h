//
//  BSTLEAF.h
//  COP3530
//
//  Created by Nicholas Alvarez on 12/1/17.
//  Copyright © 2017 Nicholas Alvarez. All rights reserved.
//
#ifndef _BSTLEAF_H_
#define _BSTLEAF_H_

#include<iostream>
#include <stdexcept>
#include <exception>
#include <stdlib.h>

// Template and Class SSLL
namespace cop3530
{
    
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    class BSTLEAF
    {
        
    public:
        // type aliases for prettier code
        using self_type = BSTLEAF;
        using self_reference = BSTLEAF&;
        
        // constructor
        BSTLEAF();
        
        // copy constructor/assignment
        BSTLEAF( const BSTLEAF<K,V,Comp,Equal>& src );
        BSTLEAF<K,V,Comp,Equal>& operator=( const BSTLEAF<K,V,Comp,Equal>& src );
        
        // move constructor/assignment
        BSTLEAF( const BSTLEAF<K,V,Comp,Equal>&& src );
        BSTLEAF<K,V,Comp,Equal>& operator=( BSTLEAF<K,V,Comp,Equal>&& src );
        
        
        // destructor
        ~BSTLEAF();
        
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
                    // Key Equals current Node
                    return current;
                else if ( Comp(key,current->key) )
                    // key < current->key  (Left Sub-Tree)
                    current = current->left;
                else
                    // key > current->key (Right Sub-Tree)
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
            
            // Check for Empty tree
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
                return node;
            
            if ( ( (node->left  != NULL) && (node->left->key)  == key) ||
                ( (node->right != NULL) && (node->right->key) == key)  )
                return node;  // this is the parent node for the current key
            
            // Check if key to left
            if ( Comp(key,node->key)  )
                return getParentNode(node->left,key);
            else
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
                
                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
            return root;
        }
        
    };
    
    
    
    // Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTLEAF<K,V,Comp,Equal>::BSTLEAF() {
        root=NULL;
    }
    
    
    // Copy Constructor
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTLEAF<K,V,Comp,Equal>::BSTLEAF( const BSTLEAF<K,V,Comp,Equal>& src ) {
        // Make a deep copy of the BST
        root=CopyTree(src.root);
    }
    
    
    // Copy Assignment
    //      ....do a bunch of stuff with dst...
    //      ....now I don't need dst anymore, so reuse it...
    //  dst= src;
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTLEAF<K,V,Comp,Equal>& BSTLEAF<K,V,Comp,Equal>::operator=( const BSTLEAF<K,V,Comp,Equal>& src ) {
        
        // Need to deallocate all of current nodes from BST
        clear();
        // Now Make a deep copy using the Src BST
        root=CopyTree(src.root);
        
        // Return Reference to destination
        return *this;
    }
    
    
    
    // Move Constructor - using source nodes
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTLEAF<K,V,Comp,Equal>::BSTLEAF( const BSTLEAF<K,V,Comp,Equal>&& src ) {
        root = src.root;
    }
    
    
    // Move Assignment  - HiJack source Binary Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    BSTLEAF<K,V,Comp,Equal>& BSTLEAF<K,V,Comp,Equal>::operator=( BSTLEAF<K,V,Comp,Equal>&& src ) {
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
    BSTLEAF<K,V,Comp,Equal>::~BSTLEAF() {
        clear();
    }
    
    
    //------------ CLEAR () ------------------
    // clear() — deletes all key-value pairs (nodes) from the BS Tree
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    void BSTLEAF<K,V,Comp,Equal>::clear() {
        
        // call deleteTree only if Root != NULL to delete all nodes
        deleteTree(root);
        root=NULL;
        return;
        
    }
    
    
    //------------ IS_FULL() ------------------
    //is_full() — returns true IFF the no more key-value pairs can be added to the map.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTLEAF<K,V,Comp,Equal>::is_full() const {
        return false;
    }
    
    
    // ----------- IS_EMPTY() -----------------
    //is_empty() — returns true IFF the map contains no key-value pairs.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    bool BSTLEAF<K,V,Comp,Equal>::is_empty() const {
        
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
    void BSTLEAF<K,V,Comp,Equal>::insert(const K& key, const V& value) {
        
        Node* InsertNode;
        if ( root==NULL )
        {
            // create a new node and assign to root
            root        = new Node(); // Create New Node
            root->left  = NULL;       // Set left    to NULL
            root->right = NULL;       // Set right   to NULL
            root->key   = key;        // write key   to root
            root->value = value;      // write value to root
            return;
        }
        else if ( Equal(key, root->key) )
        {
            // Check if Key provided matches Root Node Key
            // Root Key matches so update root node value
            root->value = value;
            return;
        }
        else if ( Comp(key, root->key) )
        {
            // Check left side of tree
            //  [ key < root->key ]
            if (root->left == NULL) {
                // If Root Left is Null -> then insert at this node
                Node* newnode  = new Node();  // Create New Node
                root->left     = newnode;
                newnode->key   = key;
                newnode->value = value;
                newnode->left  = NULL;
                newnode->right = NULL;
                return;
            } else {
                // Else ..Traverse Left Subtree and find Insert Node
                InsertNode = FindInsertNode(root->left,key);
            }
            
        }
        else //( key > root->key )
        {
            // Check right side of tree
            if (root->right == NULL) {
                // If Root Right is Null -> then insert at this node
                Node* newnode  = new Node();  // Create New Node
                root->right    = newnode;
                newnode->key   = key;
                newnode->value = value;
                newnode->left  = NULL;
                newnode->right = NULL;
                return;
            } else {
                // Else ..Traverse Right Subtree and find Insert Node
                InsertNode = FindInsertNode(root->right,key);
            }
            
        }
        
        // Now... Ptr InsertNode contains the insert Node (parent Node)
        // Still need to determine if child node needs to be
        // created to the left or to the right
        
        // First .. Check if key of Insert Node matches key provided ( duplicate )
        if ( InsertNode != NULL && Equal(InsertNode->key,key) )
        {
            // Key matched an existing Node --> so just update value
            //std::cout<<"Insert<>: Node found with same key replacing value key="<<InsertNode->key<<std::endl;
            InsertNode->value = value;
            return;
        }
        
        
        // Need to make sure that Insert Method did not return NULL
        if ( InsertNode != NULL )
        {
            // Create the New Leaf Node (Child Node)
            Node* newnode  = new Node();
            
            // Now Node InsertNode contains the parent node
            // now determine if child is left or right
            if ( Comp( key, InsertNode->key) )
            {
                //  Create child to left
                //  ( key < InsertNode->key )
                InsertNode->left  = newnode;
                newnode->key      = key;
                newnode->value    = value;
                newnode->left     = NULL;
                newnode->right    = NULL;
            }
            else
            {
                // Create child to right
                InsertNode->right = newnode;
                newnode->key      = key;
                newnode->value    = value;
                newnode->left     = NULL;
                newnode->right    = NULL;
            }
            
        }
        return;
        
    }
    
    
    // ----------- size_t size( ) -----------------
    //size() — returns the number of key-value pairs in the map
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTLEAF<K,V,Comp,Equal>::size() {
        // count number of nodes in tree starting from root
        return getSize(root);
        
    }
    
    // ----------- size_t height( ) -----------------
    //height() — [for tree-based maps only] returns the tree's height.
    template<typename K, typename V, bool (*Comp)(K,K), bool (*Equal)(K,K)>
    size_t BSTLEAF<K,V,Comp,Equal>::height() {
        
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
    size_t BSTLEAF<K,V,Comp,Equal>::balance() {
        
        // Check if Root is Null
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
    bool BSTLEAF<K,V,Comp,Equal>::contains( const K& key ) const {
        
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
    V& BSTLEAF<K,V,Comp,Equal>::lookup(const K& key) {
        
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
    void BSTLEAF<K,V,Comp,Equal>::remove( const K& key ) {
        
        root = deleteNode(root,key);
        return;
        
    }
    
    
}
#endif 
