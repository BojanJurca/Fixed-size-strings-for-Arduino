# Fixed size strings for Arduino (C char arrays with C++ operators)


Fixed size strings for Arduino address the following issues:

- Memory fragmentation of small controllers' (heap) memory when using (C++) Arduino Strings or std::strings.

- inability to detect errors (since Arduino doesn't support try-catch functionality) that occur in string calculations, usually due to the lack of memory.

- lacking the elegance of using C++ string expressions with C char arrays.

Fixed size strings are just C char arrays with C++ operators and an extra flag to keep the information about errors that occured since the string has been constructed. This error information is passed throughout all string operations. A programmer can decide whether or when to use this information or ignore it completely.

Program interface is more or less compatible with the all three types  mentioned above: C char arrays, Arduino Strings or std::strings to make the use as simple as possible.


### Examples of fsString constructors

```C++
fsString<10> fs1;                      // s1 can hold max 10 characters and is empty when constructed
fsString<15> fs2 ("abcdef");           // s2 can hold max 15 characters and is initialized with "abc" value
fsString<15> fs3 = "ghijkl";           // s3 can hold max 15 characters and is assigned "def" value after construction
fsString<5> fsi (1234);                // si can hold max 5 characters and is assigned "1234" value (converted from int)
// there are other constructors available as well
```

### Examples of fsString usage
```C++
// as C char array:
Serial.println (fs2);
Serial.printf ("%s\n", fs2);
Serial.printf ("%s\n", &fs2 [3]);

// as Arduino String:
Serial.println (fs2);
Serial.println (fs2.substring (3)); 

// as std::string
Serial.println (fs2);
Serial.println (fs2.substr (3)); 
```

### Examples of fsString assignment
```C++
fs1 = "ghi";                           // assign char array content  to s1
fs2 = fs3;                             // assign the value of s3 to s2 (note that they are of the same type: fsString<15>), beside content the error flags are alos copied
fs3 = fs1;                             // assign the value of s1 to s3 in char array manner, since they are not of the same type (fsString<10> and fsString<15>), so error flags do not get copied!
fsi = 54321;

// assignment from Arduino String (in char array manner):
String Ss3 = "ghi";
fs3 = Ss3.c_str ();                    // assignment is performed through a pointer to char, which Arduino String exposes with c_str () member function
// assignment from std::string in char array manner:
std::string ss3 = "ghi";
fs3 = ss3.c_str ();                    // assignment is performed through a pointer to char, which std::String exposes with c_str () member function
// fsString also has c_str () member function but since it is exactly the same as (char *) operator it is not really needed, so fs3 is exactly the same as fs3.c_str ()
```

### Examples of fsString comparison
```C++
if (fs2 <= fs3)
    Serial.println ("fs2 is less or equal to fs3");
else
    Serial.println ("fs2 is greater than fs3");

if (fs2 == fs3)
    Serial.println ("fs2 is equal to fs3");
else
    Serial.println ("fs2 is different than fs3");
    // note that comparison can be performed even if both operands are not of the same type (fsString<10> and fsString<15>)

if (fs1 <= fs2)
    Serial.println ("fs1 is less or equal to fs2");
else
    Serial.println ("fs1 is greater than fs2");
```

### Examples of using other fsString operators
```C++
fs1 += "jkl";
fs1 = fs1 + "mno";
fs2 = fs1 + 'c'; 
```

### Detecting errors that occured in fsString operations
```C++
fs2 = "abcdefghij";
fs3 = fs2 + "123456789"; // please note that fs2 and fs3 are of the same type! (fsString<15>) and the length exceeds 15 characters
if (fs3.errorFlags ()) {
    Serial.printf ("fs3 = %s, but there was an error %i while calculating its value\n", fs3, fs3.errorFlags ());  // in spite of the error fsString is still fully initialized up to the maximum number of characters it can contain
    if (fs3.errorFlags () & OVERFLOW_FLAG) Serial.println ("OVERFLOW");                                           // if the content should actually be longer than it fits into fsString
    if (fs3.errorFlags () & OUT_OF_RANGE_FLAG) Serial.println ("OUT_OF_RANGE");                                   // if substr or substring addressed non-existing position
}

fs3 = fs2.substr (9, 3); // please note that error information from operands is passed to the result
Serial.printf ("   fs3 = %s, fs3.errorFlags () = %i\n", fs3, fs3.errorFlags ());
if (fs3.errorFlags ()) {
    Serial.printf ("fs3 = %s, but there was an error %i while calculating its value\n", fs3, fs3.errorFlags ());  // in spite of the error fsString is still calculated from the data that is available
    if (fs3.errorFlags () & OVERFLOW_FLAG) Serial.println ("OVERFLOW");                                           // if the content should actually be longer than it fits into fsString
    if (fs3.errorFlags () & OUT_OF_RANGE_FLAG) Serial.println ("OUT_OF_RANGE");                                   // if substr or substring addressed non-existing position
}
```

### Checking if an error has occurred only once after after many fsString operations
```C++
// construct JSON array of numbers 1 .. 100 in fsString with error checking
fsString<500> fjson;

fjson = "'[\"";
for (int i = 1; i <= 99; i++) {
    fjson += i;
    fjson += "\",\"";
}
fjson += "100\"]'";

Serial.printf ("fjson construction error? %i\n", fjson.errorFlags ()); // check success
```
