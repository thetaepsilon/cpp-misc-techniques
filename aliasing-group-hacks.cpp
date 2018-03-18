/*
This fragment should be compiled but not linked; simply view it's disassembly.
-O3 is recommended to clear away unneeded stack operations which detract from the main point.

C++ for various reasons never properly included C99's restrict keyword.
This keyword informs the compiler that the pointers may be assumed to point to different objects;
in particular, that writing to one will not affect the other.
Otherwise, in some corner cases the compiler may have to assume they *may* be the same,
and will re-load values when it seems counter-intuitive.
*/

template <int Phantom>	// this parameter's purpose will be explained below
struct container {
	unsigned int i;
};
using data = container<0>;

unsigned int test1(data* a, data* b) {
	unsigned int tmp = a->i;	// 1
	b->i = tmp;			// 2
	// because writing to b may have just affected a if they're the same,
	// the compiler must emit a reload instruction here.
	return a->i;			// 3
}
// clang's assembly output for the above on x86_64:
/*
	mov    (%rdi),%eax	; 1
	mov    %eax,(%rsi)	; 2
	mov    (%rdi),%eax	; <-- this is the reload for (3), can we eliminate this?
	retq
*/

/*
We can somewhat work around this by taking advantage of the C++ strict aliasing rule.
Recall that this rule essentially states
"thou shalt not have differently typed pointers pointing to the same memory location",
and such usage is considered undefined behaviour by pretty much all C++ revisions to date.

Also recall that most UB is intended as a compiler optimisation.
Given that, in most cases, pointers to different types will indeed refer to different objects
(unless you happen to be messing with unions!),
This UB allows the compiler to assume that altering an object of one type
*cannot affect the memory representation of an object of another type*,
which if you think about it actually makes sense -
it seems a bit stupid that an int* would have to be reloaded because you wrote via a float*.

As it turns out, at least with gcc and clang as tested,
this rule extends to templated types, even those separated by a phantom non-type parameter.
In this case one has to be careful not to run afoul of the strict aliasing rule,
but if one is careful one can coax the compiler to assume that two such separated objects don't alias.
*/

template <int P1, int P2>
unsigned int test2(container<P1>* a, container<P2>* b) {
	unsigned int tmp = a->i;	// 1
	b->i = tmp;		// 2
	return a->i;		// 3
}
// explicitly instantiate so we can see the compiler output.
// note the differing phantom types on the containers.
template unsigned int test2(container<0>* a, container<1>* b);
// now we get something more like what we might expect:
/*
	mov    (%rdi),%eax	; 1
  	mov    %eax,(%rsi)	; 2
       	retq		; 3. note no second load from (%rdi),
       			; %eax (a->i) is still in register
*/

/*
This is somewhat a contrived example,
as in the above case you could just return tmp if that's what you meant.
However, this general optimisation case is well known from fortran and C99 attempted to address this.
There are a few subtle reasons why it didn't make it into C++ proper;
http://www.open-std.org/JTC1/sc22/wg21/docs/papers/2014/n3988.pdf may be a good start.

I have not done extensive testing on whether this chicanery would hold up in practice;
therefore, the standard advice applies:
DON'T PREMATURELY OPTIMISE, use profiling tools to get hard data,
examine the machine code actually output, make an informed decision,
and (if this technique turns out to work in your case) then use it if needed.
*/



