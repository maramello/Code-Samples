/*****************************************************************************/
/**
@file   lariat.h
@author Rohit Saini
@date   2/14/2021
@brief
  This file contains the definition of functions for the Lariat class, a
  templatized linked list of arrays.
*/
/*****************************************************************************/
////////////////////////////////////////////////////////////////////////////////
#ifndef LARIAT_H
#define LARIAT_H
////////////////////////////////////////////////////////////////////////////////

#include <string>     // error strings
#include <utility>    // error strings
#include <cstring>     // memcpy

//!Lariat exception class
class LariatException : public std::exception {
  private:
    //err code and description, for debug aid
    int m_ErrCode;
    std::string m_Description;

  public:
    //Constructor
    LariatException(int ErrCode, const std::string& Description) :
        m_ErrCode(ErrCode), m_Description(Description) {}
  
    //Get the error cdoe
    virtual int code(void) const { 
      return m_ErrCode; 
    }
  
    //Get the error description
    virtual const char *what(void) const throw() {
      return m_Description.c_str();
    }
    //Destructor
    virtual ~LariatException() throw() {
    }
  
    //Enum declarations for types of errors
    enum LARIAT_EXCEPTION {E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR};
};

//Class forward declaration
template<typename T, int Size> 
class Lariat;

template<typename T, int Size> 
std::ostream& operator<< (std::ostream& os, Lariat<T, Size> const & rhs);

template <typename T, int Size>
class Lariat 
{
    public:

        Lariat();                   // default constructor                        
        Lariat( Lariat const& rhs); // copy constructor

         template<class T2, int Size2>
          Lariat( Lariat<T2, Size2> const& rhs); // copy constructor

        ~Lariat(); // destructor

        template<class T2, int Size2>
        friend class Lariat;

        // operator=
        Lariat& operator=( Lariat const& rhs);
        
        //assignment operator from separate class of a different size
        template<class T2, int Size2>
        Lariat& operator=(Lariat<T2, Size2> const& rhs);

        // inserts
        void insert(int index, const T& value);
        void push_back(const T& value);
        void push_front(const T& value);

        // deletes
        void erase(int index);
        void pop_back();
        void pop_front();

        //Access
        T&       operator[](int index);       // for l-values
        const T& operator[](int index) const; // for r-values
        T&       first();
        T const& first() const;
        T&       last();
        T const& last() const;

        unsigned find(const T& value) const;   // returns index, size (one past last) if not found

        //output operator
        friend std::ostream& operator<< <T,Size>( std::ostream &os, Lariat<T, Size> const & list );

        size_t size(void) const;   // total number of items (not nodes)
        void clear(void);          // clear data

        void compact();             // push data in front reusing empty positions and delete remaining nodes
    private:
        struct LNode { // DO NOT modify provided code
            LNode *next  = nullptr;
            LNode *prev  = nullptr;
            int    count = 0;         // number of items currently in the node
            T values[Size];
        };


        //HELPER FUNCTIONS

        //finds an element with the given global index
        int findElement(int globalIndex, LNode** result) const;

        //splits a node into two
        void split(LNode* fullNode, int localIndex);

        //swaps all elements from index to the end
        void shiftUp(LNode *node, int index);

        //swaps all elements from index to the left
        void shiftDown(LNode *node, int index);

        //swaps 2 values
        void SwapValues(T& A, T& B);

        LNode *head_;           // points to the first node
        LNode *tail_;           // points to the last node
        int size_;              // the number of items (not nodes) in the list
        mutable int nodecount_; // the number of nodes in the list
        int asize_;             // the size of the array within the nodes

};

#include "lariat.cpp"

#endif // LARIAT_H
