#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <vector>

// TODO create implementation here!
namespace contact_list {
    bool add(storage& contacts, std::string_view name, number_t number) {
        if (name.empty()) {
            return false;
        }

        for (auto contact : contacts.names) {
            if (contact == name) {
                return false;
            }
        }

        contacts.numbers.push_back(number);
        contacts.names.push_back(static_cast<std::string>(name));
        return true;
    }

    size_t size(const storage& contacts) {
        return contacts.names.size();
    }

    number_t get_number_by_name(storage& contacts, std::string_view name) {
       for (size_t i=0; i<contacts.names.size(); i++) {
            if (contacts.names[i] == name) {
                return contacts.numbers[i];
            }
       }
       return -1;
    }

    std::string to_string(const storage& contacts) {
        std::ostringstream os;
        for (size_t i=0; i<contacts.names.size(); i++) {
            os << contacts.names[i] << " - " << contacts.numbers[i] << std::endl;
        }
        return os.str();

    }

    bool remove(storage& contacts, std::string_view name) {
        for (size_t i=0; i<contacts.names.size(); i++) {
            if (contacts.names[i] == name) {
                contacts.names.erase(contacts.names.begin() + static_cast<long>(i));
                contacts.numbers.erase(contacts.numbers.begin() + static_cast<long>(i));
                return true;
            }
        }
        return false;
     }

    void sort(storage& contacts) {
        std::vector<std::pair<std::string, number_t>> mid_contacts;
        for (size_t i=0; i<contacts.names.size(); i++) {
            mid_contacts.push_back(std::make_pair(contacts.names[i], contacts.numbers[i]));
        }
        std::sort(mid_contacts.begin(), mid_contacts.end());
        contacts.names.clear();
        contacts.numbers.clear();
        for (auto contact : mid_contacts) {
            contacts.names.push_back(contact.first);
            contacts.numbers.push_back(contact.second);
        }
    }

    std::string get_name_by_number(storage& contacts, number_t number) {
        for (size_t i=0; i<contacts.names.size(); i++) {
            if (contacts.numbers[i] == number) {
                return contacts.names[i];
            }
        }
        return "";
    }
}