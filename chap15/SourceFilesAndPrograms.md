when used in namespace scope(including the global scope), the keyword `static`(somewhat illogically) means "not accessible from other source files". The keyword `const` implies default internal linkage, so if you want it to have external linkage, you need to precede its definitions with `extern`

Names that a linker does not see, such as the names of local variables, are said to have no linkage.

An inline function must be defined identically in every translation unit in which it is used. The following example is not just bas taste, it is illegal.

	// file1.cpp
	inline int f(int i) {return i;}

	// file2.cpp
	inline int f(int i) {return i+1;}

This code test in gcc(No error reported)

We keep inline functions definitions consistent by using header files.

By default, `const` objects, `constexpr` objects, type aliases, and anything declared `static` in a namespace scope have internal linkage. 

To ensure consistency, place alias, consts, constexprs, and inlines in header files.

a const can be given external linkage by an explicit declaration.

If you must use global variables, at least restrict their use to a single source file. This restriction can be archive in one of two ways:

*	Place declarations in an unnamed namespace.
*	Declare an entity static

An unnamed namespace can be used to make names local to a compilation unit. The effect of an unnamed namespace is very similar to that of internal linkage.

As a rule of thumb, a header may contain:

	Named namespaces					namespace N {/* */}
	inline namespaces					inline namespace N {/* */}
	Type definitions					struct Point {int x, y;}
	Template declarations				template<class T> class Z;
	Template definitions				template<class T> class V {/* */};
	Function declarations				extern int strlen(const char *)
	inline function definitions			inline char get(char* p) {/* */}
	constexpr function definitions		constexpr int fac(int n) {...}
	Data declarations					extern int a;	
	const definitions					const float pi = 3.141593;
	constexpr definitions				constexpr float pi2 = pi * pi;
	Enumerations						enum class Light {red. ye;;pw. greem}
	Name declarations					class Matrix;
	Type aliases						using value_type = long;
	Compile-time assertions				static_assert(4<sizeof(int), 'small ints')
	Include directives					#include <algorithm>
	Macro definitions					#define VERSION 12.03
	Conditional compilation directives	#ifdef __cplusplus
	Comments							/* check for end of file */

Two definitions of a class, template, or inline function are accepted as examples of the same unique definition if and only if:

*	They appear in different translation unit, and
*	They are token-for-token identical, and
*	the meanings of those tokens are the same in both translations

Often, `extern "C"` is used to link to fortran and assembler routines that happen to conform to the conventions of a C implementation.

15.2.6 未看懂

In principle, a variable outside any function(that is, global, namespace, and class static variable) is initialized before main() is invoked.

A program can terminate in several ways:

*	By returning from `main`
*	By calling `exit`
*	By calling `abort`
*	By throwing an uncaught exception
*	By violating `noexcept`
*	By calling `quick_exit`

If a program is terminated using the standard library `exit()`, the destructor for constructed static objects are called. However, if the program is terminated using the standard library function `abort`, they are not, Note that this implies that `exit()` does no terminate a program immediately, calling exit in a destructor may cause an infinite recursion.

calling exit means that the local variable of the calling function and its callers will not have their destructors invoked. Throwing an exception and catching it ensures that local objects are properly destroyed. Also, a call of `exit` terminates the program without giving the caller of the function that called `exit` a chance to deal with the problem. It is therefore often best to leave a context by throwing an exception and letting a handler decide what to do next. 

The C and C++ standard library function `atexit()` offers the possibility to have code executed at program termination. For example:

	void my_cleanup();

	void somewhere() {
		if (atexit(&my_cleanup) == 0) {
			// my cleanup will be called at normal termination
		} else {
			// oops: too many atexit functions
		}
	}

An argument to atexit() cannot take arguments or return a result, and there is an implementation-defined limit to the number of atexit functions. A nonzero value returned by `atexit()` indicates that the limit is reached. These limitations make atexit() less useful than it appears at first glance. Basically, atexit() is a C workaround for the lack of destructors.

The destructor of a constructed statically allocated object (§6.4.2) created before a call of atexit(f) will be invoked after f is invoked. The destructor of such an object created after a call of atexit(f) will be invoked before f is invoked.

The `quick_exit()` function is like exit() except that it does not invoke any destructors. You register functions to be invoked by quick_exit() using at_quick_exit() .

The exit() , abort() , quick_exit() , atexit() , and at_quick_exit() functions are declared in <cstdlib> .