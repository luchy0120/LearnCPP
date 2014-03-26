*	If the types in the interface to the generated or deried classes need to differ, temlates have an advantage. To gain access to differing interfaces for derived classes through a base, we must use some form of explicit casting.
*	If the implementation of generated or derived classes differ only through a parameter or differ only in a few special cases, templates have an advantage. Irregular implementations can be expressed through derived classes or specializations.
*	If the actual types of objects used can not be known at compile time, a class hierarchy is essential.
*	If a hierarchical relationship is required between generated or derived types, hierarchies have an advantage. The base class provides a common interface. Conversions among template specializations must be explicitly defined by the programmer.
*	If explicit use of free store is undesirable, templates have an advantage.
*	If run-time efficiency is at such a premium that inlining of operations is essential, templates should be used(because effective use of hierarchy requires the use of pointers or references, which inhibit inlining).

###Generated Types

A class template is usefully understood as a specification of how particular types are to be created. In other words, the template implementation is a mechanism that generates types when needed based on a specification. Consequently, a class template is sometimes called a type generator.

The combination of an array and a base class is particularly nasty because a built-in array does not offer the type safety provided by containers.

*	Prefer containers oevr built-in arrays
*	Consider interfaces such as `void f(T* p, int count)` highly suspect; when T can be a base class and count is an element count, trouble awaits.
*	Consider .(dot) suspect when applied to something that is supposed to be run-time polymorphic unless it is obviously applied to a reference.

###Template Conversions

There can not be any default relationship between classes generated from the same template. However, for some templates we would like to express such a relationship. For example, when we define a pointer template, we would like to reflect inheritance relationships among the objects pointed to.

Be careful to define logically meaningful conversions only. If in doubt, use a named conversion, rather than a conversion operator. A named conversion function offers fewer opportunities for ambiguities.

The template parameter lists of a template and one of its template members can not be combined:

	template<typename T, typename T2>
	Ptr<T>::operator Ptr<T2>() {
		return Ptr<T2>(p);
	}

An alternative solution to this problem uses type traits and enable_if

##Hierarchies of Class Templates

Caution is recommended when mixing object-oriented and generic techniques.

###Templates as Interfaces

A template class can be used to provide a flexible and type-safe interface to a common implementation.

	template<typename T>
	class Vector<T*> :private Vector<void*>
	{
		// ...
	}

##Template Parameters as Base Classes

The language guarantees that if a base class has no non-static data members, no memory will allocated for it in an object of derived class.
