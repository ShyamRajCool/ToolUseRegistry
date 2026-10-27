#include "core/sentinel_scanner.h"

SentinelScanner::SentinelScanner(std::string sentinel){
        sentinel_ = sentinel;
}

SentinelScanner::Out SentinelScanner::feed(std::string_view chunk){ //gets a chunk and scans it for sentinel_ with pending_ 
        std::size_t sentinel_size {sentinel_.size()}; //For sectioning off the substring
        std::size_t total_size{}; //Total pending_ and chunk size

        struct Out temp;

        std::string total = pending_ + static_cast<std::string>(chunk);  //whole string that will be searched
        total_size = total.size();

        std::size_t found = total.find(sentinel_);
        if (found != std::string::npos){  //find returns either npos or a location where it is found
                temp.safe_text = total.substr(0,found);
                temp.sentinel_found = true;
                pending_.clear();        //this helps make sure future calls don't use anything in pending
                return temp;
        }
        else{
                if (total_size < sentinel_size){
                        pending_ = total;
                        temp.safe_text.clear(); //there isn't any confirmed safe text since the pending_ hasn't reached sentinel size yet
                        temp.sentinel_found = false;
                        return temp;
                }
                else{
                        pending_ = total.substr(total_size - sentinel_size + 1, sentinel_size - 1); //remaining that isn't safe text
                        temp.safe_text = total.substr(0,total_size - sentinel_size + 1);//since safe text size can reach sentinel_.size() - 1, then it can be confirmed to not be sentinel
                        temp.sentinel_found = false;
                        return temp;  
                }

        }
}

SentinelScanner::Out SentinelScanner::flush(){ //releases any text in pending_ that is remaining from the stream ; confirmed not to be sentinenl_
        struct Out temp;
        temp.safe_text = pending_;
        temp.sentinel_found = false;
        pending_.clear();
        
        return temp;
}