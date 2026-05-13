#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

// ==========================================
// 1. FORWARD DECLARATIONS & INTERFACES
// ==========================================
class Seat;

class SeatState {
public:
    virtual ~SeatState() = default;
    
    // Returns true if the transition was successful, false otherwise
    virtual bool lock(Seat* seat) = 0;
    virtual bool book(Seat* seat) = 0;
    virtual bool release(Seat* seat) = 0;
    
    virtual std::string getStatus() = 0;
};

// ==========================================
// 2. CONCRETE STATES
// ==========================================
class AvailableState : public SeatState {
public:
    bool lock(Seat* seat) override;
    bool book(Seat* seat) override {
        std::cout << " [Error] Cannot book directly. Seat must be locked for payment first.\n";
        return false;
    }
    bool release(Seat* seat) override {
        std::cout << " [System] Seat is already available.\n";
        return true;
    }
    std::string getStatus() override { return "AVAILABLE"; }
};

class LockedState : public SeatState {
public:
    bool lock(Seat* seat) override {
        std::cout << " [Error] Seat is currently locked by another user making a payment.\n";
        return false;
    }
    bool book(Seat* seat) override;
    bool release(Seat* seat) override;
    std::string getStatus() override { return "LOCKED (Pending Payment)"; }
};

class BookedState : public SeatState {
public:
    bool lock(Seat* seat) override {
        std::cout << " [Error] Seat is already booked!\n";
        return false;
    }
    bool book(Seat* seat) override {
        std::cout << " [Error] Seat is already booked!\n";
        return false;
    }
    bool release(Seat* seat) override {
        std::cout << " [System] Processing refund... Seat is now available.\n";
        // In a real system, releasing a booked seat requires admin or refund logic
        return true; 
    }
    std::string getStatus() override { return "BOOKED"; }
};

// ==========================================
// 3. THE CONTEXT CLASS (SEAT) with THREAD SAFETY
// ==========================================
class Seat {
private:
    std::string seatId;
    double price;
    std::shared_ptr<SeatState> currentState;
    
    // Mutex ensures that if two threads (users) try to interact with this 
    // exact seat at the exact millisecond, one will wait.
    std::mutex seatMutex; 

public:
    Seat(std::string id, double p) : seatId(id), price(p) {
        currentState = std::make_shared<AvailableState>();
    }

    std::string getId() { return seatId; }
    double getPrice() { return price; }

    void setState(std::shared_ptr<SeatState> state) {
        currentState = state;
    }

    // Thread-safe wrapper methods
    bool lockSeat() {
        std::lock_guard<std::mutex> lock(seatMutex);
        std::cout << "User attempting to lock Seat " << seatId << "...\n";
        return currentState->lock(this);
    }

    bool confirmBooking() {
        std::lock_guard<std::mutex> lock(seatMutex);
        std::cout << "System attempting to confirm booking for Seat " << seatId << "...\n";
        return currentState->book(this);
    }

    bool releaseSeat() {
        std::lock_guard<std::mutex> lock(seatMutex);
        std::cout << "System attempting to release Seat " << seatId << "...\n";
        return currentState->release(this);
    }
};

// ==========================================
// 4. STATE TRANSITION IMPLEMENTATIONS
// ==========================================
bool AvailableState::lock(Seat* seat) {
    seat->setState(std::make_shared<LockedState>());
    std::cout << " -> Success! Seat " << seat->getId() << " is now Locked for 5 minutes.\n";
    return true;
}

bool LockedState::book(Seat* seat) {
    seat->setState(std::make_shared<BookedState>());
    std::cout << " -> Success! Payment received. Seat " << seat->getId() << " is officially Booked.\n";
    return true;
}

bool LockedState::release(Seat* seat) {
    seat->setState(std::make_shared<AvailableState>());
    std::cout << " -> Timeout/Payment Failed. Seat " << seat->getId() << " released back to Available.\n";
    return true;
}

// ==========================================
// 5. MAIN RUNNER (CONCURRENCY SIMULATION)
// ==========================================
int main() {
    std::cout << "--- BookMyShow Seat Reservation System ---\n\n";

    // 1. Create a highly demanded seat
    std::shared_ptr<Seat> avengersSeat = std::make_shared<Seat>("A-12", 250.00);

    // 2. Simulate two users trying to book the exact same seat at the same time
    auto user1Attempt = [&avengersSeat]() {
        std::cout << "[User 1] clicked 'Select Seat'.\n";
        if (avengersSeat->lockSeat()) {
            // Simulate time taken to enter credit card details
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
            avengersSeat->confirmBooking();
        }
    };

    auto user2Attempt = [&avengersSeat]() {
        std::cout << "[User 2] clicked 'Select Seat'.\n";
        // User 2 tries to lock the seat slightly after User 1, or at the exact same time
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
        if (avengersSeat->lockSeat()) {
            avengersSeat->confirmBooking();
        }
    };

    // Run both users on separate threads to simulate web concurrency
    std::thread t1(user1Attempt);
    std::thread t2(user2Attempt);

    t1.join();
    t2.join();

    std::cout << "\n--- Scenario 2: Payment Timeout ---\n";
    std::shared_ptr<Seat> batmanSeat = std::make_shared<Seat>("B-01", 300.00);
    batmanSeat->lockSeat();
    std::cout << "[System] 5 minutes passed. User didn't pay.\n";
    batmanSeat->releaseSeat(); // Reverts to available

    return 0;
}
