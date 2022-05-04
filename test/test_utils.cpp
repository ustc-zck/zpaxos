#include "../src/utils.h"
#include <iostream>


//g++ -o test_utils test_utils.cpp ../src/utils.cpp
int main(){
    std::cout << "current millisecnd is " << GetCurrentMillSeconds() << std::endl;
    std::cout << "current microsecond is " << GetCurrentMicroSeconds() << std::endl;
    return 0;    
}