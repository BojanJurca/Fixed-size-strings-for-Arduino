#include "fsString.hpp"

void setup () {
    Serial.begin (115200);

    // examples of fsString constructors:

    fsString<10> fs1;                      // s1 can hold max 10 characters and is empty when constructed
    fsString<15> fs2 ("abcdef");           // s2 can hold max 15 characters and is initialized with "abc" value
    fsString<15> fs3 = "ghijkl";           // s3 can hold max 15 characters and is assigned "def" value after construction
    fsString<5> fsi (1234);                // si can hold max 5 characters and is assigned "1234" value (converted from int)
    // there are other constructors available as well


    // examples of fsString usage:

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


    // examples of fsString assignment:

    fs1 = "ghi";                           // assign char array content  to s1
    fs2 = fs3;                             // assign the value of s3 to s2 (note that they are of the same type: fsString<15>), beside content the error flags are alos copied
    fs3 = fs1;                             // assign the value of s1 to s3 in char array manner, since they are not of the same type (fsString<10> and fsString<15>), so error flags do not get copied!
    fsi = 54321;

    // assignment from Arduino String (in char array manner):
    String Ss3 = "ghi";
    fs3 = Ss3.c_str ();                    // assignment is performed through a pointer to char, which Arduino String exposes with c_str () member function
    // assignment from std::string in char array manner:
    std::string ss3 = "ghi";
    fs3 = ss3.c_str ();                    // assignment is performed through a pointer to char, which std::String exposes with c_str () member function
    // fsString also has c_str () member function but since it is exactly the same as (char *) operator it is not really neede, so fs3 is exaclty the same as fs3.c_str ()


    // examples of using fsString comparison operators:
    if ( fs2 <= fs3 )
        Serial.println ("fs2 is less or equal to fs3");
    else
        Serial.println ("fs2 is greater than fs3");

    if ( fs2 == fs3 )
        Serial.println ("fs2 is equal to fs3");
    else
        Serial.println ("fs2 is different than fs3");
    // note that comparison can be performed even if both operands are not of the same type (fsString<10> and fsString<15>)
    if ( fs1 <= fs2 )
        Serial.println ("fs1 is less or equal to fs2");
    else
        Serial.println ("fs1 is greater than fs2");


    // examples of using other fsString operators:
    fs1 += "jkl";
    fs1 = fs1 + "mno";
    fs2 = fs1 + 'c'; 


    // detecting errors that occured in fsString operations:
    fs2 = "abcdefghij";
    fs3 = fs2 + "123456789"; // please note that fs2 and fs3 are of the same type! (fsString<15>) and the length exceeds 15 characters
    if (fs3.errorFlags ()) {
        Serial.printf ("fs3 = %s, but there was an error %i while calculating its value\n", fs3, fs3.errorFlags ());  // in spite of the error fsString is still fully initialized up to the maximum number of characters it can contain
        if (fs3.errorFlags () & OVERFLOW_FLAG) Serial.println ("OVERFLOW");                                           // if the content should actually be longer than it fits into fsString
        if (fs3.errorFlags () & OUT_OF_RANGE_FLAG) Serial.println ("OUT_OF_RANGE");                                   // if substr or substring addressed non-existing position
    }

    fs3 = fs2.substr (9, 3); // please note that error information from operands is passed to the result
    Serial.printf ("   fs3 = %s, fs3.errorFlags () = %i\n", fs3, fs3.errorFlags ());
    if (fs3.errorFlags ()) {
        Serial.printf ("fs3 = %s, but there was an error %i while calculating its value\n", fs3, fs3.errorFlags ());  // in spite of the error fsString is still calculated from the data that is available
        if (fs3.errorFlags () & OVERFLOW_FLAG) Serial.println ("OVERFLOW");                                           // if the content should actually be longer than it fits into fsString
        if (fs3.errorFlags () & OUT_OF_RANGE_FLAG) Serial.println ("OUT_OF_RANGE");                                   // if substr or substring addressed non-existing position
    }


    // memory efficiency of fsString compared to Arduino String and std::string (demonstration for this particular case only, efficiency depends on the lenght of the text):

            // fsString uses only global or stack memory (unless you explicitly put it on the heap), so all the memory used will be sizeof (ftest)
            fsString<30> ftest = "This is not bad at all.";
            Serial.printf ("fsString<30> ftest uses %i bytes of stack memory (always N + 3), storage efficiency = %f\n", sizeof (ftest), (float) ftest.length () / (float) sizeof (ftest));
            // output: fsString<30> ftest uses 33 bytes of stack memory (always N + 3), storage efficiency = 0.696970

            // Arduino String uses global or stack memory, but also the heap
            size_t heapStart = ESP.getFreeHeap ();
            String Stest = "This is not bad at all.";
            size_t heapUsed = heapStart - ESP.getFreeHeap ();
            Serial.printf ("String Stest uses %i bytes of stack memory (always 16) but also %i bytes of heap, altogether %i bytes, storage efficiency = %f\n", sizeof (Stest), heapUsed, sizeof (Stest) + heapUsed, (float) Stest.length () / (float) (sizeof (Stest) + heapUsed));
            // output: String Stest uses 16 bytes of stack memory (always 16) but also 48 bytes of heap, altogether 64 bytes, storage efficiency = 0.359375

            // std::string uses global or stack memory, but also the heap
            heapStart = ESP.getFreeHeap ();
            std::string stest = "This is not bad at all.";
            heapUsed = heapStart - ESP.getFreeHeap ();
            Serial.printf ("std::string stest uses %i bytes of stack memory (always 24) but also %i bytes of heap, altogether %i bytes, storage efficiency = %f\n", sizeof (stest), heapUsed, sizeof (stest) + heapUsed, (float) stest.length () / (float) (sizeof (stest) + heapUsed));
            // output: std::string stest uses 24 bytes of stack memory (always 24) but also 40 bytes of heap, altogether 64 bytes, storage efficiency = 0.359375


    // time efficiency of fsString compared to Arduino String and std::string (demonstration for this particular case only):

            // construct JSON array of numbers 1 .. 100 in fsString with error checking
            unsigned long millisStart = millis ();
            fsString<500> fjson;

                fjson = "'[\"";
                for (int i = 1; i <= 99; i++) {
                    fjson += i;
                    fjson += "\",\"";
                }
                fjson += "100\"]'";

            unsigned long millisEnd = millis ();
            Serial.printf ("fjson construction error? %i\n", fjson.errorFlags ()); // check success
            Serial.printf ("fjson length: %i : %s\n", fjson.length (), fjson); 
            Serial.printf ("fjson construction finished in %lu milliseconds\n", millisEnd - millisStart);
            // output: fjson construction finished in 11 milliseconds
            //         this is more than Arduino Strings or std::strings need, for C char arrays do not keep string length information and hence JSON construction requires O(n^2) steps

            // construct JSON array of numbers 1 .. 100 in Arduino String with error checking
            millisStart = millis ();
            String Sjson;
            bool error = false;

                if (!Sjson.concat ("'[\"")) error = true;
                for (int i = 1; i <= 99; i++) {
                    String tmp (i);
                    if (!tmp) error = true;
                    if (!Sjson.concat (tmp)) error = true;
                    if (!Sjson.concat ("\",\"")) error = true;
                }
                if (!Sjson.concat ("100\"]'")) error = true;

            millisEnd = millis ();
            Serial.printf ("Sjson construction error? %i\n", error); // check success
            Serial.printf ("Sjson length: %i : %s\n", Sjson.length (), Sjson.c_str ()); 
            Serial.printf ("Sjson construction finished in %lu milliseconds\n", millisEnd - millisStart);
            // output: Sjson construction finished in 2 milliseconds

            // construct JSON array of numbers 1 .. 100 in std::String without error checking
            millisStart = millis ();
            std::string sjson;

                sjson = "'[\"";
                for (int i = 1; i <= 99; i++) {
                    sjson += std::to_string (i);
                    sjson += "\",\"";
                }
                sjson += "100\"]'";

            millisEnd = millis ();
            Serial.printf ("sjson length: %i : %s\n", sjson.length (), sjson.c_str ()); 
            Serial.printf ("sjson construction finished in %lu milliseconds\n", millisEnd - millisStart);
            // output: sjson construction finished in 2 milliseconds
}

void loop () {

}
