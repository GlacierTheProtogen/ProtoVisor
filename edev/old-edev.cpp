#include <iostream>
#include <fstream>
#include <linux/input.h>
using namespace std;

int main()
{
  std::ifstream input("/dev/input/event0");
  if(input.is_open()) {
    bool session = true;
    std::string output;
    while(session) {
      input >> output;
      std::cout << output << " " << std::endl;
    }
    //input.close();
  }
  else
  {
    std::cout << "event0 not present." << std::endl;
  }


}
