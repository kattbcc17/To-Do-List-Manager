#include <sys/stat.h>

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Define the Task struct
struct Task {
    string title;
    string description;
    time_t creationTime;
    bool completed;  // Add missing member variable
};

// Create a vector to store all tasks
vector<Task> tasks;

// Function to add a new task to the list
void addTask() {
    Task newTask;
    cout << "Enter task title: ";
    cin.ignore();  // Ignore newline character in input buffer
    getline(cin, newTask.title);

    cout << "Enter task description (optional): ";
    getline(cin, newTask.description);

    newTask.creationTime = time(0);
    newTask.completed = false;

    tasks.push_back(newTask);
    cout << "Task added successfully!" << endl;
}

// Function to remove a task from the list
void removeTask() {
    string taskTitle;  // Change type to string
    cout << "Enter task title to remove: ";
    cin.ignore();  // Ignore newline character in input buffer
    getline(cin, taskTitle);

    // Convert the input task title to lowercase for case-insensitive comparison
    transform(taskTitle.begin(), taskTitle.end(), taskTitle.begin(), ::tolower);

    // Search for the task title in the list (case-insensitive search)
    auto it =
        find_if(tasks.begin(), tasks.end(), [taskTitle](const Task& task) {
            string lowerCaseTitle = task.title;
            transform(lowerCaseTitle.begin(), lowerCaseTitle.end(),
                      lowerCaseTitle.begin(), ::tolower);
            return lowerCaseTitle == taskTitle;
        });

    if (it != tasks.end()) {  // Check if the task title is found
        tasks.erase(it);      // Remove the task from the list
        cout << "Task removed successfully!" << endl;
    } else {
        cout << "Task with title '" << taskTitle << "' not found." << endl;
    }
}

// Function to view all tasks
void viewTasks() {
    cout << "Tasks:" << endl;
    for (int i = 0; i < tasks.size(); i++) {
        cout << "Task " << i + 1 << ": " << tasks[i].title << endl;
        if (!tasks[i].description.empty()) {
            cout << "  Description: " << tasks[i].description << endl;
        }
        cout << "  Created: " << ctime(&tasks[i].creationTime);  // Removed endl
        cout << "  Completed: " << (tasks[i].completed ? "Yes" : "No") << endl;
        cout << endl;
    }
}

// Function to mark a task as completed
void markTaskAsCompleted() {
    // Ask the user to enter the task to mark as completed
    string taskTitle;
    cout << "Enter task title to mark as completed: ";
    cin.ignore();
    getline(cin, taskTitle);

    bool found = false;

    for (Task& task : tasks) {
        // Check if the task title is valid
        if (task.title == taskTitle) {
            task.completed = true;
            cout << "Task '" << taskTitle
                 << "' marked as completed successfully!" << endl;
            found = true;
            break;  // Exit loop once the task is found
        }
    }

    // Display an error message if the task is not found
    if (!found) {
        cout << "Task with title '" << taskTitle << "' not found." << endl;
    }

    // Debug: Output all task titles to check for leading/trailing spaces
    cout << "All task titles:" << endl;
    for (const Task& task : tasks) {
        cout << task.title << endl;
    }
}

// Function to sort tasks by creation time
void sortTasksByCreationTime() {
    // Use the sort algorithm to sort the tasks by creation time
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        // Compare the creation times of two tasks
        return a.creationTime < b.creationTime;
    });
    cout << "Tasks sorted by creation time successfully!" << endl;
}

// Function to search for tasks by title
void searchTasksByTitle() {
    // Ask the user to enter a search query
    string searchQuery;
    cin.ignore();
    cout << "Enter search query: ";
    getline(cin, searchQuery);

    // Initialize a flag to indicate if a task is found
    bool found = false;
    // Iterate through the tasks to search for a match
    for (const Task& task : tasks) {
        // Check if the task title contains the search query
        if (task.title.find(searchQuery) != string::npos) {
            // Display the task title if a match is found
            cout << "Task found: " << task.title << endl;
            found = true;
        }
    }

    // Display a message if no tasks are found
    if (!found) {
        cout << "No tasks found with that title." << endl;
    }
}

// Function to save tasks to a file
void saveTasks() {
    ofstream file(
        "/Users/kattiacontreras/Desktop/To-Do-List-Manager/txt/tasks.txt",
        ios::out);  // Open file in out mode
    if (file.is_open()) {
        for (const Task& task : tasks) {
            file << task.title << endl;
            file << task.description << endl;
            file << ctime(&task.creationTime);  // Convert creation time to
                                                // string using ctime
            file << (task.completed ? "1" : "0") << endl;
        }
        file.close();
        cout << "Tasks saved to file!" << endl;
    } else {
        cout << "Unable to open file." << endl;
        struct stat buffer;
        if (stat("/Users/kattiacontreras/Desktop/To-Do-List-Manager/txt/"
                 "tasks.txt",
                 &buffer) == 0) {
            cout << "File exists, but cannot be opened." << endl;
        } else {
            cout << "File does not exist." << endl;
        }
    }
}

// Function to load tasks from a file
void loadTasks() {
    ifstream file(
        "/Users/kattiacontreras/Desktop/To-Do-List-Manager/txt/tasks.txt");
    if (file.is_open()) {
        Task task;
        string line;
        while (getline(file, task.title)) {
            getline(file, task.description);
            file >> task.creationTime;
            file >> task.completed;
            tasks.push_back(task);
        }
        file.close();
        cout << "Tasks loaded from file!" << endl;
    } else {
        cout << "Unable to open file." << endl;
        struct stat buffer;
        if (stat("/Users/kattiacontreras/Desktop/To-Do-List-Manager/txt/"
                 "tasks.txt",
                 &buffer) == 0) {
            cout << "File exists, but cannot be opened." << endl;
        } else {
            cout << "File does not exist." << endl;
        }
    }
}

int main() {
    try {
        loadTasks();  // Load tasks from file at the beginning
    } catch (const std::exception& e) {
        cout << "Error loading tasks from file: " << e.what() << endl;
        cout << "Continuing with an empty task list." << endl;
    }
    while (true) {
        cout << "To-Do List Manager" << endl;
        cout << "-----------------" << endl;
        cout << "1. Add task" << endl;
        cout << "2. Remove task" << endl;
        cout << "3. View tasks" << endl;
        cout << "4. Save tasks to file" << endl;
        cout << "5. Exit" << endl;
        cout << "6. Mark task as completed" << endl;
        cout << "7. Sort tasks by creation time" << endl;
        cout << "8. Search for tasks by title" << endl;
        int choice;
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 ||
               choice > 8) {  // Validate input
            cout << "Invalid choice. Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                removeTask();
                break;
            case 3:
                viewTasks();
                break;
            case 4:
                saveTasks();  // Call function to save tasks
                break;
            case 5:
                return 0;
            case 6:
                markTaskAsCompleted();
                break;
            case 7:
                sortTasksByCreationTime();
                break;
            case 8:
                searchTasksByTitle();
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }

    return 0;
}