##Introduction

Programming that manipulates program entities, such as classes and functions, is commonly called *metaprogramming*, It is useful to think of templates as generators: they are used to make classes and functions

There are two main reasons for using metaprogramming techniques:

*	Improved type safety: We can compute the exact types needed for a data structure or algorithm so that we do not need to directly manipulate low-level data structures(we can eliminate many usess of explicit type conversion)
	
*	Improved run-time performance: we can compute values at compile time and select functions to be called at run time. That way, we do not need to do those computations at run time.

The C++ compile time mechanisms provide a pure functional programming language: You can create values of various types, but there are no variables, assignments, increment operators, etc.

The turing completeness implies the possibility of infinite compilcations.

Where should we draw the line between generic programming and template metaprogramming? The extreme positions are:

*	It is all template metaprogramming: after all, any use of compile-time parameterization implies instantiation generates "ordinary code"

*	It is all generic programming: after all, we are just defining and using generic types and algorithms.

Generic programming is primarily a design philosophy - a programming paradigm, if you must.

In contrast, metaprogramming is programming, the emphasis is on computation, often involving selection and some form of iteration. Metaprogramming is promarily a set of implementation techiques.

Four levels of implementation complexity:

1.  No computation(just pass type and value arguments)
2.  Simple computation(on types or values) not using compile-time tests or iteration, for example, `&&` of Booleans or addition of units.
3.  Computation using explicit compile-time tests, for example, a compile-time `if`.
4.  Computation using compile-time iteration(in the form of recursion)

Generic programming focuses on interface specification, whereas metaprogramming is programming, usually with types as the values.

##Type Functions

A *type function* is a function that either takes at least one type argument or produces at least one type as a result. For example, `sizeof(T)` is a built-in type function that given a type argument T, returns the size of an object(measured in `char`s)

Type function do not have to look like conventional functions, in fact, most do not.

The standard library convention is that a type function that returns a type does so by a member called `type`.

Type functions are compile-time functions, that is, they can only take arguments(types and values) that are known at compile time and produce results(types and values) that can be used at compile time.

C++ type functions are mostly templates. They can perform general computations using types and values. They are the backbone of metaprogramming.

###Type Alias

By introducing a template alias, we can hide the `::type` implementation details and make a type function look more like a function returning a type(or like a type)

    template<typename T>
    using Holder = typename Obj_holder<T>::type;

    void f2() {
        Holder<double> v1;              // the double goes on the stack
        Holder<array<double, 200>> v2;  // the array goes on the free store
        // ...
        *v1 = 7.7;  // scoped provides pointer-like access (* and [])
        v2[77] = 9.9;   // On_heap provides pointer-like access(* and [])
        // ...
    }

Please note that these alias are unfortunately not part of the standard

####When Not to Use an Alias

There is one case in which it is significant to use `::type` directly, rather than an alias. If only one of the alternatives is supposed to be a valid type, we should not use an alias. Consider first a simple analogy.

###When to use Metaprogramming

The most obvious constraint on metaprogramming is that code depending on complicated uses of templates can be hard to read and very hard to debug.

Alternatively, we can look at the fundamental complexity of what we are trying to do:

1.  Does it require explicit tests
2.  Does it require recursion
3.  Can we write concepts for our template arguments

This is separator;

##Conditional Definition: **Enable_if**

Here is a sample code snippet:

    template<typename T>
    class Smart_pointer {
        // ...
       T& operator*();
       if (Is_class<T>()) T* operator->();  // syntax error
    };

This does not work, C++ does not provide an `if` that can select among definitions based on a general condition. The standard library in `<type_traits>` provides `enable_if` for that:

    template<typename T>
    class Smart_pointer {
        // ...
        T& operator*();
        Enable_if<Is_class<T>(), T>* operator->();
        // ... 
    };

##A Compile-Time List: **Tuple**

The idea is to allow code like this:

    Tuple<double, int, char> x{1.1, 43, 'a'};
    cout << x << "\n";
    cout << get<1>(x) << "\n";

The resulting output is:

    {1.1, 42, 'a'};
    42

###A Simple Output Function

The definition of Tuple has a nice regular, recursive structure that we can use to define a function for displaying the list of elements;

    template<typename T1, typename T2, typename T3, typename T4>
    void print_elements(ostream& os, const Tuple<T2, T2, T3, T4>& t) {
        os << t.x << ', ';
        print_elements(os, *.t.base());
    }

###make_tuple

A class template can not deduce its template arguments, but a function template can deduce them from its function arguments.(proved by the sample `clstmp.cpp`) This implies that we can make a `Tuple` type implicit in code by having a function construct it for us:

    template<typename T1, typename T2, typename T3, typename T4>
    Tuple<T1, T2, T3, T4> make_tuple(const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
        return Tuple<T1, T2, T3, T4>{t1, t2, t3, t4};
    }

##Variadic Templates

OVer the years, many solutionns have been found. For example, default arguments can be used to allow a single function to accept a variable number of arguments, and function overloading can be used to provide a function for each number of arguments as long as the elements are all of the same type. However, to elegantly handle the case of an unknown number of arguments of unknown(and possibly differing) types, some additional language support is needed. The language feature is called **variadic template**

###A Type-Sage printf()

The `Args...` defines what is called a parameter pack. A parameter pack is a sequence of (type/value) pairs from which you can peel off arguments starting with the first.

###Technical Details

If you have a parameter pack, you can expand it into its sequence of elements by placing a `...` after it.

    template<typanem T, typename... Arga>
    void printf(const char* s, T value, Args... args) {
        // ...
        return printf(++s, args...);
    }

Expansion of a parameter pack into its elements is not restrict to function calls.

    template<typename... Bases>
    class X : public Bases... {
    public:
        X(const Bases&... b) : Bases(b)... {}
    }

    X<> x0;
    X<> x1(1);
    X<Bx, By> x2(2, 3);
    X<Bx, By, Bz> x3(2, 3, 4);

Her, **Bases...** says that `X` has zero or more bases. When it comes to initializing an X, the constructor requires zero or more values of types specified in the `Bases` variadic template argument. One by one those values are passed to the corresponding base initializer.

We can use the ellipsis to mean "zero or more elements of something" in most places where a list of elements is required, such as in:

*   A template argument list
*   A function argument list
*   An initializer list
*   A base specifier list
*   A base or member initializer list
*   A `sizeof...` expression

A `sizeof...` expression is used to obtain the number of elements in a parameter pack.

###Forwarding

One of the major uses of variadic templates is forwarding from one function to another. Consider how to write a function that takes as arguments something to be called and a possibly empty list of arguments to given to the "something" as arguments:

    template<typename F, typename... T>
    void call(F&& f, T&&...t ) {
        f(forward<T>(t)...);
    }

##SI Units Example

This example is a little bit complicated... Waiting to **RETRY**.
