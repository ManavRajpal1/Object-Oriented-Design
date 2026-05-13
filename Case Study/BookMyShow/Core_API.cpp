// 1. State Pattern Interface for Seat Lifecycle
class Seat; // Forward declaration
class SeatState {
public:
    virtual ~SeatState() = default;
    virtual bool lock(Seat* seat) = 0;
    virtual bool book(Seat* seat) = 0;
    virtual bool release(Seat* seat) = 0;
    virtual std::string getStatus() = 0;
};

// 2. The Context Object (Seat)
class Seat {
private:
    std::string seatId;
    std::shared_ptr<SeatState> currentState;
    std::mutex seatMutex; // Crucial for Thread-Safety in C++
public:
    bool lockSeat();
    bool confirmBooking();
    bool releaseSeat();
};
