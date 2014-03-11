Casting from a base class to a derived class if often called a *downcast* because of the convention of drawing inheritance trees growing from the root down. Similarly, a cast from a derived class to a base is called an *upcast*. A cast that goes from a base to a sibling class is called a *crosscast*.

###dynamic_cast

The dynamic_cast operator takes two operands: a type bracketed by < and >, and a pointer or reference bracketed by ( and ).

It is reassuring to know that dynamic_cast does not allow accidental violation of the protecton of private and protected base classes. Since a dynamic_cast used as an upcast is exactly like a simple assignment, it implies no overhead and is sensitive to its lexical context.

The purpose of dynamic_cast is to deal with the case in which the correctness of the conveersion can not be determined by the compiler. In that case, dynamic_cast<T*>(p) looks at the object pointed to by p(if any). If that object is of class T or has a unique base class of type T, then dynamic_cast returns a pointer of type T* to that object; otherwise, nullptr is returned. If the value of p is nullptr, dynamic_cast<T*>(p) returns nullptr.

A dynamic_cast requires a pointer or a reference to a polymorphic type in order to do a downcast or a crosscast.(here polymorphic type means it has virtual functions)

Restricting dynamic_cast to polymorphic types also makes sense from a logica point of view. That is, if an object has no virtual functions, it can not safely be manipulated without knowledge of its exact type. Consequently, care should be taken not to get such an object into a context in which its type is not known. If its type is known, we do not need to use dynamic_cast.

The target type of dynamic_cast need not be polymorphic. This allows us to wrap a concrete type in a polymorphic type.

A dynamic_cast to void* can be used to determine the address of the beginning of an object of polymorphic type.

There is no dynamic_cast from void*(because there would be no way of knowing where to find the vptr)

####dynamic_cast to Reference

Given a pointer result, we must consider the possibility that the result is nullptr, that is, that the pointer does not point to an object. Consequently, the result of a dynamic_cast should always be explicitly tested.

On the other hand, we may legitimately assume that a reference refers to an object. Consequently, `dynamic_cast<T&>(r)` of a reference r is not a question but an assertion: "The object refered to by r is of type T".

The difference in results of a failed dynamic pointer cast and a failed dynamic reference cast reflects a fundamental difference between references and pointers. If a use wants to protect against bad casts to references. a suitable handler must be provided.(This means try ... catch)

###Multiple Inheritance

Naturally, we try to keep hierarchies as simple as our application allows(and no simpler). However, once a nontrivial hierarchy has been constructed, we sometimes need to navigate it to find a specific class to use. This need occurs in two variants:

*	Sometimes, we want to explicitly name a base class for use as an interface, for example, to resolve an ambiguity or to call a specific function without relying on the virtual function mechanism.
*	Sometimes, we want to obtain a pointer to a subobject of a hierarchy given a pointer to another, for example, to get a pointer to the complete derived class object from a pointer to a base(a downcast) or to get a pointer to a base class object from a pointer to another base(a cross cast)

An example:

	class Component: public virtual Storable;
	class Receiver: public Component;
	class Transmitter: public Component;
	class Radio: public Receiver, public Transmitter;

Here, a `Radio` object has two subobjects of class `Component`, Consequently, a `dynamic_cast` from Storable to Component within a Radio will be ambiguous and return 0, There is simply no way of knowing which Component the programmer wanted.

###static_cast and dynamic_cast

A `dynamic_cast` can cast from a polymorphic virtual base class to a derived class or a sibling class. A `static_cast` does not examine the object it casts from. so it can not:

	void g(Radio& r) {
		Receiver* prec = &r;		// Receiver is an ordinary base of Radio
		Radio* pr = static_cast<Radio*>(prec);	// OK, unchecked
		pr = dynamic_cast<Radio*>(prec);	// OK, run-time checked

		Storable* ps = &r;		// Storable is a virtual base of Radio
		pr = static_cast<Radio*>(ps);	// error, can not cast from virtual base
		pr = dynamic_cast<Radio*>(ps);	// OK, runtime checked
	}

The `dynamic_cast` requires a polymorphic operand because there is no information stored in a nonpolymorphic object that can be used to find the objects for which it represents a base.

The compiler can not assume anything about the memory pointed to by a `void*`. This implies that `dynamic_cast` - which must look into an object to determine its type - can not cast from a `void*`. For that a `static_cast` is needed.

	Radio* f1(void* p) {
		Storable *ps = static_cast<Storable*>(p);	// trust the programmer
		return dynamic_cast<Radio*>(ps);
	}

Both `dynamic_cast` and `static_cast` respect `const` and access controls

	class Users: private set<Person>;
	void f2(Users* pu, const Receiver* pcr) {
		static_cast<set<Person>*>(pu);	// error: access violation
		dynamic_cast<set<Person>*>(pu);	// error: access violation

		static_cast<Receiver*>(pcr); // error: can not cast away const
		dynamic_cast<Receiver*>(pcr);// error: can not cast away const

		Receiver* pr = const_cast<Receiver*>(pcr); // OK
	}

It is not possible to cast to a private base class using `static_cast` and `reinterpret_cast` and "casting away const"(or volatile) requires a `const_cast`. Even then, using the result is safe only provided the object was not originally declared const(or volatile)

###Recovering an Interface

From a design perspective, dynamic_cast can be seen as a mechanism for asking an object if it provides a given interface.

This phase is a little difficult, need review(above 653)

##Double Dispatch and Visitors

###Double Dispatch

这里描述了一个问题就是求两个Shape的intersect，因为Shape是一个接口基类，所以如果要做intersect就特别麻烦，因为如果使用virtual function的话需要增加的方法太多，而且在需要增加一个子类时会导致较大的变化。

作者提供了一种解决方案，就是预先定义好所有可能的组合表。

###Visitors

Here is an example:

	class Visitor;

	class Node {
	public:
		virtual void accept(Visitor&) = 0;
	};

	class Expr: public Node {
	public:
		void accpet(Visitor&) override;
	};

	class Stmt: public Node {
	public:
		void accept(Visitor&) override;
	};

Now the Node's accept() performs the double-dispatch trick and passes the Node itself to the Visitor's accept():

	class Visitor {
	public:
		virtual void accept(Expr&) = 0;
		virtual void accept(Stmt&) = 0;
	};

We can then define sets of operations by deriving from Visitor and overriding its accept() functions.

	class Do1_visitor: public Visitor {
		void accept(Expr&) {cout << "...";}
		void accept(Stmt&) {cout << "...";}
	};

As opposed to the simple double dispatch, the visitor pattern is heavily used in real-world programming. It is only mildly intrusive(The accept() function), and many variations on the basic idea are used. However, many operations on class hierarchies are hard to express as visitors.

Most alternatives to visitors in C++ are based on the idea of explicit iteration over a homogeneous data structure(a vector or a graph of nodes containing pointers to polymorphic types), at each element or node, a call of a virtual function can perform the desired operation, or some optimization based on stored data can be applied.

##Construction and Destruction

It is unwise to rely on details of the order of construction and destruction, but you can observe that order by calling virtual functions, `dynamic_cast` or `typeid` at a point where the object is not complete. 

It is best to avoid calling virtual functions during construction and destruction.

##Type Identification

The `dynamic_cast` operator serves most needs for information about the type of an object at run time. Importantly, it ensures that code written using it works correctly with classes derived from those explicitly mentioned by the programmer.

`typeid()` returns a reference to a standard-library type called `type_info` defined in `<typeinfo>`:

*	Given the name of a type as its operand, `typeid(type_name)` returns a reference to a `type_info` that represents the `type_name`; `type_name` must be a completely defined type.
*	Given an expression as its operand, `typeid(expr)` returns a reference to a `type_info` that represents the type of the object denoted by the `expr`; the `expr` must refer to a completely defined type. If the value of expr is nullptr, typeid(expr) throws a `std::bad_typeid`.

If the operand of `typeid()` is a pointer or a reference of a polymorphic type with the value `nullptr`. `typeid()` throws a `std::bad_typeid`. If the operand of `typeid()` has a nonpolymorphic type or is not an lvalue, the result is determined at compile time without evaluating the operand expression.

If the object denoted by a dereferenced pointer or a reference to a polymorphic type, the `type_info` returned is that of the most derived class for the object, that is, the type used when the object was defined.

###Extended Type Information

Associating typeids with information without modifying system headers allows several people or tools to associate different information with types independently of each other. This is important because the likelihood that someone can come up with a single set of information that satisfies every user is close to zero.

Questions:

It is time to learn the differences between static_cast and dynamic_cast. Currently, the static_cast happens at compile time, and dynamic_cast happens at run time, besieds, the target type of dynamic_cast should be polymorphic in that it need the vptr information, there are three types of dynamic_cast, upcast, downcast, crosscast.


