#include "SocialNetwork.h"
#include <iostream>
#include <sstream>
using namespace std;

// Helper function to safely read integer input
int getIntInput() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a valid integer: ";
    }
    cin.ignore();
    return value;
}

int main(){
    SocialNetwork sn;
    sn.loadUsers();
    sn.loadFriends();

    //start
    cout<<"Welcome to Your Personalised Friend Reccomendation System"<<endl;

    //choice for login
    int choice;
    
    //dashboard
    while(true){
        cout<<"1.Register New User"<<endl;
        cout<<"2.Login"<<endl;
        cout<<"3.Delete User Account"<<endl;
        cout<<"4.Exit"<<endl;

        cout<<"Enter choice"<<endl;
        choice = getIntInput();
        if(choice==1){
            //register new user
            string userName, firstName, lastName, college, location, password; 
            int age;
            vector<string> hobbies;
            cout<<"Enter username: ";
            getline(cin,userName);
            if (sn.getUserIdByUsername(userName) != -1) {
                cout << " Username already exists. Try another.\n";
                continue;
            }
            cout<<"Enter password: ";
            getline(cin,password);
            cout<<"Enter first name: ";
            getline(cin,firstName);
            cout<<"Enter last name: ";
            getline(cin,lastName);
            cout<<"Enter your college: ";
            getline(cin,college);
            cout<<"Enter your city: ";
            getline(cin,location);
            cout << "Enter age: ";
            age = getIntInput();
            cout << "Enter hobbies separated by commas: ";
            //input hobbies

            string hobbyInput;
            getline(cin, hobbyInput);
            stringstream ss(hobbyInput);
            string hobby;
            while (getline(ss, hobby, ',')) {
                hobbies.push_back(hobby);
            }

            int newId = sn.getNextUserId();
            User u(newId, userName, firstName, lastName, college, location, age, hobbies);
            u.password = password;
            sn.registerUser(u);
            cout<<"registration succesfull";
            sn.saveUsers();
            

        }
        else if(choice==2){
            //Login to existing account
            string userName;
            cout<<"Enter username: ";
            cin>>userName;
            int userId = sn.loginUser(userName);
            //check if userId exist
            if (userId == -1) {
                cout << "Username not found!\n";
                continue;
            }
            cout<<"Logged in successfully as "<<userName<<"\n";
            while(true){
                cout<<"User Menu: \n";
                cout<<"1. View Profile\n";
                cout<<"2. View friend list\n";
                cout<<"3. People you might want to be your friend\n";
                cout<<"4. Interact with a friend\n";
                cout<<"5. Delete Account\n";
                cout<<"6. Logout\n";
                cout<<"Enter choice\n";
                int op = getIntInput();
                if(op==1){
                    sn.viewProfile(userId);
                }
                else if(op==2){
                    sn.displayCurrentFriends(userId);
                }
                else if(op==3){
                    vector<int> suggestions = sn.suggestedFriends(userId);
                    if (suggestions.empty()) {
                        cout << "No friend suggestions available.\n";
                    }else{
                        cout << "Friend Suggestions:\n";
                        for(size_t i=0;i<suggestions.size();i++){
                            cout<<i+1<<".\n";
                            sn.viewProfile(suggestions[i]);
                        }
                        cout << "\nEnter the numbers of users you want to add as friends (comma separated), or 0 to skip: ";
                        string input;
                        getline(cin, input); 
                        if(input!="0"){
                            size_t start = 0, end;
                            while ((end = input.find(',', start)) != string::npos) {
                                string token = input.substr(start, end - start);
                                if (!token.empty()) {
                                    int index = stoi(token) - 1;
                                    if (index >= 0 && index < (int)suggestions.size()) {
                                        int fid = suggestions[index];
                                        sn.addFriend(sn.getUser(userId), sn.getUser(fid));
                                        cout << sn.getUser(fid).username << " added as a friend!\n";
                                    }
                                }
                                start = end+1;
                            }
                            // handle the last (or only) number
                            if (start < input.size()) {
                                string token = input.substr(start);
                                if (!token.empty()) {
                                    int index = stoi(token) - 1;
                                    if (index >= 0 && index < (int)suggestions.size()) {
                                        int fid = suggestions[index];
                                        sn.addFriend(sn.getUser(userId), sn.getUser(fid));
                                        cout << sn.getUser(fid).username << " added as a friend!\n";
                                    }
                                }
                            }
                        }
                    }
                    sn.saveFriends();
                    
                }
                else if(op==4){
                   cout << "Enter friend's username to interact with: ";
                    string fname;
                    cin >> fname;

                    int friendId = sn.getUserIdByUsername(fname);
                    if (friendId == -1) {
                        cout << "No such user exists!\n";
                    } else {
                        sn.interact(sn.getUser(userId), sn.getUser(friendId));
                        sn.saveFriends();
                    }
                }
                else if(op==5){
                    // Delete account from login menu
                    User &u = sn.getUser(userId);
                    string inputPassword;
                    cout << "Enter password to confirm deletion: ";
                    getline(cin, inputPassword);
                    
                    if (inputPassword != u.password) {
                        cout << "Incorrect password. Deletion cancelled.\n";
                        continue;
                    }
                    
                    // Confirm deletion
                    cout << "Are you sure you want to delete your account? (yes/no): ";
                    string confirm;
                    getline(cin, confirm);
                    
                    if (confirm == "yes" || confirm == "Yes" || confirm == "YES") {
                        if (sn.deleteUser(userId)) {
                            cout << "User account deleted successfully.\n";
                            sn.saveUsers();
                            sn.saveFriends();
                            cout << "Logging out...\n";
                            break;
                        } else {
                            cout << "Failed to delete user account.\n";
                        }
                    } else {
                        cout << "Deletion cancelled.\n";
                    }
                }
                else if(op==6){
                    cout << "Logging out...\n";
                    break;
                }
                else    cout<<"Invalid choice\n";
            }
            
        }
        else if(choice==3){
            //delete user account
            string userName;
            cout<<"Enter username to delete: ";
            getline(cin, userName);
            
            int userId = sn.getUserIdByUsername(userName);
            if (userId == -1) {
                cout << "Username not found!\n";
                continue;
            }
            
            // Verify password before deletion
            User &u = sn.getUser(userId);
            string inputPassword;
            cout << "Enter password to confirm deletion: ";
            getline(cin, inputPassword);
            
            if (inputPassword != u.password) {
                cout << "Incorrect password. Deletion cancelled.\n";
                continue;
            }
            
            // Confirm deletion
            cout << "Are you sure you want to delete your account? (yes/no): ";
            string confirm;
            getline(cin, confirm);
            
            if (confirm == "yes" || confirm == "Yes" || confirm == "YES") {
                if (sn.deleteUser(userId)) {
                    cout << "User account deleted successfully.\n";
                    sn.saveUsers();
                    sn.saveFriends();
                } else {
                    cout << "Failed to delete user account.\n";
                }
            } else {
                cout << "Deletion cancelled.\n";
            }
        }   
        else if(choice==4){
            //exit
            cout << "Exiting FriendBook...\n";
            sn.saveUsers();
            sn.saveFriends();
            break;
        }

        else cout<<"invalid option. Try again"<<endl;

    }

    return 0;
}
