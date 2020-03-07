#ifndef VERSION_H
#define VERSION_H

#include <sstream>
#include <string>
#include <vector>
class Version {
public:
  Version(const std::string v):digits(0) { parse(v); }

  Version & operator=(const std::string v){
    parse(v);
    return *this;
  }
  bool operator==(const Version &b) const {
    size_t max =
        digits.size() > b.digits.size() ? digits.size() : b.digits.size();
    for (size_t i = 0; i < max; ++i) {
      if (getVal(i) != b.getVal(i)) {
        return false;
      }
    }
    return true;
  }

  bool operator>(const Version &b) const {
    size_t max =
        digits.size() > b.digits.size() ? digits.size() : b.digits.size();
    for (size_t i = 0; i < max; ++i) {
      if (getVal(i) > b.getVal(i)) {
        return true;
      }
    }
    return false;
  }

  bool operator<(const Version &b) const {
    size_t max =
        digits.size() > b.digits.size() ? digits.size() : b.digits.size();
    for (size_t i = 0; i < max; ++i) {
      if (getVal(i) < b.getVal(i)) {
        return true;
      }
    }
    return false;
  }

  bool operator>=(const Version &b) const { return !(*this < b); }
  bool operator<=(const Version &b) const { return !(*this > b); }
  bool operator!=(const Version &b) const { return !(*this == b); }

  bool operator==(const std::string v) const { return *this == Version(v); }
  bool operator>(const std::string v) const { return *this > Version(v); }
  bool operator<(const std::string v) const { return *this < Version(v); }
  bool operator>=(const std::string v) const { return !(*this < v); }
  bool operator<=(const std::string v) const { return !(*this > v); }
  bool operator!=(const std::string v) const { return !(*this == v); }

private:
  std::vector<int> digits;

  void parse(const std::string &v) {
    std::stringstream ss(v);
    std::string s;
    while (std::getline(ss, s, '.')) {
      digits.push_back(std::stoi(s));
    }
  }

  int getVal(size_t i) const {
    try {
      return digits.at(i);
    } catch (std::out_of_range e) {
      return 0;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Version &v);
  friend bool operator==(const std::string &s, const Version &v);
  friend bool operator!=(const std::string &s, const Version &v);
  friend bool operator>(const std::string &s, const Version &v);
  friend bool operator<(const std::string &s, const Version &v);
  friend bool operator>=(const std::string &s, const Version &v);
  friend bool operator<=(const std::string &s, const Version &v);
};

inline std::ostream &operator<<(std::ostream &out, const Version &v) {
  for (auto i : v.digits) {
    out << i << ".";
  }
  out << 0;
  return out;
}

inline bool operator==(const std::string &s, const Version &v) { return v == s; }
inline bool operator!=(const std::string &s, const Version &v) { return v != s; }
inline bool operator>(const std::string &s, const Version &v) { return v < s; }
inline bool operator<(const std::string &s, const Version &v) { return v > s; }
inline bool operator>=(const std::string &s, const Version &v) { return v <= s; }
inline bool operator<=(const std::string &s, const Version &v) { return v >= s; }

#endif // VERSION
