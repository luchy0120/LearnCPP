The type of a function consists of the return type and the argument types. For class member functions, the name of the class is also part of the function type.

In addition to specifying a name, a set of arguments, and a return type, a function declaration can contain a variety of specifiers and modifiers. In all we can have:

*	The name of the function; required
*	The argument list, which maybe empty `()`; require
*	The return type, which may be `void` and which may be prefix or suffix(using `auto`); required
*	`inline`, indicating a desire to have function calls implemented by inlining the function body
*	`constexpr`, indicating that it should be possible to evalutate the function at compile time if given constant expression arguments
*	`noexcept`, indicating that the function may not throw an exception
*	A linkage specification, for example, `static`
*	[[noreturn]], indicating that the function will not return using the normal call/return mechanism

In addition, a member function may be specified as:

*	`virtual`, indicating that it can be overridden in a derived class.
*	`override`, indicating that it must be overriding a virtual function from a base class.
*	`final`, indicating that it can not be overridden in a derived class
*	`static`, indicating that it is not assosiated with a particular object
*	`const`, indicating that it may not modify its object

	struct S {
		[[noreturn]] virtual inline auto f(const unsigned long int *const) -> void const noexcept;
	};

The definition and all declarations for a function must specify the same type. Unfortunately, to preserver C compatibility, a `const` is ignored at the highest level of an argument type. For example:

	void f(int);		// type is void(int)
	void f(const int);	// type is void(int)

*Constructors* are technically not functions; in particular, they do not return a value, can initialize bases and members and can not have their address taken

*Destructors* can not be overloaded and can not have their address taken

*Function objects* are not functions(they are objects) and can not be overloaded, but their `operator()` are functions

*lambda expressions* are basically a shorthand for defining function objects.

A function declaration can also be written using a syntax that places the return type after the argument list. For example, the following two declarations are equivalent:

	string to_string(int a);		// prefix return type
	auto to_strng(int a) -> string;	// suffix return type

The essential use for a suffix return type comes in function template declarations in which the return type depends on the arguments, For example

	template<class T, class U>
	auto product(const vector<T>& x, const vector<U>& y) ->decltype(x*y)

There are no `void` values. However, a call of a void function may be used as the return type of a void function.

	void g(int* p);

	void h(int* p) {
		// ...
		return g(p);		// OK
	}

A return-statement is one of five ways of exiting a function:

*	Exectuting a return statement
*	"Falling off the end" of a function; that is, simply reacing the end of the function body. This is allowed only in function that are not declared to return a value(`void` functions and in `main()`, where falling off the end indicates successful completion)
*	Throwing an exception that isn't caught locally
*	Terminating because an exception was thrown and not caught locally in a noexcept function
*	Directly or indirectly invoking a system function that does not return(`exit()`)

If you want a guarantee that a value is computed at compile time, declare it `constexpr` and make sure that all functions used in its evaluation are constexpr.

If an inline function is defined in more than one translation unit(typically because it was defined in a header), its definition in the different translation units must be identical

By specifying a function `constexpr`, we indicate that we want it to be usable in constant expressions if given constant expression as arguments.

	constexpr int fac(int n) {
		return (n > 1) ? n * fac(n - 1) : 1;
	}
	
	constexpr int f9 = fac(9);

Another example:

	void f(int n) {
		int f5 = fac(5);		// maybe evaluated at compile time
		int fn = fac(n);		// evaluate at run time

		constexpr int f6 = fac(6);	// must be evaluate at compile time
		constexpr int fnn = fac(n);	// error, can not guarantee compile time evaluation(n is vairable)

		char a[fac(4)];				// OK, array bounds must be constants and fac() is constexpr
		char a2[fac(n)];			// error: array bounds must be constants and n is a variable
	}

A `constexpr` function must consist of a single return-statement; no-loops and no local variables are allowed. Also, a constexpr function may not have side effects. That is, a constexpr function is a pure function.

The rule for a constexpr constructor are suitably different; there, only simple initialization of members is allowed.

A constexpr function allows recursion and conditional expressions.

By using literal types, constexpr functions can be defined to use user-defined types.(a class with a constexpr constructor is called a literal type)

Like inline functions, constexpr functions obey the ODR(one-definition rule), so that definition in the different translation units must be identical. You can think of constexpr function as a restricted form of inline functions

A construct `[[]]` is called an attribute and can be placed just about anywhere in the C++ syntax. In general, an attribute specifies some implementation-depedent property about the syntactic entity that precedes it. In addition, an attribute can be placed in front of a declaration. There are only two standard attributes and `[[noreturn]]` is one of them, the other is `[[carries_dependency]]`

	[[noreturn]] void exit(int);

Initialization of a static local variable does not lead to a data race unless you enter the function containing it recursively or a deadlock occurs. 

The effect of initializing a local static recursively is undefined.

	int fn(int n) {
		static int n1 = n;		// OK
		static int n2 = fn(n - 1) + 1;	// undefined
		return n;
	}

Following the rules for reference initialization, a literal, a constant, and an argument that requires conversion can be passed as a `const T&` argument, but not as a plain(non-const)T& argument. Allowing conversions for a const T& ensures that such an argument canbe given exactly the same set values as a T argument by passing the value in a temporary.

	float fsqrt(const float& );

	void g(double d) {
		float r = fsqrt(2.0f);		// pass reference to temp holding 2.0f
		r = fsqrt(r);				// pass reference to r
		r = fsqrt(d);				// pass reference to temp holding static_cast<float>(d)

	}

If you want to pass an array, rather than a container or a pointer to the first element of an array, you can declare a parameter of type reference to array.

	void f(int(&r)[4]);

Note that the number of elements is part of a reference-to-array type. That makes such references far less flexible than pointers and containers(such as vector). The main use references to arrays is in tempaltes, where the number of elements is then deduced.

	template<class T, int N> void f(T(&r)[N]) {
		// ...
	}

	int a1[10];
	double d2[100];

	void g() {
		f(a1);			// T is int, N is 10
		f(d2);			// T is double, N is 100
	}

A `{}`-delimited list can be used as an argument to a parameter of:

*	Type `std::initializer_list<T>`, where the values of the list can be implicitly converted to T
*	A type that can be initialized with the values provided in the list.
*	A reference to an array of T, where the values of the list can be implicitly converted to T.

If there is a possible ambiguity, an `initializer_list` parameter takes priority.

For some functions, it is not possible to specify the number and type of all arguments expected in a call, there are three choices:

*	use a *variadic template*, this allows us to handle an arbitrary number of arbitrary types in a type-safe manner by writing a small template metaprogram that interprets the argument list to determine its meaning and take approriate actions.
*	use an `initializer_list` as the argument type. This allows us to handle an arbitrary number of arguments of a single type in a type safe manner. In many contexts, such homogeneous lists are the most common and important case
*	Terminate the argument list with the ellipsis(`...`), which means "and maybe some more arguments", this allows us to handle an arbitrary number of(almost) arbitrary types by using some macros from `<cstdarg>`. This solution is not inherently type-safe and can be hard to use with sophisticated user-defined types. However, this mechanism has been used from the earliest days of C.

An example to illustrate chocie three:

	#include <cstdarg>

	void error(int severity ...) {
		va_list ap;
		va_start(ap, severity);

		for(;;) {
			char *p = va_arg(ap, char *);
			if (p == nullptr) break;
			cerr << p << ' ';
		}

		va_end(ap);

		cerr << '\n';
		if (severity) exit(severity);
	}

A default argument is type checked at the time of the function declaration and evaluated at the time of the call.

Using the same name for operations on different types is called *overloading*.

Overload Function Match Rule

*	Exact match; that is, match using no or only trivial conversions(for example, array name to pointer, function name to pointer to function and T to const T)
*	Match using promotions; that is, integral promotions(bool to int, char to int, short to int, and their unsigned conterparts) and float to double
*	Match using standard conversions, int to double, double to int, double to long double, Derived* to Base*, T* to void*, int to unsigned int
*	Match using user-defined conversions, double to complex<double>
*	March using the ellipsis ..., in a function declaration

Functions declared in different non-namespaces scopes do not overload

A base class and a derived class provide different scopes so that overloading between a base class function and derived class function does not happen by default.

	struct Base {
		void f(int);
	};

	struct Derived : Base {
		void f(double);
	};

	void g(Derived& d) {
		d.f(1);		// call Derived::f(double)
	}

A pointer to function does not allow the code to be modified, there are only two things one can do to a function: call it and take its address.

Dereferencing a pointer to function using `*` is optional, using `&` to get the address of a function is optional.

In pointer assignments, the complete function type must match exactly, it means the argument list type and the return type

There is no implicit conversion of argument or return types when pointers to functions are assigned or initialized.

You can take the address of an overloaded function by assigning to or initializing a pointer to function. In that case, the type of the target is used to select from the set of overload functions.

	void f(int);
	int f(char);

	void (*pf1)(int) = &f;		// void f(int)
	int (*pf2)(char) = &f;		// int f(char)
	
	void (*pf3)(char) = &f;		// error, no void f(char)

A pointer to a `noexcept` function can be declared `noexcept`

	void f(int) noexcept;
	void g(int);

	void (*p1)(int) = f;			// OK: but we throw away useful information
	void (*p2)(int) noexcept = f;	// OK; we preserve the noexcpet information
	void (*p3)(int) noexcept = g;	// ERROR: we do not know that g does not throw

A pointer to function must reflect the linkage of a function. Neither linkage specification nor `noexcept` may appear in type alias

	using Pc = extern "C" void (int);// ERROR: linkage specification in alias
	using Pn = void(int) noexcept;	// ERROR: noexcept in alias

Macro names can not be overloaded and the macro prepocessor can not handle recursive calls

If you muse use a macro, use the scope resolution operator, `::`, when referring to global names and eclose occurrences of a macro argument name in parentheses whenever possible.

If you must write macros complicated enough to require comments, it is wise to use `/* */` comments.

When writting a macro, it is not unusual to need a new name for something. A string can be created by concatenating two strings using the `##` macro operator.

	#define NAME(a,b) a##b

	int NAME(hack,cah)();

will produce `int hackcah()`.

A single `#` before a parameter name in a replacement string means a string containing the macro argument.

	#define printx(x) cout << #x "=" << x << '\n';

	int a = 7;
	string str = "asdf";

	void f() {
		printx(a);	// cout << "a" << "=" << a << '\n';
		printx(srt);// cout << "str" << "=" << str << '\n';
	}

Writing `#x "=" rather than `#x << " = "` is obscure clever code rather than an error. Adjacent string literals are concatenated.

`#undef x` ensures that no macro called X is defined - whether or not one was before the directive.

Macros can even be variadic. For example:

	#define err_print(...) fprintf(strerr, "err:%s %s\n", __VA_ARGS__)

the ellipsis (`...`) means tha `__VA_ARGS__` represents the arguments actually passed as a tring

###Predefined Macros

*	`__cplusplus`: defined in a C++ compilation(and not in a C compilation), Its value is `201103L` in a C++11 source program; previous C++ standards have lower values
*	`__DATA__`: data in "yyyy:mm:dd" format
*	`__TIME__`: time in "hh:mm:ss" format
*	`__FILE__`: name of current source file
*	`__LINE__`: source line number within the current source file
*	`__FUNC__`: an implementation-defned C-style string naming the current function
*	`__STDC_HOSTED__`: 1 if the implementation is hosted; otherwise 0

In addition, a few macros are conditionally defined by the implementation

*	`__STDC__`: define in a C compilation(and not in a C++ compilation)
*	`__STDC_MB_MIGHT_NEW_WC__`: 1 if in the encoding for wchar_t, a member of the basic character set might have a code value that differs from its value as an ordinary character literal
*	`__STDCPP_STRICT_POINTER_SAFETY__`: 1 if the implementation has strict pointer safety; otherwise undefined
*	`__STDCPP_THREADS__`: 1 if a program can have more than one thread of execution; otherwise undefined


