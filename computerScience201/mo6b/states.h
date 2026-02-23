#ifndef STATES_H
#define STATES_H

#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

inline string getStateName(const string& abbrev) { // i dont want to implement this in a .cpp rather have it here
    string key = abbrev;
    for (char& c : key) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }

    static const unordered_map<string, string> stateMap {
        {"AL", "Alabama"}, {"AK", "Alaska"}, {"AZ", "Arizona"},
        {"AR", "Arkansas"}, {"CA", "California"}, {"CO", "Colorado"},
        {"CT", "Connecticut"}, {"DE", "Delaware"}, {"FL", "Florida"},
        {"GA", "Georgia"}, {"HI", "Hawaii"}, {"ID", "Idaho"},
        {"IL", "Illinois"}, {"IN", "Indiana"}, {"IA", "Iowa"},
        {"KS", "Kansas"}, {"KY", "Kentucky"}, {"LA", "Louisiana"},
        {"ME", "Maine"}, {"MD", "Maryland"}, {"MA", "Massachusetts"},
        {"MI", "Michigan"}, {"MN", "Minnesota"}, {"MS", "Mississippi"},
        {"MO", "Missouri"}, {"MT", "Montana"}, {"NE", "Nebraska"},
        {"NV", "Nevada"}, {"NH", "New Hampshire"}, {"NJ", "New Jersey"},
        {"NM", "New Mexico"}, {"NY", "New York"}, {"NC", "North Carolina"},
        {"ND", "North Dakota"}, {"OH", "Ohio"}, {"OK", "Oklahoma"},
        {"OR", "Oregon"}, {"PA", "Pennsylvania"}, {"RI", "Rhode Island"},
        {"SC", "South Carolina"}, {"SD", "South Dakota"}, {"TN", "Tennessee"},
        {"TX", "Texas"}, {"UT", "Utah"}, {"VT", "Vermont"},
        {"VA", "Virginia"}, {"WA", "Washington"}, {"WV", "West Virginia"},
        {"WI", "Wisconsin"}, {"WY", "Wyoming"}
    };

    auto it = stateMap.find(key); // this will be of type unordered_map<string, string>
    if (it != stateMap.end()) {
        return it->second;
    }

    return "";
}

#endif