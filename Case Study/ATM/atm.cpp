#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

// ==========================================
// 1. FORWARD DECLARATIONS
// ==========================================
class ATM;

// ==========================================
// 2. INTERFACES
// ==========================================

// State Interface
class ATMState {
public:
    virtual ~ATMState() = default;
    virtual void insertCard() = 0;
    virtual void ejectCard() = 0;
    virtual void enterPIN(const std::string& pin) = 0;
    virtual void requestCash(int amount) = 0;
};

// Bank Service Interface
class BankService {
public:
    virtual ~BankService() = default;
    virtual bool authenticateUser(const std::string& cardNumber, const std::string& pin) = 0;
    virtual double getBalance(const std::string& cardNumber) = 0;
    virtual bool executeWithdrawal(const std::string& cardNumber, int amount) = 0;
};

// Chain of Responsibility for Cash Dispenser
class DispenseHandler {
protected:
    std::shared_ptr<DispenseHandler> nextHandler;
public:
    virtual ~DispenseHandler() = default;
    void setNext(std::shared_ptr<DispenseHandler> next) {
        nextHandler = next;
    }
    virtual void dispense(int amount) = 0;
};

// ==========================================
// 3. HARDWARE & BACKEND IMPLEMENTATIONS
// ==========================================

// Mock Bank System for testing
class MockBankSystem : public BankService {
private:
    // Simple mock database: CardNumber -> {PIN, Balance}
    struct AccountData { std::string pin; double balance; };
    std::unordered_map<std::string, AccountData> database;

public:
    MockBankSystem() {
        database["1234-5678"] = {"1234", 500.00};
        database["8765-4321"] = {"9999", 1500.00};
    }

    bool authenticateUser(const std::string& cardNumber, const std::string& pin) override {
        if (database.find(cardNumber) != database.end()) {
            return database[cardNumber].pin == pin;
        }
        return false;
    }

    double getBalance(const std::string& cardNumber) override {
        return database[cardNumber].balance;
    }

    bool executeWithdrawal(const std::string& cardNumber, int amount) override {
        if (database[cardNumber].balance >= amount) {
            database[cardNumber].balance -= amount;
            return true;
        }
        return false;
    }
};

// Concrete Dispense Handlers (Chain of Responsibility)
class NoteDispenser : public DispenseHandler {
private:
    int denomination;
public:
    NoteDispenser(int denom) : denomination(denom) {}

    void dispense(int amount) override {
        if (amount >= denomination) {
            int numNotes = amount / denomination;
            int remainder = amount % denomination;
            std::cout << "Dispensing " << numNotes << " x $" << denomination << " note(s)." << std::endl;
            
            if (remainder != 0 && nextHandler != nullptr) {
                nextHandler->dispense(remainder);
            } else if (remainder != 0) {
                std::cout << "Error: Cannot dispense exact amount. Unfulfilled: $" << remainder << std::endl;
            }
        } else if (nextHandler != nullptr) {
            nextHandler->dispense(amount);
        } else if (amount > 0) {
             std::cout << "Error: Cannot dispense exact amount. Unfulfilled: $" << amount << std::endl;
        }
    }
};

// ==========================================
// 4. ATM CONTEXT CLASS (FACADE)
// ==========================================

class ATM {
private:
    std::shared_ptr<ATMState> idleState;
    std::shared_ptr<ATMState> hasCardState;
    std::shared_ptr<ATMState> authenticatedState;
    std::shared_ptr<ATMState> currentState;

    std::shared_ptr<BankService> bankService;
    std::shared_ptr<DispenseHandler> cashDispenser;
    
    std::string currentCardNumber;

public:
    ATM(std::shared_ptr<BankService> bankService);

    // Hardware Facade Methods
    void insertCard(const std::string& cardNumber) {
        currentCardNumber = cardNumber;
        currentState->insertCard();
    }
    void ejectCard() { currentState->ejectCard(); }
    void enterPIN(const std::string& pin) { currentState->enterPIN(pin); }
    void requestCash(int amount) { currentState->requestCash(amount); }

    // State Management
    void setState(std::shared_ptr<ATMState> state) { currentState = state; }
    std::shared_ptr<ATMState> getIdleState() { return idleState; }
    std::shared_ptr<ATMState> getHasCardState() { return hasCardState; }
    std::shared_ptr<ATMState> getAuthenticatedState() { return authenticatedState; }

    // Service Accessors
    std::shared_ptr<BankService> getBankService() { return bankService; }
    std::shared_ptr<DispenseHandler> getCashDispenser() { return cashDispenser; }
    std::string getCurrentCardNumber() { return currentCardNumber; }
    void clearCurrentCard() { currentCardNumber = ""; }
};

// ==========================================
// 5. CONCRETE STATES
// ==========================================

class IdleState : public ATMState {
private:
    ATM* atm;
public:
    IdleState(ATM* machine) : atm(machine) {}

    void insertCard() override {
        std::cout << "[Screen]: Card inserted. Please enter your PIN." << std::endl;
        atm->setState(atm->getHasCardState());
    }
    void ejectCard() override { std::cout << "[Screen]: No card to eject." << std::endl; }
    void enterPIN(const std::string& pin) override { std::cout << "[Screen]: Please insert a card first." << std::endl; }
    void requestCash(int amount) override { std::cout << "[Screen]: Please insert a card first." << std::endl; }
};

class HasCardState : public ATMState {
private:
    ATM* atm;
public:
    HasCardState(ATM* machine) : atm(machine) {}

    void insertCard() override { std::cout << "[Screen]: A card is already inserted." << std::endl; }
    
    void ejectCard() override {
        std::cout << "[Hardware]: Card ejected." << std::endl;
        atm->clearCurrentCard();
        atm->setState(atm->getIdleState());
    }
    
    void enterPIN(const std::string& pin) override {
        if (atm->getBankService()->authenticateUser(atm->getCurrentCardNumber(), pin)) {
            std::cout << "[Screen]: PIN accepted. Choose an action." << std::endl;
            atm->setState(atm->getAuthenticatedState());
        } else {
            std::cout << "[Screen]: Invalid PIN!" << std::endl;
            ejectCard();
        }
    }
    
    void requestCash(int amount) override { std::cout << "[Screen]: Please enter your PIN first." << std::endl; }
};

class AuthenticatedState : public ATMState {
private:
    ATM* atm;
public:
    AuthenticatedState(ATM* machine) : atm(machine) {}

    void insertCard() override { std::cout << "[Screen]: A card is already inserted." << std::endl; }
    
    void ejectCard() override {
        std::cout << "[Hardware]: Card ejected. Thank you." << std::endl;
        atm->clearCurrentCard();
        atm->setState(atm->getIdleState());
    }
    
    void enterPIN(const std::string& pin) override { std::cout << "[Screen]: You are already authenticated." << std::endl; }
    
    void requestCash(int amount) override {
        if (amount % 10 != 0) {
            std::cout << "[Screen]: Amount must be a multiple of $10." << std::endl;
            return;
        }

        std::string card = atm->getCurrentCardNumber();
        if (atm->getBankService()->executeWithdrawal(card, amount)) {
            std::cout << "[Network]: Withdrawal of $" << amount << " approved. Processing cash..." << std::endl;
            atm->getCashDispenser()->dispense(amount);
            std::cout << "[Screen]: Please take your cash." << std::endl;
        } else {
            std::cout << "[Network]: Insufficient funds!" << std::endl;
        }
        ejectCard(); // Transaction complete, eject card
    }
};

// ==========================================
// 6. ATM CONSTRUCTOR IMPLEMENTATION
// ==========================================
// Must be defined after concrete states to allow std::make_shared to resolve them

ATM::ATM(std::shared_ptr<BankService> bankSvc) : bankService(bankSvc) {
    // Initialize States
    idleState = std::make_shared<IdleState>(this);
    hasCardState = std::make_shared<HasCardState>(this);
    authenticatedState = std::make_shared<AuthenticatedState>(this);
    currentState = idleState; // Default state

    // Initialize Chain of Responsibility for Cash Dispenser
    auto dispense100 = std::make_shared<NoteDispenser>(100);
    auto dispense50 = std::make_shared<NoteDispenser>(50);
    auto dispense20 = std::make_shared<NoteDispenser>(20);
    auto dispense10 = std::make_shared<NoteDispenser>(10);

    dispense100->setNext(dispense50);
    dispense50->setNext(dispense20);
    dispense20->setNext(dispense10);
    
    cashDispenser = dispense100; // Start of the chain
}

// ==========================================
// 7. MAIN RUNNER (TESTING)
// ==========================================

int main() {
    std::cout << "--- Booting up ATM System ---" << std::endl;
    
    // Wire up backend
    std::shared_ptr<BankService> bankBackend = std::make_shared<MockBankSystem>();
    ATM myATM(bankBackend);

    std::cout << "\n--- Scenario 1: Successful Withdrawal ---" << std::endl;
    myATM.insertCard("1234-5678");
    myATM.enterPIN("1234");
    myATM.requestCash(180); // Should give 1x$100, 1x$50, 1x$20, 1x$10

    std::cout << "\n--- Scenario 2: Invalid PIN ---" << std::endl;
    myATM.insertCard("8765-4321");
    myATM.enterPIN("0000"); // Wrong PIN
    myATM.requestCash(50);  // Should fail because state reverted to idle

    std::cout << "\n--- Scenario 3: Insufficient Funds ---" << std::endl;
    myATM.insertCard("1234-5678");
    myATM.enterPIN("1234");
    myATM.requestCash(1000); // Only has $500 - $180 = $320 left

    return 0;
}
