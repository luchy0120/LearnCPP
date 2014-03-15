##Introduction

Templates are key to techniques for:

*	Improving type safety(eg. by eliminating the use of casts)
*	Raising the general level of abstraction of programs(eg. by using standard containers and algorithms)
*	Providing more flexible, type-safe, and efficient parameterization of types and algorithms

A template can take parameters:

*	Type parameter of "type type"
*	Value parameter of built-in types such ints and pointers to functions
*	Template parameters of "type template"

###Types as Arguments

A template argument is defined to be a type parameter by prefixing it with typename or class. 

A type argument is unconstrainted; that it, there is nothing in the interface of a class that constrains it to be a certain kind of type or part of a class hierarchy. The validity of an argument type depends exclusively on its use in the templates, providing a form of duck typing.

There is no space or time overhead implied by using either compared to the other:

*	Values of built-in types are not "boxed" into special container objects.*	Values of all types are retrieved directly from a vector without use of potentially expensive get() functions
*	Values of user-defined types are not implicitly accessed through references

###Values as arguments

A template parameter that is not a type or a template is called a value parameter and an argument passed to it a value argument.

An argument for a template value parameter can be:

*	An integral constant expression
*	A pointer or a reference to an object or a function with external linkage.
*	A nonoverloaded pointer to member
*	A null pointer.

A pointer used as a template argument must be of the form &of, where of is the name of an object or a function, or of the form f, where f is the name of a function. A pointer to member must be of the form `&x::of`, where of is the name of a member. In particular, a string literal is not acceptable as a template argument:

	template<typename T, char* label>
	class X {
		// ...
	};

	X<int, "BMW323Ci"> x1;		// error, string literal as template argument
	char lx2[] = "BMW323Ci";
	X<int, lx2> x2;			// OK, lx2 has external linkage

It is best to think of template value arguments as a mechanism for passing integers and pointers to functions. Resist the temptation to try something more clever. Unfortunately(for no fundamental reason), literal types can not be used as template value parameters. The value template arguments are the mechanism for some more advanced compile-time computation techniques.

An integer template argument must be a constant. Conversely, a value template parameter is a constant within the template so that an attempt to change the value of a parameter is an error.

A type template parameter can be used as a type later in a template parameter list:

	template<typename T, T default_value>
	class Vec {
		// ...
	};

This becomes particularly useful when combined with a default template argument.

###Operations as arguments

Passing the comparison operations as a function object has significant benefits compared to passing pointers to functions:

*	A simple class member function defined in-class is trivial to inline, whereas inlining a call through a pointer to function requires exceptional attention from a coompiler
*	A function object with no data members can be passed with no run-time cost.
*	Several operations can be passed as a single object with no additional run-time cost.

If we had only one use of a comparison criterion in our program, it might make sense to use a lambda to express the function object version a bit more tersely:

	map<string, int, Cmp>c3{[](const string& x, const string& y) const {return x < y;}};	// error

Unfortunately, that does not work because there is no conversion of a lambda to function object type, we could name the lambda and then use that name.

	autp cmp = [](const string& x, const string& y) const {return x<y;}
	map<string, int, decltype(cmp)> c4{cmp};

###Templates as Arguments

Sometimes it is useful to pass templates - rather than classes or values - as template arguments. For example:

	template<typename T, template<typename> class C>
	class Xrefd {
		C<T> mems;
		C<T*> refs;
		// ...
	};

	template<typename T> using My_vec = vector<T>;	// use default allocator

	Xrefd<Entry, My_vec> x1;	// store cross references for Entrys in a vector

	template<typename T>
	class My_container {
		// ...
	};

	Xrefd<Record, My_container> x2;	// store cross references for Records in a My_container.

To declare a template as a template parameter, we must specify its required arguments. The point of using a template as a template parameter is usually that we want to instantiate it with a variety of argument types. That is, we want to express the member declarations of a template in terms of another template, but we want that other template to be a parameter so that it can be specified by users.

Only class templates can be template arguments.

###Default Template Arguments

Just as for default function arguments, the default template arguments can be specified and supplied for trailing arguments only.

The technique of supplying a policy through a template argument and then defaulting that argument to supply the most common policy is almost universal in the standard library. Curiously enough, it is not used for `basic_string` comparisons. Instead, the standard library relies on `char_traits`. Similarly, the standard algorithms rely on `iterator_traits` and the standard-library containers rely on `allocators`.

####Default Function Template Arguments

A function template argument needs to be explicitly mentioned only if it cannot be deduced or if there is no default.

If all function template argument are defaulted, the `<>` can be left out(exactly as in function template specializations)

##Specialization

The `template<>` prefix says that this is a specialization that can be specified without a template parameter.

	template<>
	class Vector<void*> {
		void** p;
		// ...
		void*& operator[](int i);
	};

The `Vector<void*>` is a complete specialization. That is, there is no template parameter to specify or deduce when we use the specialization;

To define a specialization that is used for every `Vector`s of pointers

	template<typename T>
	class Vector<T*>:private Vector<void*> {
		using Base = Vector<void*>;

		Vector(){}
		explicit Vector(int i):Base(i){};

		T*& ele(int i) {return reinterpret_cast<T*&>(Base::ele(i));}

	};

A specialization with a pattern containing a template parameter is called a *partial specialization` in contrast to *complete specializations`(as in the definition of `vector<void*>`), where "the pattern" is simply a specific type.

Specialization is a way of specifying alternative implementations for different uses of a common interface.

###The Primary Template

When we have both a general definition of a template and specializations defining implementations for specific sets of template arguments, we refer to the most general template as the primary template.

Only after a primary template has been chosen are specializations considered.

The primary template must be declared before any specialization.

A declaration of the primary template is sufficient to allow the definition of a specialization:

	template<typename T>
	class List;				// not a definition

	template<typename T>
	class List<T*> {
		// ...
	};

If used, the primary template must be defined somewhere. If the primary template is never instantiated, it need not be defined.

It is essentia that every use of a template for a gievn set of template arguments be implemented by the same specialization. 

All specializations of a template must be declared in the same namespace as the primary template. If used, a specialization that is explicitly declared must also be explicitly defined somewhere. In other words, explicitly specializing  a template implies that no(other) definition is generated for that specialization.

###Order of Specialization

One specialization is more specialized than another if every argument list that matches its specialization pattern also matches the other, but not vice versa.

A specialization pattern can be specified in terms of types composed using the constructs allowed for template parameter deduction.

####Specialization and Overloading

The definition of less

	template<typename T>
	bool less(T a, T b) {
		return a < b;
	}

Specialization

	template<>
	bool less<const char*>(const char* a, const char* b) {
		return strcmp(a, b) < 0;
	}

More:

	template<>
	bool less<>(const char* a, const char* b) {
		return strcmp(a, b) < 0;
	}

Remove Redundant

	template<>
	bool less(const char* a, const char* b);

Normal Function

	bool less(const char* a, const char* b);

Select among functions taking no arguments

	template<typename T> T max_value();		// no definition

	template<> constexpr int max_value<int>() {return INT_MAX;}


