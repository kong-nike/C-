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

    // Destructor to recursively delete subordinates
    ~EmployeeNode() {
        for (EmployeeNode* subordinate : subordinates) {
            delete subordinate;
        }
    }
};

int main() {
    int main ;
    switch (main){
        case 1:  EmployeeNode* ceo = new EmployeeNode(1, "Alice", "CEO");
    }
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
    // Create employees
    EmployeeNode* ceo = new EmployeeNode(1, "Alice", "CEO");

    EmployeeNode* headOfEngineering = new EmployeeNode(2, "Bob", "Head of Engineering");
    EmployeeNode* headOfMarketing = new EmployeeNode(3, "Eve", "Head of Marketing");

    EmployeeNode* engineer1 = new EmployeeNode(4, "Charlie", "Engineer");
    EmployeeNode* engineer2 = new EmployeeNode(5, "Dave", "Engineer");
    EmployeeNode* engineer3 = new EmployeeNode(6, "Eve", "Engineer");

    EmployeeNode* marketer1 = new EmployeeNode(7, "Fiona", "Marketer");

    // Build hierarchy
    ceo->addSubordinate(headOfEngineering);
    ceo->addSubordinate(headOfMarketing);

    headOfEngineering->addSubordinate(engineer1);
    headOfEngineering->addSubordinate(engineer2);
    headOfEngineering->addSubordinate(engineer3);

    headOfMarketing->addSubordinate(marketer1);

    // Display hierarchy
    std::cout << "Company Hierarchy:\n";

    // Update an employee's details
    int idToUpdate = 4;
    std::string newName = "Charles";
    std::string newPosition = "Senior Engineer";

    if (ceo->updateEmployee(idToUpdate, newName, newPosition)) {
        std::cout << "\nEmployee with ID " << idToUpdate << " has been updated.\n";
    } else {
        std::cout << "\nEmployee with ID " << idToUpdate << " not found.\n";
    }

    // Display updated hierarchy
    std::cout << "\nUpdated Company Hierarchy:\n";
    ceo->displayHierarchy();

    // Search for employees with position "Engineer"
    std::string positionToSearch = "Engineer";
    std::vector<EmployeeNode*> foundEmployees;
    ceo->findAllByPosition(positionToSearch, foundEmployees);

    std::cout << "\nEmployees with position '" << positionToSearch << "':\n";
    if (foundEmployees.empty()) {
        std::cout << "No employees found with position '" << positionToSearch << "'.\n";
    } else {
        for (EmployeeNode* employee : foundEmployees) {
            std::cout << "ID: " << employee->id 
                      << ", Name: " << employee->name 
                      << ", Position: " << employee->position << "\n";
        }
    }
    
    // Use findById to search for an employee by ID
    int idToSearch = 4;
    EmployeeNode* foundEmployee = ceo->findById(idToSearch);

    // Display results of the search
    std::cout << "\nSearch Results:\n";
    if (foundEmployee) {
        std::cout << "Employee found:\n";
        std::cout << "ID: " << foundEmployee->id 
                  << ", Name: " << foundEmployee->name 
                  << ", Position: " << foundEmployee->position << "\n";
    } else {
        std::cout << "No employee found with ID: " << idToSearch << "\n";
    }

        // Call deleteEmployee to remove an employee with ID 4 (Engineer Charlie)
    int idToDelete = 4;
    if (ceo->deleteEmployee(idToDelete)) {
        std::cout << "\nEmployee with ID " << idToDelete << " has been deleted.\n";
    } else {
        std::cout << "\nEmployee with ID " << idToDelete << " not found.\n";
    }

    // Display hierarchy after deletion
    std::cout << "\nCompany Hierarchy (After Deletion):\n";
    ceo->displayHierarchy();

    // Clean up memory
    delete ceo;

    return 0;
}
