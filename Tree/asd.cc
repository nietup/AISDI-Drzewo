/**
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <time.h>

#include "TreeMap.h"

static TreeMap::Val DEFAULT_VALUE = "Automatically generated node";
static TreeMap::Key DEFAULT_KEY = -1;

/// A helper class.
class TreeMapDetail {                       //Helper
public:
    ~TreeMapDetail () {
        delete (sentinel);
    }

protected:
    friend class TreeMap;
    TreeMap::Node * sentinel;

    void setSentinel (TreeMap::Node * newSentinel) {
        sentinel = newSentinel;
    }

    TreeMap::Node * getSentinel () {
        return sentinel;
    }

    void copy (const TreeNode* from, TreeNode* toParent, bool lessThan) {

        if (from == NULL)
            return;

        TreeNode* newNode = new TreeNode (from->data, toParent);

        if (lessThan) {
            if (toParent != NULL)
                toParent->left = newNode;
        }

        else {
            if (toParent != NULL)
                toParent->right = newNode;
        }

        copy (from->left, newNode, true);
        copy (from->right, newNode, false);
    }
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

/*
Koncepcja:
root jest zwyklym nodem przechowujacym wartosc
jego parentem jest straznik
left straznika to rool, right to NULL
w sumie nie ma to za wiele sensu, ale juz tak zaczalem wiec sie tego trzymam
*/


TreeMap::TreeMap () {
    root = NULL;
    TreeNode * sentinel = new TreeNode (std::make_pair (DEFAULT_KEY, DEFAULT_VALUE));
    sentinel->left = root;
    detail = new TreeMapDetail ();
    detail->setSentinel (sentinel);
};

/// Content of existing TreeMap object is copied into the new object.
TreeMap::TreeMap (const TreeMap& m) {
    TreeNode * sentinel = new TreeNode (std::make_pair (DEFAULT_KEY, DEFAULT_VALUE));
    detail = new TreeMapDetail ();
    detail->setSentinel (sentinel);
    detail->copy (m.root, sentinel, true);
};

TreeMap::~TreeMap () {
    clear ();
    delete (detail);
};

// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert (const std::pair<Key, Val>& entry) {
    iterator insertIterator = find (entry.first);

    if (insertIterator == end ())                                        //nie ma jeszcze takiego klucza
        return std::make_pair (unsafe_insert (entry), (bool)true);

    return std::make_pair (insertIterator, (bool)false);
}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.

TreeMap::iterator TreeMap::unsafe_insert (const std::pair<Key, Val>& entry) {

    if (root == NULL) {
        root = new Node (entry);
        root->parent = detail->getSentinel ();
        (detail->getSentinel ())->left = root;              //dla pewnosci
        return iterator (root);
    }

    Node * currentNode = root;

    while (true) {
        if (currentNode->data.first > entry.first) {
            if (currentNode->left != NULL)
                currentNode = currentNode->left;
            else {
                Node * newNode = new Node (entry);
                newNode->parent = currentNode;
                currentNode->left = newNode;
                return iterator (newNode);
            }
        }

        else if (currentNode->data.first < entry.first) {
            if (currentNode->right != NULL)
                currentNode = currentNode->right;
            else {
                Node * newNode = new Node (entry);
                newNode->parent = currentNode;
                currentNode->right = newNode;
                return iterator (newNode);
            }
        }
    }
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find (const Key& k) {
    Node * currentNode = root;

    if (root == NULL)
        return end ();

    while (true) {
        if (currentNode->data.first == k)
            return iterator (currentNode);

        if (currentNode->data.first > k) {
            if (currentNode->left != NULL)
                currentNode = currentNode->left;
            else
                return end ();
        }

        else if (currentNode->data.first < k) {
            if (currentNode->right != NULL)
                currentNode = currentNode->right;
            else
                return end ();
        }
    }
}

TreeMap::const_iterator TreeMap::find (const Key& k) const {
    if (root == NULL)
        return end ();

    Node * currentNode = root;

    while (true) {
        if (currentNode->data.first == k)
            return const_iterator (currentNode);

        if (currentNode->data.first > k) {
            if (currentNode->left != NULL)
                currentNode = currentNode->left;
            else
                return end ();
        }

        else if (currentNode->data.first < k) {
            if (currentNode->right != NULL)
                currentNode = currentNode->right;
            else
                return end ();
        }
    }
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k) {
    iterator found = find (k);

    if (found == end ())
        found = unsafe_insert (std::make_pair (k, DEFAULT_VALUE));

    return found->second;
}

// Tests if a map is empty.
bool TreeMap::empty () const {
    return (begin ()) == end ();
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size () const {
    TreeMap::size_type number = 0;

    for (const_iterator i = begin (); i != end (); i++)
        number++;

    return number;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count (const Key& _Key) const {
    if (find (_Key) == end ())
        return 0;

    return 1;  // this is not a multimap
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase (TreeMap::iterator i) {
    if (i == end ())
        return i;

    bool wasRoot = false;
    if (i.node == root)
        wasRoot = true;

    iterator returnIterator = i;
    returnIterator++;

    //najprostrzy przypadek
    if (i.node->left == NULL && i.node->right == NULL) {

        if (i.node == i.node->parent->left)
            i.node->parent->left = NULL;
        else
            i.node->parent->right = NULL;

        if (wasRoot)
            root = NULL;

        delete (i.node);
        return returnIterator;
    }

    //jedna raczka wolna
    if (i.node->left == NULL && i.node->right != NULL) {

        if (wasRoot)
            root = i.node->right;

        if (i.node == i.node->parent->left)
            i.node->parent->left = i.node->right;
        else
            i.node->parent->right = i.node->right;

        i.node->right->parent = i.node->parent;
        delete (i.node);
        return returnIterator;
    }

    if (i.node->left != NULL && i.node->right == NULL) {

        if (wasRoot)
            root = i.node->left;

        if (i.node == i.node->parent->left)
            i.node->parent->left = i.node->left;
        else
            i.node->parent->right = i.node->left;

        i.node->left->parent = i.node->parent;
        delete (i.node);
        return returnIterator;
    }

    //najgorszy przypadek: obie raczki zajete :c

    Node * temporaryNode = i.node->right;

    while (temporaryNode->left != NULL)
        temporaryNode = temporaryNode->left;

    //spr czy przenoszony wezel ma synka:
    if (temporaryNode->right != NULL) {
        if (temporaryNode == temporaryNode->parent->left)           //przepiecie od rodzice
            temporaryNode->parent->left = temporaryNode->right;
        else
            temporaryNode->parent->right = temporaryNode->right;

        temporaryNode->right->parent = temporaryNode->parent;       //przepiecie od synka

        temporaryNode->parent = i.node->parent;                     //wpiecie zamiennika
        temporaryNode->left = i.node->left;
        temporaryNode->right = i.node->right;

        if (i.node == i.node->parent->left)                         //nowy rodzic
            i.node->parent->left = temporaryNode;
        else
            i.node->parent->right = temporaryNode;

        //synki
        i.node->left->parent = temporaryNode;
        i.node->right->parent = temporaryNode;

        if (wasRoot)
            root = temporaryNode;

        delete (i.node);
        return returnIterator;
    }

    else {
        if (temporaryNode == temporaryNode->parent->left)           //przepiecie od rodzice
            temporaryNode->parent->left = NULL;
        else
            temporaryNode->parent->right = NULL;

        temporaryNode->parent = i.node->parent;
        temporaryNode->left = i.node->left;
        temporaryNode->right = i.node->right;

        if (i.node == i.node->parent->left)
            i.node->parent->left = temporaryNode;
        else
            i.node->parent->right = temporaryNode;

        i.node->left->parent = temporaryNode;
        i.node->right->parent = temporaryNode;

        if (wasRoot)
            root = temporaryNode;

        delete (i.node);
        return returnIterator;
    }
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase (TreeMap::iterator f, TreeMap::iterator l) {
    TreeMap::iterator temporary = f;
    while (temporary != l)
        temporary = erase (temporary);
    return temporary;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase (const Key& key) {
    iterator toKill = find (key);

    if (toKill != end ()) {
        erase (toKill);
        return 1;
    }

    return 0;
}

// Erases all the elements of a map.
void TreeMap::clear () {
    erase (begin (), end ());
}

bool TreeMap::struct_eq (const TreeMap& another) const {
    if (size () != another.size ())
        return false;

    const_iterator thisIterator = begin ();
    const_iterator anotherIterator = another.begin ();

    while (thisIterator != end ()) {

        if (thisIterator.node->data != anotherIterator.node->data)
            return false;

        ++thisIterator;
        ++anotherIterator;
    }

    return true;
}

//@ACHTUNG kk this is so random i dont believe its actually gonna work, but its good for start
bool TreeMap::info_eq (const TreeMap& another) const {
    if (size () != another.size ())
        return false;

    const_iterator thisIterator = begin ();

    while (thisIterator != end ()) {

        if (another.find (thisIterator->first) == another.end () || another.find (thisIterator->first)->second != thisIterator->second) {
            /*std::cout << "\nWykrzaczing with parameters:\n"
                << "another.find (thisIterator->first) == another.end () has value of: " << (another.find (thisIterator->first) == another.end ())
                << "\nanother.find (thisIterator->first)->second != thisIterator->second) has value of: " << (another.find (thisIterator->first)->second != thisIterator->second)
                << "\n\nWykrzaczejszyn executed for parameters:"
                << "\nFor this: key == " << thisIterator->first << ", value == " << thisIterator->second << "\n\n";*/
            return false;
        }

        thisIterator++;
    }

    return true;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++() {
    if (node->parent == NULL)   //end
        return *this;

    if (node->right != NULL) {
        node = node->right;

        while (node->left != NULL) {
            node = node->left;
        }

        return *this;
    }

    if (node->parent->left == node) {
        node = node->parent;
        return *this;
    }

    //return end:
    while (node->parent->right == node)    // w ten sposob dojdziemy do enda
        node = node->parent;
    node = node->parent;                   //root to left enda

    return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int) {
    const_iterator notIncremented (node);    //Trzeba zinkrementowac, ale zwrocic sprzed inkrementacji
    ++(*this);
    return notIncremented;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {
    if (node->parent == NULL) {                 //if end we must reach last elemento
        if (node->left == NULL)
            return *this;

        node = node->left;

        while (node->right != NULL) {
            node = node->right;
        }

        return *this;
    }

    if (node->left != NULL) {
        node = node->left;

        while (node->right != NULL) {
            node = node->right;
        }

        return *this;
    }

    if (node->parent->right == node) {
        node = node->parent;
        return *this;
    }

    Node * SafeNode = node;          //lista nie jest cykliczna wiec jesli damy -- na siebie to jestesmy dalej soba

    while (node->parent->left == node)
        node = node->parent;

    if (node->parent != NULL)
        node = node->parent;

    else
        node = SafeNode;

    return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int) {
    const_iterator notDecremented (node);
    --(*this);
    return notDecremented;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& other) {
    if (*this == other) return *this;

    this->clear ();

    detail->copy (other.root, detail->getSentinel (), true);

    return *this;
}

/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin () {

    Node * actualNode = detail->getSentinel ();

    while (actualNode->left != NULL) {
        actualNode = actualNode->left;
    }

    return iterator (actualNode);
}

TreeMap::const_iterator TreeMap::begin () const {

    Node * actualNode = detail->getSentinel ();

    while (actualNode->left != NULL) {
        actualNode = actualNode->left;
    }

    return const_iterator (actualNode);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end () {
    return iterator (detail->getSentinel ());
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end () const {
    return const_iterator (detail->getSentinel ());
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print (const std::pair<int, std::string>& p) {
    std::cout << p.first << ", " << p.second << std::endl;
}

#include <map>

/// The big mean test function ;)
void test () {
    std::cout << "Testy uzytkownika" << std::endl;

    TreeMap m;

    m[2] = "Merry";
    m[4] = "Jane";
    m[8] = "Korwin";
    m[4] = "Magdalena";
    m[9] = "Mateusz";
    for (int i = 10; i < 20; i++) m[i];

    for_each (m.begin (), m.end (), print);

    TreeMap::iterator testIterator1, testIterator2;

    testIterator1 = testIterator2 = ++(m.begin ());

    for (int i = 0; i < 7; i++)
    ++testIterator2;

    std::cout << "\n--------------\n";
    for_each (testIterator1, testIterator2, print);

    /*/m.erase (m.begin(), m.end());
    testIterator1 = m.end();
    testIterator1--;
    for (; testIterator1 != m.begin(); --testIterator1) {
        std::cout << "\nUSUWAM TAKIE: ";
        //for_each (testIterator1, m.end(), print);
        m.erase(testIterator1);
    }

    std::cout << "\n--------------\n";
    for_each (m.begin (), m.end (), print);*/
}
