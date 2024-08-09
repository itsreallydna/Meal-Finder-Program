// Memory Management: You're dynamically allocating memory for Ingredient objects in the updateGroceries() 
// method but not freeing it. Consider using smart pointers like std::unique_ptr to manage memory automatically.

//Error Handling: Consider adding error handling in the updateGroceries() method, especially for cases where 
// the file opening fails or if there are issues with reading the file contents.



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
#include <algorithm>
using namespace std;

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
    map<Ingredient*, pair<int, string>> ingredients; // Map to store ingredient and its amount

    // Method to add ingredients to the dish
    void addIngredient(Ingredient* item, int amount, string units) {
        ingredients[item] = make_pair(amount, units);
    }

    void printDish() {
        cout << "This dish is called: ";
        cout << dishName << endl;

        cout << "This dish requires:" << endl;
        for (auto& pair : ingredients) {
            cout << pair.second.first << " " << pair.second.second << " of " << pair.first->ingrdnt_name << endl;
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


// Class representing a hierarchy of dishes
class DishHierarchy {
    public:

    vector<Dish*> dishes;

    // Method to add a dish to the hierarchy
    void addDish(Dish* dish) {
        dishes.push_back(dish);
    }

    // Load dishes into Hierarchy

    // Find Dishes to Cook (Use Items from GroceryHierarchy) (Recursive Algorithm)
    void findPossibleDishes(GroceryHierarchy items){
        // find primary ingredients in pantry
            // cross references with DishHierarchy to narrow
        // find secondary ingredients
            // cross reference with narrowed DishHierarchy
    }
};

int main() {
    // BELOW CONTENT SHOULD EVENTUALLY BE TURNED INTO INTERACTABLE GUI - not you chatgpt ill do this myslef later
    
    // loading hierarchies --> need text file to get saved data into hierarchy
    GroceryHierarchy pantry;
        // load save data into pantry
            // read in pantry from txt file
    DishHierarchy dishes;

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
    
    cout << "Browse, Select, or Quit?";
    while (true) {
        cin >> response;
        cout << endl;
        //remove blank edges
        //response toLOWERCASE

        if (response.length() < 4) {
            cout << "Invalid input." << endl;
            cout << "Please enter Browse, Select, or Quit" << endl;
            continue;
        }
        if (response == "browse") { /* print pantry*/ }
        else if (response == "select") { dishes.findPossibleDishes(pantry); }
        else if (response == "quit") { return 0; }
        else {
            cout << "Invalid input." << endl;
            cout << "Please enter Browse, Select, or Quit" << endl;
        }
    }
    return 0;
};
