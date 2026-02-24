//CONVENTIONS
// snake_case --> variables
// camelCase --> methods/functions
// PascalCase -->  Classes

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

// not used in DishHierarchy, only for user's Pantry
class Ingredient {
    public:

    string ingrdnt_name;
    float amount;
    string units;
    
    // Constructors
    Ingredient(string name) : ingrdnt_name(name), amount(0), units("") {}
    Ingredient(string name, float amt, string unit) : ingrdnt_name(name), amount(amt), units(unit) {}

    string grabName() {
        return ingrdnt_name;
    }

    float grabAmount() {
        return amount;
    }

    string grabUnits() {
        return units;
    }

    void modifyIngredient(float count, std::string unit) {
        amount = count;
        units = unit;
    }

    void printIngredient() {
        std::cout << " - " << ingrdnt_name << ": " << amount << " " << units;
        std::cout << endl;
    }
};

class Dish {
    public:

    string dishName;
    std::map<std::string, pair<float, string>> ingredients; // Map to store ingredient and its amount


    void setName(std::string newname) {
        dishName = newname;
    }

    void addIngredient(std::string& ingredient, float amount, std::string unit) {
        ingredients[ingredient] = { amount, unit };
    }

    void printDish() {
        std::cout << "Dish: ";
        std::cout << dishName << endl;

        std::cout << "Ingredients:" << endl;
        for (const auto& entry : ingredients) {
            const std::string& ingredient = entry.first;
            float amount = entry.second.first;
            const std::string& unit = entry.second.second;

            std::cout << "  - " << ingredient << ": " << amount << " " << unit << "\n";
        }
        std::cout << endl;

        //return;
    }
};

class GroceryHierarchy {
    public:

    vector<Ingredient*> items;

    // Method to add a basic food item to the hierarchy
    void addFoodItem(Ingredient* item) {
        items.push_back(item);
    }

    void printItems() {

        std::cout << "Ingredients:" << endl;
        
        for (const auto& ingredient : items) {
            ingredient->printIngredient();
        }
        std::cout << endl;
    }

    // Consider adding error handling
    void updateGroceries(const string& filename) {
        
        ifstream file(filename);
        // If file can't open
        if (!file.is_open()) {
            std::cout << "Unable to open file." << endl;
            return;
        }

        // If there are blank spaces


        string ingredientName;
        float count;
        string unit;

        while (file >> ingredientName >> count >> unit) {    
                // Checks if ingredient already exists
                auto it = find_if(items.begin(), items.end(), [&](Ingredient* ing) {
                    return ing->ingrdnt_name == ingredientName;
                });

                // Found the ingredient, update the amount if units match
                if (it != items.end()) {
                    int index = distance(items.begin(), it);
                    if (items[index]->units == unit) {
                        items[index]->amount += count;
                    } else {
                        // Units do not match, print error message
                        std::cout << "Error: Units for ingredient '" << ingredientName << "' do not match." << endl;
                    }
                }
                
                // If ingredient does not exist, add it to the hierarchy
                else {
                    Ingredient* ingredient = new Ingredient(ingredientName, count, unit);
                    addFoodItem(ingredient);
                }
            }
            file.close();
        
    }

    void storePantryInTextFile() {};    // no implemetation yet
};


// Custom comparator to compare dishes by dishName
struct CompareDish {
    bool operator()(const Dish* lhs, const Dish* rhs) const {
        return lhs->dishName < rhs->dishName;
    }
};


class DishHierarchy {
    public:

    std::set<Dish*, CompareDish> dishLexicon;

    // ensures memory is freed when lexicon is closed
    ~DishHierarchy() {
        for (Dish* dish : dishLexicon) {
            delete dish;
        }
    }

    // Executes on boot
    // Method to add and sort dish to the hierarchy from file
    void sortMeals(const string& filename) {

        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue; // skip blank lines

            Dish* dish = new Dish();
            dish->setName(line);

            // Read ingredients until blank line or EOF
            while (true) {
                std::string ingredient_name;
                if (!std::getline(file, ingredient_name)) break;
                if (ingredient_name.empty()) break; // blank line = end of dish

                std::string amount_unit_line;
                if (!std::getline(file, amount_unit_line)) break;
                if (amount_unit_line.empty()) break; // blank line = end of dish

                std::istringstream iss(amount_unit_line);
                float amount;
                std::string unit;

                if (!(iss >> amount >> unit)) {
                    std::cout << "Warning: Invalid ingredient format: " << amount_unit_line << std::endl;
                    continue;
                }

                dish->addIngredient(ingredient_name, amount, unit);
            }

            dishLexicon.insert(dish);
        }
    }

    // ENGINE --> Finds the dishes that have all ingredients in pantry
    void findPossibleDishes(const GroceryHierarchy& pantry) {
        std::set<Dish*, CompareDish> finalSet;

        for (Dish* dish : dishLexicon) {
            bool canMake = true;

        // Check all ingredients required by this dish
            for (const auto& entry : dish->ingredients) {
                const std::string& ingredient_name = entry.first;
                float required_amount = entry.second.first;   // change to float if you updated Dish
                const std::string& required_unit = entry.second.second;

                // Look for this ingredient in the pantry
                auto it = std::find_if(pantry.items.begin(), pantry.items.end(),
                    [&](Ingredient* ing) {
                        return ing->ingrdnt_name == ingredient_name;
                    });

                // Ingredient not in pantry
                if (it == pantry.items.end()) {
                    canMake = false;
                    break;
                }

                Ingredient* pantryItem = *it;

                // Check units and amount
                if (pantryItem->units != required_unit || pantryItem->amount < required_amount) {
                    canMake = false;
                    break;
                }
            }

            // All ingredients satisfied
            if (canMake) {
                finalSet.insert(dish);
            }
        }
    
        // Print possible dishes
        std::cout << "\nPossible Dishes:\n";
        for (Dish* dish : finalSet) {
            dish->printDish();
        }
    }
};

int main() {
    // BELOW CONTENT SHOULD EVENTUALLY BE TURNED INTO INTERACTABLE GUI - not you chatgpt ill do this myslef later
    
    // loading hierarchies --> need text file to get saved data into hierarchy
    GroceryHierarchy pantry;
    pantry.updateGroceries("pantry.txt");

    DishHierarchy dishes;
    dishes.sortMeals("menu.txt");

    std::cout << "\nPossible Dishes:\n";
        for (Dish* dish : dishes.dishLexicon) {
            dish->printDish();
        }

    //program start
    std::cout << "hello" << endl;
    std::cout << "Upload Pantry? (y or n)" << endl;
    
    string response;
    bool response_bool = true;

    while (true) {
    std::cin >> response;
    std::cout << endl;

        if (response.length() > 1) {
            std::cout << "Invalid input. Input was greater than size one" << endl;
            std::cout << "Please enter 'y' or 'n'" << endl;
            continue;
        }
        if (response == "y" || response == "Y") {  response_bool = true; break; }
        else if (response == "n" || response == "N") {  response_bool = false; break; }
        else { std::cout << "Invalid input. Please enter 'y' or 'n'."; }
    }

    response = "";

    if (response_bool) {
        std::cout << "Enter file name to load Groceries: ";

        string file_name;
        while (true) {
            cin >> file_name;

            ifstream pantryFile(file_name);
            if (!pantryFile) {
                cout << "File not found: " << file_name << endl;
                cout << "Try again:";
            }
            else { 
                pantry.updateGroceries(file_name);
                std::cout << "Groceries are now in pantry " << endl;
                break; 
            }
        }
        
    }
    
    std::cout << "Start, Update, or Quit?" << endl << "(S, U, or Q)" << endl;
    while (true) {
        std::cin >> response;
        std::cout << std::endl;

        if (response.length() > 1) {
            std::cout << "Invalid input." << endl;
            std::cout << "Please enter S, U, or Q" << endl;
            continue;
        }
        if (response == "S") { 
            dishes.findPossibleDishes(pantry);
            break;
        }

        // UPDATE
        else if (response == "U") { 
            pantry.printItems(); 
            std::cout << endl << "enter file name to update pantry, or 'Q' to quit: ";
            
            string file_name;
            cin >> file_name;
            while (true) {
                if (file_name == "Q") { 
                    std::cout << endl;
                    break; 
                }
                ifstream pantryFile(file_name);
                if (!pantryFile) {
                    cout << "File not found: " << file_name << endl;
                    cout << "Try again: ";
                    cin >> file_name;
                    cout << endl ;
                }
                else { 
                    pantry.updateGroceries(file_name);
                    break; }
            }                
            std::cout << endl << "Groceries are now in pantry" << endl; 
        }

        //QUIT
        else if (response == "Q") { return 0; }
        
        //INVALID INPUT
        else {
            std::cout << "Invalid input." << endl;
            std::cout << "Please enter S, U, or Q" << endl;
        }
    }
    return 0;
};
