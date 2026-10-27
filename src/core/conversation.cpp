#include "core/conversation.h"

//default constructor
Conversation::Conversation(){
}


//destructor
Conversation::~Conversation(){
        delete[] data_;
}

//copy constructor
Conversation::Conversation(const Conversation& other){
        if (other.capacity_ == 0 ){
            capacity_ = other.capacity_;
            data_ = nullptr;
            size_ = other.size_;
        }
        else{
            capacity_ = other.capacity_; //Getting the other Message object's cap helps with the next step

            data_ = new Message[capacity_]; //clarify if this can be grouped
            
            for (std::size_t i = 0; i < other.size_; i++){
                    data_[i] = other.data_[i];      //Simple copy per item
            }

            
        
            size_ = other.size_; //size is equivalent to other
        }
}


//assignment operator
Conversation& Conversation::operator=(const Conversation& other){
        if (this != &other){  //checks that it is a different object ("this" represents memory)
            delete[] data_;  
            if (other.capacity_ == 0){ //If other doesn't have any space for messages
                capacity_ = other.capacity_;   
                size_ = other.size_;              
                data_ = nullptr;
            }
            else{
                capacity_ = other.capacity_;   
                size_ = other.size_;  
                data_ = new Message[capacity_]; 
                if (other.size_ != 0){ //has space but no messages
                    for (std::size_t i = 0; i < other.size_; i++){
                        data_[i] = other.data_[i];
                    }
                }
            }       
        }
        return *this;
}

Conversation::Conversation(Conversation&& other) noexcept { //move constructor
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr; //no need for other.data to point to anything since it's prev memory location is now pointed to by data
}


Conversation& Conversation:: operator=(Conversation&& other) noexcept{ //move assignment
        if (this != &other){
            delete[] data_;

            data_ = other.data_;
            capacity_ = other.capacity_;
            size_ = other.size_;

            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }

        return *this;
}

void Conversation::append(Message m){

        Message* olddata = data_; //getting ready to put messages in old data_ into new data_

        if (capacity_ == 0){
            capacity_ = 1;
            data_ = new Message[capacity_];
            data_[ORIGINAL_INDEX] = m;
            size_ = size_ + 1;
        }
        else if (size_ == capacity_){
            std::size_t old_size = size_;
        
            capacity_ = capacity_ * 2;
            
            data_ = new Message[capacity_];

            for (std::size_t i = 0; i < old_size; i++){ //it stops right before going out of bounds
                data_[i] = olddata[i]; //old messages need to be copied
            }

            delete[] olddata;

            data_[old_size] = m; //next open slot which is at old_size since everything up to old_size was copied
            size_ = size_ + 1;   //size increase
        }
        else{ //this is the condition for when there is ample space
            data_[size_] = m;
            size_ = size_ + 1;
        }
}

std::size_t Conversation:: size() const noexcept{
        return size_;
}



const Message& Conversation:: at(std::size_t i) const{ //assuming that the user know's that the index goes from 0 to size - 1 b.c. of spec
        if (i >= size_){
            throw std::out_of_range("You are out of range, please try again");
        }
        else{
            return data_[i];
        }
}

//beginning pointer
const Message* Conversation:: begin() const noexcept{
        return data_;
}


//end pointer
const Message* Conversation:: end() const noexcept{
        if (size_ == 0){
            return data_;
        }
        else{
            return data_ + size_;
        }
}


