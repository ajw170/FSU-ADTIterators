/*
 mapiter_adt.h
 Andrew J Wood
 COP 4530
 March 14, 2017
 
 This header file defines and implements iterators for use with the fsu "Map" Abstract Data Type
 (ADT).  The iterators implemented are:
 
 -Constant Inorder Map Iterator (Bidirectional)
 -Inorder Map Iterator (Bidirectional)
 -Preorder Map Iterator (not implemented)
 -Postorder Map Iterator (not implemented)
 -Levelorder Map Iterator (Forward only)
 
 The code is self-documenting.  Note that the Inorder iterators are implemnted using fsu::Stack ADT and
 the levelorder iterators are implemented using fsu::Queue ADT.
*/

#include <queue.h>
#include <stack.h>
#include <vector.h>
#include <list.h>
#include <entry.h>
#include <debug.h>     // argh

#ifndef _MAPITER_ADT_H
#define _MAPITER_ADT_H

namespace fsu
{
    //note: "C" template parameter is any BST-type container
  template < class C >
  class ConstInorderMapIterator; // patterns: ConstIterator, BidirectionalIterator

  template < class C >
  class InorderMapIterator;      // patterns: Iterator, BidirectionalIterator

  template < class C >
  class PreorderMapIterator;     // patterns: ConstIterator, BidirectionalIterator

  template < class C >
  class PostorderMapIterator;    // patterns: ConstIterator, BidirectionalIterator

  template < class C >
  class LevelorderMapIterator;   // patterns: ConstIterator, ForwardIterator

  /******************************************************************/
  /*    LevelorderMapIterator < C >                                 */
  /******************************************************************/

  template < class C >
  class LevelorderMapIterator // patterns: ConstIterator, ForwardIterator
  {
  public: // terminology support
    typedef typename fsu::Entry<typename C::KeyType, typename C::DataType> EntryType;
    typedef typename C::Node                                               Node;
    typedef LevelorderMapIterator<C>                                       ConstIterator;
    typedef LevelorderMapIterator<C>                                       Iterator;

  private: // inner sanctum
    friend C; //needed for nodes, etc
    fsu::Queue < Node* > que_; // default is deque-based

  public:
    // first class
    LevelorderMapIterator                 () : que_() {}
    virtual  ~LevelorderMapIterator       () { que_.Clear(); }
    LevelorderMapIterator                 (const LevelorderMapIterator& i) : que_(i.que_) {}
    LevelorderMapIterator<C>&  operator=  (const LevelorderMapIterator& i) { que_ = i.que_; return *this; }

    // information/access
    bool  Valid   () const { return !que_.Empty(); } // Iterator can be de-referenced

    // various operators
    bool                       operator== (const LevelorderMapIterator& i2) const { return que_ == i2.que_; }
    bool                       operator!= (const LevelorderMapIterator& i2) const { return !(*this == i2); }
    const EntryType&           operator*  () const { return que_.Front()->value_; }
    LevelorderMapIterator<C>&  operator++ ();    // prefix
    LevelorderMapIterator<C>   operator++ (int); // postfix

    // developers helper
    void Dump( std::ostream& os = std::cout , char ofc = '\0' ) const;

  private:
    void Init      (Node* n);
    void Increment ();
  };

    
  template < class C >
  void LevelorderMapIterator<C>::Dump(std::ostream& os, char ofc) const
  {
    fsu::Queue < Node* > que(que_);
    if (ofc == '\0')
    {
      while (!que.Empty())
      {
        os << que.Front()->value_;
        que.Pop();
      }
    }
    else
    {
      while (!que.Empty())
      {
        os << ofc << que.Front()->value_;
        que.Pop();
      }
    }
    os << '\n';
  }
  
    
  template < class C >
  void LevelorderMapIterator<C>::Init(Node* n)
    //intended to be useed with root_
  {
      que_.Clear();
      if (n == nullptr) return;
      que_.Push(n);
      while (!que_.Empty() && que_.Front()->IsDead()) //if the front item is a deaad node
          Increment();
  }

    
  template < class C >
  void LevelorderMapIterator<C>::Increment()
  {
      if (que_.Empty())
          return;
      Node * n = que_.Front();
      que_.Pop();
      if (n->HasLeftChild()) que_.Push(n->lchild_);
      if (n->HasRightChild()) que_.Push(n->rchild_);
  }
    

  template < class C >
  LevelorderMapIterator<C>&  LevelorderMapIterator<C>::operator++ ()
  {
      do
      {
          Increment();
      }
      while (Valid() && que_.Front()->IsDead());
      return *this;
  }

    
  template < class C >
  LevelorderMapIterator<C>   LevelorderMapIterator<C>::operator++ (int)
  {
      LevelorderMapIterator<C> i = *this;
      operator++();
      return i;
  }

  /******************************************************************/
  /*    InorderMapIterator < C >                                    */
  /******************************************************************/

  template < class C >
  class ConstInorderMapIterator // patterns: ConstIterator, BidirectionalIterator
  {
  public: // terminology support
    typedef typename fsu::Entry<typename C::KeyType, typename C::DataType> EntryType;
    typedef typename C::Node                                               Node;
    typedef ConstInorderMapIterator<C>                                     ConstIterator;
    typedef InorderMapIterator<C>                                          Iterator;

  protected: // inner sanctum; keep stack implementation choice compatible w ChechRBLLT
    friend C;
    // fsu::Stack < Node* > stk_; // default is deque-based - better safety & error detection
    fsu::Stack < Node* , fsu::Vector < Node* > > stk_; // faster

  public:
    // first class
    ConstInorderMapIterator                 () : stk_() {}
    virtual  ~ConstInorderMapIterator       () { stk_.Clear(); }
    ConstInorderMapIterator                 (const ConstInorderMapIterator& i) : stk_(i.stk_) {}
    ConstInorderMapIterator<C>&  operator=  (const ConstInorderMapIterator& i) { stk_ = i.stk_; return *this; }

    // information/access
    bool  Valid   () const { return !stk_.Empty(); } // Iterator can be de-references

    // various operators
    bool                         operator== (const ConstInorderMapIterator& i2) const { return stk_ == i2.stk_; }
    bool                         operator!= (const ConstInorderMapIterator& i2) const { return !(*this == i2); }
    const EntryType&             operator*  () const { return stk_.Top()->value_; }
    ConstInorderMapIterator<C>&  operator++ ();    // prefix
    ConstInorderMapIterator<C>   operator++ (int); // postfix
    ConstInorderMapIterator<C>&  operator-- ();    // prefix
    ConstInorderMapIterator<C>   operator-- (int); // postfix

    // developers helper
    void Dump( std::ostream& os = std::cout , char ofc = '\0' ) const;

  private:
    void Init      (Node* n); // live nodes only
    void sInit     (Node* n); // structural (all nodes) version - pair with Increment for structural traversal
    void rInit     (Node* n); // live nodes only
    void Increment (); // structural version of ++
    void Decrement (); // structural version of --
  };

  template < class C >
  void ConstInorderMapIterator<C>::Dump(std::ostream& os, char ofc) const
  {
    fsu::Stack < Node* , fsu::Vector < Node* > > s(stk_);
    fsu::Stack < Node* , fsu::Vector < Node* > > stk;
    while (!s.Empty())
    {
      stk.Push(s.Top());
      s.Pop();
    }
    if (ofc == '\0')
    {
      while (!stk.Empty())
      {
        os << stk.Top()->value_;
        stk.Pop();
      }
    }
    else
    {
      while (!stk.Empty())
      {
        os << ofc << stk.Top()->value_;
        stk.Pop();
      }
    }
    os << '\n';
  }
  
    
  template < class C >
  void ConstInorderMapIterator<C>::sInit(Node* n)
  // only intended to be used with n = root_; sets it to first node regardless of Dead or Alive status
  {
      if (n == nullptr) return;
      stk_.Clear();
      stk_.Push(n);
      while (n != nullptr && n->HasLeftChild()) //slide down to the left as far as possible
      {
          n = n->lchild_;
          stk_.Push(n); //push node pointer onto stack
      }
  }

    
  template < class C >
  void ConstInorderMapIterator<C>::Init(Node* n)
  // only intended to be used with n = root_
  {
    if (n == nullptr) return;
    stk_.Clear();
    stk_.Push(n);
    while (n != nullptr && n->HasLeftChild())
    {
      n = n->lchild_;
      stk_.Push(n);
    }
    while (Valid() && stk_.Top()->IsDead())
      Increment();
  }


  template < class C >
  void ConstInorderMapIterator<C>::rInit(Node* n)
  // only intended to be used with n = root_
  {
      if (n == nullptr) return;
      stk_.Clear();
      stk_.Push(n);
      while (n != nullptr && n->HasRightChild())
      {
          n = n->rchild_;
          stk_.Push(n);
      }
      while (Valid() && stk_.Top()->IsDead())
          Decrement();
  }
    

  template < class C >
  void ConstInorderMapIterator<C>::Increment()
  {
    // fsu::Debug("Increment()");
    if ( stk_.Empty() )
      return;
    Node * n;
    if ( stk_.Top()->HasRightChild() )
    {
      // fsu::Debug("Increment branch 1");
      n = stk_.Top()->rchild_;
      stk_.Push(n);
      while ( n != nullptr && n->HasLeftChild() )
      {
        n = n->lchild_;
        stk_.Push(n);
      }
    }
    else
    {
      // fsu::Debug("Increment branch 2");
      do
      {
        n = stk_.Top();
        stk_.Pop();
      }
      while( !stk_.Empty() && stk_.Top()->HasRightChild() && n == stk_.Top()->rchild_ );
    }
  }

    
  template < class C >
  void ConstInorderMapIterator<C>::Decrement()
  {
      if ( stk_.Empty() )
          return;
      Node * n;
      if ( stk_.Top()->HasLeftChild() ) //if the stack has a left child, go left and slide down right
      {                                 // as far as possible
          n = stk_.Top()->lchild_;
          stk_.Push(n);
          while (n != nullptr && n->HasRightChild() )
          {
              n = n->rchild_;
              stk_.Push(n);
          }
      }
      else //if the top item in stack does not have a left child
      {
          do
          {
              n = stk_.Top();
              stk_.Pop();
          }
          while ( !stk_.Empty() && stk_.Top()->HasLeftChild() && n == stk_.Top()->lchild_ );
      }
  }

    
  template < class C >
  ConstInorderMapIterator<C>&  ConstInorderMapIterator<C>::operator++ ()
  {
      do
      {
          Increment();
      }
      while (Valid() && stk_.Top()->IsDead()); //increment until non-dead node is found
      return *this;
  }

    
  template < class C >
  ConstInorderMapIterator<C>   ConstInorderMapIterator<C>::operator++ (int)
  {
      ConstInorderMapIterator<C> i = *this;
      operator++();
      return i;
  }

  template < class C >
  ConstInorderMapIterator<C>&  ConstInorderMapIterator<C>::operator-- ()
  {
      do
      {
          Decrement();
      }
      while (Valid() && stk_.Top()->IsDead());
      return *this;
  }

  template < class C >
  ConstInorderMapIterator<C>   ConstInorderMapIterator<C>::operator-- (int)
  {
      ConstInorderMapIterator<C> i = *this;
      operator--();
      return i;
  }

  /******************************************************************/
  /*    InorderMapIterator < C >                                    */
  /******************************************************************/

  template < class C >
  class InorderMapIterator : public ConstInorderMapIterator<C>
  {
  private: // inner sanctum; keep stack implementation choice compatible w ChechRBLLT
    friend C;

  public:
    typedef typename fsu::Entry<typename C::KeyType,typename C::DataType> EntryType;
    typedef typename C::Node                                              Node;
    typedef ConstInorderMapIterator<C>                                    ConstIterator;
    typedef InorderMapIterator<C>                                         Iterator;

    // first class
    InorderMapIterator                 () : ConstInorderMapIterator<C>() {}
    virtual  ~InorderMapIterator       () { }
    InorderMapIterator                 (const InorderMapIterator& i) : ConstInorderMapIterator<C> (i) {}
    InorderMapIterator<C>&  operator=  (const InorderMapIterator& i) { this->stk_ = i.stk_; return *this; }

    // various operators
    bool                       operator== (const InorderMapIterator& i2) const { return this->stk_ == i2.stk_; }
    bool                       operator!= (const InorderMapIterator& i2) const { return !(*this == i2); }

    const EntryType&           operator*  () const { return this->stk_.Top()->value_; }
    EntryType&                 operator*  ()       { return this->stk_.Top()->value_; }
    InorderMapIterator<C>&     operator++ ();    // prefix
    InorderMapIterator<C>      operator++ (int); // postfix
    InorderMapIterator<C>&     operator-- ();    // prefix
    InorderMapIterator<C>      operator-- (int); // postfix

  };

  template < class C >
  InorderMapIterator<C>&  InorderMapIterator<C>::operator++ ()
  {
    ConstInorderMapIterator<C>::operator++();
    return *this;
  }

  template < class C >
  InorderMapIterator<C>   InorderMapIterator<C>::operator++ (int)
  {
    InorderMapIterator<C> i(*this);
    this->operator++();
    return i;
  }

  template < class C >
  InorderMapIterator<C>&  InorderMapIterator<C>::operator-- ()
  {
    ConstInorderMapIterator<C>::operator--();
    return *this;
  }

  template < class C >
  InorderMapIterator<C>   InorderMapIterator<C>::operator-- (int)
  {
    InorderMapIterator<C> i(*this);
    this->operator--();
    return i;
  }

} // namespace fsu 

#endif

