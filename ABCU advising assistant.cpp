
//Name: ABCU advising assistant
//Author: Adam Schmidt
//Version: 1.0


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>


using namespace std;



struct Course {
	string courseNum;
	string courseName;
	vector<string> preReq;
	Course() {//default
		courseNum = "Unknown course number";
		courseName = "Unknown course name";
		preReq.push_back("No PreReq found");
	}
	Course(string inputString) {//overloaded consturct

		stringstream input(inputString); //open string stream

		if (!input.eof()) { //setting course num
			//if not empty
			getline(input, courseNum, ',');
		}
		else {
			courseNum = "Unknown course number";
		}

		if (!input.eof()) {//setting course name
			//if not empty
			getline(input, courseName, ',');
		}
		else {
			courseName = "Unknown course name";
		}

		while (!input.eof()) { //setting prereq's
			string curPreReq;
			getline(input, curPreReq, ',');
			preReq.push_back(curPreReq);

		}
		return;
	}
};


string menuDisplay() {//displays the menu, kind of self explanatory lol
	string input = " ";

	cout << endl << "Advising assitant" << endl;
	cout << endl << "Menu:" << endl;
	cout << "1: Load Courses" << endl;
	cout << "2: Print all courses" << endl;
	cout << "3: Search for a course" << endl;
	cout << "4: Exit assitant" << endl;
	cin >> input;
	return input;

}

vector<Course> loadCourses(string fileName) {
	vector<Course> courseVector;//used to store the courses
	string curString;
	Course curCourse;
	vector<string> courseNums; //stores a list of course numbers for later error checking


	ifstream file;
	file.open(fileName, ios::in);
	if (file.is_open()) {
		while (!file.eof()) {
			getline (file, curString);
			curCourse = Course(curString); //create course from csv string
			courseVector.push_back(curCourse); //add course to vector
		}
		file.close();
	}
	else {
		cout << "Unable to open file, check file name and try again" << endl;
		return courseVector;
	}

	for (unsigned int i = 0; i < courseVector.size(); i++) { //collect all course numbers
		courseNums.push_back(courseVector.at(i).courseNum);
	}
	


	for (unsigned int i = 0; i < courseVector.size(); i++) {//check all course prereq's
		for (unsigned int j = 0; j < courseVector.at(i).preReq.size(); j++) {

			if (find(courseNums.begin(), courseNums.end(), courseVector.at(i).preReq.at(j)) == courseNums.end()) {//check if preReq exists in course listing

				courseVector.at(i).preReq.at(j).append("[Course Not Found]");//append if preReq not found
			}

		}
	}


	return courseVector;

}

bool operator< (Course courseA, Course courseB) { // overload < operator to use the built in vector sort and allow me to compare to course objects
	return (courseA.courseNum < courseB.courseNum);
}

void coursePrintAll(vector<Course> courseVector) {//print all courses in courseVector in order
	

	sort(courseVector.begin(), courseVector.end());// sort the vector
	cout << "============================" << endl;
	for (unsigned int i = 0; i < courseVector.size(); i++) { //print out each course in the vector
		Course curCourse = courseVector.at(i);
		cout << endl << "Course Name: " << curCourse.courseName << endl;
		cout << "Course Number: " << curCourse.courseNum << endl;
		cout << "Pre-requisites: ";

		if (curCourse.preReq.empty()) {
			cout << "No Pre-requisites" << endl;
		}
		else {//print out each course prereq
			for (unsigned int j = 0; j < curCourse.preReq.size(); j++) {
				cout << curCourse.preReq.at(j) << " ";
			}
			cout << endl;
		}
		cout << endl << "============================" << endl;
	}

	return;
}

void coursePrint(vector<Course> courseVector, string courseNum) {


	for (unsigned int i = 0; i < courseVector.size(); i++) {//search the vector
		if (courseVector.at(i).courseNum == courseNum) {//if found print out details
			cout << endl << "=========================" << endl << endl;
			cout << "Course Name: " << courseVector.at(i).courseName << endl;
			cout << "Course Number: " << courseVector.at(i).courseNum << endl;
			cout << "Pre-requisites: ";
			if (courseVector.at(i).preReq.empty()) {
				cout << "No Pre-requisites" << endl;
			}
			else {
				for (unsigned int j = 0; j < courseVector.at(i).preReq.size(); j++) {
					cout << courseVector.at(i).preReq.at(j) << " ";
				}
				cout << endl;
			}
			cout << endl << "============================" << endl;
			return; //return once found
		}
	}
	cout << "Course number " << courseNum << " not found" << endl;//if not found

	

	return;
}


int main() {
	//general variable declerations
	string input;
	string courseNum;
	int choice;
	string fileName;
	vector<Course> courseVector;
	bool menuCheck = true; // used for while loop

	while (menuCheck) {
		input = menuDisplay();

		if (isdigit(input[0])) {//checks input for a number and then converts
			choice = stoi(input);

			switch (choice) {

			case 1: //load courses
				cout << endl << "File must be stored in same location as this .cpp file" << endl;
				cout << "Name of file to load: (include the file extension)" << endl;
				
				cin.clear();
				cin.ignore();
				getline(cin, fileName);
				courseVector = loadCourses(fileName); // load courses and store in vector
				break;

			case 2: // print all
				if (courseVector.empty()) {// check for empty vector
					cout << "no courses loaded, please load a course catalogue file and try again" << endl;
					break;
				}
				coursePrintAll(courseVector);
				break;

			case 3: // search
				if (courseVector.empty()) {// check for empty vector
					cout << "no courses loaded, please load a course catalogue file and try again" << endl;
					break;
				}
				cout << endl << "Course number to search for: " << endl;
				cin.clear();
				cin.ignore();
				cin >> courseNum;
				for (unsigned int i = 0; i < courseNum.size(); i++) {//convert to all caps for search
					courseNum.at(i) = toupper(courseNum.at(i));
				}
				coursePrint(courseVector, courseNum);
				break;

			case 4: //exit
				menuCheck = false;//exit while loop
				break;

			default: // error on input, out of range
				cout << "Invalid selection, please try again" << endl;
				break;


			}
		}
		else {
			cout << "Invalid input, please try again" << endl;
		}
	}
	cout << endl << endl << "Thank you, have a nice day." << endl;
	exit(0);
}


