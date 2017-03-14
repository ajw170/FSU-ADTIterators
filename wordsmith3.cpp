/*
 Andrew J Wood
 FSU ID: ajw14m
 February 20, 2017
 
 Implementation for WordSmith2 API
 
 This file implements the API described in wordsmith2.h
 
 */

#include <wordsmith2.h> // included to indicate that this is the implementation file
#include <fstream> // Allows for read access to files
#include <iomanip>

WordSmith::WordSmith() : frequency_(), infiles_(), count_(0)  //default constructor
{}

WordSmith::~WordSmith() // destructor
{} //note - destructors of each element will be called

bool WordSmith::ReadText (const fsu::String& infile, bool showProgress)
{
    const char * fileForOpen = infile.Cstr();
    std::ifstream inClientFile(fileForOpen, std::ios::in); //open file for read
    
    if (!inClientFile)
    {
        return 0; //return 0, indicating file could not be read
    }
    
    const unsigned long tickerVal = 65536;
    fsu::String wordString;
    size_t wordCounter = 0;
    size_t initVocabSize = VocabSize();
    
    while (inClientFile >> wordString) //read words from file separated by whitespace, continue until EOF
    {
        WordSmith::Cleanup(wordString); //cleans up the string as per rules
        
        if (wordString.Length() != 0) //if cleanup operation resulted in non-zero length string
        {
            ++frequency_[wordString]; //get data value based on key value, increment by one if it exists already.
                                      //if it does not exist, create new and increment to 1.
            ++wordCounter;            //increment the word Counter for this read
        }
        
        if (showProgress) //if ticker is enabled
        {
            if (wordCounter % tickerVal == 0) //if the read word count is a multiple of the ticker value
            {
                std::cout << "  ** reading progress : numwords == " << wordCounter << "\n";
            }
        }
        
    } // end reading file
    
    count_ += wordCounter; //add to count_ var
    
    std::cout << "\n\tNumber of words read:    " << wordCounter;
    
    std::cout << "\n\tNew words in vocabulary: " << VocabSize() - initVocabSize << "\n";
    
    infiles_.PushBack(infile); //pushes the file name to the infiles_ list
    
    return 1; //operation was successful
}

bool WordSmith::WriteReport (const fsu::String& outfile, unsigned short kw, unsigned short dw,
                             std::ios_base::fmtflags kf, std::ios_base::fmtflags df) const
{
    const char * fileForWrite = outfile.Cstr();
    std::ofstream outClientFile(fileForWrite, std::ios::out); //opens file for output
    
    if (!outClientFile)
    {
        return 0; //error - file could not be written
    }
    
    //check to see if infiles_ is empty
    if (infiles_.Empty())
    {
        std::cout << "\n No files in read list, leaving " << outfile << " unopened\n";
        outClientFile.close();
        return 1;
    }
    
    outClientFile.seekp(0); //ensure the pointer is at the beginning of the file
    
    outClientFile << "Text Analysis for files: ";
    
    ListType::ConstIterator i; //declare itatator for list
    for (i = infiles_.Begin(); i != infiles_.End(); ++i)
    {
        outClientFile << *i;
        if (i != infiles_.rBegin()) //if the iterator is not on the last file
            outClientFile << ", "; //comma space
    }
    
    outClientFile << "\n\n";
    
    outClientFile << std::setw(kw) << std::left << "word";
    outClientFile << std::setw(dw) << std::right << "frequency";
    outClientFile << "\n";
    outClientFile << std::setw(kw) << std::left << "----";
    outClientFile << std::setw(dw) << std::right << "---------";
    
    outClientFile << "\n";
    
    //loop through all words
    frequency_.Display(outClientFile, kw, dw, kf, df); //use OAA's display method to write to file
    
    //create temp vars to avoid multiple calls
    size_t numWords = WordsRead();
    size_t vocabSize = VocabSize();
    
    //once file is finished, output summary
    outClientFile << "\n";
    outClientFile << "Number of words: " << numWords << "\n";
    outClientFile << "Vocabulary size: " << vocabSize << "\n";
    
    outClientFile.close(); //close the file
    
    //output summary information to screen
    std::cout << "\n\tNumber of words:         " << numWords << "\n";
    std::cout << "\tVocabulary size:         " << vocabSize << "\n";
    std::cout << "\tAnalysis written to file ";
    std::cout << outfile;
    std::cout << "\n\n";
    
    return 1; //file written successfully
}

void WordSmith::ShowSummary () const
{
    std::cout << "\nCurrent files:           ";
    ListType::ConstIterator i; //declare itatator for list
    for (i = infiles_.Begin(); i != infiles_.End(); ++i)
    {
        std::cout << *i;
        if (i != infiles_.rBegin()) //if the iterator is not on the last file
            std::cout << ", "; //comma space
    }
    std::cout << "\nCurrent word count:      ";
    std::cout << WordsRead();
    std::cout << "\nCurrent vocabulary size: ";
    std::cout << VocabSize();
    std::cout << "\n\n";
}

void WordSmith::ClearData ()  //temporarily using as debugger
{
    frequency_.Clear(); //empty the data
    infiles_.Clear(); //empty the list of file names
}

size_t WordSmith::WordsRead() const
{
    return count_;
}

size_t WordSmith::VocabSize() const
{
    return frequency_.Size(); //returns size of wordset
}

#include <cleanup.cpp> //logically include cleanup.cpp here
