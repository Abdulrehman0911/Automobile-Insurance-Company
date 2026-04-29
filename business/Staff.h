#pragma once
#include <string>

class Staff {
protected:
    std::string id, name, username, password, role;
public:
    Staff();
    Staff(const std::string& id, const std::string& name, const std::string& username,
          const std::string& password, const std::string& role);
    virtual ~Staff() = default;
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    virtual std::string getRole() const { return role; }
    void setId(const std::string& v) { id = v; }
    void setName(const std::string& v) { name = v; }
    void setUsername(const std::string& v) { username = v; }
    void setPassword(const std::string& v) { password = v; }
    void setRole(const std::string& v) { role = v; }
};
