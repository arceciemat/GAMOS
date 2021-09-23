#ifndef GetWordsInLine_C
#define GetWordsInLine_C

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//----------------------------------------------------------------------- 
int GetWordsInLine(std::ifstream& fin, std::vector<std::string>& wordlist)
{
  int isok = 1;

  //---------- Read a line of file:
  //@@@@--- Cannot be read with a istream_iterator, becasuse it uses std::cout, and then doesn't read '\n'
  //----- Clear wordlist
  int wsiz = wordlist.size();
  for (int ii = 0; ii < wsiz; ii++) {
    wordlist.pop_back();
  } 

  //---------- Loop lines while there is an ending '\' or line is blank   
  const int NMAXLIN = 1000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters
  int theLineNo = 0;
  for (;;) {
    theLineNo++;
    for( int ii = 0; ii < NMAXLIN; ii++) ltemp[ii] = ' ';
    fin.getline( ltemp, NMAXLIN ); 
    //---------- Check for lines longer than NMAXLIN character
    int jj;
    for ( jj=0; jj < NMAXLIN; jj++) {
      if ( ltemp[jj] == '\0' ) break;
    }
    if ( jj == NMAXLIN-1 ) {
      std::cerr << "!! EXITING: ERROR IN LINE No " << theLineNo << " file: " << std::endl;
      std::cerr << "!!!! line longer than " << NMAXLIN << " characters" << 
	std::endl << " please split it putting a '\\' at the end of line" << std::endl;
      std::exception();
    }
    
    //---------- End of file
    //-    if ( theFiles[theCurrentFile]->eof() ) {
    if ( fin.eof() ) {
      //t          exit(0);
      return 0;
    }
    
    //---------- Convert line read to istrstream to split it in words 
    std::istringstream istr_line(ltemp);
     
    //--------- count how many words are there in ltemp (this sohuld not be needed, but sun compiler has problems) !! this has to be nvestigated...
    int NoWords = 0;
    char* tt = ltemp;
    std::string stemp(ltemp);
    do{ 
      if( *tt != ' ' && *(tt) != '\0' ) {
	if( tt == ltemp) {
	  NoWords++;
	  //     std::cout << "dNoWords" << NoWords << ltemp << std::endl;
	} else if( *(tt-1) == ' ' ||  *(tt-1) == '\015' ||  *(tt-1) == '\t') {
	  NoWords++; 
	  //     std::cout << "NoWords" << NoWords << ltemp << std::endl;
	}
      }
      tt++;
    }while((*tt != '\0') && (stemp.length()!=0));
    std::string stempt (ltemp);
    if(stempt.length() == 0) NoWords = 0;
    
    //--------- Read words from istr_line and write them into wordlist
    //    int stre = 1;
    for( int ii=0; ii < NoWords; ii++) {
      std::string stemp2 = "";
      istr_line >> stemp2;   //?? gives warning in Insure++
      if ( stemp2.length() == 0 ) break;
      int comment = stemp2.find(std::string("//") );
      //    std::cout << "!!!COMMENT" << comment << stemp2.c_str() << std::endl;
      if ( comment == 0 ) {
	break; 
      } else if ( comment > 0 ) {
	stemp2 = stemp2.substr( 0, comment );
	wordlist.push_back(stemp2);
	break;
	//-   for( int jj=0; jj < stemp2.length()-comment; jj++) stemp2.pop_back();
      } 
      wordlist.push_back(stemp2);
    }
    
    //These two algorithms should be the more STL-like way, but they don't work for files whose lines end without '\015'=TAB (STL problem: doesn't find end of string??)
    // istream_iterator<std::string, ptrdiff_t> std::string_iter(istr_line);
    // istream_iterator<std::string, ptrdiff_t> eosl;
    // copy(std::string_iter, eosl, back_inserter(wordlist));
    // typedef istream_iterator<std::string, ptrdiff_t> std::string_iter;
    // copy(std::string_iter(istr_line), std::string_iter(), back_inserter(wordlist));
    
    if ( wordlist.size() != 0 ) {
      if( (*(wordlist.end()-1)).compare("\\") == 0 ) {   //use '\' to mark continuing line  
	wordlist.pop_back();
      } else {
	break;
      }
    }
  }
  
  //or why not like this?:
  //typedef istream_iterator<std::string, ptrdiff_t> string_iter;
  //copy(string_iter(istr_line), string_iter(), back_inserter(wordlist));

  bool bSuppressQuotes = true;
  bool bCheckQuotes = true;
  if( bSuppressQuotes ) {
    //--------- A pair of double quotes delimits a word, therefore, look for the
    //          case where there is more than one word between two double quotes
    std::vector<std::string> wordlist2;
    std::string wordq = "";
    unsigned int imerge = 0;
    for( size_t ii = 0; ii < wordlist.size(); ii++)
      {
	if( wordlist[ii].substr(0,1) == "\"" )
	  {
	    imerge = 1;
	  } 
    if( wordlist[ii][ wordlist[ii].size()-1 ] == '\"' )
      {
	if( imerge != 1 )
	  {
	    if( bCheckQuotes ) {
	      std::string err1 = " word with trailing '\"' while there is no";
	      std::string err2 = " previous word with leading '\"' in line ";
	      std::string err = err1 + err2;
	      std::cerr << "!! EXITING: ERROR " << err << std::endl;
	      std::exception();
	    }
	  }
	imerge = 2;
      }
    if( imerge == 0 )
      {
	wordlist2.push_back( wordlist[ii] );
      }
    else if( imerge == 1 )
      {
	if( wordq == "" )
	  {
	    wordq.append( wordlist[ii].substr(1,wordlist[ii].size()) );
	  }
	else
	  {
	    wordq.append( wordlist[ii].substr(0,wordlist[ii].size()) );
	  }
	wordq.append(" ");
      }
    else if( imerge == 2 )
      {
	if( wordq == "" )
	  {
	    wordq.append( wordlist[ii].substr(1,wordlist[ii].size()-2));
	  }
	else
	  {
	    wordq.append( wordlist[ii].substr(0,wordlist[ii].size()-1) );
	  } 
	wordlist2.push_back( wordq );
	wordq = "";
	imerge = 0;
      }
      }
    if( imerge == 1 )
      {
	if( bCheckQuotes ) {
	  std::string err1 = " word with leading '\"' in line while there is no";
	  std::string err2 = " later word with trailing '\"' in line ";
	  std::string err = err1 + err2;
	  std::cerr << "!! EXITING: ERROR " << err << std::endl;
	  std::exception();
	}
  }
    
    wordlist = wordlist2;

  }
    
  return isok;  
}

#endif
