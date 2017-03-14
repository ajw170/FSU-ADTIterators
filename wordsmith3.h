/*
 Andrew J Wood
 FSU ID: ajw14m
 March 1, 2017
 
 Header file for wordsmith2.h -- enhancement to wordsmith.h
 
 A WordSmith object stores words in an underlying set available set in the FSU library.
 For this implementation, Wordsmith uses an FSU::OAA (Ordered Assocaitive Array).
 Instead of using a Set container containing pairs, the OAA contains the keys
 and data directly.
 
 The OAA is implemented using a Left Leaing Red Black Tree (LLRBT) to ensure that
 all operations (except rehash) run in log n time.  Note - rehash would not be used
 with WordSmith.
 
 
 The API gives the ability to Read text from a file, write a report showing each individual word read and the frequency, 
 display a summary of all words and quanties read so far, and to clear the set of all data.
 
 The cleanup method is a helper method used to make it easy for the client to store words;
 it removes junk characters according to a set of rules for the program.
 
 The copy constructor and assignment operator are marked as private methods and are not implemented 
 so that the compiler does not generate default versions.
 */

#ifndef WORDSMITH_H
#define WORDSMITH_H

#include <xstring.h> //fsu::String
#include <list.h> //fsu::List
#include <oaa.h>

class WordSmith
{
    
public:
    WordSmith();            //default constructor
    ~WordSmith();           //destructor
    bool ReadText       (const fsu::String& infile, bool showProgress = 0); //read file contents
    bool WriteReport    (const fsu::String& outfile, unsigned short kw = 15, unsigned short dw = 15,
                         std::ios_base::fmtflags kf = std::ios_base::left, //key justify
                         std::ios_base::fmtflags df = std::ios_base::right //data justify
                         ) const;
    void ShowSummary    () const;
    void ClearData      ();
    
private:
    
    //internal class terminology
    typedef fsu::List < fsu::String >                   ListType;
    
    typedef fsu::String                                 KeyType;
    typedef size_t                                      DataType;
    
    typedef fsu::OAA <KeyType,DataType>                 SetType;
    
    SetType                     frequency_; //specified set; holds frequency of keys
    ListType                    infiles_; //list of file names
    size_t                      count_; //keeps track of how many words were read
    
    static void Cleanup (fsu::String&); //removes invalid characters from string
    
    size_t WordsRead() const; //outputs word count (non-unique)
    size_t VocabSize() const; //outputs size of vocabulary (unique)
    
    // copy constructor and assignment operator - not implemented
    WordSmith (const WordSmith &);
    WordSmith& operator = (const WordSmith &);
    
    
}; // end class WordSmith

#endif // wordsmith2.h
