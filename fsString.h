/*
 * Fixed size strings (fsString.h) for Arduino
 * 
 * This file is part of C++ vectors for Arduino: https://github.com/BojanJurca/Cplusplus-fixed-size-strings
 * 
 *  Bojan Jurca, April 1, 2023
 *  
 */


#ifndef __FS_STRING_H__
    #define __FS_STRING_H__


    // fixed size string, actually C char arrays with additional functions and operators
    template<size_t N> struct fsString {
        
        private: 

            // internal storage: char array (= 0-terminated C string)
            char __c_str__ [N + 2] = {}; // add 2 characters at the end __c_str__ [N] will detect if string gets too long (owerflow), __c_str__ [N + 1] will guard the end of the string and will always be 0, initialize it with zeros
            unsigned char __error__ = 0;
        
        public:
        
            // errors
            unsigned char error () { return __error__; };
            static const unsigned char OVERFLOW     = 0b00000001;
            static const unsigned char OUT_OF_RANGE = 0b00000010;
        
        
            // constructors
            fsString () {}                                      // for declarations like fsString<15> a;
            
            fsString (const char *other) {                      // for declarations with initialization, like fsString<15> b = "abc";
                strncpy (this->__c_str__, other, N + 1);
                if (this->__c_str__ [N]) __error__ = OVERFLOW;        // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
            }
            
            fsString (const fsString& other) {                  // for declarations with initialization, like fsString<15> b = a;
                strncpy (this->__c_str__, other.__c_str__, N + 1);
                this->__error__ = other.__error__;                    // inherit all errors from original string
            }
        
            fsString (const char& other) {                      // for declarations with initialization, like fsString<15> c = 'c';
                if (0 < N) this->__c_str__ [0] = other;
                else this->__error__ = OVERFLOW;                      // error? overflow?
            }

            fsString (int number) {                             // convert int to fsString
                char buffer [12];
                sprintf (buffer, "%i", number);
                strncpy (this->__c_str__, buffer, N + 1);
                if (this->__c_str__ [N]) __error__ = OVERFLOW;        // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
            }

            fsString (unsigned int number) {                    // convert unsigned int to fsString
                char buffer [12];
                sprintf (buffer, "%u", number);
                strncpy (this->__c_str__, buffer, N + 1);
                if (this->__c_str__ [N]) __error__ = OVERFLOW;        // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
            }

            fsString (long number) {                             // convert long to fsString
                char buffer [22];
                sprintf (buffer, "%l", number);
                strncpy (this->__c_str__, buffer, N + 1);
                if (this->__c_str__ [N]) __error__ = OVERFLOW;        // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
            }
            
            fsString (unsigned long number) {                    // convert unsigned int to fsString
                char buffer [22];
                sprintf (buffer, "%lu", number);
                strncpy (this->__c_str__, buffer, N + 1);
                if (this->__c_str__ [N]) __error__ = OVERFLOW;        // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
            }
        
        
            // char *() operator so that fsString can be used as C strings, like; fsString<5> a = "123"; sprintf ("%s\n", a);
            operator char *() { return __c_str__; }
            
          
            // = operator
            fsString operator = (const char *other) {           // for assigning C string to fsString, like: a = "abc";
                strncpy (this->__c_str__, other, N + 1);
                this->__error__ = this->__c_str__ [N] ? OVERFLOW : 0;     // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
                return *this;
            }
            
            fsString operator = (const fsString& other) {       // for assigning other fsString to fsString, like: a = b;
                strncpy (this->__c_str__, other.__c_str__, N + 1);
                this->__error__ = other.__error__;                          // inherit all errors from original string
                return *this;
            }
        
            fsString operator = (const char& other) {           // for assigning character to fsString, like: a = 'b';
                if (0 < N) { this->__c_str__ [0] = other; this->__c_str__ [1] = 0; } 
                else __error__ = OVERFLOW;                              // error? overflow?
                return *this;
            }   
        
        
            // [] operator
            char &operator [] (size_t i) { return __c_str__ [i]; }    
        
        
            // += operator
            fsString operator += (const char *other) {          // for adding C string to fsString, like: a += "abc";
                strncat (this->__c_str__, other, N + 1 - strlen (this->__c_str__));
                this->__error__ |= this->__c_str__ [N] ? OVERFLOW : 0;    // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
                return *this;
            }
        
            fsString operator += (const fsString& other) {      // for concatenating one fsString with anoterh, like: a += b;
                strncat (this->__c_str__, other.__c_str__, N + 1 - strlen (this->__c_str__));
                this->__error__ |= other.__error__;                         // add all errors from other string
                this->__error__ |= this->__c_str__ [N] ? OVERFLOW : 0;    // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
                this->__error__ |= other.__error__;                         // add also other errors to this errors
                return *this;
            }
        
            fsString operator += (const char& other) {          // for adding a charactr to fsString, like: a += 'b';
                size_t l = strlen (this->__c_str__);
                if (l < N) { this->__c_str__ [l] = other; this->__c_str__ [l + 1] = 0; } 
                else __error__ |= OVERFLOW;                             // error? overflow?
                return *this;
            }   
        
        
            // + operator
            fsString operator + (char *other) {                 // for adding C string to fsString, like: a + "abc";
                strncat (this->__c_str__, other, N + 1 - strlen (this->__c_str__));
                this->__error__ |= this->__c_str__ [N] ? OVERFLOW : 0;    // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
                return *this;
            }
        
            fsString operator + (const fsString& other) {       // for concatenating one fsString with anoterh, like: a + b;
                strncat (this->__c_str__, other.__c_str__, N + 1 - strlen (this->__c_str__));
                this->__error__ |= other.__error__;                         // add all errors from other string
                this->__error__ |= this->__c_str__ [N] ? OVERFLOW : 0;    // error? overflow?
                this->__c_str__ [N] = 0;                              // mark the end of the string regardles the possible owerflow
                this->__error__ |= other.__error__;                         // add also other errors to this errors
                return *this;
            }
            /* this doesn't work:
            fsString operator + (const char& other) {           // for adding a charactr to fsString, like: a + 'b';
                size_t l = strlen (this->__c_str__);
                if (l < N) { this->__c_str__ [l] = other; this->__c_str__ [l + 1] = 0; } 
                else __error__ |= OVERFLOW;                             // error? overflow?
                return *this;
            } 
            */
        
        
            // logical operators: ==, !=, <, <=, >, >=, ignore all possible errors
            inline bool operator == (const char *other) __attribute__((always_inline))        { return !strcmp (this->__c_str__, other); }              // fsString : C string   
            inline bool operator == (const fsString& other) __attribute__((always_inline))    { return !strcmp (this->__c_str__, other.__c_str__); }      // fsString : fsString
            inline bool operator != (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other); }               // fsString : C string
            inline bool operator != (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__); }       // fsString : fsString
            inline bool operator <  (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) < 0; }           // fsString : C string
            inline bool operator <  (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) < 0; }   // fsString : fsString
            inline bool operator <= (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) <= 0; }          // fsString : C string
            inline bool operator <= (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) <= 0; }  // fsString : fsString
            inline bool operator >  (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) > 0; }           // fsString : C string    
            inline bool operator >  (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) > 0; }   // fsString : fsString
            inline bool operator >= (const char *other) __attribute__((always_inline))        { return strcmp (this->__c_str__, other) >= 0; }          // fsString : C string    
            inline bool operator >= (const fsString& other) __attribute__((always_inline))    { return strcmp (this->__c_str__, other.__c_str__) >= 0; }  // fsString : fsString
        
          
            // some std::string-like member functions
            inline char *c_str () __attribute__((always_inline)) { return __c_str__; } // not really needed, use char *() operator instead
        
            inline size_t length () __attribute__((always_inline)) { return strlen (__c_str__); } 
            
            fsString<N> substr (size_t pos = 0, size_t len = N + 1) {
                fsString<N> r;
                r.__error__ = this->__error__; // inherit all errors from original string
                if (pos >= strlen (this->__c_str__)) {
                    r.__error__ |= OUT_OF_RANGE;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + pos, len); // can't overflow 
                }
                return r;
            }

            static const size_t npos = (size_t) 0xFFFFFFFFFFFFFFFF;

            size_t find (char *str, size_t pos = 0) {
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

            size_t max_size () { return N; }
        
        
            // some Arduino String-like member functions
            fsString<N> substring (size_t from = 0, size_t to = N - 1) {
                fsString<N> r;
                r.__error__ = this->__error__; // inherit all errors from original string
                if (from >= strlen (this->__c_str__) || to < from) {
                    r.__error__ |= OUT_OF_RANGE;
                } else {
                    strncpy (r.__c_str__, this->__c_str__ + from, to - from); // can't overflow 
                }
                return r;
            }

            int indexOf (char *str, size_t pos = 0) {
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

    };

#endif
