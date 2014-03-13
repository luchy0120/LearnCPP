##Introduction and Overview

The standard library provides the template class `basic_string` that is similar to the templatized `String`. In the standard library, `string` is a synonym for `basic_string<char>`

	using string = std::basic_string<char>

In general, type alias are useful for shortening the long names of classes generated from templates. Also, we often prefer not to know the details of how a type is defined, and an alias allows us to hide the fact that a type is generated from a template.

###Defining a Template

Using a template can lead to a decrease of code generated because code for a member function of a class template is only generated if that member is used.

A template is a specification of hwo to generate something given suitable template arguments; the language mechanism for doing that generatin(instantiation) and specification do not care much whether a class or a function is generated. So, unless otherwise stated, the rules for templates apply equally to class templates and function templates. Templates can also be defined as alias, but other plausible constructs, such as namespae templates , are not provided.

A generic component should be developed as a generalization of one or more concrete examples, rather than simply being designed from first principles.

A template member need not be defined within the template class itself. In that case, its definition must be provided somewhere else, just as non-templaet class members.

It is not possible to overload a class template name.

###Template INstantiation

The process of generating a class or a function from a template plus a template argument is often called template instantiation. A version of template fro a specific template argument list is called a specification.

##Type Checking

The fundamental weakness of the template mechanism is that it is not possible to directly express requirements on a template argument.

###Error Detection

Not all simple errors are easy to detect, here we made three mistakes:

*	A simple syntax errr: Leaving out a semicolon at the end of a declaration
*	A simple type error: Independently of what the template parameter is, a pointer can not be initialized by the integer 7
*	A name lookup error: The identifier o(a mistypes 0 of course) can not be an argument .

##Class Template Members

Exactly like a class, a template class can have members of several kinds

*	Data members
*	Member functions
*	Member type alias
*	static members(function and data)
*	Member types(e.g. a member class)
*	Member templates(e.g. a member class template)

In addition, a class template can declare friends just as ordinary class can.

###Data Members

Non-static data memers can be const, but unfortunately not constexpr

###Member Functions

A member function of a template can be virtual or not. However, a virtual member function can not also be a member function template.

###Member Type Aliases

Type aliases play a major role in generic programming by allowing the designer of classes to provide common names for types in different classes(and class templates) with common semantics. Type names as member aliases are often refered to as associated types. The `value_type` and `iterator` names are borrowed from the standard library's container design.',  If a class is missing a desired member alias, a trait can be used to compensate.

###static Members

A `static` data or function member that is not defined in-class must have a unique definition in a program.

	template<typename T>
	struct X {
		static constexpr Point p {100, 250};	// point must be a literal type
		static const int m1 = 7;
		static int m2 = 8;			// error, not const
		static int m3;
		static void f1() {}
		static void f2();
	};

As for non-temmplate classes, a `const` or `constexpr static` data member of literal type can be initialized in-class and need not be defined outside the class.

A static member need only be defined if its used.

	template<typename T>
	struct X {
		static int a;
		static int b;
	};

	int* p = &X<int>::a;

If this is all the mention of `X<int>` in a program, we will get a "not defined" error for `X<int>::a`, but not for	`X<int>::b`.

###Member Types

As for an `ordinary class`, we can define types as members. As usual, such a type can be a class or an enumeration.

	template<typename T>
	struct X {
		enum E1 {a, b};
		enum E2;	// error: underlying type not known
		enum class E3;
		enum E4: char;

		struct C1 {/*...*/};
		struct C2;
	};

	template<typename T>
	enum class X<T>::E3 {a,b};		// needed

	template<typename T>
	enum class X<T>::E4 : char {x, y};	// needed

	template<typename T>
	struct X<T>::C2 {/* .. */};		// needed

The out of class definition of a member enumeration is only allowed for an enumeration for which we know the underlying type.

###Member Templates

A class or a class template  can have members that are themselves templates. For example, complex numbers are best represented as pairs of values of some scalar type:

	template<typename Scalar>
	class complex {
		Scalar re, im;
	public:
		complex: re{}, im{} {}	// default constructor
		template<typename T>
		complex(T rr, T ii=0):re{rr}, im{ii}{}

		complex(const complex&)=default;
		template<typename T>
		complex(const complex<T>&c):re{c.real()},im{c.imag()}{}
	
	};

This allows mathematically meaningful conversions among complex types, while prohibiting the undesirable narrowing conversions.

####Templates and Constructors

A template constructor is never used to generate a copy constructor, so without the explicitly declared copy constructor, a default copy constructor would have been generated. Similarly, copy assignments, move constructors, and move assignments must be defined as non-template operators or the default versions will be generated.

####Templates and virtual

A member template can not be virtual.

	class Shape {
		// ...
		template<typename T>
			virtual bool intersect(const T&) const = 0;	// error, virtual template
	
	};

####Use of Nesting

Avoid nested types in templates unless they genuinely rely on every template parameter.

###Friends

	template<typename T>
	class Vector {
		T v[4];
	public:
		friend Vector operator*<>(const Matrix<T>&, const Vector&);
		// ...
	};

The `<>` after the name of the friend function is needed to make clear that the friend is a template function, without the `<>`, a non-template function would be assumed. The multiplication operator can then be defined to access data from `Vector` and `Matrix` directly.

Friends do not affect the scope in which the template class is defined, nor do they affect the scope in which the template is used. Instead, friend functions and operators are found using a lookup based on their argument types. Like a member function, a friend function is instantiated only if it is used.

Like other classes, a class template can designate other classes as friends.

We cannot directly make a template a friend of a class, but we can make a friend declaration a template.

	template<typename T, typename A>
	class List;

	template<typename T>
	class Link {
		template<typename U, typename A>
			friend class List;
	};

Unfortunately, there is no way of saying that `Link<X>` should only be a friend of `List<X>`.

Friend classes are designed to alow the representation of small clusters of closely related concepts. A complicated pattern of friendship is almost certainly a design error.

##Function Templates Overloading

We apply the usual function overload resolution rules to these specializatons and all ordinary functions

*	Find the set of function template specifications that will take part in overload resolution. Do this by considering each function template and deciding which template arguments, if any, would be used if no other function templates or functions of the same name were in scope.

*	If two function templates can be called and one is more specialized than the other, consider only the most specialized template functon in the following steps.

*	Do overload resolution for this set of functions, plus any ordinary functions, as for ordinary functions. If a function template's argument has been determined by template argument deduction, that argument cann't also have promotions, standard conversions or user-defined conversions applied.

*	If a function and a specialization are equally good matches, the function is prefered. 

*	If no match is found, the call is an error, if we end up with two or more equally good matched, the call is ambiguous and is an error.

####Argument Substitution Failure

There is a language rule that says that such a substitution failure is not an errir. It simply causes the template to be ignored.

##Template Aliases

In general, if we bind all arguments of a template, we get a type, but if we bind only some, we get a template. Note that what we get from using in an alias definition is always an alias.

##Source Code Organization

There are three reasonably obvious ways of organizing code using templates:

*	Include template definitions before their use in a translation unit
*	Include template declarations(only) before their use in a translation unit. Include definitions of templates later in the translation unit(potentially after their use).
*	Include template declarations(only) before their use in a translation unit. Define the templates in some other translation unit.

For technical and historical reasons, options[3], the sepatate compilatio of template definitions and their uses, is not offered. By far the most common approach is to include(usually include) the definitions of the templates you use in every translation unit in which you use them and rely on your implementation to optimize compile times and eliminate object code dupication.

As ever, non-inline, non-template functions and static members must have a unique definition in some compilation unit. This implies that such members are best not used for templates that are otherwise included in many translation units.
