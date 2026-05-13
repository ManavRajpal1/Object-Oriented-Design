"""# Object-Oriented Design Vault 🏗️

Welcome to my comprehensive knowledge base for Object-Oriented Design (OOD) and Low-Level Design (LLD). 

This repository serves as a deep-dive reference guide, distilling complex architectural concepts, structural relationships, and design patterns into actionable principles for building scalable, maintainable software.

---

## 🧱 OOP Fundamentals 

* **Classes & Objects:** 
    * **Class:** A blueprint or template that defines the state (attributes/fields) and behavior (methods) that the objects created from it will have.
    * **Object:** An instance of a class occupying memory, possessing a specific state, and capable of executing the behaviors defined by its class.
* **Encapsulation:** The practice of bundling data (attributes) and the methods that operate on that data into a single unit (class), while restricting direct external access to the internal state. This is typically achieved using access modifiers (e.g., `private`, `protected`) and exposed via controlled getters/setters to protect data integrity.
* **Abstraction:** Hiding complex implementation details and exposing only the essential features of an object. In practice, this is achieved using Interfaces and Abstract Classes, allowing developers to interact with a system at a high level without needing to understand its internal workings.
* **Inheritance:** An `IS-A` relationship that allows a new class (child/subclass) to inherit attributes and methods from an existing class (parent/superclass). It promotes code reusability but should be used cautiously to avoid rigid, deeply nested class hierarchies.
* **Polymorphism:** The ability of different objects to respond to the same method call in their own specific way.
    * **Compile-time (Static):** Achieved through Method Overloading (same method name, different parameters).
    * **Runtime (Dynamic):** Achieved through Method Overriding (child class provides a specific implementation of a method defined in its parent class).
* **Enums:** A specialized data type used to define a fixed, unchangeable set of constants (e.g., `DaysOfWeek`, `OrderStates`). Enums ensure type safety and prevent invalid values from being assigned to variables.
* **Interfaces:** A strict contract that defines *what* behaviors a class must implement without dictating *how* they are implemented. Interfaces are crucial for achieving loose coupling and multiple inheritance of type.

---

## 🔗 Class Relationships 

Understanding how objects interact and rely on each other is the core of LLD. These are ordered from weakest to strongest.

* **Dependency (`USES-A`):** The weakest relationship. A class uses another class temporarily, often by receiving it as a method parameter or instantiating it locally within a method. Changes in the depended-upon class may affect the dependent class.
* **Association (`KNOWS-A`):** A general, flat relationship where two independent objects are linked and aware of each other, but neither owns the other. (e.g., A `Patient` and a `Doctor` can exist independently, but they interact).
* **Aggregation (`HAS-A` - Weak):** A specialized form of association implying a "whole-part" relationship, but the parts can exist independently of the whole. If the parent object is destroyed, the child objects continue to exist. (e.g., A `Department` has `Professors`. If the department closes, the professors still exist).
* **Composition (`PART-OF` - Strong):** The strongest relationship. A "whole-part" relationship where the child objects are completely dependent on the parent object's lifecycle. If the parent is destroyed, the children are destroyed as well. (e.g., A `House` is composed of `Rooms`. If the house is demolished, the rooms cease to exist).

---

## 🧭 Design Principles

### Core Principles
* **DRY (Don’t Repeat Yourself):** Every piece of knowledge or logic must have a single, unambiguous, authoritative representation within a system.
* **YAGNI (You Aren’t Gonna Need It):** Do not add functionality, features, or architectural complexity until it is strictly necessary. 
* **KISS (Keep It Simple, Stupid):** Favor readability and straightforward logic over clever, convoluted solutions.
* **Composition over Inheritance:** Assemble complex objects by composing them of smaller, interchangeable behavior objects instead of building deep inheritance trees.
* **Law of Demeter (Principle of Least Knowledge):** An object should only talk to its immediate friends, not strangers. Avoid chained method calls.

### S.O.L.I.D. Principles
The five pillars of modern object-oriented design:
1. **Single Responsibility Principle (SRP):** A class should have one, and only one, reason to change. 
2. **Open/Closed Principle (OCP):** Software entities should be *open for extension* but *closed for modification*. 
3. **Liskov Substitution Principle (LSP):** Objects of a superclass shall be replaceable with objects of its subclasses without breaking the application. 
4. **Interface Segregation Principle (ISP):** Clients should not be forced to depend upon interfaces that they do not use. Prefer small, specific interfaces.
5. **Dependency Inversion Principle (DIP):** High-level modules should not depend on low-level modules; both should depend on abstractions.

### 🧠 G.R.A.S.P. Principles
General Responsibility Assignment Software Patterns (How to assign responsibilities to classes):
* **Information Expert:** Assign a responsibility to the class that has the information necessary to fulfill it.
* **Creator:** Class A should create instances of Class B if A aggregates/contains B, closely uses B, or has the initializing data for B.
* **Controller:** Assign the responsibility for handling a system event to a non-UI class that represents the overall system or a use case scenario.
* **Low Coupling:** Assign responsibilities so that dependencies between classes remain low.
* **High Cohesion:** Assign responsibilities so that a class's methods are strongly related and focused on a single purpose.

---

## 🧩 Design Patterns 

### 🏗️ Creational Patterns (Object Creation)
* **Singleton:** Ensures a class has only one global instance.
* **Factory Method:** Delegates object instantiation to subclasses.
* **Abstract Factory:** Creates families of related objects without specifying concrete classes.
* **Builder:** Constructs complex objects step-by-step.
* **Prototype:** Creates new objects by cloning an existing instance.

### 🧱 Structural Patterns (Object Assembly)
* **Adapter:** Bridges two incompatible interfaces.
* **Bridge:** Decouples an abstraction from its implementation.
* **Composite:** Composes objects into tree structures (part-whole hierarchies).
* **Decorator:** Attaches additional responsibilities dynamically at runtime.
* **Facade:** Provides a simplified interface to a complex subsystem.
* **Flyweight:** Minimizes memory usage by sharing data with similar objects.
* **Proxy:** Provides a placeholder for another object to control access.

### 🔄 Behavioral Patterns (Object Communication)
* **Strategy:** Encapsulates interchangeable algorithms at runtime.
* **Observer:** Notifies dependents automatically when an object changes state (Pub/Sub).
* **Command:** Encapsulates a request as an object (allows undo/redo queues).
* **State:** Alters an object's behavior when its internal state changes.
* **Iterator:** Accesses elements of an aggregate sequentially.
* **Template Method:** Defines an algorithm's skeleton, deferring steps to subclasses.
* **Chain of Responsibility:** Passes a request along a chain of handlers.
* **Mediator:** Centralizes complex communications between objects.
* **Memento:** Captures and restores an object's internal state safely.
* **Visitor:** Adds new operations to object structures without modifying their classes.

---

## ⚠️ Anti-Patterns & Code Smells

Avoid these common pitfalls in object-oriented design:
* **God Object:** A class that knows too much or does too much (violates SRP).
* **Spaghetti Code:** Code with a complex and tangled control structure, making it difficult to read and maintain.
* **Magic Numbers/Strings:** Hardcoding unexplained values directly in the logic instead of using named constants or enums.
* **Shotgun Surgery:** Making a single small change requires modifying many different classes (indicates high coupling/low cohesion).
* **Primitive Obsession:** Using primitive data types (int, string) to represent domain ideas (like Zip Codes, Money, or Phone Numbers) instead of creating dedicated classes.

---

## 🗂️ UML Diagrams 

* **Class Diagram:** The backbone of LLD. Shows static structure, attributes, methods, and relationships.
* **Use Case Diagram:** Represents the system from the user's perspective (actors and goals).
* **Sequence Diagram:** Details how objects interact with one another sequentially over time.
* **Activity Diagram:** Object-oriented flowchart modeling workflows and parallel processes.
* **State Machine Diagram:** Represents states and transitions of an object over its lifecycle.

---

## ⚡ Concurrency & Multithreading (LLD Considerations)

When designing systems, consider how objects behave when accessed by multiple threads simultaneously:
* **Thread Safety:** Ensuring shared data structures are manipulated in a way that guarantees safe execution by multiple threads.
* **Race Conditions:** Occur when thread timing affects the correctness of an operation. Solved using synchronization.
* **Deadlocks:** When two or more threads are blocked forever, waiting for each other to release a lock.
* **Mutexes & Semaphores:** Concurrency primitives used to control access to shared resources.

---

## 🛠️ The LLD Interview Playbook (Methodology)

1. **Gather Requirements & Clarify:** Define core use cases, system boundaries, and actors. 
2. **Identify Core Entities (Nouns):** E.g., `User`, `ParkingTicket`, `ParkingSpot`.
3. **Identify Behaviors (Verbs):** E.g., `assignSpot()`, `processPayment()`.
4. **Establish Class Relationships:** Determine Aggregation, Composition, Inheritance, etc.
5. **Apply Design Patterns:** Look for specific problems that fit standard patterns (Strategy, State, Factory).
6. **Design APIs / Interfaces:** Outline public contracts, inputs, and outputs.
7. **Write the Code:** Implement the skeleton focusing on business logic and S.O.L.I.D. principles.

---

## 🎯 Top LLD Practice Problems

* Design a Parking Lot
* Design a Vending Machine
* Design an Elevator System
* Design a Movie Ticket Booking System (BookMyShow)
* Design a Library Management System
* Design Tic-Tac-Toe / Chess
* Design an ATM
"""

with open("README.md", "w") as f:
    f.write(content)

print("Expanded README.md generated successfully")
