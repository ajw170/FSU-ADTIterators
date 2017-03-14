/* Andrew J Wood
   FSU ID: ajw14m
   February 20, 2017
 
   This file implements the "Cleanup" function contained in the wordsmith.h header file.  It is included separately for easy use in future applications.
 
   The cleanup function is passed a string which may contain junk characters.  It follows the following rules to clean it out
   then re-wraps back into a string.
 
   Cleanup rules
   -------------
 
   leading characters:
   -skip ch until one of the following is true:
   -ch is the null character
   -ch is a letter
   -ch is a digit
   -ch is backslash ('\')
   -ch is hyphen ('-') AND the following character is a digit
   -ch is colon (':') AND the following character is a colon AND the character after that is a char or digit
 
   acceptance: after skipping leading junk,
   -keep the char ch if any of these are true:
   -ch is a letter or a digit alnum
   -ch is backslash  and next char is a letter or digit
   -ch is apostrophe and next char is a letter or digit
   -ch is hyphen     and next char is a letter or digit
   -ch is period     and next char is a letter or digit
   -ch is comma      and is surrounded by digits
   -ch is colon      and is surrounded by digits
   -ch is the first or second of a pair of colons, the pair surrounded by letters or digits
 */

#include <cctype>
#include <cstdlib>

void WordSmith::Cleanup(fsu::String &s)
{
    size_t length = s.Length();
    char newCharString[length + 1]; //create array to store new cleaned values of string; length is at most the same as input

    size_t charArrayIndex = 0;
    size_t n = 0; //set value to 0 to start
    fsu::String cleanedString; //declare cleanString
    
    //skip leading junk loop - checks for leading character conditions
    while (
           !(s.Element(n) == '\0' ||
             isalpha(s.Element(n)) ||
             isdigit(s.Element(n)) ||
             s.Element(n) == '\\' ||
             (s.Element(n) == '-' && isdigit(s.Element(n+1))) ||
             (s.Element(n) == ':' && s.Element(n+1) == ':' && isalnum(s.Element(n+2)))
            ) && (n < length)
          ) // end condition check
    {
        ++n; //iterate through junk characters
    }
    
    //at this point the acceptance threshold has been hit
    //now, decide whether to keep each character, if yes add to cleanString
    
    while (
           (
            isalnum(s.Element(n)) ||
            (s.Element(n) == '\\' && isalnum(s.Element(n+1))) ||
            (s.Element(n) == '\'' && isalnum(s.Element(n+1))) ||
            (s.Element(n) == '-' && isalnum(s.Element(n+1)))  ||
            (s.Element(n) == '.'&& isalnum(s.Element(n+1)))   ||
            (s.Element(n) == ',' && isdigit(s.Element(n-1)) && isdigit(s.Element(n+1))) ||
            (s.Element(n) == ':' && isdigit(s.Element(n-1)) && isdigit(s.Element(n+1))) || //colon surrounded by digits
            (s.Element(n) == ':' && s.Element(n+1) == ':' && isalnum(s.Element(n-1)) && isalnum(s.Element(n+2))) || //first colon in pair, surrounded by letters or digits
            (s.Element(n) == ':' && s.Element(n-1) == ':' && isalnum(s.Element(n-2)) && isalnum(s.Element(n+1))) || //second colon in pair, surrounded by letters or digits
            (s.Element(n) == ':' && s.Element(n+1) == ':' && isalnum(s.Element(n+2))) ||  //first colon in leading pair (special case)
            (s.Element(n) == ':' && s.Element(n-1) == ':' && isalnum(s.Element(n+1)))   //second colon in leading pair, character after is char or digit
           ) && (n < length)
          ) // end condition check
    {
        newCharString[charArrayIndex] = tolower(s.Element(n));
        ++charArrayIndex;
        ++n; //increment n
    }
    
    newCharString[charArrayIndex] = '\0'; //add null character to last space
    s.Wrap(newCharString); //create string from character array
}
