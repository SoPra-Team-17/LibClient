#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <string>


class TestClass {
 public:
    TestClass() {
        text = "init";
    }

    std::string text;

    void setText(std::string t);

    std::string getText();

    void printText();
};

void printTestClass(TestClass tc);


#endif  // LIBRARY_H_
