//
//  test_AVL_catch.cpp
//  COP3530
//
//  Created by Nicholas Alvarez on 12/7/17.
//  Copyright © 2017 Nicholas Alvarez. All rights reserved.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "AVL.h"
#include <iostream>


bool compare_Kint( int key1, int key2) {
    return ( key1 < key2);
}

bool is_equals_Kint( int key1, int key2) {
    return ( key1 == key2);
}


// Create BST Obj
cop3530::BSTAVL<int, char, compare_Kint, is_equals_Kint> bst;

TEST_CASE( "BST Empty Test ") {
    
    // Initially BST should be empty
    REQUIRE( bst.is_empty() == true  );
    REQUIRE( bst.is_full()  == false );
    REQUIRE( bst.size()     == 0     );
    REQUIRE( bst.height()   == 0     );
    REQUIRE( bst.balance()  == 0     );
    
    bst.insert( 10, 'a');
    // Now Size=1 Height=1  Balance=0
    
    REQUIRE( bst.is_empty() == false );
    REQUIRE( bst.is_full()  == false );
    REQUIRE( bst.size()     == 1     );
    REQUIRE( bst.height()   == 1     );
    REQUIRE( bst.balance()  == 0     );
    
}


TEST_CASE( "BST Clear Test" ) {
    
    bst.clear();
    bst.insert( 20, 'a');
    bst.insert( 10, 'b');
    bst.insert( 30, 'c');
    bst.insert( 5,  'd');
    bst.insert( 25, 'e');
    REQUIRE( bst.is_empty() == false );
    REQUIRE( bst.is_full()  == false );
    REQUIRE( bst.size()     == 5     );  // 5 Nodes
    REQUIRE( bst.height()   == 2     );
    REQUIRE( bst.balance()  == 0     );
    
    bst.clear();
    REQUIRE( bst.is_empty() == true  );
    REQUIRE( bst.is_full()  == false );
    REQUIRE( bst.size()     == 0     );
    REQUIRE( bst.height()   == 0     );
    REQUIRE( bst.balance()  == 0     );
    
}


TEST_CASE( "Exceptions Test" ) {
    
    bst.clear();
    bst.insert( 50, 'z');
    bst.insert( 49, 'd');
    REQUIRE_THROWS( bst.lookup(40) );
    
}


TEST_CASE( "BST Duplicate Key Insertion Test" ) {
    
    bst.clear();
    bst.insert( 20, 'a');
    bst.insert( 30, 'b');
    
    REQUIRE( bst.lookup(20)    == 'a'   );
    REQUIRE( bst.lookup(30)    == 'b'   );
    
    // Now Swap the Node's value
    bst.insert( 20, 'b');
    bst.insert( 30, 'a');
    REQUIRE( bst.lookup(20)    == 'b'   );
    REQUIRE( bst.lookup(30)    == 'a'   );
    
}


TEST_CASE( "BST Look-Up/Contains Test" ) {
    
    bst.clear();
    bst.insert( 50, 'a');
    bst.insert( 60, 'b');
    bst.insert( 58, 'c');
    bst.insert( 40, 'd');
    bst.insert( 38, 'e');
    bst.insert( 18, 'f');
    bst.insert( 55, 'g');
    bst.insert( 54, 'h');
    bst.insert( 56, 'i');
    bst.insert( 59, 'j');
    bst.insert( 20, 'k');
    bst.insert( 32, 'l');
    bst.insert( 5,  'm');
    bst.insert( 25, 'n');
    
    REQUIRE( bst.is_empty() == false );
    REQUIRE( bst.is_full()  == false );
    REQUIRE( bst.size()     == 14    );  // 14 Nodes
    REQUIRE( bst.height()   == 4     );
    REQUIRE( bst.balance()  <= 1     );
    
    REQUIRE( bst.contains(50)  == true  );
    REQUIRE( bst.contains(51)  == false );
    REQUIRE( bst.lookup(50)    == 'a'   );
    
    REQUIRE( bst.contains(40)  == true  );
    REQUIRE( bst.contains(41)  == false );
    REQUIRE( bst.lookup(40)    == 'd'   );
    
    REQUIRE( bst.contains(5)   == true  );
    REQUIRE( bst.contains(6)   == false );
    REQUIRE( bst.contains(7)   == false );
    REQUIRE( bst.lookup(25)    == 'n'   );
    REQUIRE( bst.lookup(5)     == 'm'   );
    REQUIRE( bst.lookup(32)    == 'l'   );
    REQUIRE( bst.lookup(60)    == 'b'   );
    REQUIRE( bst.lookup(58)    == 'c'   );
    
}

TEST_CASE( "BST Remove Test" ) {
    
    // This Test uses previous Test BST
    REQUIRE( bst.size()     == 14    );  // 14 Nodes
    // now remove the root nodes - has two children
    // new root will be node key=54  Height=6
    REQUIRE( bst.contains(50)  == true  );
    bst.remove(50);
    REQUIRE( bst.contains(50)  == false );
    REQUIRE( bst.size()        == 13    );  // 13 Nodes
    REQUIRE( bst.height()      == 4     );
    REQUIRE( bst.balance()     <= 1     );
    REQUIRE( bst.lookup(58)    == 'c'   );
    
    // Node key=25 leaf node ( no children )
    REQUIRE( bst.contains(25)  == true  );
    bst.remove(25);
    REQUIRE( bst.contains(25)  == false );
    REQUIRE( bst.size()        == 12    );  // 12 Nodes
    REQUIRE( bst.height()      == 4     );
    REQUIRE( bst.balance()     <= 1     );
    
    // Node key=20 ( one children )
    REQUIRE( bst.contains(20)  == true  );
    bst.remove(20);
    REQUIRE( bst.contains(20)  == false );
    REQUIRE( bst.size()        == 11    );  // 11 Nodes
    REQUIRE( bst.balance()     <= 1     );
    
    // Node key=56 ( no children )
    REQUIRE( bst.contains(56)  == true  );
    bst.remove(56);
    REQUIRE( bst.contains(56)  == false );
    REQUIRE( bst.size()        == 10    );  // 10 Nodes
    REQUIRE( bst.balance()     <= 1     );
    
    // look up 59
    // lets check that key=59 is still in BST
    // by looking up the nodes value
    REQUIRE( bst.lookup(59)    == 'j'   );
    
    REQUIRE( bst.contains(18)  == true  );
    bst.remove(18);
    REQUIRE( bst.contains(18)  == false );
    REQUIRE( bst.size()        == 9     );
    REQUIRE( bst.balance()     <= 1     );
    
    REQUIRE( bst.contains(54)  == true  );
    bst.remove(54);
    REQUIRE( bst.contains(54)  == false );
    REQUIRE( bst.size()        == 8     );
    REQUIRE( bst.balance()     <= 1     );
    
    REQUIRE( bst.contains(40)  == true  );
    bst.remove(40);
    REQUIRE( bst.contains(40)  == false );
    REQUIRE( bst.size()        == 7     );
    REQUIRE( bst.balance()     <= 1     );
    
    // Key =59 still in BST
    REQUIRE( bst.lookup(59)    == 'j'   );
    
    REQUIRE( bst.contains(5)   == true  );
    bst.remove(5);
    REQUIRE( bst.contains(5)   == false );
    REQUIRE( bst.size()        == 6     );
    REQUIRE( bst.balance()     <= 1     );
    
    REQUIRE( bst.contains(58)  == true  );
    bst.remove(58);
    REQUIRE( bst.contains(58)  == false );
    REQUIRE( bst.size()        == 5     );
    REQUIRE( bst.balance()     <= 1     );
    
    // Key =38 still in BST
    REQUIRE( bst.lookup(38)    == 'e'   );
    
    REQUIRE( bst.contains(59)  == true  );
    bst.remove(59);
    REQUIRE( bst.contains(59)  == false );
    REQUIRE( bst.size()        == 4     );
    REQUIRE( bst.balance()     <= 1     );
    
    REQUIRE( bst.contains(60)  == true  );
    bst.remove(60);
    REQUIRE( bst.contains(60)  == false );
    REQUIRE( bst.size()        == 3     );
    REQUIRE( bst.balance()     <= 1     );
    
    // Key =55 still in BST
    REQUIRE( bst.lookup(55)    == 'g'   );
    
    REQUIRE( bst.contains(32)  == true  );
    bst.remove(32);
    REQUIRE( bst.contains(32)  == false );
    REQUIRE( bst.size()        == 2     );
    REQUIRE( bst.balance()     <= 1     );
    
    REQUIRE( bst.contains(55)  == true  );
    bst.remove(55);
    REQUIRE( bst.contains(55)  == false );
    REQUIRE( bst.size()        == 1     );
    REQUIRE( bst.balance()     <= 1     );
    
    // Key =38 still in BST
    REQUIRE( bst.contains(38)  == true  );
    REQUIRE( bst.lookup(38)    == 'e'   );
    bst.remove(38);
    REQUIRE( bst.contains(38)  == false );
    REQUIRE( bst.size()        == 0     );
    REQUIRE( bst.balance()     <= 1     );
    
}

TEST_CASE( "BST Insertion Algorithm Test" ) {
    
    // This test will test insertion algorithm by Height of the Tree
    // It will insert 100 nodes all in increasing fashion
    //
    // Height of Random Buil tree is O(logN)
    // Random Insertion Worst Mean=2Lg(N) = 2(log2 100 )=2(6.64) = 13.24
    // Random Insertion Worst Mean=2Lg(N) = 2(log2 1000)=2(9.64) = 18.65
    // BSTRAND Worst Case is N-1
    // FOR AVL a Perfectly balanced tree has height of logBase2(N) if N=1000 Height=9
    bst.clear();
    for ( int x=0; x<1000; x++ )
    {
        bst.insert(x,'a');
    }
    
    REQUIRE( bst.size()     == 1000  );
    REQUIRE( bst.height()   <= 10    );  // BSTAVL=9  BSTRAND=170   BSTLEAF/ROOT=1000
    REQUIRE( bst.balance()  <= 1     );  // BSTAVL=0  BSTRAND=170   BSTLEAF/ROOT=1000
    std::cout<<"BSTAVL: Inserting Ascending Keys 0-1000 Balance= "<<bst.balance()<<" Height ="<<bst.height()<<std::endl;
    
    // Now lets do random numbers 0-1000 to see how the height changes
    bst.clear();
    REQUIRE( bst.size()     == 0     );
    for ( int x=0; x<1000; x++ )
    {
        bst.insert((rand()%1000),'a');
    }
    // Note:  <= 1000 cause keys duplicated cause random
    REQUIRE( bst.size()    <=  1000  );
    REQUIRE( bst.height()  <=  12    );  // BSTAVL=11  BSTRAND=21  BSTLEAF/BSROOT=
    REQUIRE( bst.balance() <=  1     );  // BSTAVL=1   BSTRAND=3   BSTLEAF/BSROOT=
    std::cout<<"BSTAVL: Inserting Random    Keys 0-1000 Balance=  "<<bst.balance()<<" Height ="<<bst.height()<<std::endl;
    
}


TEST_CASE( "BST Copy Constructor/Assignment Test" ) {
    
    bst.clear();
    bst.insert( 53, 'Z');
    bst.insert( 70, 'D');
    bst.insert( 48, 'E');
    bst.insert( 28, 'F');
    bst.insert( 18, 'G');
    bst.insert( 88, 'A');
    bst.insert( 35, 'C');
    bst.insert( 54, 'H');
    bst.insert( 55, 'I');
    bst.insert( 92, 'J');
    bst.insert( 30, 'K');
    bst.insert( 22, 'M');
    REQUIRE( bst.size()  == 12 );
    
    // Make new List DST (dst) using the Copy Constructor
    cop3530::BSTAVL<int, char, compare_Kint, is_equals_Kint> dst(bst);
    REQUIRE( dst.size()  == 12 );
    REQUIRE( bst.contains(22) == true  );
    REQUIRE( bst.lookup(22)   == 'M'   );
    REQUIRE( bst.contains(15) == false );
    
    // Now Add to bst list
    bst.insert( 36, 'L');
    bst.insert( 60, 'O');
    REQUIRE( bst.size()  == 14 );
    // Copy should still be at 12 nodes
    REQUIRE( dst.size()  == 12 );
    
    // now Add to dst list
    dst.insert( 37, 'Q');
    dst.insert( 52, 'R');
    dst.insert( 62, 'S');
    dst.insert( 63, 'T');
    // Copy should now be at 16 nodes
    REQUIRE( dst.size()  == 16 );
    // Original should be at 14
    REQUIRE( bst.size()  == 14 );
    
    // now assign dst to current bst
    // so dst = current size of bst
    dst = bst;
    REQUIRE( dst.size()  == 14 );
    // These keys should not be in dst since
    // we copied bst over
    REQUIRE( dst.contains(37) == false );
    REQUIRE( dst.contains(63) == false );
    
    // These keys,values should be in DST
    // since copy was made
    REQUIRE( dst.contains(36) == true  );
    REQUIRE( dst.lookup(36)   == 'L'   );
    REQUIRE( dst.lookup(60)   == 'O'   );
    REQUIRE( dst.contains(60) == true  );
    
}


