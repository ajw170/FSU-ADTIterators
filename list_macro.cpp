/*
    list_macro.cpp
    12/27/2016
    Chris Lacher

    Implementation of List<T>:: macroscopic (whole list) mutators
    Merge, Reverse, Shuffle

    Copyright 2016, R. C. Lacher
*/

//-----------------------------------
//     List<T>::Merge Implementations
//-----------------------------------

template < typename T >
template < class P >
void List<T>::Merge (List<T>& y, P& p)
// merges x into this list; post: true = x.Empty()
// if both lists are ordered result is ordered
{
  if (this == &y) return;
  Link * xcurr = head_->next_;   // x = this list
  Link * ycurr = y.head_->next_; // y = that list

  // merge while both lists are not empty
  while (xcurr != tail_ && ycurr != y.tail_)
  {
    if (p(ycurr->Tval_,xcurr->Tval_)) // y < x
    {
      ycurr = ycurr->next_;
      LinkIn(xcurr,ycurr->prev_);
    }
    else // x <= y
    {
      xcurr = xcurr->next_;
    }
  }

  // splice in remainder of y
  if (ycurr != y.tail_)
  {
    tail_->prev_->next_ = ycurr;
    ycurr->prev_ = tail_->prev_;
    tail_->prev_ = (y.tail_)->prev_;
    (y.tail_)->prev_->next_ = tail_;
  }

  // make y structurally correct for empty
  (y.head_)->next_ = y.tail_;
  (y.tail_)->prev_ = y.head_;
}

template < typename T >
void List<T>::Merge (List<T>& y )
{
  fsu::LessThan<T> p;
  Merge(y,p);
}

//------------------------------------
//     List<T>::Reverse Implementation
//------------------------------------

template < typename T >
void List<T>::Reverse ()
{
  if (head_->next_ == tail_)
    return;

  // swap prev_ and next_ for each link
  typename List<T>::Link * link(head_), * temp(nullptr);
  while (link != nullptr)
  {
    temp        = link->next_;
    link->next_ = link->prev_;
    link->prev_ = temp;
    link        = temp; // move to "old next" = link->prev_
  }
  // swap head_ and tail_
  temp  = head_;
  head_ = tail_;
  tail_ = temp;
}

//------------------------------------
//     List<T>::Shuffle Implementation
//------------------------------------

template < typename T >
void List<T>::Shuffle ()
{
  if (head_->next_ == tail_ || head_->next_->next_ == tail_)
    return;
  char  toggle = 0;

  // manipulate pointers instead of using API which would call new/delete
  List<T> aux1,aux2;
  Iterator i = Begin();
  Link * link;
  // put every 2nd, 3rd links into aux lists in reverse order
  // std::cout << Size()      << " List: "; Display(std::cout, ' '); std::cout << '\n';
  while (i != End())
  {
    toggle == 2 ? toggle = 0 : toggle += 1;
    if (toggle == 0)
    {
      ++i;                           // leave in List
    }
    else
    {
      link = i.curr_;
      i.curr_ = i.curr_->next_;      // ++i
      link = LinkOut(link);          // Remove(i)
      if (toggle == 1)
        LinkIn(aux1.head_->next_,link); // aux1.PushFront
      else
        LinkIn(aux2.head_->next_,link); // aux2.PushFront        
    }
  }
  /* // show the three lists at this stage:
  std::cout << Size()      << " aux0: "; Display(std::cout, ' '); std::cout << '\n';
  std::cout << aux1.Size() << " aux1: "; aux1.Display(std::cout, ' '); std::cout << '\n';
  std::cout << aux2.Size() << " aux2: "; aux2.Display(std::cout, ' '); std::cout << '\n';
  // */

  // 2: splice aux2 to front of list and fix aux structure as empty list
  Link* afirst = aux2.head_->next_;
  Link* alast  = aux2.tail_->prev_;
  head_->next_->prev_ = alast;
  alast->next_ = head_->next_;
  afirst->prev_ = head_;
  head_->next_ = afirst;
  aux2.tail_->prev_ = aux2.head_;
  aux2.head_->next_ = aux2.tail_;

  // 3: splice aux1 to front of list and fix aux structure as empty list
  afirst = aux1.head_->next_;
  alast  = aux1.tail_->prev_;
  head_->next_->prev_ = alast;
  alast->next_ = head_->next_;
  afirst->prev_ = head_;
  head_->next_ = afirst;
  aux1.tail_->prev_ = aux1.head_;
  aux1.head_->next_ = aux1.tail_;
  // */
  
  /* // 3: splice aux1 to back of list and fix aux structure as empty list
  afirst = aux1.head_->next_;
  alast  = aux1.tail_->prev_;
  tail_->prev_->next_ = afirst;
  afirst->prev_ = tail_->prev_;
  alast->next_ = tail_;
  tail_->prev_ = alast;
  aux1.tail_->prev_ = aux1.head_;
  aux1.head_->next_ = aux1.tail_;
  // */
  
  // std::cout << Size()      << " List: "; Display(std::cout, ' '); std::cout << '\n';
}

// end macromutators */

