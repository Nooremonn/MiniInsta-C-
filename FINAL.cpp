#include <iostream>
#include <string>

using namespace std;

// Custom queue implementation
struct QueueNode {
    string data;
    QueueNode* next;

    QueueNode(string value) : data(value), next(NULL) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    Queue() : front(NULL), rear(NULL) {}

    void enqueue(string value) {
        QueueNode* newNode = new QueueNode(value);
        if (rear == NULL) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    string dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty!");
        }
        QueueNode* temp = front;
        string value = front->data;
        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }
        delete temp;
        return value;
    }

    bool isEmpty() {
        return front == NULL;
    }

    string peek() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty!");
        }
        return front->data;
    }
    // Show all pending requests
    void showPendingRequests() {
        if (isEmpty()) {
            cout << "No pending requests!" << endl;
            return;
        }
        QueueNode* temp = front;
        cout << "Pending Requests:  ";
        while (temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
    // Bulk approve requests (dequeue multiple requests)
    void bulkApprove(int count) {
        if (isEmpty()) {
            cout << "No pending requests to approve!" << endl;
            return;
        }
        int approvedCount = 0;
        while (approvedCount < count && !isEmpty()) {
            cout << "Approved: " << dequeue() << endl;
            approvedCount++;
        }
        if (approvedCount == 0) {
            cout << "No requests approved. Queue is empty!" << endl;
        }
    }
};

// Custom Stack Implementation
struct StackNode {
    string data;
    StackNode* next;

    StackNode(string value) : data(value), next(NULL) {}
};

class Stack {
private:
    StackNode* top;

public:
    Stack() : top(NULL) {}

    void push(string value) {
        StackNode* newNode = new StackNode(value);
        newNode->next = top;
        top = newNode;
    }

    string pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty!");
        }
        StackNode* temp = top;
        string value = top->data;
        top = top->next;
        delete temp;
        return value;
    }

    bool isEmpty() {
        return top == NULL;
    }

    string peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty!");
        }
        return top->data;
    }
};

// Node for User Profile
struct User {
    string name;
    string password;
    string email;
    string epassword;
    Queue friendRequests;
    Queue notifications;
    User* next;

    // Separate stacks for messages and posts
    Stack posts;
    Stack followersPosts;
    Stack messages;

    User(string uname, string upass, string email, string password)
        : name(uname), password(upass), email(email), epassword(password), next(NULL) {}
};



// Linked list for managing users
class UserList {
private:
    User* head;

public:
    UserList() : head(NULL) {}
    // Show all pending friend requests for a given user
    void showPendingRequests(User* currentUser) {
        if (currentUser != NULL) {
            cout << "Pending friend requests for " << currentUser->name << ": ";
            currentUser->friendRequests.showPendingRequests();
        }
        else {
            cout << "No user is logged in." << endl;
        }
    }

    void bulkApproveRequests(User* currentUser, int count) {
        if (currentUser != nullptr) {
            cout << "Bulk approving friend requests for " << currentUser->name << ": " << endl;
            currentUser->friendRequests.bulkApprove(count);
        }
        else {
            cout << "No user is logged in." << endl;
        }
    }
    void signup(string name, string password, string email, string epassword) {
        User* temp = head;
        while (temp != NULL) {
            if (temp->name == name) {
                cout << "Error: Username already exists.\n";
                return;
            }
            temp = temp->next;
        }
        User* newUser = new User(name, password, email, epassword);
        newUser->next = head;
        head = newUser;
        cout << "User signed up successfully.\n";
    }

    User* login(string name, string password) {
        User* temp = head;
        while (temp != NULL) {
            if (temp->name == name && temp->password == password) {
                cout << "Login successful. Welcome, " << name << "!\n";
                return temp;
            }
            temp = temp->next;
        }
        cout << "Error: Invalid username or password.\n";
        return NULL;
    }

    void addPost(User* user, string postContent) {
        user->posts.push(postContent);
        user->notifications.enqueue("You added a new post: " + postContent);
        cout << "Post added successfully.\n";
    }

    void viewOwnPosts(User* user) {
        if (user->posts.isEmpty()) {
            cout << "No posts available.\n";
            return;
        }
        cout << "Your Posts:\n";
        Stack tempStack;
        while (!user->posts.isEmpty()) {
            string post = user->posts.pop();
            cout << "- " << post << "\n";
            tempStack.push(post);
        }
        while (!tempStack.isEmpty()) {
            user->posts.push(tempStack.pop());
        }
    }

    void viewUserProfile(string username) {
        User* temp = head;
        while (temp != NULL) {
            if (temp->name == username) {
                cout << "Profile of " << username << ":\n";
                cout << "email: " << temp->email << "\n";
                viewOwnPosts(temp);
                return;
            }
            temp = temp->next;
        }
        cout << "Error: User does not exist.\n";
    }

    void sendMessage(User* sender, string receiverName, string messageContent) {
        User* receiver = head;
        while (receiver != NULL) {
            if (receiver->name == receiverName) {
                receiver->messages.push(sender->name + ": " + messageContent);
                receiver->notifications.enqueue("New message from " + sender->name + ": " + messageContent);
                cout << "Message sent to " << receiverName << ".\n";
                return;
            }
            receiver = receiver->next;
        }
        cout << "Error: User not found.\n";
    }

    void viewMessages(User* user) {
        if (user->messages.isEmpty()) {
            cout << "No messages available.\n";
            return;
        }
        cout << "Your Messages:\n";
        Stack tempStack;
        while (!user->messages.isEmpty()) {
            string message = user->messages.pop();
            cout << "- " << message << "\n";
            tempStack.push(message);
        }
        while (!tempStack.isEmpty()) {
            user->messages.push(tempStack.pop());
        }
    }

    void sendFriendRequest(User* sender, string receiverName) {
        User* receiver = head;
        while (receiver != NULL) {
            if (receiver->name == receiverName) {
                receiver->friendRequests.enqueue(sender->name);
                receiver->notifications.enqueue("You received a friend request from " + sender->name);
                cout << "Friend request sent to " << receiverName << ".\n";
                return;
            }
            receiver = receiver->next;
        }
        cout << "Error: User not found.\n";
    }
    void viewNotifications(User* user) {
        if (user->notifications.isEmpty()) {
            cout << "No new notifications.\n";
            return;
        }

        cout << "Notifications for " << user->name << ":\n";

        // Display and handle notifications
        while (!user->notifications.isEmpty()) {
            string notification = user->notifications.dequeue();

            // Check if the notification is about a friend request acceptance
            if (notification.find("accepted your friend request") != string::npos) {
                cout << "- " << notification << " (Friend request accepted!)\n";
            }
            else if (notification.find("rejected your friend request") != string::npos) {
                cout << "- " << notification << " (Friend request rejected!)\n";
            }
            else if (notification.find("You are now friends with") != string::npos) {
                cout << "- " << notification << " (New friendship notification)\n";
            }
            else {
                cout << "- " << notification << "\n";
            }
        }
    }

    void handleFriendRequests(User* currentUser) {
        if (currentUser == nullptr) {
            cout << "No user is logged in." << endl;
            return;
        }

        // Display all pending friend requests
        if (currentUser->friendRequests.isEmpty()) {
            cout << "No pending friend requests for " << currentUser->name << ".\n";
            return;
        }

        cout << "Pending friend requests for " << currentUser->name << ":\n";
        currentUser->friendRequests.showPendingRequests();

        // Ask user for bulk approval or individual handling
        cout << "Do you want to bulk approve requests? (y/n): ";
        char bulkResponse;
        cin >> bulkResponse;

        if (bulkResponse == 'y' || bulkResponse == 'Y') {
            cout << "Enter the number of requests to approve: ";
            int count;
            cin >> count;

            // Bulk approve requests
            currentUser->friendRequests.bulkApprove(count);
        }
        else {
            // Handle requests individually
            cout << "Handling friend requests one by one:\n";
            while (!currentUser->friendRequests.isEmpty()) {
                string sender = currentUser->friendRequests.dequeue();
                cout << "Accept friend request from " << sender << "? (y/n): ";
                char response;
                cin >> response;
                if (response == 'y' || response == 'Y') {
                    cout << "Friend request accepted from " << sender << ".\n";
                    currentUser->notifications.enqueue("You are now friends with " + sender);

                    // Notify the sender about the acceptance
                    User* temp = head;
                    while (temp != NULL) {
                        if (temp->name == sender) {
                            temp->notifications.enqueue(currentUser->name + " accepted your friend request.");
                            break;
                        }
                        temp = temp->next;
                    }
                }
                else {
                    cout << "Friend request rejected from " << sender << ".\n";

                    // Notify the sender about the rejection
                    User* temp = head;
                    while (temp != NULL) {
                        if (temp->name == sender) {
                            temp->notifications.enqueue(currentUser->name + " rejected your friend request.");
                            break;
                        }
                        temp = temp->next;
                    }
                }
            }
        }
    }
    void suggestFriends(User* currentUser) {
        if (currentUser == nullptr) {
            cout << "No user is logged in." << endl;
            return;
        }

        if (head == nullptr) {
            cout << "No accounts available to suggest." << endl;
            return;
        }

        cout << "Friend Suggestions for " << currentUser->name << ":\n";

        // Traverse through all users
        User* temp = head;
        bool foundSuggestions = false;
        while (temp != nullptr) {
            // Skip the current user
            if (temp->name != currentUser->name) {
                cout << "- " << temp->name << "\n";
                foundSuggestions = true;
            }
            temp = temp->next;
        }

        if (!foundSuggestions) {
            cout << "No friend suggestions available.\n";
        }
    }

    void newsfeed(User* currentUser) {
        if (currentUser == nullptr) {
            cout << "No user is logged in." << endl;
            return;
        }

        if (head == nullptr) {
            cout << "No accounts available to show posts." << endl;
            return;
        }

        cout << "Newsfeed: \n ";

        // Traverse through all users
        User* temp = head;
        bool foundPosts = false;

        while (temp != nullptr) {
            // Skip the current user to avoid showing their own posts in their newsfeed
            if (temp->name != currentUser->name) {
                // Check if the user has any posts
                if (!temp->posts.isEmpty()) {
                    cout << "Posts by " << temp->name << ":\n";

                    Stack tempStack;

                    // Retrieve and display all posts of the current user
                    while (!temp->posts.isEmpty()) {
                        string post = temp->posts.pop();
                        cout << "- " << post << "\n";
                        tempStack.push(post);  // Save posts back to a temporary stack
                    }

                    // Restore posts back to the user's posts stack
                    while (!tempStack.isEmpty()) {
                        temp->posts.push(tempStack.pop());
                    }

                    foundPosts = true;
                }
            }
            temp = temp->next;  // Move to the next user
        }

        if (!foundPosts) {
            cout << "No posts available in the newsfeed.\n";
        }
    }


};





int main() {
    UserList userList;
    int choice;
    User* currentUser = nullptr;

    do {
        cout << "\n--- Mini INSTGRAM ---\n";
        cout << "1. Signup\n2. Login\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, password, email, epassword;
            cout << "Enter username: ";
            cin >> name;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> epassword;
            userList.signup(name, password, email, epassword);
            break;
        }
        case 2: {
            string name, password;
            cout << "Enter username: ";
            cin >> name;
            cout << "Enter password: ";
            cin >> password;
            currentUser = userList.login(name, password);
            if (currentUser == nullptr) {
                cout << "Invalid login credentials.\n";
            }
            else {
                cout << "Login successful!\n";
            }
            break;
        }
        case 0: {
            cout << "Exiting...\n";
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        if (currentUser != nullptr) {
            do {
                // Assuming user is logged in, show additional options
                cout << "\n--- Welcome, " << currentUser->name << " ---\n";
                cout << "1. Add Post\n2. View Own Posts\n3. Send Message\n4. View Messages\n5. Send Friend Request\n6. Handle Friend Requests\n7. View Notifications\n8. Search User Profile\n9. Suggested friends\n10. Pending requests\n11. Bulk Approval\n12. viewnewsfeed\n13. logout\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case 1: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        string postContent;
                        cin.ignore();
                        cout << "Enter post content: ";
                        getline(cin, postContent);
                        userList.addPost(currentUser, postContent);
                    }
                    break;
                }
                case 2: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.viewOwnPosts(currentUser);
                    }
                    break;
                }
                case 3: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        string recipient, message;
                        cin.ignore();
                        cout << "Enter recipient's username: ";
                        cin >> recipient;
                        cin.ignore();
                        cout << "Enter your message: ";
                        getline(cin, message);
                        userList.sendMessage(currentUser, recipient, message);
                    }
                    break;
                }
                case 4: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.viewMessages(currentUser);
                    }
                    break;
                }
                case 5: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        string recipient;
                        cout << "Enter the username to send a friend request: ";
                        cin >> recipient;
                        userList.sendFriendRequest(currentUser, recipient);
                    }
                    break;
                }
                case 6: { // Handle Friend Requests
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.handleFriendRequests(currentUser);
                    }
                    break;
                }

                case 7: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.viewNotifications(currentUser);
                    }
                    break;
                }
                case 8: {
                    string username;
                    cout << "Enter username to search: ";
                    cin >> username;
                    userList.viewUserProfile(username);
                    break;
                }
                case 9: {
                    // Suggest Friends
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.suggestFriends(currentUser);
                    }
                    break;
                }
                case 10:
                {
                    // Handle Pending Friend Requests
                    if (currentUser) {
                        userList.showPendingRequests(currentUser);
                    }
                    else {
                        cout << "Please login first.\n";
                    }
                    break;
                }
                case 11: {  // Bulk Approve Requests
                    if (currentUser) {
                        int count;
                        cout << "Enter the number of requests to approve: ";
                        cin >> count;
                        userList.bulkApproveRequests(currentUser, count);
                    }
                    else {
                        cout << "Please login first.\n";
                    }
                    break;
                }
                case 12: {
                    if (!currentUser) {
                        cout << "Please login first.\n";
                    }
                    else {
                        userList.newsfeed(currentUser);
                    }
                    break;
                }

                case 13: {
                    currentUser = NULL;
                    cout << "Logged out successfully.\n";
                    break;
                }

                default: {
                    cout << "Invalid choice. Please try again.\n";
                }
                }
            } while (choice != 13);
        }
    } while (choice != 0);

    return 0;
}
