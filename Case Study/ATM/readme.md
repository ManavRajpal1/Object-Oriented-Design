# 1. Problem Statement

Design an Automated Teller Machine (ATM) system that allows customers to perform basic financial transactions—such as cash withdrawals, deposits, and balance inquiries—without the need for a human teller. The system must seamlessly interface with physical hardware components (card reader, cash dispenser, keypad) and communicate securely with the bank's backend network.

**Primary Actors:** Customer (User), Bank System (Backend), ATM Operator (Maintains cash inventory).

---

# 2. Requirements

## Functional Requirements

- Customers must be able to authenticate using a bank card and a PIN.
- Customers can view their account balance.
- Customers can withdraw cash (the ATM must dispense the appropriate combination of denominations).
- Customers can deposit cash or checks.
- The system must handle hardware errors gracefully (e.g., out of cash, card stuck, network timeout).

## Non-Functional & Concurrency Requirements

### Atomicity (ACID)

Transactions must be atomic. If money is deducted from the bank account but the physical cash dispenser jams, the transaction MUST roll back completely.

### Concurrency

The system must prevent race conditions if two users attempt to withdraw from the exact same account simultaneously from different ATMs.

### Security

PINs and sensitive card data must be encrypted before being transmitted to the Bank System.

---

# 3. Core Entities & Class Design

## ATM

The central controller and context class. It aggregates all hardware components and maintains the current state of the machine.

## Hardware Abstractions

### CardReader
Classes responsible for handling specific physical interactions and hardware triggers.

### CashDispenser
Classes responsible for handling specific physical interactions and hardware triggers.

### Keypad
Classes responsible for handling specific physical interactions and hardware triggers.

## BankService

An interface (abstract class in C++) representing the network client that calls the bank's central database to verify funds and execute ledgers.

## Transaction

An abstract base class representing a user action. Extended by concrete classes like Withdrawal, Deposit, and BalanceInquiry.

## ATMState

An interface representing the various operational states of the machine (e.g., Idle, CardInserted, Authenticated) to restrict invalid actions based on context.
