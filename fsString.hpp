/*
 * Fixed size strings (fsString.hpp) (for Arduino)
 * 
 * This file is part of C++ vectors for Arduino: https://github.com/BojanJurca/Cplusplus-fixed-size-strings
 * 
 *  March 12, 2024, Bojan Jurca
 *  
 */


#ifndef __FS_STRING_H__
    #define __FS_STRING_H__


        // missing C function in Arduino
        char *stristr (const char *haystack, const char *needle) { 
            if (!haystack || !needle) return NULL; // nonsense
            int nCheckLimit = strlen (needle);                     
            int hCheckLimit = strlen (haystack) - nCheckLimit + 1;
            for (int i = 0; i < hCheckLimit; i++) {
                int j = i;
                int k = 0;
                char hChar, nChar;
                while (*(needle + k)) {
                    hChar = *(haystack + j); if (hChar >= 'a' && hChar <= 'z') hChar -= 32; // convert to upper case
                    nChar = *(needle + k); if (nChar >= 'a' && nChar <= 'z') nChar -= 32; // convert to upper case
                    if (nChar != hChar || hChar) break;
                    j ++;
                    k ++;
                }
                if (nChar == hChar) return (char *) haystack + i; // match!
            }
            return NULL; // no match
        }


    // ----- TUNNING PARAMETERS -----

    #ifndef fsstring
        #ifdef SHOW_COMPILE_TIME_INFORMATION
            #pragma message "fsstring with default fsString size was not defined previously, #defining the default fsstring as fsString<300> in fsString.hpp" 
        #endif
        #define fsstring fsString<300>
    #endif


    // error flags: there are only two types of error flags that can be set: OVERFLOW and OUT_OF_RANGE - please note that all errors are negative (char) numbers
    #define OK                ((signed char) 0b00000000) //    0 - no error    
    #define OVERFLOW_FLAG     ((signed char) 0b10000001) // -127 - buffer overflow
    #define OUT_OF_RANGE_FLAG ((signed char) 0b10000010) // -126 - invalid index


    // fixed size string, actually C char arrays with additional functions and operators
    template<size_t N> struct fsString {
        
        private: 

            // internal storage: char array (= 0-terminated C string)
            char __c_str__ [N + 2] = {}; // add 2 characters at the end __c_str__ [N] will detect if string gets too long (owerflow), __c_str__ [N + 1] will guard the end of the string and will always be 0, initialize it with zeros
            signed char __errorFlags__ = 0;
        
        public:
        
            signed char errorFlags () { return __errorFlags__; }
            void clearErrorFlags () { __errorFlags__ = 0; }
        
        
            // constructors
            fsString () {}                                      // for declarations like fsString<15> a;
            
            fsString (const char *other) {                      // for declarations with initialization, like fsString<15> b ("abc");
                if (other) {                                                  // check if NULL char * pointer to overcome from possible programmer's errors
                    strncpy (this->__c_str__, other, N + 1);
                    if (this->__c_str__ [N]) {
                         __errorFlags__ = OVERFLOW_FLAG;                      // OVEVRFLOW
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    }
                } 
            }

            fsString (const fsString& other) {                  // for declarations with initialization, like fsString<15> c = a;            
                strncpy (this->__c_str__, other.__c_str__, N + 1);
                this->__errorFlags__ = other.__errorFlags__;                  // inherit all errors from other fsString
            }
        
            fsString (const char& other) {                      // for declarations with initialization, like fsString<15> d ('c'); (convert char to fsString)
                this->__c_str__ [0] = other;
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (int number) {                             // for declarations with initialization, like fsString<15> e (3); (convert int to fsString)
                snprintf (this->__c_str__, N + 2, "%i", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (unsigned int number) {                    // for declarations with initialization, like fsString<15> e (3); (convert unsigned int to fsString)
                snprintf (this->__c_str__, N + 2, "%u", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (long number) {                            // for declarations with initialization, like fsString<15> e (3); (convert long to fsString)
                snprintf (this->__c_str__, N + 2, "%l", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (unsigned long number) {                   // for declarations with initialization, like fsString<15> e (3); (convert unsigned long to fsString)
                snprintf (this->__c_str__, N + 2, "%lu", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (float number) {                           // for declarations with initialization, like fsString<15> e (3.1); (convert float to fsString)
                snprintf (this->__c_str__, N + 2, "%f", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }

            fsString (double number) {                          // for declarations with initialization, like fsString<15> e (3.1); (convert float to fsString)
                snprintf (this->__c_str__, N + 2, "%lf", number);
                if (this->__c_str__ [N]) {
                    __errorFlags__ = OVERFLOW_FLAG;                           // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                }
            }


            // char *() operator so that fsString can be used the same way as C strings, like; fsString<5> a = "123"; Serial.printf ("%s\n", a);
            inline operator char *() __attribute__((always_inline)) { return __c_str__; }
            

            // = operator
            fsString operator = (const char *other) {           // for assigning C string to fsString, like: a = "abc";
                if (other) {                                                  // check if NULL char * pointer to overcome from possible programmers' errors
                    strncpy (this->__c_str__, other, N + 1);
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ = OVERFLOW_FLAG;                 // OVEVRFLOW
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    } else {
                        this->__errorFlags__ = 0;                             // clear error after new assignment
                    }
                }
                return *this;
            }
    
            fsString operator = (const fsString& other) {       // for assigning other fsString to fsString, like: a = b;
                strncpy (this->__c_str__, other.__c_str__, N + 1);
                this->__errorFlags__ = other.__errorFlags__;                  // inherit all errors from original string
                return *this;
            }

            fsString operator = (const char& other) {           // for assigning character to fsString, like: a = 'b';
                this->__c_str__ [0] = other; 
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                #if N > 0
                    this->__c_str__ [1] = 0;                                  // mark the end of the string
                #else
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                #endif
                return *this;
            }   

            fsString operator = (int number) {                   // for assigning int to fsString, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%i", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            fsString operator = (unsigned int number) {           // for assigning unsigned int to fsString, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%u", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles the OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            fsString operator = (long number) {                   // for assigning long to fsString, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%l", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            fsString operator = (unsigned long number) {          // for assigning unsigned long to fsString, like: a = 1234;
                snprintf (this->__c_str__, N + 2, "%lu", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            fsString operator = (float number) {                  // for assigning float to fsString, like: a = 1234.5;
                snprintf (this->__c_str__, N + 2, "%f", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVEFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }

            fsString operator = (double number) {                 // for assigning double to fsString, like: a = 1234.5;
                snprintf (this->__c_str__, N + 2, "%lf", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ = OVERFLOW_FLAG;                     // OVEVRFLOW
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } else {
                    this->__errorFlags__ = 0;                                 // clear error after new assignment
                }
                return *this;
            }


            // += operator
            fsString operator += (const char *other) {          // concatenate C string to fsString, like: a += "abc";
                if (other) {                                                  // check if NULL char * pointer to overcome from possible programmer's errors
                    strncat (this->__c_str__, other, N + 1 - strlen (this->__c_str__));
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= OVERFLOW_FLAG;                // add OVERFLOW flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    } 
                }
                return *this;
            }

            fsString operator += (const fsString& other) {      // concatenate one fsString to anoterh, like: a += b;
                strncat (this->__c_str__, other.__c_str__, N + 1 - strlen (this->__c_str__));
                this->__errorFlags__ |= other.__errorFlags__;                 // add all errors from other string
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }

            fsString operator += (const char& other) {          // concatenate a charactr to fsString, like: a += 'b';
                size_t l = strlen (this->__c_str__);
                if (l < N) { 
                    this->__c_str__ [l] = other; 
                    this->__c_str__ [l + 1] = 0; 
                } else {
                    __errorFlags__ |= OVERFLOW_FLAG;                          // add OVERFLOW flag to possibe already existing error flags
                }
                return *this;
            }   

            fsString operator += (int number) {                 // concatenate an int to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%i", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }

            fsString operator += (unsigned int number) {        // concatenate an int to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%u", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }   

            fsString operator += (long number) {                // concatenate a long to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%l", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }   

            fsString operator += (unsigned long number) {        // concatenate an unsigned long to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%lu", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }   

            fsString operator += (float number) {                // concatenate a flaot to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%f", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }   

            fsString operator += (double number) {                // concatenate a double to fsString, like: a += 12;
                size_t l = strlen (this->__c_str__);
                snprintf (this->__c_str__ + l, N + 2 - l, "%lf", number);
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }   


            // + operator
            fsString operator + (const char *other) {             // for adding C string to fsString, like: a + "abc";
                if (other) {                                                  // check if NULL char * pointer to overcome from possible programmer's errors
                    strncat (this->__c_str__, other, N + 1 - strlen (this->__c_str__));
                    if (this->__c_str__ [N]) {
                        this->__errorFlags__ |= OVERFLOW_FLAG;                // add OVERFLOW flag to possibe already existing error flags
                        this->__c_str__ [N] = 0;                              // mark the end of the string regardles OVERFLOW
                    } 
                }
                return *this;
            }
        
            fsString operator + (const fsString& other) {       // for concatenating one fsString with anoterh, like: a + b;
                strncat (this->__c_str__, other.__c_str__, N + 1 - strlen (this->__c_str__));
                this->__errorFlags__ |= other.__errorFlags__;                 // add all errors from other string
                if (this->__c_str__ [N]) {
                    this->__errorFlags__ |= OVERFLOW_FLAG;                    // add OVERFLOW flag to possibe already existing error flags
                    this->__c_str__ [N] = 0;                                  // mark the end of the string regardles OVERFLOW
                } 
                return *this;
            }

            fsString operator + (const char& other) {           // for adding a character to fsString, like: a + 'b';
                size_t l = strlen (this->__c_str__);
                if (l < N) { 
                    this->__c_str__ [l] = other; 
                    this->__c_str__ [l + 1] = 0; 
                } else {
                    __errorFlags__ |= OVERFLOW_FLAG;                          // add OVERFLOW flag to possibe already existing error flags
                }
                return *this;
            } 

            // can't use + operator for integers, this would make impossible to use for example fsstring + int to calculate the pointer to the location 

        
            // logical operators: ==, !=, <, <=, >, >=, ignore all possible errors
            inline bool operator == (const char *other) __attribute__((always_inline))        { return !strcmp (this->__c_str__, other); }              // fsString : C string   
            inline bool operator == (char *other) __attribute__((always_inline))              { return !strcmp (this->__c_str__, other); }              // fsString : C string   
            inline bool operator == (const fsString& other) __attribute__((always_inline))    { return !strcmp (this->__c_str__, other.__c_str__); }    // fsString : fsString
            inline bool operator != (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other); }               // fsString : C string
            inline bool operator != (char *other) __attribute__((always_inline))              { return strcmp (this->__c_str__, other); }               // fsString : C string
            inline bool operator != (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__); }     // fsString : fsString
            inline bool operator <  (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) < 0; }           // fsString : C string
            inline bool operator <  (char *other) __attribute__((always_inline))              { return strcmp (this->__c_str__, other) < 0; }           // fsString : C string
            inline bool operator <  (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) < 0; } // fsString : fsString
            inline bool operator <= (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) <= 0; }          // fsString : C string
            inline bool operator <= (char *other) __attribute__((always_inline))              { return strcmp (this->__c_str__, other) <= 0; }          // fsString : C string
            inline bool operator <= (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) <= 0; }// fsString : fsString
            inline bool operator >  (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) > 0; }           // fsString : C string    
            inline bool operator >  (char *other) __attribute__((always_inline))              { return strcmp (this->__c_str__, other) > 0; }           // fsString : C string    
            inline bool operator >  (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) > 0; } // fsString : fsString
            inline bool operator >= (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) >= 0; }          // fsString : C string    
            inline bool operator >= (char *other) __attribute__((always_inline))              { return strcmp (this->__c_str__, other) >= 0; }          // fsString : C string    
            inline bool operator >= (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) >= 0; }// fsString : fsString


            // [] operator
            inline char &operator [] (size_t i) __attribute__((always_inline)) { return __c_str__ [i]; }
            inline char &operator [] (int i) __attribute__((always_inline)) { return __c_str__ [i]; }
            // inline char &operator [] (unsigned int i) __attribute__((always_inline)) { return __c_str__ [i]; }
            inline char &operator [] (long i) __attribute__((always_inline)) { return __c_str__ [i]; }
            inline char &operator [] (unsigned long i) __attribute__((always_inline)) { return __c_str__ [i]; }


            // some std::string-like member functions
            inline char *c_str () __attribute__((always_inline)) { return __c_str__; } // not really needed, use char *() operator instead
        
            inline size_t length () __attribute__((always_inline)) { return strlen (__c_str__); } 
            
            inline size_t max_size () __attribute__((always_inline)) { return N; } 

            fsString<N> substr (size_t pos = 0, size_t len = N + 1) {
                fsString<N> r;
                r.__errorFlags__ = this->__errorFlags__;                      // inherit all errors from original string
                if (pos >= strlen (this->__c_str__)) {
                    r.__errorFlags__ |= OUT_OF_RANGE_FLAG;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + pos, len);        // can't overflow 
                }
                return r;
            }

            static const size_t npos = (size_t) 0xFFFFFFFFFFFFFFFF;

            size_t find (const char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                if (p)  return p - __c_str__;
                return npos;
            }

            size_t find (fsString str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                if (p)  return p - __c_str__;
                return npos;
            }

            size_t rfind (char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return npos;
            }            

            size_t rfind (fsString str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return npos;
            }            

            void erase (size_t pos = 0) {
                if (pos > N) pos = N;
                __c_str__ [pos] = 0;
            }
        
            // some Arduino String-like member functions
            fsString<N> substring (size_t from = 0, size_t to = N - 1) {
                fsString<N> r;
                r.__errorFlags__ = this->__errorFlags__;                      // inherit all errors from original string
                if (from >= strlen (this->__c_str__) || to < from) {
                    r.__errorFlags__ |= OUT_OF_RANGE_FLAG;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + from, to - from); // can't overflow 
                }
                return r;
            }

            int indexOf (const char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                if (p)  return p - __c_str__;
                return -1;
            }

            int indexOf (fsString str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                if (p)  return p - __c_str__;
                return -1;
            }

            int lastIndexOf (char *str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return -1;
            }

            int lastIndexOf (fsString str, size_t pos = 0) {
                char *p = strstr (__c_str__ + pos, str.__c_str__);
                char *q = NULL;
                while (p) { q = p; p = strstr (p + 1, str); }
                if (q) return q - __c_str__;
                return -1;
            }

            bool endsWith (char *str) { 
              size_t lStr = strlen (str);
              size_t lThis = strlen (__c_str__);
              if (lStr > lThis) return false;
              return !strcmp (str, __c_str__ + (lThis - lStr));
            }

            void remove (size_t pos = 0) {
                if (pos > N) pos = N;
                __c_str__ [pos] = 0;
            }

            void trim () {
                lTrim ();
                rTrim ();
            }
        
        
            // add other functions that may be useful 
            void lTrim () {
                size_t i = 0;
                while (__c_str__ [i ++] == ' ');
                if (i) strcpy (__c_str__, __c_str__ + i - 1);
            }    
            
            void rTrim () {
                size_t i = strlen (__c_str__);
                while (__c_str__ [-- i] == ' ');
                __c_str__ [i + 1] = 0;
            }

            void rPad (size_t toLength, char withChar) {
                if (toLength > N) {
                  toLength = N;
                  __errorFlags__ |= OVERFLOW_FLAG;                            // error? overflow?
                }
                size_t l = strlen (__c_str__);
                while (l < toLength) __c_str__ [l ++] = withChar;
                __c_str__ [l] = 0;
            }

    };

#endif
