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

// not used in DishHierarchy, only for Pantry
class Ingredient {
    public:

    string ingrdnt_name;
    float amount;
    string units;
    Ingredient(string name) : ingrdnt_name(name) {}

    string grabName() {
        return ingrdnt_name;
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
        cout << "This dish is called: ";
        cout << dishName << endl;

        cout << "This dish requires:" << endl;
        for (const auto& entry : ingredients) {
            const std::string& ingredient = entry.first;
            int amount = entry.second.first;
            const std::string& unit = entry.second.second;

            std::cout << "  - " << ingredient << ": " << amount << " " << unit << "\n";
        }
        cout << endl;

        //return;
    }
};

// Class representing a hierarchy of food items in storage
class GroceryHierarchy {
    public:

    vector<Ingredient*> items;

    // Method to add a basic food item to the hierarchy
    void addFoodItem(Ingredient* item) {
        items.push_back(item);
    }

    string printItems() {}

    // Consider adding error handling
    void updateGroceries(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string ingredientName;
            int amount;
            string units;
            
            while (getline(file, ingredientName)) {
                file >> amount >> units;
                file.ignore(); // Ignore newline character
                
                // Check if the ingredient already exists
                bool ingredientExists = false;
                for (Ingredient* ingredient : items) {
                    if (ingredient->ingrdnt_name == ingredientName) {
                        // Ingredient already exists
                        auto it = find_if(items.begin(), items.end(), [&](Ingredient* ing) {
                            return ing->ingrdnt_name == ingredientName;
                        });
                        if (it != items.end()) {
                            // Found the ingredient, update the amount if units match
                            int index = distance(items.begin(), it);
                            if (items[index]->units == units) {
                                items[index]->amount += amount;
                            } else {
                                // Units do not match, print error message
                                cout << "Error: Units for ingredient '" << ingredientName << "' do not match." << endl;
                            }
                        }
                        ingredientExists = true;
                        break;
                    }
                }
                
                // If ingredient does not exist, add it to the hierarchy
                if (!ingredientExists) {
                    Ingredient* ingredient = new Ingredient(ingredientName);
                    ingredient->amount = amount;
                    ingredient->units = units;
                    addFoodItem(ingredient);
                }
            }
            file.close();
        } else {
            cout << "Unable to open file." << endl;
        }
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
    // pantry.updateGroceries(nonexistantfile.txt); // dunno what to do here

    DishHierarchy dishes;
    // dishes.sortMeals(MealList.txt); // dunno what to do here

    //program start
    cout << "hello" << endl;
    cout << "Update Pantry? (y or n)" << endl;
    
    string response;
    bool response_bool = true;

    while (true) {
    std::cin >> response;
    cout << endl;

    if (response.length() > 1) {
        cout << "Invalid input. Input was greater than size one" << endl;
        cout << "Please enter 'y' or 'n'" << endl;
        continue;
    }
    if (response == "y" || response == "Y") {  response_bool = true; break; }
    else if (response == "n" || response == "N") {  response_bool = false; break; }
    else { cout << "Invalid input. Please enter 'y' or 'n'."; }
    }

    response = "";

    if (response_bool) {
        cout << "Enter file name to load Groceries: ";
        string file_name;
        cin >> file_name;
        pantry.updateGroceries(file_name);
        cout << "Groceries are now in pantry ";
    }
    
    cout << "Browse, Update, or Quit?" << endl << "(B, U, or Q)" << endl;
    while (true) {
        cin >> response;
        cout << endl;

        if (response.length() > 1) {
            cout << "Invalid input." << endl;
            cout << "Please enter B, U, or Q" << endl;
            continue;
        }
        if (response == "B") { /* print pantry*/ }
        // else if (response == "U") { dishes.findPossibleDishes(pantry); }
        else if (response == "Q") { return 0; }
        else {
            cout << "Invalid input." << endl;
            cout << "Please enter B, U, or Q" << endl;
        }
    }
    return 0;
};
