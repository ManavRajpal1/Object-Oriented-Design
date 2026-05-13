#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// ==========================================
// 1. FORWARD DECLARATIONS & INTERFACES
// ==========================================
class Order;

// Strategy: Payment Interface
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0;
};

// Observer: Notification Interface
class OrderObserver {
public:
    virtual ~OrderObserver() = default;
    virtual void update(const std::string& orderId, const std::string& status) = 0;
};

// State: Order Lifecycle Interface
class OrderState {
public:
    virtual ~OrderState() = default;
    virtual void ship(Order* order) = 0;
    virtual void deliver(Order* order) = 0;
    virtual void cancel(Order* order) = 0;
    virtual std::string getStatusString() = 0;
};

// ==========================================
// 2. CONCRETE STRATEGIES (Payments)
// ==========================================
class CreditCardPayment : public PaymentStrategy {
private:
    std::string name;
    std::string cardNumber;
public:
    CreditCardPayment(std::string n, std::string c) : name(n), cardNumber(c) {}
    bool pay(double amount) override {
        std::cout << "[Stripe API] Processing $" << amount << " via Credit Card ending in " 
                  << cardNumber.substr(cardNumber.length() - 4) << "...\n";
        return true; // Assume success
    }
};

class UPIPayment : public PaymentStrategy {
private:
    std::string upiId;
public:
    UPIPayment(std::string id) : upiId(id) {}
    bool pay(double amount) override {
        std::cout << "[NPCI API] Processing $" << amount << " via UPI ID: " << upiId << "...\n";
        return true;
    }
};

// ==========================================
// 3. CONCRETE OBSERVERS (Customers)
// ==========================================
class Customer : public OrderObserver {
private:
    std::string name;
    std::string email;
public:
    Customer(std::string n, std::string e) : name(n), email(e) {}
    void update(const std::string& orderId, const std::string& status) override {
        std::cout << ">> [Email to " << email << "]: Hi " << name 
                  << ", your order " << orderId << " is now " << status << "!\n";
    }
};

// ==========================================
// 4. ORDER CONTEXT CLASS
// ==========================================
class Order {
private:
    std::string orderId;
    double totalAmount;
    std::shared_ptr<OrderState> currentState;
    std::vector<std::shared_ptr<OrderObserver>> observers;

public:
    Order(std::string id, double amount);

    void setState(std::shared_ptr<OrderState> state) {
        currentState = state;
        notifyObservers();
    }

    void addObserver(std::shared_ptr<OrderObserver> observer) {
        observers.push_back(observer);
    }

    void notifyObservers() {
        for (auto& obs : observers) {
            obs->update(orderId, currentState->getStatusString());
        }
    }

    // Process payment using Strategy
    void checkout(std::shared_ptr<PaymentStrategy> paymentMethod) {
        std::cout << "\nInitiating checkout for Order " << orderId << "...\n";
        if (paymentMethod->pay(totalAmount)) {
            std::cout << "Payment successful!\n";
        } else {
            std::cout << "Payment failed.\n";
        }
    }

    // State transitions
    void ship() { currentState->ship(this); }
    void deliver() { currentState->deliver(this); }
    void cancel() { currentState->cancel(this); }
};

// ==========================================
// 5. CONCRETE STATES (Order Lifecycle)
// ==========================================
class PendingState : public OrderState {
public:
    void ship(Order* order) override;
    void deliver(Order* order) override {
        std::cout << "[System Error] Cannot deliver an order that hasn't shipped.\n";
    }
    void cancel(Order* order) override;
    std::string getStatusString() override { return "PENDING"; }
};

class ShippedState : public OrderState {
public:
    void ship(Order* order) override {
        std::cout << "[System Error] Order is already shipped.\n";
    }
    void deliver(Order* order) override;
    void cancel(Order* order) override {
        std::cout << "[System Error] Cannot cancel an order that is already in transit. Please return it after delivery.\n";
    }
    std::string getStatusString() override { return "SHIPPED"; }
};

class DeliveredState : public OrderState {
public:
    void ship(Order* order) override { std::cout << "[System Error] Already delivered.\n"; }
    void deliver(Order* order) override { std::cout << "[System Error] Already delivered.\n"; }
    void cancel(Order* order) override { std::cout << "[System Error] Already delivered. Initiate a return request instead.\n"; }
    std::string getStatusString() override { return "DELIVERED"; }
};

class CancelledState : public OrderState {
public:
    void ship(Order* order) override { std::cout << "[System Error] Cannot ship a cancelled order.\n"; }
    void deliver(Order* order) override { std::cout << "[System Error] Cannot deliver a cancelled order.\n"; }
    void cancel(Order* order) override { std::cout << "[System Error] Order is already cancelled.\n"; }
    std::string getStatusString() override { return "CANCELLED"; }
};

// State Transition Implementations (Must be defined after states)
Order::Order(std::string id, double amount) : orderId(id), totalAmount(amount) {
    currentState = std::make_shared<PendingState>(); // Initial state
}
void PendingState::ship(Order* order) { order->setState(std::make_shared<ShippedState>()); }
void PendingState::cancel(Order* order) { order->setState(std::make_shared<CancelledState>()); }
void ShippedState::deliver(Order* order) { order->setState(std::make_shared<DeliveredState>()); }

// ==========================================
// 6. MAIN RUNNER (TESTING)
// ==========================================
int main() {
    std::cout << "--- Amazon Order Management System ---\n";

    // 1. Setup User (Observer)
    auto customer = std::make_shared<Customer>("Manav", "manav@example.com");

    // 2. Create an Order
    Order macbookOrder("ORD-98765", 1299.99);
    macbookOrder.addObserver(customer);

    // 3. Checkout with a specific Strategy (UPI)
    auto upiPayment = std::make_shared<UPIPayment>("manav@okhdfcbank");
    macbookOrder.checkout(upiPayment);

    // 4. Test State Lifecycle & Observer Notifications
    std::cout << "\n--- Warehouse Fulfillment ---\n";
    macbookOrder.ship();    // Will trigger notification
    
    std::cout << "\n--- Attempting invalid action ---\n";
    macbookOrder.cancel();  // Should fail gracefully because it's already shipped

    std::cout << "\n--- Final Delivery ---\n";
    macbookOrder.deliver(); // Will trigger notification

    return 0;
}
