    
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>

namespace stringutils
{
    using std::vector;  
    typedef std::vector<std::string> string_vector;

    string_vector &split(const std::string &s, char delim, string_vector &elems);
    string_vector split(const std::string &s, char delim);

    std::vector<int> vecstr_to_vecint(std::vector<std::string> vs);

     std::string itoa(int i);
 //    std::string itos(int num);
     std::string dashbar(int size);
     std::string dashbar(unsigned int size, std::string label);
     std::string nbsp(int size);


     // trim from start
     static inline std::string& ltrim(std::string &s) {
             s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
             return s;
     }

     // trim from end
     static inline std::string& rtrim(std::string &s) {
             s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
             return s;
     }

     // trim from both ends
     static inline std::string& trim(std::string &s) {
             return ltrim(rtrim(s));
     }


     typedef std::string::value_type char_t;

     char_t up_char( char_t ch )
     {
         return std::use_facet< std::ctype< char_t > >( std::locale() ).toupper( ch );
     }

     std::string toupper( const std::string &src )
     {
         std::string result;
         std::transform( src.begin(), src.end(), std::back_inserter( result ), up_char );
         return result;
     }

     char_t down_char( char_t ch )
     {
         return std::use_facet< std::ctype< char_t > >( std::locale() ).tolower( ch );
     }

     std::string tolower( const std::string &src )
     {
         std::string result;
         std::transform( src.begin(), src.end(), std::back_inserter( result ), down_char );
         return result;
     }

     // move to stringutils
    bool is_numeric(const std::string& str) {

        if(str.size() == 0) return false;

        std::stringstream conv;
        double tmp;
        conv << str;
        conv >> tmp;
        return conv.eof();
    }



string_vector& split(const std::string &s,
        char delim, string_vector &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string_vector split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<int> vecstr_to_vecint(string_vector vs) {
    std::vector<int> ret;
    for (string_vector::iterator it = vs.begin(); it != vs.end(); ++it) {
        std::istringstream iss(*it);
        int temp;
        iss >> temp;
        ret.push_back(temp);
    }
    return ret;
}

/*
std::vector<int> strvec_to_intvec(std::vector<std::string> strvec)
{
   std::vector<int> intvec;
   std::vector<std::string>::iterator strit = strvec.begin();

   for (; strit != strvec.end(); strit++) {
      intvec.push_back(atoi((*strit).c_str()));
   }

   return intvec;
} // ------------------------------------------------------------------------

*/


std::string itoa(int i) {

    //EDITED BY Philip Westrich
    std::ostringstream oss;
    oss << i;
    return oss.str();

    //this line would not compile
    //return (static_cast<std::ostringstream*> (&(std::ostringstream() << i))->str());
}



std::string itos(int num) {
   std::ostringstream ss;
   ss << num;
   return ss.str();
}






}




#endif
