#include "fsString.h"

void setup () {
    Serial.begin (115200);

    // examples of fsString constructors:
    fsString<15> s1;
    fsString<15> s2 ("abc");
    fsString<15> s3 = "def";

    // examples of fsString assignment:
    s1 = "ghi";
    s2 = s3;

    // examples of fsString comparison:
    if ( s1 <= s2 )
        Serial.println ("less or equal");
    else
        Serial.println ("greater");

    if ( s2 == s3 )
        Serial.println ("equal");
    else
        Serial.println ("not equal");

    // examples of fsString operators:
    s1 += "jkl";
    s1 = s1 + "mno";

    // example of using std::string like functions:
    size_t i = s1.find ("jkl");
    if ( i != s1.npos )
        Serial.println ( s1.substr (i, 3) );

    // example of using Arduino String like functions:
    int j = s1.indexOf ("jkl");
    if ( j >= 0 )
        Serial.println ( s1.substring (j, j + 3) );

    // example of using fsString as C string (array of chars) :
    Serial.printf ("s1 = %s\n", s1 );
    int k = 0; 
    while ( s1 [k] ) {
      Serial.println ( s1 [k] );
      k ++;
    }

    // handling errors in fsString operations:
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


    // combining fsString with C string (array of chars):
    char cstr [] = "456";
    s3 += cstr;
    Serial.println (s3);

    // combining fsStrings of different sizes:
    fsString<100> bigS;
    bigS = s3;
    if ( bigS.error () /* != 0 */ )
        Serial.printf ("bigS = %s, but there was an error %i while calculating its value\n", bigS, bigS.error () );
    else
        Serial.printf ("bigS = %s, with no error\n", bigS); // please note that error information is not passed from s3 to bigS, since both fsStrings are actually of different types

}

void loop () {

}
