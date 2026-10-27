// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"

ToolRegistry::ToolRegistry(){
    size_ = 0;
    buckets_.resize(16);
}    

ToolRegistry::~ToolRegistry() = default;   // unique_ptrs in your buckets free the tools

std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    /*Copy and pasted from Appendix A*/
    std::uint64_t h = 1469598103934665603ULL;                         // "offset basis"
    for (unsigned char c : key) { h ^= c; h *= 1099511628211ULL; }     // "FNV prime"
    return static_cast<std::size_t>(h); 
    //return 0;   // TODO: FNV-1a (spec Appendix A) or another real string hash
}

bool ToolRegistry::add(std::unique_ptr<Tool> tool) {

    if (tool == nullptr){
        return false;
    }

    if (find(tool->name()) != nullptr){
        return false; //pre existing tool, so no point to add
    }

    //If load factor >1.0, must resize. Just means all buckets are full
    if (size_ + 1 > bucket_count() * 1.0){ 
        std::size_t bucket_count_new = bucket_count() * 2;
        std::vector<std::vector<std::unique_ptr<Tool>>> new_bucket(bucket_count_new);

        for (auto& bucket: buckets_){
            for (auto& sub_bucket: bucket){
                std::size_t i = hash(sub_bucket -> name()) & (bucket_count_new - 1);
                new_bucket[i].push_back(std::move(sub_bucket));
            }
        }
        buckets_ = std::move(new_bucket); //temp move into old and no double free
    }

    //Normal addition
    std::size_t i = hash(tool -> name()) & (bucket_count() - 1);
    buckets_[i].push_back(std::move(tool));
    size_++;

    return true;
}

Tool* ToolRegistry::find(std::string_view name) const {
    if (bucket_count() == 0){
        return nullptr;
    }
    else{
        std::size_t index = hash(name) & (bucket_count() - 1);
        const auto& vector_chain = buckets_[index]; //type is too long

        for (const auto& vector_element : vector_chain){
            if (vector_element->name() == name){//found vector tool
                return vector_element.get();
            }
        }

        return nullptr; //vector tool not in hash map
    }

}

std::size_t ToolRegistry::size() const noexcept {
    return size_;   
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& fn) const {
    for (const auto& bucket: buckets_){
        for (const auto& sub_bucket : bucket){
            fn(*sub_bucket); //
        }
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets_.size();
}

double ToolRegistry::load_factor() const noexcept {
    if (bucket_count() == 0){
        return 0.0;
    }
    else{

        return static_cast<double>(size()) / bucket_count();
    }
}

std::size_t ToolRegistry::max_probe_length() const {
    std::size_t maximum = 0;
    for (const auto& bucket : buckets_){
        std::size_t current_size = bucket.size();
        if ( current_size > maximum){
            maximum = current_size;
        }
    }
    return maximum;
}
