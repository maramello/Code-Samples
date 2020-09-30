/*****************************************************************************/
/*!
\file   List.cpp
\author Rohit Saini
\par    email: rohitsaini429@gmail.com
\brief
    This file contains the implementation of the following functions for a
    Linked List implementation.

    Functions include:

      + Node(value)
      + new_node(value)
      + List
      + List(list) (copy constructor)
      + List(array, size)
      + operator= (copy assignment)
      + operator+= (self-addition of lists)
      + operator+ (addition of lists)
      + operator[] (array subscripting of lists)
      + push_front
      + push_back
      + pop_front
      + front
      + size
      + empty
      + clear
      + list_count
      + node_count
*/
/*****************************************************************************/

//Assign the initial number of alive nodes
template <typename T>
int List<T>::Node::nodes_alive = 0;

/**************************************************************************/
/*!
  \brief
    Gets the number of Nodes that are still alive in the given list.

  \return
    Returns the number of alive nodes in the list.
*/
/**************************************************************************/
template <typename T>
int List<T>::node_count(void)
{
  return Node::nodes_alive;
}

/**************************************************************************/
/*!
  \brief
    Initializes the variables of a newly created node structure.

  \param value
    The value of the node's data variable to be stored.

*/
/**************************************************************************/
template <typename T>
List<T>::Node::Node(T value) : data(value)
{
  //Count new number of nodes alive
  nodes_alive++;
}

/**************************************************************************/
/*!
  \brief
    Destroys a node structure.
*/
/**************************************************************************/
template <typename T>
List<T>::Node::~Node()
{
  //Decrease nodes alive since a node is being destroyed
  nodes_alive--;
}

/**************************************************************************/
/*!
  \brief
    Initializes the variables of a newly created List.
*/
/**************************************************************************/
template <typename T>
List<T>::List()
{
  //Set the head and tail to NULL.
  head_ = NULL;
  tail_ = NULL;

  //Initialize the size to 0.
  size_ = 0;
}

/**************************************************************************/
/*!
  \brief
    Initializes the variables of a newly created List with the contents of
    the given list.

  \param list
    The list to copy the contents of and initialize with.
*/
/**************************************************************************/
template <typename T>
List<T>::List(const List& list)
{
  //Set the head and tail to NULL.
  head_ = NULL;
  tail_ = NULL;

  //Initialize the size to 0.
  size_ = 0;

  //Start at the list's header node.
  Node* current = list.head_;

  //Loop through the list's nodes.
  while (current)
  {
    //Push the list's nodes onto this list.
    push_back(current->data);

    //Move to the next node.
    current = current->next;
  }

}

/**************************************************************************/
/*!
  \brief
    Initializes the variables of a newly created List with the contents of
    an array with a specified size.

  \param array
    The array of integers containing the node's values to create.

  \param size
    The size of the array.
*/
/**************************************************************************/
template <typename T>
List<T>::List(const T *array, int size)
{
  //Set the head and tail to NULL.
  head_ = NULL;
  tail_ = NULL;

  //Initialize the size to 0.
  size_ = 0;

  //Loop through the array's elements and push them onto the List.
  for (int i = 0; i < size; i++)
    push_back(array[i]);
}

/**************************************************************************/
/*!
  \brief
    Frees all memory associated with the given List.
*/
/**************************************************************************/
template <typename T>
List<T>::~List()
{
  //Delete all the nodes from the list.
  clear();
}

/**************************************************************************/
/*!
  \brief
     Clears a list by deleting all of its nodes.
*/
/**************************************************************************/
template <typename T>
void List<T>::clear()
{
  //While the list is not empty, remove the first element
  while (!empty())
    pop_front();
}

/**************************************************************************/
/*!
  \brief
    Copies the contents of the given list into this class' list and
    returns a self-reference.

  \param rhs
    The list to copy the contents of and initialize with.

  \return
    Returns a reference to this class' list after assigning the new
    values.
*/
/**************************************************************************/
template <typename T>
List<T>& List<T>::operator=(const List& rhs)
{
  //Make sure there is no self assignment
  if (&rhs != this)
  {
    //Set the head and tail to NULL.
    head_ = NULL;
    tail_ = NULL;

    //Initialize the size to 0.
    size_ = 0;

    //Start at the list's header node.
    Node* current = rhs.head_;

    //Loop through the list's nodes.
    while (current)
    {
      //Push the list's nodes onto this list.
      push_back(current->data);

      //Move to the next node.
      current = current->next;
    }
  }

  //Return a reference to this class
  return *this;
}

/**************************************************************************/
/*!
  \brief
    Performs self-addition on a given list and this class' list.

  \param rhs
    The list to use to add values to this class' list.

  \return
    Returns a reference to this class' list after adding the new values.
*/
/**************************************************************************/
template <typename T>
List<T>& List<T>::operator+=(const List& rhs)
{
  //Start at the header node of the given list.
  Node* current = rhs.head_;

  //Loop while there is another node.
  while (current)
  {
    //Push a node with the same value onto this class' list.
    push_back(current->data);

    //Move to the next node.
    current = current->next;
  }

  //Return a self-reference.
  return *this;
}

/**************************************************************************/
/*!
  \brief
    Adds 2 lists together (concatenates them) by creating a new one and
    putting values into it.

  \param rhs
    The list to add to this class' list.

  \return
    Returns a new List containing both the lists values.
*/
/**************************************************************************/
template <typename T>
List<T> List<T>::operator+(const List& rhs) const
{
  //Create a new list starting with the LHS (this class' list).
  List<T> newList(*this);

  //Start at the new list's header node.
  Node* current = rhs.head_;

  //Loop while there is a node in the list.
  while (current)
  {
    //Push that node's data onto the new list.
    newList.push_back(current->data);

    //Update the node
    current = current->next;
  }

  //Return the newly created list.
  return newList;
}

/**************************************************************************/
/*!
  \brief
    Gets the value of the specific data variable of a node at the given
    index in the list.

  \param index
    The index of the list from which to obtain the node value.

  \return
    Returns an constant integer reference to the node's data variable at
    the given index.
*/
/**************************************************************************/
template <typename T>
const T& List<T>::operator[](int index) const
{
  //If the index is out of bounds, return the first node's value.
  if (index <= 0 || index > size() - 1)
    return head_->data;

  //Start at the header node.
  Node* toReturn = head_;

  //Loop until you reach the specified index.
  for (int i = 0; i <= index; i++)
  {
    //If the index is found, return the data variable of that node.
    if (i == index)
      return toReturn->data;

    //Otherwise move to the next node.
    toReturn = toReturn->next;
  }

  //If code reaches here, the index was never found, so return first value.
  return head_->data;
}

/**************************************************************************/
/*!
  \brief
    Gets the address of the specific data variable of a node at the given
    index in the list.

  \param index
    The index of the list from which to obtain the address.

  \return
    Returns an integer reference to the node's data variable at the given
    index.
*/
/**************************************************************************/
template <typename T>
T& List<T>::operator[](int index)
{
  //If the index is out of bounds, return the first node's value.
  if (index <= 0 || index > size() - 1)
    return head_->data;

  //Start at the header node.
  Node* toReturn = head_;

  //Loop until you reach the specified index.
  for (int i = 0; i <= index; i++)
  {
    //If the index is found, return the data variable of that node.
    if (i == index)
      return toReturn->data;

    //Otherwise move to the next node.
    toReturn = toReturn->next;
  }

  //If code reaches here, the index was never found, so return first value.
  return head_->data;
}

/**************************************************************************/
/*!
  \brief
    Adds a node to the front of the list. If no nodes exist, then a header
     node is created.

  \param value
    The value of the new node to be added to the front of the list.
*/
/**************************************************************************/
template <typename T>
void List<T>::push_front(const T& value)
{
  //If there is no header node, create one.
  if (!head_)
  {
    //Allocate new header node.
    head_ = new_node(value);

    //Update the tail node.
    tail_ = head_;

    //Update the node count and list size.
    size_++;
    Node::nodes_alive++;

    //Return since a node has been added.
    return;
  }

  //Create the new header node.
  Node* newHead = new_node(value);

  //Make the new header node's next the current head.
  newHead->next = head_;

  //Update the header node.
  head_ = newHead;

  //Increment the node count and list size.
  Node::nodes_alive++;
  size_++;
}

/**************************************************************************/
/*!
  \brief
    Adds a node to the end of the list. If no nodes exist, then a header
     node is created.

  \param value
    The value of the new node to be added to the end of the list.
*/
/**************************************************************************/
template <typename T>
void List<T>::push_back(const T& value)
{
  //If there is no header node, create one.
  if (!head_)
  {
    //Allocate new header node.
    head_ = new_node(value);

    //Update the tail node.
    tail_ = head_;

    //Increment the node count and list size.
    size_++;
    Node::nodes_alive++;

    //Return since the node has been added already.
    return;
  }

  //If code reaches here, just add to the end of the list using the tail.
  tail_->next = new_node(value);

  //Update the tail pointer.
  tail_ = tail_->next;

  //Increment the node count and list size.
  size_++;
  Node::nodes_alive++;
}

/**************************************************************************/
/*!
  \brief
     Removes the very first item in the list (if it exists). If the first
     node does not exist, nothing is done.
*/
/**************************************************************************/
template <typename T>
void List<T>::pop_front()
{

  //Only remove if the first node exists.
  if (head_)
  {
    //Store the node to delete.
    Node* toDelete = head_;

    //Preserve the link of the list by updating the header node.
    head_ = head_->next;

    //Delete the old header node.
    delete toDelete;

    //Update the nodes alive and the size of the list.
    Node::nodes_alive--;
    size_--;
  }

}

/**************************************************************************/
/*!
  \brief
     Checks the value of the first item in the list.

  \return
    Returns the value of the first node in the list.
*/
/**************************************************************************/
template <typename T>
T List<T>::front() const
{
  //Return header node's T value (data).
  return head_->data;
}

/**************************************************************************/
/*!
  \brief
     Checks the number of items that are currently in the list.

  \return
    Returns the number of items in the list.
*/
/**************************************************************************/
template <typename T>
int List<T>::size() const
{
  //Return the value of the number of nodes from the list class.
  return size_;
}

/**************************************************************************/
/*!
  \brief
     Checks whether the given list is empty or not.

  \return
    Returns true if the list is not empty, and false if the list is empty.
*/
/**************************************************************************/
template <typename T>
bool List<T>::empty() const
{
  //Return whether the list is empty or not by checking if the size is 0.
  return (size_ == 0);
}

/**************************************************************************/
/*!
  \brief
    Creates a node and returns it to the calling code. Initializes the
    data and next variables.

  \param data
    The value to store in the new node's data variable.

  \return
    Returns a pointer to the newly created node.
*/
/**************************************************************************/
template <typename T>
typename List<T>::Node *List<T>::new_node(const T& data) const
{
  Node *node = new Node(data); // create the node
  node->next = 0;              // no next pointer yet

  return node;
}

#include <iomanip> //ostream, setw, endl

/*!**********************************************************************
  Outputs all of the data in a list to an output stream.

  \param os
    The stream to output the list to.

  \param list
    The list to output to the stream.

  \return
    The ouput stream (ref) that was passed in (for chaining)
************************************************************************/
template <typename T>
std::ostream &CS170::operator<<(std::ostream & os, const CS170::List<T> &list)
{
  //Start at the beginning
  typename CS170::List<T>::Node *pnode = list.head_;

  //Print each item
  while (pnode != 0)
  {
    //Keep consistent spacing
    os << std::setw(4) << pnode->data;
    pnode = pnode->next;
  }

  //Print a new line for readability
  os << std::endl;

  //Returns the stream which was written to
  return os;
}
