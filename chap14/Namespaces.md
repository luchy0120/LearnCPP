The global scope is a namespace and can be explicitly referred to using `::`.

	int f();		// global function

	int g() {
		int f;		// local variable; hides the global function
		f();		// error: we can not call an int
		::f();		// OK: call the global function
	}

We can use a using-declaration to say that in this code `string` means `std::string`

We must be careful with using-directives in the global scope. In particular, do not place a using-directive in the global scope in a header file except in very specialized circumstances because you never know where a header file might be included

A function taking an argument of user-defined type X is more often than not defined in the same namespace as X. Consequently, if a function is not found in the context of its use, we look in the namespaces of its arguments.

	namespace chrono {
		class Data {/*...*/};
		bool operator == (const Data&, const std::string&);
		std::string format(const Data&);	// make string representation
	}

	void f(chrono::Data d, int i) {
		std::string s = format(d);		// chrono::format
		std::string t = format(i);		// error, no fotmat() in scope
	}

Argument-dependent lookup is especially useful for operator operands and template arguments where explicit qualification can be quite cumbersome

When a class member invokes a named function, other members of the same class and its base classes are preferred over functions potentially found based on the argument type(operators follow a different rule)

	namespace N {
		struct S {int i};
		void f(S);
		void g(S);
		void h(int);
	}

	struct Base {
		void f(N::S);
	};

	struct D : Base {
		void mf();
		void g(N::S x) {
			f(x);			// call Base::f()
			mf(x);			// call D::mf()
			h(1);			// error: no h(int) available
		}	
	};

The rules for argument-dependent lookup:

*	If an argument is a class member, the associated namespaces are the class itself(including its base classes) and the class's enclosing namespaces.
*	If an argument is a member of a namespace, the associated namespaces are the enclosing namespaces.
*	If an argument is a built-in type, there are no associated namespaces.

The search for a declaration of a function f() does not have a preference for functions in a namespace in which f() is called(the way it does for functions in a class in which f() is called)

	namespace N {
		template<class T> void f(T, int);		// N::f()
		class X {};
	}

	namespace N2 {
		N::X x;
		void f(N::X, unsigned);

		void g() {
			f(x, 1);		// calls N::f(X, int)
		}
	}

A namespace is open, that is, you can add names to it from several separate namespace declarations.

	namespace A {
		int f();		// now A has member f()
	}

	namespace A {
		int g();		// now A has two members, f() and g()
	}

Notice: a namespace alias can not be used to re-open a namespace

Global names are not given preference over names from namespaces mode accessible in the global scope.

Namespace alias can allow a user to refer to "the library" and have a single declaration defining what library that really is.

	namespace Lib = Foundation_library_v2r11;

	// ...

	Lib::set s;
	Lib::string s = "Sibelius";

Ideally, a namespace should:

*	express a logically coherent set of features
*	not give users access to unrelated features, and
*	not impose a significant notational burden on users

There is no general language mechanism for renaming, but for types and templates, we can introduce alias with using.

inline namespace is the default meaning of a namespace


