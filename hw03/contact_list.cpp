#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

// TODO create implementation here!

namespace contact_list{

/**
 * Given a contact storage, create a new contact entry by name and number.
 */
bool add(storage& contacts, std::string_view name, number_t number){
    if(name != ""){
        for (auto it = contacts.names.begin(); it!= contacts.names.end();++it){
            if(*it == name) return false;
        }
        contacts.names.emplace_back(name);
        contacts.numbers.push_back(number);
        return true;
    } else return false;
    
}
/**
 * Given a contact storage, how many contacts are currently stored?
 */
size_t size(const storage& contacts){
    return contacts.numbers.size();
}


/**
 * Fetch a contact number from storage given a name.
 */
number_t get_number_by_name(storage& contacts, std::string_view name){
    for (size_t i = 0; i< size(contacts); ++i){
        if(contacts.names[i] == name) return contacts.numbers[i];
    }

    return -1;
}

/**
 * Return a string representing the contact list.
 */
std::string to_string(const storage& contacts){
    std::string str;
    for (size_t i = 0; i< size(contacts); ++i){
        str.append(contacts.names[i]);
        str.append(" - ");
        str.append(std::to_string(contacts.numbers[i]));
        str.append("\n");
    }

    return str;
}


/**
 * Remove a contact by name from the contact list.
 */
bool remove(storage& contacts, std::string_view name){
    for (size_t i = 0; i< size(contacts); ++i){
        if(contacts.names[i] == name) {
            contacts.names.erase(contacts.names.begin() + i);
            contacts.numbers.erase(contacts.numbers.begin() + i);
            return true;
        }
    }
    return false;
}


/**
 * Sort the contact list in-place by name.
 */
void sort(storage& contacts){

}


/**
 * Fetch a contact name from storage given a number.
 */
std::string get_name_by_number(storage& contacts, number_t number){
    for (size_t i = 0; i< size(contacts); ++i){
        if(contacts.numbers[i] == number) return contacts.names[i];
    }

    return "";
}

}