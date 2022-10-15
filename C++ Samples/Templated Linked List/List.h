/*****************************************************************************/
/*!
\file   List.h
\author Rohit Saini
\par    email: rohitsaini429@gmail.com

\brief
    This file contains the definition of functions for the a templated 
    Linked List implementation in C++.
*/
/*****************************************************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream> /* ostream, endl */

//! Declaration of class List
template <typename T> class List;

//! Definiton of output operator function
template <typename T>
std::ostream & operator<<(std::ostream & os, const List<T> &list);

//! The list class
template <typename T>
class List
{
  public:

    //! Default constructor
    List();

    //! Copy contructor for constructing a list from an existing list
    List(const List &list);

    //! Contruct a list from a T array
    List(const T *array, int size);

    //! Destructor
    ~List();

    //! adds the item to the front of the list
    void push_front(const T& Value);
    //! adds the item to the end of the list
    void push_back(const T& Value);
    //! removes the first item in the list
    void pop_front();

    //! retrieves the first item in the list
    T front() const;

    //! returns the number of items in list
    int size() const;

    //! true if empty, else false
    bool empty() const;

    //! clears the list
    void clear();

    //! Overloaded assignment operator (=) for assigning one list to another
    List& operator=(const List &list);

    //! Overloaded addition operator (+) for adding two lists
    List operator+(const List &list) const;

    //! Overloaded addition/assignment (+=) for adding to a list "in place"
    List& operator+=(const List &list);

    //! Overloaded subscript operator (get)
    const T& operator[](int index) const;

    //! Overloaded subscript operator (set)
    T& operator[](int index);

    /*!**********************************************************************
      Outputs all of the data in a list to an output stream.

      \param os
        The stream to output the list to.

      \param list
        The list to output to the stream.

      \return
        The ouput stream (ref) that was passed in (for chaining)
    ************************************************************************/
    friend std::ostream& operator<< <T>(std::ostream & os, const List &list);

    //! Returns the number of Nodes that have been created
    static int node_count();

  private:

    //! Used to build the linked list
    struct Node
    {
      //! constructor
      Node(T value);

      //! destructor
      ~Node();

      //! pointer to the next Node
      Node *next;

      //! the actual data in the node
      T data;

      //! number of Nodes created
      static int nodes_alive;
    };

    //! pointer to the head of the list
    Node *head_;

    //! pointer to the last node
    Node *tail_;

    //! number of items on the list
    int size_;

    //! All nodes are created in this method
    Node *new_node(const T& data) const;
};

#include "List.cpp"

#endif
