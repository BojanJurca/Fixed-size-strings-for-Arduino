# C++ fixed size strings (for Arduino)

Fixed size strings try to bypass two Arduino String problems:
 - heap memory fragmentation (especially with long-living Strings in a multitasking environment)
 - errors in String operations that go by completely undetected, which really is a serious issue Arduino Strings have (please see: https://github.com/arduino/ArduinoCore-API/issues/186). The reason for this problem lies in missing try-catch functionality that is not supported by Arduino. Hence another mechanism for error handling should be implemented.

 
Fixed size strings use C-like strings (arrays of chars) that primarily reside on the stack. This solves both of the problems mentioned above but also introduces other kinds of problems, like not optimal use of (stack) memory and having to deal with strings that can not expand above the given number of characters. But within the given limitations the code performs deterministically, which actually is a must.


## fsString functionality

Although fsStrings are actually C arrays of chars they also behave similar to C++ std::strings or Arduino Strings.

fsString constructors:
```C++
fsString<15> s1;         // s1 can contain max 15 characters
fsString<15> s2 ("abc"); // s2 can contain max 15 characters
fsString<15> s3 = "def"; // s3 can contain max 15 characters
```

Examples of fsString assignment:
```C++
s1 = "ghi"; // assign C string (array of chars) to fsString
s2 = s3;    // assign one fsString to another
```

Examples of fsString comparison:
```C++
if ( s1 <= s2 )
    Serial.println ("less or equal");
else
    Serial.println ("greater");

if ( s2 == s3 )
    Serial.println ("equal");
else
    Serial.println ("not equal");
```

Examples of fsString operators:
```C++
s1 += "jkl";
s1 = s1 + "mno";
```

Example of using std::string like functions:
```C++
size_t i = s1.find ("jkl");
if ( i != s1.npos )
    Serial.println ( s1.substr (i, 3) );
```

Example of using Arduino String like functions:
```C++
int j = s1.indexOf ("jkl");
if ( j >= 0 )
    Serial.println ( s1.substring (j, j + 3) );
```

Example of using fsString as C string (array of chars):
```C++
Serial.printf ("s1 = %s\n", s1 );
int k = 0;
while ( s1 [k] ) {
    Serial.println ( s1 [k] );
    k ++;
}
```

### Handling errors in fsString operations

Opposite to Arduino Strings, fsString constructors can not fail as long as there is enough stack memory where they reside (with an exception when the initialization string is longer than fits into fsString).

If due to string assignment or concatenation the result should be longer than fits into fsString, fsString will get filled to the maximum number of characters it can contain and the OVERFLOW error flag will be set. This flag can be checked so the code can react to it if needed.

If due to string assignment or concatenation the result should be longer than fits into fsString, fsString will get filled to the maximum number of characters it can contain and the OVERFLOW error flag will be set. This flag can be checked so the code can react to it if needed.

if substr or substring member functions are called with a position parameter outside of string range OUT_OF_RANGE error flag will be set. This flag can be checked so the code can react to it if needed.

Whenever a fsString with errors flags set is involved in fsString operations, the resulting fsString will inherit all the error flags. Thus it is not necessary to check errors after each string operation for errors will be preserved until fsString gets assigned with errorless fsString.

```C++
s2 = s1 + "123456789"; // please note that total length exceeds 15 characters
if ( s2.error () /* != 0 */ ) {
    Serial.printf ("s2 = %s, but there was an error %i while calculating its value\n", s2, s2.error () ); // in spite of the error fsString is still fully initialized up to the maximum number of characters it can contain
    if ( s2.error () & s2.OVERFLOW ) Serial.println ("OVERFLOW");         // if the content should actually be longer than it fits into fsString
    if ( s2.error () & s2.OUT_OF_RANGE ) Serial.println ("OUT_OF_RANGE"); // if substr or substring addressed non-existing position
}

s3 = s2.substr (9, 3); // please note that error information from operands is passed to the result
if ( s3.error () /* != 0 */ ) {
    Serial.printf ("s3 = %s, but there was an error %i while calculating its value\n", s3, s3.error () ); // in spite of the error fsString is still calculated from the data that is available
    if ( s3.error () & s3.OVERFLOW ) Serial.println ("OVERFLOW");         // if the content should actually be longer than it fits into fsString
    if ( s3.error () & s3.OUT_OF_RANGE ) Serial.println ("OUT_OF_RANGE"); // if substr or substring addressed non-existing position
}
```

Combining fsString with C string (array of chars):
```C++
char cstr [] = "456";
s3 += cstr;
Serial.println (s3);
```

Combining fsStrings of different sizes is only possible through the char * operator since fsStrings are, from C++ point of view, of different types. This means that such operations are actually the same as operations among fsStrings and C strings (arrays of chars).

Basically all the operations should work normally. The only difference is that error information will not get passed to the resulting fsString.
```C++
fsString<100> bigS;
bigS = s3;
if ( bigS.error () /* != 0 */ )
    Serial.printf ("bigS = %s, but there was an error %i while calculating its value\n", bigS, bigS.error () );
else
    Serial.printf ("bigS = %s, with no error\n", bigS); // please note that error information is not passed from s3 to bigS, since both fsStrings are actually of different types
```
