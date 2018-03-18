# cpp-misc-techniques: random bits of C++ for experimentation

Mostly a brain-dump of things I think to try in C++,
or that I come across on the web and find the need to type it out and elaborate
in orde to comprehend it better.

Many of these examples are not intended to be compiled, despite their extension.
Their validity however can be determined using -fsyntax-only on gcc-like compilers;
in cases where a fragment is intended to demonstrate a performance-related technique,
they can be compiled to unlinked object form with gcc's -c flag
(you'll likely want -O3 also to see the full benefit)
and the source file will point this out in a header comment.
Similarly if the example can be compiled to a standalone executable,
it will have a main function and the source file again will indicate so.

These code snippets are public domain unless otherwise noted;
in cases where a fragment of code has come from elsewhere,
appropriate attribution/notices as required can be found in the relevant source files.

