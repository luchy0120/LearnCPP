##Multiple Inheritance

Inheritance aims to privide one of two benefits:

*	Shared interfaces: leading to leass replication of code using classes and making such code more uniform. This is often called run-time polymorphism or interface inheritance.
*	Shared implementation: leading to less code and more uniform implementation code. This is often called implementation inheritence.

###Multiple Interfaces

In face, any class without mutable state can be used as an interface in a multiple-inheritance lattice without significant complications and overhead.

###Ambiguity Resolution

When two bases have member functions with the same name. For example:

	class A {
		virtual void f();
	};

	class B {
		virtual void f();
	};

	class C: public A, public B {
		
	};

	void g(C& c) {
		c.f();		// error ambiguous
		c.A::f();	// OK
		c.B::f();	// OK
	};

However, explicit disambiguation is messy, so it is usually best to resolve such problems by defining a new function in the derived class.

	class C: public A, public B {
		void f() {
			// ...
		}
	};

A function declared in a derived class overrides all functions of the same name and type in its base classes. Typically, that is exactly the right thing to do because it is generally a bad idea to use the same name for operations with different semantics in a single class.

Here is a mapping example

	class A {
		virtual void f() = 0;
	};

	class B {
		virtual void f() = 0;
	};

	class C: public A {
		using A::A;			// constructor inheritance
		void f() override final {af();}
		virtual void af() = 0;
	};

	class D: public B {
		using B::B;
		void f() override final {bf();}
		virtual void bf() = 0;
	};

###Repeated Use of a Base Class

Given such a case: A is a base, B inherit from A, C inherite from A, and D inherite from B and C. We could imagine two cases:

1.	A D object has two subobjects of class A, one for B and one for C.
2.	A D object has one subobject of class A, shared by B and C.

A virtual function of a replicated base class can be overriden by a single function in a derived class.

###Virtual Base Classes

We avoid replication by declaring a base virtual; every virtual base of a derived class is represented by the same(shared) object.

Why would someone want to use a virtual base containing data? I can think of three obvious ways for two classes in a class hierarchy to share data:

*	Make the data nonlocal(outside the class as a global or namespace variable)
*	Put the data in a base class.
*	Allocate an object somewhere and give each of the two classses a pointer

####Constructing virtual bases

Using virtual bases you can create complicated lattices. Naturally, we would prefer to keep the lattices simple, but however complicated we make them, the language ensures that a constructor of a virtual base is called exactly once. Furthermore, the constructor of a base(whether virtual or not) is called before its derived classes

####Overriding Virtual Base Functions

A derived class can override a virtual function of its direct or indirect virtual base class. In particular, two different classes might override different virtual functions from the virtual base. In that way, several derived classses can contribute implementations to the interface presented by a virtual base class.

What if different derived class override the same function? This is allowed if and only if some overriding class is derived from every other class that overrides the function. That is, one function must override all others.

A class that provides some - but not all - of the implementation for a virtual base class is often called a *mixin*


