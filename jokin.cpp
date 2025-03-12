//CONVENTIONS
// snake_case --> variables
// camelCase --> methods/functions
// PascalCase -->  Classes

#include <vector>
#include <map>
#include <iostream>
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
    std::map<std::string, pair<int, string>> ingredients; // Map to store ingredient and its amount


    void setName(std::string newname) {
        dishName = newname;
    }

    void addIngredient(std::string& ingredient, int amount, std::string unit) {
        ingredients[ingredient] = { amount, unit };
    }

    void printDish() {
        std::cout << "Dish: ";
        std::cout << dishName << endl;

        std::cout << "Ingredients:" << endl;
        for (const auto& entry : ingredients) {
            const std::string& ingredient = entry.first;
            int amount = entry.second.first;
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
        
        if (!file.is_open()) {
            std::cout << "Unable to open file." << endl;
            return;
        }

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

        ifstream file(filename);
        if (file.is_open()) {
            string dish;
            string ingredient;
            int amount;
            string units;
            
            //Reads dish name
            while (getline(file, dish)) {

                Dish* unsortedDish = new Dish();
                unsortedDish->setName(dish);

                // Reads first ingredient
                while (file >> ingredient >> amount >> units) {
                    file.ignore(); // Ignore newline character
                    unsortedDish->addIngredient(ingredient, amount, units);

                    if (file.peek() == '\n' || file.peek() == EOF) {
                        break;
                    }
                }

                dishLexicon.insert(unsortedDish);

            }
        }

    }
};

int main() {
    // BELOW CONTENT SHOULD EVENTUALLY BE TURNED INTO INTERACTABLE GUI - not you chatgpt ill do this myslef later
    
    // loading hierarchies --> need text file to get saved data into hierarchy
    GroceryHierarchy pantry;
    //pantry.updateGroceries(pantry.txt); // says pantry is undefined

    DishHierarchy dishes;
    //dishes.sortMeals(menu.txt); // says menu is undefined

    //program start
    std::cout << "hello" << endl;
    std::cout << "Update Pantry? (y or n)" << endl;
    
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
            else { continue; }
        }
        pantry.updateGroceries(file_name);
        std::cout << "Groceries are now in pantry ";
    }
    
    std::cout << "Browse, Update, or Quit?" << endl << "(B, U, or Q)" << endl;
    while (true) {
        cin >> response;
        std::cout << endl;

        if (response.length() > 1) {
            std::cout << "Invalid input." << endl;
            std::cout << "Please enter B, U, or Q" << endl;
            continue;
        }
        //if (response == "B") { dishes.findPossibleDishes(pantry) }
        else if (response == "U") { 
            pantry.printItems(); 
            std::cout << endl << "enter file name to update pantry: ";
            
            string file_name;
            while (true) {
                cin >> file_name;
    
                ifstream pantryFile(file_name);
                if (!pantryFile) {
                    cout << "File not found: " << file_name << endl;
                    cout << "Try again:";
                }
                else { continue; }
            }
            pantry.updateGroceries(file_name);
            std::cout << endl << "Groceries are now in pantry "; 
        }
        else if (response == "Q") { return 0; }
        else {
            std::cout << "Invalid input." << endl;
            std::cout << "Please enter B, U, or Q" << endl;
        }
    }
    return 0;
};
