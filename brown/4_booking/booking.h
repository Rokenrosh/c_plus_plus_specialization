#include <memory>

namespace RAII{

template<typename Provider>
class Booking{
public:
    Booking(Provider* provider, const int id): provider_(provider), id_(id){}

    Booking(Booking&& other): provider_(other.provider_), id_(other.id_){
        other.provider_ = nullptr;
    }

    Booking& operator=(Booking&& other){
        std::swap(provider_, other.provider_);
        std::swap(id_, id_);
        return *this;
    }

    ~Booking(){
        if(provider_){
            provider_->CancelOrComplete(*this);
        }
    }
private:
    Booking() = delete;

    Booking(const Booking& other) = delete;

    Booking& operator=(const Booking& other) = delete;

    Provider* provider_;
    int id_;
};
}