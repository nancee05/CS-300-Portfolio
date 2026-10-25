#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

struct CourseNode {
	Course course;
	CourseNode* left;
	CourseNode* right;

	CourseNode(Course newCourse)
		: course(newCourse), left(nullptr), right(nullptr) {

	}
};

// Insert a course into the binary search tree.
CourseNode* insertCourse(CourseNode* root, Course newCourse) {
	// Create a new node when the correct position is empty.
	if (root == nullptr) {
		return new CourseNode(newCourse);
	}

	// Compare course numbers to choose the left or right branch.
	if (newCourse.courseNumber < root->course.courseNumber) {
		root->left = insertCourse(root->left, newCourse);
	}
	else if (newCourse.courseNumber > root->course.courseNumber) {
		root->right = insertCourse(root->right, newCourse);
	}

	// Return the root so the tree connection is kept.
	return root;
}

// Search the tree for a course number.
CourseNode* searchCourse(CourseNode* root, const string& courseNumber) {
	// Stop if the course is not in the tree.
	if (root == nullptr) {
		return nullptr;
	}

	// Return the node if its course number matches.
	if (courseNumber == root->course.courseNumber) {
		return root;
	}

	// Search the left side if the number comes before this node.
	if (courseNumber < root->course.courseNumber) {
		return searchCourse(root->left, courseNumber);
	}

	// Otherwise, search the right side.
	return searchCourse(root->right, courseNumber);
}

// Print courses in alphanumeric order.
void printCoursesInOrder(CourseNode* root) {
	if (root == nullptr) {
		return;
	}

	// Print the left side first.
	printCoursesInOrder(root->left);

	// Print this course.
	cout << root->course.courseNumber << ", "
		<< root->course.courseName << endl;

	// Print the right side.
	printCoursesInOrder(root->right);
}

// Load courses from a comma-seperated file into the tree.
CourseNode* loadCourses(const string& fileName, CourseNode* root) {
	ifstream inputFile(fileName);

	// Stop if the file cannot be opened.
	if (!inputFile.is_open()) {
		cout << "Error: Could not open the file." << endl;
		return root;
	}

	string line;
	int loadedCount = 0;

	// Read and process one line at a time.
	while (getline(inputFile, line)) {
		if (line.empty()) {
			continue;
		}

		stringstream lineStream(line);
		Course newCourse;

		// Read the required course number and title.
		if (!getline(lineStream, newCourse.courseNumber, ',') ||
			!getline(lineStream, newCourse.courseName, ',')) {
			cout << "Skipping an invalid course record." << endl;
			continue;
		}

		// Read any prerequisite course numbers.
		string prerequisite;
		while (getline(lineStream, prerequisite, ',')) {
			if (!prerequisite.empty()) {
				newCourse.prerequisites.push_back(prerequisite);
			}
		}

		// Add the completed course to the binary search tree.
		root = insertCourse(root, newCourse);
		loadedCount++;
	}

	inputFile.close();
	cout << loadedCount << " courses loaded." << endl;

	return root;
}

// Look up a course and display its information.
void printCourseInformation(CourseNode* root) {
	string courseNumber;

	cout << "What course do you want to know about? ";
	cin >> courseNumber;

	CourseNode* foundCourse = searchCourse(root, courseNumber);

	if (foundCourse == nullptr) {
		cout << "Course was not found." << endl;
		return;
	}

	cout << foundCourse->course.courseNumber << ", "
		<< foundCourse->course.courseName << endl;

	if (foundCourse->course.prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
	}
	else {
		cout << "Prerequisites:" << endl;

		for (int i = 0; i < foundCourse->course.prerequisites.size(); i++) {
			string prerequisiteNumber =
				foundCourse->course.prerequisites[i];

			CourseNode* prerequisiteCourse =
				searchCourse(root, prerequisiteNumber);

			if (prerequisiteCourse != nullptr) {
				cout << prerequisiteCourse->course.courseNumber << ", "
					<< prerequisiteCourse->course.courseName << endl;
			}
			else {
				cout << prerequisiteNumber << " (course not found)" << endl;
			}
		}
	}
}

int main() {
	CourseNode* root = nullptr;
	int choice = 0;

	cout << "Welcome to the course planner." << endl;

	while (choice != 9) {
		cout << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << "What would you like to do? ";

		cin >> choice;

		if (choice == 1) {
			string fileName;
			cout << "Enter the course data file name: ";
			getline(cin >> ws, fileName);
			root = loadCourses(fileName, root);
		}
		else if (choice == 2) {
			if (root == nullptr) {
				cout << "Please load the course data first." << endl;
			}
			else {
				printCoursesInOrder(root);
			}
		}
		else if (choice == 3) {
			if (root == nullptr) {
				cout << "Please load the course data first." << endl;
			}
			else {
				printCourseInformation(root);
			}
		}
		else if (choice == 9) {
			cout << "Thank you for using the course planner!" << endl;
		}
		else {
			cout << choice << " is not a valid option." << endl;
		}
	}

	return 0;
}

