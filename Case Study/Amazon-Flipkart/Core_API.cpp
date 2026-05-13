// 1. Strategy Pattern Interface for Payments
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0;
};

// 2. Observer Pattern Interface for Notifications
class OrderObserver {
public:
    virtual ~OrderObserver() = default;
    virtual void update(const std::string& orderId, const std::string& status) = 0;
};

// 3. State Pattern Interface for Order Lifecycle
class Order; // Forward declaration
class OrderState {
public:
    virtual ~OrderState() = default;
    virtual void ship(Order* order) = 0;
    virtual void deliver(Order* order) = 0;
    virtual void cancel(Order* order) = 0;
    virtual std::string getStatusString() = 0;
};
