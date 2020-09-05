#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl

int main() {
  TestRunner tr;
  tr.RunTest([] {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }, "PRINT_VALUES usage example");
  tr.RunTest([] {
    ostringstream output;
    if(true){
      PRINT_VALUES(output, 5, "red belt");
      ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }
    else{
      PRINT_VALUES(output, 5, "red belt");
    }
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }, "PRINT_VALUES usage example");
  tr.RunTest([] {
    ostringstream output;
    if(true)
      PRINT_VALUES(output, 5, "red belt");
    else
      PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }, "PRINT_VALUES usage example");
  tr.RunTest([] {
    ostringstream output;
    for(auto i = 0; i < 2; i++){
      PRINT_VALUES(output, i, "red belt");
    }
    ASSERT_EQUAL(output.str(), "0\nred belt\n1\nred belt\n");
  }, "PRINT_VALUES usage example");
}
