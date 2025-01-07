#ifndef IODIALOG_H_
#define IODIALOG_H_

#include <iostream>
#include <sstream>
#include <list>

using namespace std;

namespace IODialog {

    void showTreeOperation() {
        cout << "\033[1;36m==============================================\033[0m" << endl;
        cout << "\033[1;33m           Red-Black Tree Operations          \033[0m" << endl;
        cout << "\033[1;36m==============================================\033[0m" << endl;
        cout << "\033[1;32m 1. \033[0mAdd nodes" << endl
             << "\033[1;32m 2. \033[0mDelete node" << endl
             << "\033[1;32m 3. \033[0mGet minimum node" << endl
             << "\033[1;32m 4. \033[0mGet maximum node" << endl
             << "\033[1;32m 5. \033[0mGet successor of a node" << endl
             << "\033[1;32m 6. \033[0mGet predecessor of a node" << endl
             << "\033[1;32m 7. \033[0mShow tree" << endl
             << "\033[1;32m 8. \033[0mShow inorder traversal" << endl
             << "\033[1;32m 9. \033[0mShow black-height of the tree" << endl
             << "\033[1;32m10. \033[0mShow the maximum key of a black node of the tree" << endl
             << "\033[1;32m11. \033[0mShow the maximum key of a red node of the tree" << endl
             << "\033[1;32m12. \033[0mShow tree depth" << endl
             << "\033[1;31m13. \033[0mExit" << endl;
        cout << "\033[1;36m==============================================\033[0m" << endl;
        cout << "\033[1;34mEnter your choice (1-13): \033[0m";
    }

    int getTreeOperation() {
        int option = 0;
        while (true) {
            showTreeOperation();
            if (!(cin >> option)) {
                cout << "\033[1;31mUnknown option. Try again.\033[0m" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            } else if (option < 1 || option > 13) {
                cout << "\033[1;31mUnknown option. Try again.\033[0m" << endl;
            } else {
                cout << "\033[1;32mProceed with selection \033[0m" << option << " .." << endl;
                cin.ignore(10000, '\n');
                break;
            }
        }
        return option;
    }

    void getNodeKeys(list<int>& nodeKeys) {
        cout << "\033[1;34mType in the keys of the nodes (separated by spaces): \033[0m" << flush;
        string line;
        if (getline(cin, line)) {
            istringstream iss(line);
            int key;
            string invalidInput;

            while (iss >> key) {
                nodeKeys.push_back(key);
            }

            // Check if there are invalid inputs
            if (iss.fail() && !iss.eof()) {
                cout << "\033[1;31mError: Invalid input detected. Please enter only integers.\033[0m" << endl;
                nodeKeys.clear(); // Clear the list if invalid input is found
            }
        } else {
            cout << "\033[1;31mError: Input reading failed.\033[0m" << endl;
        }
    }

    int getNodeKey() {
        cout << "\033[1;34mType in the key of the node: \033[0m";
        string input;
        int key;

        cin >> input;
        istringstream iss(input);

        if (iss >> key && iss.eof()) { // Ensure the entire input is valid and an integer
            return key;
        } else {
            cout << "\033[1;31mError: Invalid input detected. Please enter only integers.\033[0m" << endl;
            return -1;
        }
    }
}

#endif