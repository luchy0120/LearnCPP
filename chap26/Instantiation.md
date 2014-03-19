From a template function and a set of template arguments, a function needs to be generated. This process is commonly called template instantiation.

The generated classes and functions are called specializations.

###When is Instantiation Needed

It is necessary to generate a specialization of a class template only if the class's definition is needed. In particular, to declare a pointer to some class, the actual definition of a class is not needed.

	class X;
	X* p;		// OK: no definition of X needed
	X a;		// error: definition of X needed

When defining template classes, this distinction can be crucial. A template class is not instantiated unless its definition is actually needed.

	template<typename T>
	class Link {
		Link *suc;		// OK: no definition of Link needed(yet)
		// ...
	};

	Link<int> *pl;		// no instantiation of Link<int> needed(yet)
	Link<int> lnk;		// now we need to instantiate link<int>

Instantiation of a class template does not imply the instantiation of all of its member functions.

###Manual Control of Instantiation

An explicit instantiation request(often simply called an explicit instantiation) is a declaration of a specialization prefixed by the keyword `template`(not followed by `<`)

	template class vector<int>;
	template int& vector<int>::operator[](int);

Note that `template` prefixes a complete declaration; just stating a name is not sufficient:

	template vector<int>::operator[];	// syntax error
	template convert<int, double>;		// syntax erro

As in template function calls, the template arguments that can be deduced from the function arguments can be immited.

	template int convert<int, double>(double);	// OK, redundant
	template int convert<int>(double);			// OK

When a class template is explicitly instantiated, every member function is also instantiated.

##Name Binding

The process of finding the declaration for each name explicitly or implicitly used in a template is called *name binding*, The general problem with template name binding is that three contexts are involved in a template instantiation and they can not be cleanly separated:

*	The context of the template definition
*	The context of the argument type declaration
*	The context of the use of the template

###Dependent Names

A function call is said to depend on a template argument if and only of one of these condition holds:

*	The type of the actual argument depends on a template parameter `T` according to the type deduction rules.
*	The function called has a parameter that depends on T according to the type deduction rules.

Basically, the name of a called function is dependent if it is obviously dependent by looking at its argument or at its formal parameters.

By default, a dependent name is assumed to name something that is not a type. So, to use a dependent name as a type, you have to say so, using the keyword `typename`.

	template<typename Container>
	void fct(Container& c) {
		Container::value_type v1 = c[7];	// syntax error: value_type is assumed to be a non-type name
		typename Container::value_type v2 = c[9];	// OK, value_type assumed to name a type
		auto v3 = c[11];		// ok, let the compiler figure it out
	}

We can avoid such awkward use of `typename` by introducing a type alias.

	template<typename T>
	using Value_type<T> = typename T::value_type;

	template<typename Container>
	void fct2(Container& c) {
		Value_type<Container> v1 = c[7];	// OK
	}

Naming a member template after a `.`, `->` or `::` requires similar use of the keyword `template`.

	class Pool {
	public:
		template<typename T> T* get();
		template<typename T> void release(T*);
	};

	template<typename Alloc>
	void f(Alloc& all) {
		int *p1 = all.get<int>();	// syntax error: get is assumed to name a non-template
		int* p2 = all.template get<int>();	// OK, get() is assumed to be a template
	}

Compared to the use of `typename` to explicitly state that a name is assumed to name a type, the use of `template` to explicitly state that a name is assumed to name a template is rare.

###Point-of-Definition Binding
From a template function and a set of template arguments, a function needs to be generated. This process is commonly called template instantiation.

The generated classes and functions are called specializations.

###When is Instantiation Needed

It is necessary to generate a specialization of a class template only if the class's definition is needed. In particular, to declare a pointer to some class, the actual definition of a class is not needed.

	class X;
	X* p;		// OK: no definition of X needed
	X a;		// error: definition of X needed

When defining template classes, this distinction can be crucial. A template class is not instantiated unless its definition is actually needed.

	template<typename T>
	class Link {
		Link *suc;		// OK: no definition of Link needed(yet)
		// ...
	};

	Link<int> *pl;		// no instantiation of Link<int> needed(yet)
	Link<int> lnk;		// now we need to instantiate link<int>

Instantiation of a class template does not imply the instantiation of all of its member functions.

###Manual Control of Instantiation

An explicit instantiation request(often simply called an explicit instantiation) is a declaration of a specialization prefixed by the keyword `template`(not followed by `<`)

	template class vector<int>;
	template int& vector<int>::operator[](int);

Note that `template` prefixes a complete declaration; just stating a name is not sufficient:

	template vector<int>::operator[];	// syntax error
	template convert<int, double>;		// syntax erro

As in template function calls, the template arguments that can be deduced from the function arguments can be immited.

	template int convert<int, double>(double);	// OK, redundant
	template int convert<int>(double);			// OK

When a class template is explicitly instantiated, every member function is also instantiated.

##Name Binding

The process of finding the declaration for each name explicitly or implicitly used in a template is called *name binding*, The general problem with template name binding is that three contexts are involved in a template instantiation and they can not be cleanly separated:

*	The context of the template definition
*	The context of the argument type declaration
*	The context of the use of the template

###Dependent Names

A function call is said to depend on a template argument if and only of one of these condition holds:

*	The type of the actual argument depends on a template parameter `T` according to the type deduction rules.
*	The function called has a parameter that depends on T according to the type deduction rules.

Basically, the name of a called function is dependent if it is obviously dependent by looking at its argument or at its formal parameters.

By default, a dependent name is assumed to name something that is not a type. So, to use a dependent name as a type, you have to say so, using the keyword `typename`.

	template<typename Container>
	void fct(Container& c) {
		Container::value_type v1 = c[7];	// syntax error: value_type is assumed to be a non-type name
		typename Container::value_type v2 = c[9];	// OK, value_type assumed to name a type
		auto v3 = c[11];		// ok, let the compiler figure it out
	}

We can avoid such awkward use of `typename` by introducing a type alias.

	template<typename T>
	using Value_type<T> = typename T::value_type;
	template<typename Container>
	void fct2(Container& c) {
		Value_type<Container> v1 = c[7];	// OK
	}

Naming a member template after a `.`, `->` or `::` requires similar use of the keyword `template`.

	class Pool {
	public:
		template<typename T> T* get();
		template<typename T> void release(T*);
	};
	template<typename Alloc>
	void f(Alloc& all) {
		int *p1 = all.get<int>();	// syntax error: get is assumed to name a non-template
		int* p2 = all.template get<int>();	// OK, get() is assumed to be a template
	}

Compared to the use of `typename` to explicitly state that a name is assumed to name a type, the use of `template` to explicitly state that a name is assumed to name a template is rare.

###Point-of-Definition Binding

If a declaration is found, that declaration is used even if a "better" declaration might be found later.

###Point-of-Instantiation Binding

The context used to determine the meaning of a dependent name is determined by the use of a template with a given set of arguments. This is called a point of instantiation for that specialization.

	void g(int);

	template<typename T>
	void f(T a) {
		g(a);	// g is bound at a point of instantiation
	}

	void h(int i) {
		extern void g(double);
		f(i);
	}

The point of instantiation for `f<int>()` is outside `h()`. This is essential to ensure that the `g()` called in `f()` is the global `g(int)` rather than the local `g(double)`. An unqualified name used in a template definition can never be bound to a local name. Ignoring local names is essential to prevent a lot of nasty macro-like behavior.

To enable recursive calls, the point of declaration for a function template is after the declaration that instantiates it.

	void g(int);
	template<typename T>
	void f(T a) {
		g(a);		// g is bound at a point of instantiation
		if (i) {
			h(a-1);	// h is bound at a point of instantiation
		}
	}
	void h(int i) {
		extern void g(double);
		f(i);
	}
	// point of declaration for f<int>

For a template class or a class member, the point of instantiation is just before the declaration containing its use.

Relying on template arguments to make dependencies explicit simplifies our thinking about the template code and even allows us to access local information.

	void fff() {
		struct S {int a, b;};
		vector<S> vs;
	}

###Multiple Instantiation Points

A template specialization may be generated

*	At any point of instantiation
*	At any point subsequent to that in a translation unit
*	or in a translation unit specifically created for generating specializations.

This reflects three obvious strategies an implementation can use for generating specializations:

1.	Generate a specialization the first time a call is seen
2.	At the end of a translation unit, generate all specialization needed for it.
3.	Once every translation unit of a program has been seen. generate all specializations needed for the program.

Here is an example:

	void f(int);
	namespace N {
		class X{};
		char g(X, int);
	}

	template<typename T>
	void ff(T t, double d) {
		f(d);
		return g(t.d);
	}

	auto x1 = ff(N::X{}, 1.1);

	namespace N {
		double g(X, double);
	}

	auto x2 = ff(N::X{}, 2.2);

It is sloppy programming to call an overloaded function between two of its declarations.

To avoid surprising name bindings, try to limit context dependencies in templates.

###Templates and Namespaces

The binding of dependent names is done by looking at:

1.	the names in scope at the point where the template is defined.
2.	the names in the namespace of an argument of a dependent call.

###Names from Base Classes

When a class template has a bse class, it can access names from that base. As for other names, there are two distinct possibilities:

*	The base class depends on a template argument
*	The base class does not depend on a template argument

Here the example goes:

	void g(int);

	struct B {
		void g(char);
		void h(char);
	};

	template<typename T>
	class X: public B {
	public:
		void h(int);
		void f() {
			g(2);	// call B::g(char)
			h(2);	// call X::h(int)
		}
	};

the call `g(2)` is bound to `B::g(char)` without any concern for functions declared outside X. That is, the global `g()` is never considered.


