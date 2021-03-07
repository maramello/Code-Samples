/*****************************************************************************/
/**
@file   lariat.cpp
@author Rohit Saini
@date   2/14/2021
@brief
  This file contains implementation of functions for the Lariat class, a
  linked list of arrays. Templatized.
*/
/*****************************************************************************/
#include <iostream>
#include <iomanip>

#if 1

//Helper functions

/**************************************************************************/
/**
 * @brief
 *  returns element found in list with local index
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param globalIndex - global index of value to find
 * @param result - resultant node to store in
 *
 * @return
 * returns the local index of the node
 */
/**************************************************************************/
template <typename T, int Size>
int Lariat<T, Size>::findElement(int globalIndex, LNode** result) const
{
    //start at 0 index for search
    int index = 0;

    // Start from current node
    LNode *start = head_;

    //for first node exception
    bool first = true;

    //loop through lists
    while (start)
    {
        if (first)
        {
            index += (start->count - 1);
            first = false;
        }

        //increment index start
        else {
            index += start->count;
        }

        //no next node
        if (!start->next)
            break;

        //make sure next wouldn't have nothing
        if (index >= globalIndex || ((index + 1) == globalIndex && (!start->next)))
            break;

        start = start->next;
    }

    //store resultant node
    *result = start;

    //return local index
    return (globalIndex - (index - start->count + 1));
}

//splits a node into two
/**************************************************************************/
/**
 * @brief
 *  splits a node in two
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param fullNode - full node before splitting
 * @param localIndex - local index
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::split(LNode* fullNode, int localIndex)
{
    //create new node
    LNode* newNode = new LNode;

    nodecount_++;
    newNode->prev = fullNode;
    newNode->next = fullNode->next;
    newNode->count = 0;

    if (fullNode->next == nullptr)
        //make sure tail pointer is last node
        tail_ = newNode;
    else
        fullNode->next->prev = newNode;

    fullNode->next = newNode;

    //get correct new number of elements after split for the full node
    int newCount = asize_ / 2; // assume a start in the middle

    // even numbered array size and insertion will be in the next node (not this one)
    if (asize_ % 2 == 0 && localIndex > (asize_ / 2))
        newCount = (asize_ / 2) + 1;

    if (asize_ % 2 != 0 && localIndex > (asize_ / 2))
        newCount = (asize_ + 1) / 2;

    //loop through elements to the right of the center and assign the values
    for (int i = newCount; i < asize_; i++)
    {
        newNode->values[newNode->count] = fullNode->values[i];
        fullNode->count--;
        newNode->count++;
    }
}

/**************************************************************************/
/**
 * @brief
 *  shifts all elements up
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param node - node to shift up
 * @param index - index to shift up
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::shiftUp(LNode *node, int index)
{
    //swap all values with next one
    for (int i = node->count; i > index; i--)
    {
        SwapValues(node->values[i], node->values[i-1]);
    }
}

//swaps all elements from index to the left
/**************************************************************************/
/**
 * @brief
 *  shifts all elements down
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param node - node to shift down
 * @param index - index to shift down
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::shiftDown(LNode *node, int index)
{
    //swap all values with next one
    for (int i = index; i < node->count - 1; i++)
    {
        SwapValues(node->values[i], node->values[i+1]);
    }
}

/**************************************************************************/
/**
 * @brief
 *  swaps 2 elements
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param A - value to swap
 * @param B - second value to swap
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::SwapValues(T& A, T& B)
{
    T temp = A;
    A = B;
    B = temp;
}

/**************************************************************************/
/**
 * @brief
 *  constructor
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 */
/**************************************************************************/
template <typename T, int Size>
Lariat<T, Size>::Lariat() : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{

}

/**************************************************************************/
/**
 * @brief
 *  copy constructor
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param rhs - lariat to copy
 *
 */
/**************************************************************************/
template <typename T, int Size>
Lariat<T, Size>::Lariat( Lariat const& rhs) : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    auto start = rhs.head_;

    while (start)
    {
        for (int i = 0; i <  start->count; i++)
        {
            push_back(start->values[i]);
        }

        start = start->next;
    }
}

/**************************************************************************/
/**
 * @brief
 *  copy constructor
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @tparam T2 - other type
 * @tparam Size2 - other count
 *
 * @param rhs - lariat to copy
 *
 */
/**************************************************************************/
template <typename T, int Size>
template<class T2, int Size2>
Lariat<T, Size>::Lariat( Lariat<T2, Size2> const& rhs) : head_(nullptr), tail_(nullptr), size_(0), nodecount_(0), asize_(Size)
{
    auto start = rhs.head_;

    while (start)
    {
        for (int i = 0; i < start->count; i++)
        {
            push_back(static_cast<T>(start->values[i]));
        }

        start = start->next;
    }
}

/**************************************************************************/
/**
 * @brief
 *  operator=
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param rhs - lariat to copy
 *
 * @return
 * returns the lariat reference
 */
/**************************************************************************/
template <typename T, int Size>
Lariat<T, Size>& Lariat<T, Size>::operator=( Lariat const& rhs)
{
    if (this != &rhs)
    {
        clear();

        auto *start = rhs.head_;

        while (start) {
            for (int i = 0; i < start->count; i++) {
                push_back(start->values[i]);
            }

            start = start->next;
        }

    }
    return *this;
}

/**************************************************************************/
/**
 * @brief
 *  operator=
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @tparam T2 - other type
 * @tparam Size2 - other count
 *
 * @param rhs - lariat to copy
 *
 * @return
 * returns the lariat reference
 */
/**************************************************************************/
template <typename T, int Size>
template<class T2, int Size2>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat<T2, Size2> const& rhs)
{
    clear();

    auto *start = rhs.head_;

    while (start) {
        for (int i = 0; i < start->count; i++) {
            push_back(static_cast<T>(start->values[i]));
        }

        start = start->next;
    }


    return *this;
}

/**************************************************************************/
/**
 * @brief
 *  Destructor
 */
/**************************************************************************/
template <typename T, int Size>
Lariat<T, Size>::~Lariat()
{
    clear();
}

/**************************************************************************/
/**
 * @brief
 *  Inserts element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param index        - index to insert at
 * @param value - Value of new node
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::insert(int index, const T& value)
{
    //check for out of boundary condition
    if (index < 0 || index > size_)
    {
        throw(LariatException(LariatException::LARIAT_EXCEPTION::E_BAD_INDEX, "Subscript is out of range"));
    }

    //check for edge cases
    //insert at front
    if (index == 0)
    {
        push_front(value);
        return;
    }

    //try and put a condition for push back
    else if (index == size_)
    {
        push_back(value);
        return;
    }

    //find node to insert at and the local index
    LNode* node = nullptr;
    volatile int localIndex = findElement(index, &node);

    //insert at the local index
    if (node)
    {
        if (node->count == asize_)
        {
            split(node, localIndex);

            if (localIndex > asize_ / 2)
            {
                node = node->next;
                localIndex = localIndex - (asize_ / 2) - 1;
            }
        }

        shiftUp(node, localIndex);

        node->values[localIndex] = value;
        node->count++;
        size_++;

        return;
    }
}

/**************************************************************************/
/**
 * @brief
 *  push data in front reusing empty positions and delete remaining nodes
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::compact()
{
    //no nodes or only one array worth of elements
    if (!head_ || size_ < asize_)
        return;

    //node to insert in
    LNode *left = head_;

    //node to delete from
    LNode *right = head_->next;

    // Loop while the right foot points at something
    while (right)
    {
        // Check if the left foot can hold more values (use count)
        if (left->count < asize_)
        {
            // store the Number of elements in the right foot to transfer
            int nElements = right->count;

            // set the right foot count to 0
            right->count = 0;
            // loop for the number of elements to transfer
            for (int i = 0; i < nElements; i++)
            {
                // insert in the left node (subscripting with lCount) with a value = rFoot[i]
                left->values[left->count++] = right->values[i];
                // If the left foot count reached capacity
                if (left->count == asize_)
                {
                    // move the left foot forward
                    left = left->next;
                }
            }
        }

        // If the left foot is still in the same node, only move forward if it is filled
        if (left->count == asize_)
            left = left->next;

        // move the right foot forward
        right = right->next;
    }

    //Delete nodes that have a count of 0 and update the tail
    while(tail_->count == 0)
    {
        LNode* temp = tail_;

        tail_ = tail_->prev;

        tail_->next = nullptr;

        delete temp;

        nodecount_--;
    }
}

/**************************************************************************/
/**
 * @brief
 *  returns index, size (one past last) if not found
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param value - value of element to find
 * @return
 * Returns global index of element
 */
/**************************************************************************/
template <typename T, int Size>
unsigned Lariat<T, Size>::find(const T& value) const
{
    LNode* start = head_;
    unsigned globalIndex = 0;

    while (start)
    {
        for (int i = 0; i < start->count; i++)
        {
            if (start->values[i] == value)
                return globalIndex;

            globalIndex++;
        }

        start = start->next;
    }

    return size_;
}

/**************************************************************************/
/**
 * @brief
 *   total number of items (not nodes)
 *
 * @return
 * Returns number of items
 */
/**************************************************************************/
template <typename T, int Size>
size_t Lariat<T, Size>::size(void) const
{
    return size_;
}

/**************************************************************************/
/**
 * @brief
 * make it empty
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::clear(void)
{
    while (head_)
    {
        LNode* temp = head_;

        head_ = head_->next;

        delete temp;

        nodecount_--;
    }

    size_ = 0;
    nodecount_ = 0;
}


/**************************************************************************/
/**
 * @brief
 *  erases a node from the global index given
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param index - value of element to erase
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::erase(int index)
{
    //handle start and end cases
    if (index == 0)
    {
        pop_front();
        return;
    }

    if (index == size_)
    {
        pop_back();
        return;
    }

    //find local node to delete
    LNode *node = nullptr;
    int localIndex = findElement(index, &node);

    if (node)
    {
        shiftDown(node, localIndex);
        node->count--;
        size_--;
    }
}

/**************************************************************************/
/**
 * @brief
 *  remove back element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::pop_back()
{
    if (!tail_)
        return;

    tail_->count--;
    size_--;
}

/**************************************************************************/
/**
 * @brief
 *  remove front element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::pop_front()
{
    if (!head_)
        return;

    shiftDown(head_, 0);
    head_->count--;
    size_--;

    if (head_->count <= 0)
    {
        LNode* toDelete = head_;
        head_ = head_->next;
        head_->prev = nullptr;
        delete toDelete;

        nodecount_--;
    }
}

// for l-values
/**************************************************************************/
/**
 * @brief
 *  subscript operator
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param index - value of element to get
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
T& Lariat<T, Size>::operator[](int index)
{
    LNode* node = nullptr;

    int localIndex = findElement(index, &node);

    if (node)
    {
        return node->values[localIndex];
    }

    throw(LariatException(LariatException::LARIAT_EXCEPTION::E_BAD_INDEX, "Subscript is out of range"));
}

// for r-values
/**************************************************************************/
/**
 * @brief
 *  subscript operator
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param index - value of element to get
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
const T& Lariat<T, Size>::operator[](int index) const
{
    LNode* node = nullptr;

    int localIndex = findElement(index, &node);

    if (node)
    {
        return node->values[localIndex];
    }

    throw(LariatException(LariatException::LARIAT_EXCEPTION::E_BAD_INDEX, "Subscript is out of range"));
}

/**************************************************************************/
/**
 * @brief
 *  return first element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
T& Lariat<T, Size>::first()
{
    return head_->values[0];
}

/**************************************************************************/
/**
 * @brief
 *  return first element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
T const& Lariat<T, Size>::first() const
{
    return head_->values[0];
}

/**************************************************************************/
/**
 * @brief
 *  return last element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
T& Lariat<T, Size>::last()
{
    return tail_->values[tail_->count - 1];
}

/**************************************************************************/
/**
 * @brief
 *  return last element
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @return
 * returns value at given index
 */
/**************************************************************************/
template <typename T, int Size>
T const& Lariat<T, Size>::last() const
{
    return tail_->values[tail_->count - 1];
}

/**************************************************************************/
/**
 * @brief
 *  pushes an element onto the array
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param value - value to push back
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::push_back(const T& value)
{
    if (head_ == nullptr)
    {
        head_ = new LNode;
        nodecount_++;
        head_->next = nullptr;
        head_->prev = nullptr;
        tail_ = head_;
    }

    if (tail_->count == asize_)
    {
        split(tail_, asize_ - 1);
    }

    tail_->values[tail_->count] = value;
    tail_->count++;
    size_++;
}

/**************************************************************************/
/**
 * @brief
 *  pushes an element onto the array
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param value - value to push onto front
 *
 */
/**************************************************************************/
template <typename T, int Size>
void Lariat<T, Size>::push_front(const T& value)
{
    if (head_ == nullptr)
    {
        head_ = new LNode;
        nodecount_++;
        head_->next = nullptr;
        head_->prev = nullptr;
        tail_ = head_;
    }

    //no need for shifting if the header node is empty
    if (head_->count == 0)
    {
        head_->values[0] = value;
        head_->count++;
        size_++;
        return;
    }

    //make sure node has enough space
    if (head_->count == Size)
    {
        //if head is only node in the list, update tail pointer
        if (head_->next == nullptr)
        {
            split(head_, 0);
            tail_ = head_->next;
        }
        else
            split(head_, 0);
    }

    //shift all values so you can push at the front
    shiftUp(head_, 0);
    head_->values[0] = value;
    head_->count++;
    size_++;
}


/**************************************************************************/
/**
 * @brief
 *  output operator
 *
 * @tparam T    - The type of the elements in the Lariat
 * @tparam Size - The logical size of arrays within each node
 *
 * @param os - stream to print to
 * @param list - lariat to print
 *
 * @return
 * returns stream
 */
/**************************************************************************/
template <typename T, int Size>
std::ostream& operator<<( std::ostream &os, Lariat<T, Size> const & list )
{
    typename Lariat<T, Size>::LNode * current  = list.head_;
    int index = 0;
    while (current) {
        os << "Node starting (count " << current->count << ")\n";
        for ( int local_index = 0; local_index < current->count; ++local_index ) {
            os << index << " -> " << current->values[local_index] << std::endl;
            ++index;
        }
        os << "-----------\n";
        current = current->next;
    }
    return os;
}
#else // fancier 
#endif
