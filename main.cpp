#include <iostream>
#include <vector>
#include <string>

class EmployeeNode {
public:
    int id; // Unique identifier
    std::string name;
    std::string position;
    std::vector<EmployeeNode*> subordinates;

    EmployeeNode(int id, const std::string& name, const std::string& position)
        : id(id), name(name), position(position) {}

    void addSubordinate(EmployeeNode* subordinate) {
        subordinates.push_back(subordinate);
    }

    void displayHierarchy(int level = 0) {
        for (int i = 0; i < level; ++i) std::cout << "  ";
        std::cout << position << " (ID: " << id << "): " << name << "\n";

        for (EmployeeNode* subordinate : subordinates) {
            subordinate->displayHierarchy(level + 1);
        }
    }

    // Search by position and display all employees with that position
    void findAllByPosition(const std::string& position, std::vector<EmployeeNode*>& foundEmployees) {
        if (this->position == position) {
            foundEmployees.push_back(this);
        }

        for (EmployeeNode* subordinate : subordinates) {
            subordinate->findAllByPosition(position, foundEmployees);
        }
    }

    // Search by ID
    EmployeeNode* findById(int searchId) {
        if (this->id == searchId) {
            return this;
        }

        for (EmployeeNode* subordinate : subordinates) {
            EmployeeNode* found = subordinate->findById(searchId);
            if (found) {
                return found;
            }
        }
        return nullptr;
    }

    // Delete an employee by ID
    bool deleteEmployee(int deleteId) {
        for (auto it = subordinates.begin(); it != subordinates.end(); ++it) {
            if ((*it)->id == deleteId) {
                delete *it; // Delete the employee and their subtree
                subordinates.erase(it); // Remove from subordinates list
                return true;
            } else if ((*it)->deleteEmployee(deleteId)) {
                return true;
            }
        }
        return false;
    }

    // Update an employee's name and/or position by ID
    bool updateEmployee(int updateId, const std::string& newName, const std::string& newPosition) {
        EmployeeNode* employeeToUpdate = findById(updateId);
        if (employeeToUpdate) {
            if (!newName.empty()) {
                employeeToUpdate->name = newName;
            }
            if (!newPosition.empty()) {
                employeeToUpdate->position = newPosition;
            }
            return true; // Update was successful
        }
        return false; // Employee not found
    }

    // Promote an employee
    bool promoteEmployee(int promoteId, const std::string& newPosition) {
        EmployeeNode* employeeToPromote = findById(promoteId);
        if (employeeToPromote && !newPosition.empty()) {
            employeeToPromote->position = newPosition;
            return true;
        }
        return false;
    }

    // Demote an employee
    bool demoteEmployee(int demoteId, const std::string& newPosition) {
        EmployeeNode* employeeToDemote = findById(demoteId);
        if (employeeToDemote && !newPosition.empty()) {
            employeeToDemote->position = newPosition;
            return true;
        }
        return false;
    }

    // Destructor to recursively delete subordinates
    ~EmployeeNode() {
        for (EmployeeNode* subordinate : subordinates) {
            delete subordinate;
        }
    }
};

int main() {
    EmployeeNode* ceo = nullptr;

    int operation;
    do {
        std::cout << "\nChoose an operation:\n";
        std::cout << "1. Create Company\n";
        std::cout << "2. Update Employee\n";
        std::cout << "3. Search by Position\n";
        std::cout << "4. Search by ID\n";
        std::cout << "5. Delete Employee\n";
        std::cout << "6. Display Hierarchy\n";
        std::cout << "7. Promote Employee\n";
        std::cout << "8. Demote Employee\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> operation;

        switch (operation) {
            case 1: {
                if (ceo) {
                    delete ceo;
                }

                std::cout << "Enter Company details:\n";
                int memberId;
                std::string memberName, memberPosition;
                std::cout << "ID: ";
                std::cin >> memberId;
                std::cin.ignore(); // Clear newline from input buffer
                std::cout << "Name: ";
                std::getline(std::cin, memberName);
                std::cout << "Position: ";
                std::getline(std::cin, memberPosition);

                ceo = new EmployeeNode(memberId, memberName, memberPosition);

                int numEmployees;
                std::cout << "\nEnter the number of employees under the CEO: ";
                std::cin >> numEmployees;

                for (int i = 0; i < numEmployees; ++i) {
                    int id;
                    std::string name, position;
                    std::cout << "\nEnter details for employee " << (i + 1) << ":\n";
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cin.ignore();
                    std::cout << "Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Position: ";
                    std::getline(std::cin, position);

                    EmployeeNode* employee = new EmployeeNode(id, name, position);
                    ceo->addSubordinate(employee);

                    int numSubordinates;
                    std::cout << "Enter the number of subordinates for " << name << ": ";
                    std::cin >> numSubordinates;

                    for (int j = 0; j < numSubordinates; ++j) {
                        int subId;
                        std::string subName, subPosition;
                        std::cout << "\nEnter details for subordinate " << (j + 1) << " of " << name << ":\n";
                        std::cout << "ID: ";
                        std::cin >> subId;
                        std::cin.ignore();
                        std::cout << "Name: ";
                        std::getline(std::cin, subName);
                        std::cout << "Position: ";
                        std::getline(std::cin, subPosition);

                        EmployeeNode* subordinate = new EmployeeNode(subId, subName, subPosition);
                        employee->addSubordinate(subordinate);
                    }
                }

                std::cout << "Company created successfully!\n";
                break;
            }
            case 2: {
                int updateId;
                std::string newName, newPosition;
                std::cout << "Enter ID of the employee to update: ";
                std::cin >> updateId;
                std::cin.ignore();
                std::cout << "Enter new name (or leave empty): ";
                std::getline(std::cin, newName);
                std::cout << "Enter new position (or leave empty): ";
                std::getline(std::cin, newPosition);

                if (ceo && ceo->updateEmployee(updateId, newName, newPosition)) {
                    std::cout << "Employee updated successfully.\n";
                } else {
                    std::cout << "Employee not found.\n";
                }
                break;
            }
            case 3: {
                std::string searchPosition;
                std::cin.ignore();
                std::cout << "Enter position to search: ";
                std::getline(std::cin, searchPosition);

                if (ceo) {
                    std::vector<EmployeeNode*> foundEmployees;
                    ceo->findAllByPosition(searchPosition, foundEmployees);

                    std::cout << "\nEmployees with position '" << searchPosition << "':\n";
                    if (foundEmployees.empty()) {
                        std::cout << "No employees found.\n";
                    } else {
                        for (EmployeeNode* emp : foundEmployees) {
                            std::cout << "ID: " << emp->id 
                                      << ", Name: " << emp->name 
                                      << ", Position: " << emp->position << "\n";
                        }
                    }
                } else {
                    std::cout << "No company exists.\n";
                }
                break;
            }
            case 4: {
                int searchId;
                std::cout << "Enter ID to search: ";
                std::cin >> searchId;

                if (ceo) {
                    EmployeeNode* foundEmployee = ceo->findById(searchId);
                    if (foundEmployee) {
                        std::cout << "Employee found: ID: " << foundEmployee->id 
                                  << ", Name: " << foundEmployee->name 
                                  << ", Position: " << foundEmployee->position << "\n";
                    } else {
                        std::cout << "No employee found with that ID.\n";
                    }
                } else {
                    std::cout << "No company exists.\n";
                }
                break;
            }
            case 5: {
                int deleteId;
                std::cout << "Enter ID of the employee to delete: ";
                std::cin >> deleteId;

                if (ceo && ceo->deleteEmployee(deleteId)) {
                    std::cout << "Employee deleted successfully.\n";
                } else {
                    std::cout << "Employee not found or no company exists.\n";
                }
                break;
            }
            case 6: {
                if (ceo) {
                    std::cout << "\nCompany Hierarchy:\n";
                    ceo->displayHierarchy();
                } else {
                    std::cout << "No company exists.\n";
                }
                break;
            }
            case 7: {
                int promoteId;
                std::string newPosition;
                std::cout << "Enter ID of the employee to promote: ";
                std::cin >> promoteId;
                std::cin.ignore();
                std::cout << "Enter new position: ";
                std::getline(std::cin, newPosition);

                if (ceo && ceo->updateEmployee(promoteId, "", newPosition)) {
                    std::cout << "Employee promoted successfully.\n";
                } else {
                    std::cout << "Employee not found or no company exists.\n";
                }
                break;
            }
            case 8: {
                int demoteId;
                std::string newPosition;
                std::cout << "Enter ID of the employee to demote: ";
                std::cin >> demoteId;
                std::cin.ignore();
                std::cout << "Enter new position: ";
                std::getline(std::cin, newPosition);

                if (ceo && ceo->updateEmployee(demoteId, "", newPosition)) {
                    std::cout << "Employee demoted successfully.\n";
                } else {
                    std::cout << "Employee not found or no company exists.\n";
                }
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (operation != 0);

    // Clean up memory
    delete ceo;

    return 0;
}
