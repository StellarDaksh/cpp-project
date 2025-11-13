#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <sstream>

// Forward declarations
class Student;
class Course;
class Faculty;

// =====================================================================
// MODULE 5: Exception Handling
// Custom Exception Class for system errors
// =====================================================================
class SystemException : public std::runtime_error {
public:
    // Demonstrates Constructor with default arguments (optional)
    SystemException(const std::string& message = "An unknown system error occurred.")
        : std::runtime_error(message) {}
};

// =====================================================================
// MODULE 5: Template Class
// A generic container to manage lists of any type of record (Student, Course, etc.)
// =====================================================================
template <typename T>
class RecordList {
private:
    std::vector<T> records; // Internal storage for the array of objects

public:
    // Demonstrates Function with Default Arguments
    void add_record(const T& record, bool verbose = true) {
        records.push_back(record);
        if (verbose) {
            std::cout << "[INFO] Record successfully added to the list." << std::endl;
        }
    }

    T* find_record(int id) {
        for (T& record : records) {
            // Note: Assumes T has a get_id() or equivalent method
            if (record.get_id() == id) {
                return &record;
            }
        }
        return nullptr;
    }

    void display_all() const {
        if (records.empty()) {
            std::cout << "No records found." << std::endl;
            return;
        }
        for (const T& record : records) {
            record.display_details();
            std::cout << "--------------------------------------" << std::endl;
        }
    }

    // Reference usage to return the internal vector (avoids copy)
    std::vector<T>& get_records() {
        return records;
    }

    // Demonstrates Inline Function definition
    inline size_t count() const { return records.size(); }
};

// =====================================================================
// MODULE 1, 4: Course Class
// Demonstrates Class, Object, Static Members, and Operator Overloading
// =====================================================================
class Course {
private:
    // Data Members (private access specifier)
    const int course_id; // MODULE 4: Constant Data Member
    std::string title;
    int capacity;
    int enrolled_students;

public:
    // MODULE 4: Static Data Member (tracks total courses created)
    static int total_courses;

    // MODULE 2: Constructor (Default Argument & 'this' pointer)
    Course(int id, std::string t = "Untitled Course", int cap = 30)
        : course_id(id), title(t), capacity(cap), enrolled_students(0)
    {
        total_courses++;
        // Explicit use of 'this' pointer
        this->capacity = cap;
    }

    // MODULE 2: Destructor (Simple demonstration)
    ~Course() {
        // total_courses--; // We don't decrement here to avoid confusion in main demo
    }

    // Member Function (inside class definition - often for inline)
    int get_id() const {
        return course_id;
    }
    
    // FIX: Public Getter to access private data
    inline int get_enrolled_students() const { return enrolled_students; }

    // MODULE 4: Constant Member Function (cannot modify the object's state)
    void display_details() const;

    void increment_enrollment() {
        if (enrolled_students >= capacity) {
            // MODULE 5: Exception Handling (Throwing an exception)
            throw SystemException("Course is already full. Enrollment failed.");
        }
        enrolled_students++;
    }

    // MODULE 4: Static Member Function
    static int get_total_courses() {
        return total_courses;
    }

    // MODULE 4: Operator Overloading (Overloading the insertion operator <<)
    // Declared as a friend function to allow access to private members
    friend std::ostream& operator<<(std::ostream& os, const Course& course);

    // File Handling helper for saving
    std::string to_string() const {
        return std::to_string(course_id) + "|" + title + "|" + std::to_string(capacity) + "|" + std::to_string(enrolled_students);
    }
};

// MODULE 4: Initialization of Static Data Member
int Course::total_courses = 0;

// MODULE 4: Operator Overloading implementation
std::ostream& operator<<(std::ostream& os, const Course& course) {
    os << "Code: " << course.course_id
       << " | Title: " << course.title
       << " | Enrollment: " << course.enrolled_students << "/" << course.capacity;
    return os;
}

// MODULE 1: Defining Member Function Outside Class
void Course::display_details() const {
    std::cout << "Course Details:\n" << *this << std::endl;
}

// =====================================================================
// MODULE 3, 4: Abstract Base Class
// Demonstrates Pure Virtual Function and the start of Polymorphism
// =====================================================================
class User {
protected:
    // Protected Access Specifier (accessible by derived classes)
    int user_id;
    std::string name;

public:
    // Constructor
    User(int id, std::string n) : user_id(id), name(n) {}

    // MODULE 3: Pure Virtual Function (Makes User an Abstract Class)
    virtual void display_details() const = 0;

    // MODULE 4: Virtual Destructor (Essential for proper cleanup in inheritance hierarchies)
    virtual ~User() {}

    // Inline Function
    inline int get_id() const { return user_id; }
    inline const std::string& get_name() const { return name; }
};

// =====================================================================
// MODULE 1, 3, 4: Student Class
// Demonstrates Inheritance, Static Members, Const, Friend, Overloading
// =====================================================================
class Student : public User {
private:
    // Data Members
    static int next_roll_number; // MODULE 4: Static Data Member (for auto-incrementing ID)
    int roll_number;
    std::vector<int> enrolled_course_ids;

    // MODULE 2: Friend Class Declaration
    friend class DatabaseManager; // Allows DatabaseManager to access private members

public:
    // MODULE 2: Constructor (Initializes User base class, uses 'this' pointer)
    Student(std::string n, int id) : User(id, n), roll_number(next_roll_number++) {
        // Explicit use of 'this' pointer
        this->user_id = id;
        std::cout << "Student " << n << " created with Roll No: " << roll_number << std::endl;
    }

    // Constructor for loading from file (without auto-incrementing static counter)
    Student(std::string n, int id, int roll) : User(id, n), roll_number(roll) {
        // Ensure static counter is up-to-date
        if (roll >= next_roll_number) {
            next_roll_number = roll + 1;
        }
    }

    // MODULE 2: Destructor
    ~Student() {
        std::cout << "Student object for " << name << " (Roll: " << roll_number << ") destroyed." << std::endl;
    }

    // MODULE 3, 4: Function Overriding (Implements Pure Virtual Function)
    // Demonstrates Dynamic Binding/Runtime Polymorphism
    void display_details() const override {
        std::cout << "--- Student Details ---" << std::endl;
        std::cout << "ID (User): " << user_id << std::endl;
        std::cout << "Roll No: " << roll_number << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Enrolled Courses: " << enrolled_course_ids.size() << std::endl;
    }

    // MODULE 2: Function Overloading (enroll by ID)
    void enroll(int course_id) {
        if (std::find(enrolled_course_ids.begin(), enrolled_course_ids.end(), course_id) == enrolled_course_ids.end()) {
            enrolled_course_ids.push_back(course_id);
            std::cout << name << " enrolled in course ID " << course_id << " (via ID)." << std::endl;
        } else {
            std::cout << name << " is already enrolled in course ID " << course_id << "." << std::endl;
        }
    }

    // MODULE 2: Function Overloading (enroll by Course object reference)
    void enroll(const Course& course) {
        // MODULE 2: Reference usage
        int course_id = course.get_id();
        if (std::find(enrolled_course_ids.begin(), enrolled_course_ids.end(), course_id) == enrolled_course_ids.end()) {
            try {
                // Throws an exception if the course is full
                Course& mutable_course = const_cast<Course&>(course); // Casting away const for enrollment count
                mutable_course.increment_enrollment();
                enrolled_course_ids.push_back(course_id);
                std::cout << name << " successfully enrolled in " << course.get_id() << " (via Object)." << std::endl;
            } catch (const SystemException& e) {
                // MODULE 5: Exception Handling (Catching the exception)
                std::cerr << "ENROLLMENT ERROR: " << e.what() << std::endl;
            }
        } else {
            std::cout << name << " is already enrolled in course ID " << course_id << "." << std::endl;
        }
    }

    // MODULE 4: Static Member Function
    static int get_total_students() {
        return next_roll_number - 1;
    }

    // File Handling helper for saving
    std::string to_string() const {
        std::string courses_str;
        for (int id : enrolled_course_ids) {
            courses_str += std::to_string(id) + ",";
        }
        if (!courses_str.empty()) {
            courses_str.pop_back(); // Remove trailing comma
        }
        return std::to_string(user_id) + "|" + name + "|" + std::to_string(roll_number) + "|" + courses_str;
    }
};

// MODULE 4: Initialization of Static Data Member
int Student::next_roll_number = 1001;

// =====================================================================
// MODULE 3: Faculty Class (Simple Inheritance Demo)
// =====================================================================
class Faculty : public User {
private:
    std::string department;
    // MODULE 1: Array of Objects (using Course objects)
    std::vector<Course*> courses_taught; // MODULE 2: Demonstrating Dynamic Memory with pointers

public:
    // Constructor
    Faculty(std::string n, int id, std::string dept) : User(id, n), department(dept) {
        // Initial setup
    }

    // MODULE 3, 4: Function Overriding (Implements Pure Virtual Function)
    void display_details() const override {
        std::cout << "--- Faculty Details ---" << std::endl;
        std::cout << "ID: " << user_id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Department: " << department << std::endl;
        std::cout << "Courses Taught: " << courses_taught.size() << std::endl;
    }

    // MODULE 2: Dynamic Memory Allocation (using new)
    void assign_course(Course* course_ptr) {
        if (course_ptr) {
            courses_taught.push_back(course_ptr);
            std::cout << name << " assigned course " << course_ptr->get_id() << "." << std::endl;
        }
    }

    // MODULE 2: Destructor (Essential for cleaning up dynamically allocated resources)
    ~Faculty() {
        // Since the Course objects are managed externally, we only clear the pointers here.
        // If Faculty owned the courses, we would delete the pointers in this loop.
        courses_taught.clear();
        std::cout << "Faculty object for " << name << " destroyed." << std::endl;
    }
};

// =====================================================================
// MODULE 2, 5: DatabaseManager Class
// Demonstrates Friend Class, Stream Class, and File Handling
// =====================================================================
class DatabaseManager {
private:
    const std::string STUDENT_FILE = "student_records.txt";
    const std::string COURSE_FILE = "course_records.txt";

    // Helper to tokenize a line
    std::vector<std::string> split(const std::string& s, char delimiter) const {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    // MODULE 5: File Handling (Writing to file - Stream Class usage)
    void save_students(RecordList<Student>& student_list) {
        std::ofstream outfile(STUDENT_FILE); // Stream Class
        if (!outfile.is_open()) {
            // MODULE 5: Exception Handling (Throwing for file error)
            throw SystemException("Could not open student file for writing.");
        }

        for (const Student& s : student_list.get_records()) {
            // MODULE 2: Accessing private members of Student via Friend Class
            outfile << s.to_string() << "\n";
        }
        outfile.close();
        std::cout << "\n[DB] Student records saved successfully." << std::endl;
    }

    // MODULE 5: File Handling (Reading from file - Stream Class usage)
    void load_students(RecordList<Student>& student_list) {
        student_list.get_records().clear();
        std::ifstream infile(STUDENT_FILE); // Stream Class

        if (!infile.is_open()) {
            std::cout << "\n[DB] Student file not found. Starting with empty database." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(infile, line)) {
            if (line.empty()) continue;

            try {
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() < 3) continue;

                int id = std::stoi(parts[0]);
                std::string name = parts[1];
                int roll = std::stoi(parts[2]);

                // Constructor that takes roll number for loading
                Student s(name, id, roll);

                // Load enrolled courses
                if (parts.size() > 3 && !parts[3].empty()) {
                    std::vector<std::string> course_ids = split(parts[3], ',');
                    for (const std::string& cid_str : course_ids) {
                        s.enroll(std::stoi(cid_str)); // Uses Function Overloading
                    }
                }
                student_list.add_record(s, false); // Add loaded student to list
            } catch (const std::invalid_argument& e) {
                // MODULE 5: Exception Handling (Catching conversion errors)
                std::cerr << "[DB ERROR] Corrupt data line skipped: " << line << " (" << e.what() << ")" << std::endl;
            }
        }
        infile.close();
        std::cout << "[DB] Student records loaded successfully. Total: " << student_list.count() << std::endl;
    }
    
    // Simple course saving (for demonstration)
    void save_courses(const std::vector<Course*>& course_list) {
        std::ofstream outfile(COURSE_FILE); 
        if (!outfile.is_open()) {
            throw SystemException("Could not open course file for writing.");
        }

        for (const Course* c : course_list) {
            outfile << c->to_string() << "\n";
        }
        outfile.close();
        std::cout << "[DB] Course records saved successfully." << std::endl;
    }
};

// =====================================================================
// MAIN FUNCTION (Demonstration of all concepts)
// =====================================================================
int main() {
    std::cout << "=== University Management System (RTU Syllabus Project) ===" << std::endl;
    std::cout << "Demonstrating C++ OOP concepts from Classes/Objects to File Handling." << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;

    // --- MODULE 1, 4, 5: Classes, Objects, Static, Operator Overloading, Exceptions ---
    std::cout << "\n[Module 1/4/5: Course and Objects]" << std::endl;

    // Creation of Course Objects (demonstrates Constructor)
    Course math_c(101, "Advanced Mathematics", 40);
    Course cs_c(201, "Data Structures & Algos"); // Uses default argument for capacity (30)

    // Demonstrates Operator Overloading (<<) and Static Member Function
    std::cout << cs_c << std::endl;
    std::cout << "Total Courses Created (Static): " << Course::get_total_courses() << std::endl;
    
    // Use an Array/Vector of Pointers to Course objects (for easier management)
    std::vector<Course*> available_courses;
    available_courses.push_back(&math_c);
    available_courses.push_back(&cs_c);

    // Demonstrate Exception Handling (Attempting to enroll in a full course)
    try {
        for (int i = 0; i < 30; ++i) {
            cs_c.increment_enrollment();
        }
        // FIX: Accessing private member via public getter function
        std::cout << "[Test] Current CS enrollment: " << cs_c.get_enrolled_students() << std::endl;
        
        // This will throw the exception defined in Course::increment_enrollment()
        cs_c.increment_enrollment(); 
    } catch (const SystemException& e) {
        // Catch block demonstrates Module 5 requirement
        std::cerr << "\n[EXCEPTION CAUGHT]: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    }
    std::cout << "----------------------------------------------------------------" << std::endl;

    // --- MODULE 3, 4: Inheritance, Polymorphism, Dynamic Binding, Abstract Class ---
    std::cout << "\n[Module 3/4: Inheritance, Polymorphism & Abstract Class]" << std::endl;

    // Creation of Derived Objects
    Student s1("Alice Smith", 5001); // Uses Static Member to assign Roll No 1001
    Student s2("Bob Johnson", 5002); // Uses Static Member to assign Roll No 1002
    Faculty f1("Dr. Chen", 7001, "Computer Science");
    
    // Demonstrate Static Member Function of Student
    std::cout << "Total Students Created (Static): " << Student::get_total_students() << std::endl;
    
    // Polymorphism and Dynamic Binding demonstration (using User* pointers)
    std::vector<User*> users;
    users.push_back(&s1);
    users.push_back(&f1);

    for (const auto& user_ptr : users) {
        // Calls the correct display_details() based on the actual object type (Dynamic Binding)
        user_ptr->display_details();
    }
    std::cout << "----------------------------------------------------------------" << std::endl;

    // --- MODULE 2: Function Overloading, Reference, Friend Class ---
    std::cout << "\n[Module 2: Overloading, Enrollment & Friend Class Setup]" << std::endl;

    // Demonstrate Function Overloading
    s1.enroll(101);         // Calls enroll(int)
    s2.enroll(math_c);      // Calls enroll(const Course&), demonstrating reference usage
    s2.enroll(cs_c);        // Demonstrates the enrollment system (might catch the 'full' exception)
    
    f1.assign_course(&math_c); // Dynamic Memory Usage (Pointer to Course)
    
    // --- MODULE 5: Template Class and File Handling ---
    std::cout << "\n[Module 5: Template & File Handling]" << std::endl;

    // Demonstrates Template Class
    RecordList<Student> student_db;
    student_db.add_record(s1);
    student_db.add_record(s2);
    
    DatabaseManager db_manager;
    
    // Demonstrates File Handling (Writing to file)
    try {
        db_manager.save_students(student_db); // Uses Friend Class access to save private Student data
        db_manager.save_courses(available_courses);
    } catch (const SystemException& e) {
        std::cerr << "\n[FILE ERROR]: " << e.what() << std::endl;
    }
    
    // --- Post-Save/Load Demonstration ---
    std::cout << "\n--- DEMONSTRATING LOAD (New DB object created) ---" << std::endl;
    RecordList<Student> loaded_student_db;
    
    // Demonstrates File Handling (Reading from file)
    try {
        db_manager.load_students(loaded_student_db);
    } catch (const SystemException& e) {
        std::cerr << "\n[FILE ERROR]: " << e.what() << std::endl;
    }
    
    // Display loaded records to confirm File I/O success
    loaded_student_db.display_all();
    
    // Search demonstration
    Student* found_s = loaded_student_db.find_record(5001);
    if(found_s) {
        std::cout << "Found record for ID 5001: " << found_s->get_name() << std::endl;
    }

    // Program termination (demonstrates Destructors being called for s1, s2, f1)
    std::cout << "\n=== Program End: Global and stack objects are being destroyed ===" << std::endl;
    
    return 0;
}