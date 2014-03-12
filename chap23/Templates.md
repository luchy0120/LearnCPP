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



