# C++ fixed size strings

Right now I'm still developing fixed size strings. The code is going to be published here soon. I'm trying to solve two Arduino String problems:
 - heap memory fragmentation (especially with long-living Strings in a multi-taskin environment)
 - errors in String operations that are impossible to detect, which I think that really is a serious issue (please see: https://github.com/arduino/ArduinoCore-API/issues/186). The reason for this lies in missing try-catch functionality that is not supported by Arduino. Hence another mechanism for error handling should be implemented.

 
The idea is using C-like strings (arrays of chars) that would primarily reside on the stack. This would solve both of the problems listed above but would also introduce other kinds of problems, like not optimal use of stack memory and having to deal with strings that can not expand above the given number of characters. But within the given limitations the code would perform deterministically, which actually is a must.
