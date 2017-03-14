  // diagnostic utilities

  template <typename K, typename D, class P>
  bool Map_ADT<K,D,P>::CheckBST(bool verboseFlag) const
  // Checks the order conditions for BST.
  // Note this implicitly also checks unimodality - there are no duplicate entries.
  {
    if (root_ == nullptr) return 1;
    ConstIterator i,j,k;
    bool ok = 1;
    for (j = BeginStructuralInorder(); j != End(); j.Increment())
    {
      i = k = j;
      --i;
      ++k;
      if (i.Valid())
      {
        if (!pred_((*i).key_,(*j).key_))
        {
          ok = 0;
          std::cout << "  ** CheckBST: order discrepancy between nodes at "
                    << "--i = " << *i
                    <<  " and " 
                    << "i = " << *j << '\n';
        }
      }
      if (k.Valid())
      {
        if (!pred_((*j).key_,(*k).key_))
        {
          ok = 0;
          std::cout << "  ** CheckBST: order discrepancy between nodes at "
                    << "i = " << *j
                    <<  " and " 
                     << "++i = " << *k << '\n';
        }
      }
    }
    if (ok && verboseFlag) std::cout << "  ** CheckBST: OK\n";
    return ok;
  } // CheckBST() */

  template <typename K, typename D, class P>
  bool Map_ADT<K,D,P>::CheckRBLLT (int verboseFlag) const
  // 0 = no output, 1 = messages, 2 = messages & data
  {
    if (this->root_ == nullptr) return 1;

    // first check for the BST, Property 0:
    bool ok = this->CheckBST((bool)verboseFlag);
    if (!ok) return ok;

    // now check the red-black tree color properties 
    Node * n = this->root_; // ConstNavigator n = Root();

    // Check Property 2.  If the root isn't black, there is a violation.
    ok = n->IsBlack(); // n.IsBlack();
    if (!ok)
    {
      std::cout << "  ** RBLLT Property 2 failure: "
                << "root is not black!\n";
      return ok;
    }

    // This loop does a traversal checking the remaining properties.
    // Pair < ConstNavigator, size_t >          bottomNode;
    // List < Pair < ConstNavigator , size_t> > bottomNodeList;
    Pair < Node* , size_t >          bottomNode;
    List < Pair < Node* , size_t> >  bottomNodeList;
    size_t blackCount = 0;
    Node * nR, * nL; // * nLL;
    for (ConstIterator i = this->BeginStructuralInorder(); i != this->End(); i.Increment())
    {
      n = i.stk_.Top(); // i.GetNavigator();
      // Check Property 5. No right child is red
      if (n->HasRightChild())
      {
        nR  = n->rchild_; // ConstNavigator nR = n.GetRightChild();
        if (nR->IsRed())
        {
          ok = 0;
          std::cout << "  ** RBLLT Property 5 failure:  " 
                    << nR->value_ << " is a red right child"
                    << " - TERMINATING TEST.\n";
          return ok;
        }
      } // if (n->HasRightChild())

      // Check Property 3. A red node cannot have a red (left) child.
      if (n->HasLeftChild())
      {
        nL  = n->lchild_; // ConstNavigator nL = n.GetLeftChild();
        if (n->IsRed() && nL->IsRed())
        {
          ok = 0;
          std::cout << "  ** RBLLT Property 3 failure:  "
                    << n->value_ << ", " << nL ->value_
                    << " are contiguous red nodes in a descending path"
                    << " - TERMINATING TEST.\n";
          return ok;
        }
      } // if (n->HasLeftChild()) */
    
      /*
        Check Property 4.  If n is a node with <= 1 children, then n is at the
        bottom of a "root-null" path. Count the number of black nodes from n to
        root (including both n and root). Insert the pair <n, blackCount> into
        a list for later analysis.

                 D(b)      This is not red-black.
                /   \      Paths through D have different black counts.
               -    E(b)
                    /  \
                   -    -


      */

      if(!n->HasLeftChild() || !n->HasRightChild())
      {
        // n is at the bottom of a root-null path; set a pair's first_ to n
        bottomNode.first_ = n;
        blackCount = 0;
        // fsu::Stack < Node* > rootnull = i.stk_;
        fsu::Stack < Node*, fsu::Vector<Node* > > rootnull = i.stk_;
        do
        {
          n = rootnull.Top();
          if (n->IsBlack()) ++blackCount;
          rootnull.Pop();
        }
        while (!rootnull.Empty());
        // set the pair's second_ to the number of black nodes.
        bottomNode.second_ = blackCount;

        // add this pair to the list
        bottomNodeList.PushBack(bottomNode);
      } // if(!n->HasLeftChild() || !n->HasRightChild())
    } // for (InorderIterator i = this->BeginStructuralInorder(); i != this->EndInorder(); i.Increment())
 
    // if we get this far, properties 1,2,3,5 are verified
    if (ok && (bool)verboseFlag)
      std::cout << "  ** Color, Left-Leaning and Height-Limiting Properties (1,2,3,5): OK\n"; 
 
    // Now we have all of the bottom nodes saved, along with the black
    // node count for the path. Verify that the black counts are the same:

    typename List < Pair < Node* , size_t> > :: ConstIterator x,y;
    for (x = bottomNodeList.Begin(), y = x++; x != bottomNodeList.End(); ++x)
    {
      // check number of black nodes for each path
      if ((*y).second_ != (*x).second_)
      {
        ok = 0;
        std::cout << "  ** RBLLT Property 4 failure:  "
                  << "black count not constant for all root-null paths!\n";
        break;
      }
    }
    if (verboseFlag == 2)
    {
      std::cout << "  =============================\n"
                << "  Root - Null black node counts\n"
                << "  =============================\n";
      std::cout << std::setw(14) << "bottomNode" << std::setw(12) <<  "blackCount" << '\n';
      std::cout << std::setw(14) << "----------" << std::setw(12) <<  "----------" << '\n';
      for (x = bottomNodeList.Begin(); x != bottomNodeList.End(); ++x)
      {
        std::cout << std::setw(14) << ((*x).first_)->value_ 
                  << std::setw(12) << (*x).second_ << '\n';
      }
      std::cout << "  =============================\n";
    }
    if (ok && (bool)verboseFlag)
    {
      x = bottomNodeList.Begin();
      std::cout << "  ** Root-Null paths = " << bottomNodeList.Size()
                << ",  black node counts = " << (*x).second_ << '\n'; 
      std::cout << "  ** Constant Black Node Count Property 4: OK\n"; 
    } 
    if (ok && (bool)verboseFlag)
      std::cout << "  ** CheckRBLLT: OK\n";
    return ok;
  } // CheckRBLLT()



