*Project was deleted just prior to initial turn in, rebuilt from p2 and an outdated copy of semantics.cpp. Everything seems to be in working order.
**The method used to determine maximum variable stack depth, by calling semScan twice and suppressing the output on the initial run, is sloppy. I worked on a more efficient method but abandoned it due to growing complexity.
***Optimally the semScan calls would happen inside a wrapper function, with the final call of semScan causing the wrapper to return the completed variable stack.
