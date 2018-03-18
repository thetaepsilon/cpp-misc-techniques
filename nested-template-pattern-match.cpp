#include <optional>	// std::optional only used as an example below

/*
Template specialisation can be utilised to wrangle type deduction
into being a form of pattern matching for types.
Usual routine with the base type, generic template has no type member.
Then, we have a templated specialisation for certain "container" types,
where those container types themselves have a parameter of interest.

For instance, take the case below,
where we're interested in the "inner" type of optional containers:
*/

template <typename T>
struct option_subtype {
	// generic case: non-option types don't have a subtype we're interested in
	static_assert(false, "T for option_subtype was not a specialised optional");
};

template <typename S>
struct option_subtype<std::optional<S>> {
	// pull the subtype from that of the std::optional.
	using type = S;
};

struct random {
	// this is NOT an optional type and has no option_subtype specialisation;
	// trying to accessing the type member in that case will cause explosions.
};

using T1 = option_subtype<std::optional<int>>::type;	// evals to int
using T2 = option_subtype<struct random>::type;	// BANG as type doesn't exist

