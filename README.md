# C++ fixed size strings

Right now I'm still developing fixed size strings. I'm trying to solve two Arduino String problems:
 - heap memory fragmentation (especially with long-living Strings in a multi-taskin environment)
 - errors in String operations that are impossible to detect, which I think that really is a serious issue (please see: https://github.com/arduino/ArduinoCore-API/issues/186). 
 
The idea is using C-like strings (arrays of chars) that would primarily reside on the stack. This would solve both the problems listed above but would introduce other problems, like not optimal use of stack memory and having to deal with strings that can not expand above the given number of characters.
