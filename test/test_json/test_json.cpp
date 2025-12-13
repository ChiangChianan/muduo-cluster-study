#include <iostream>
#include <map>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

void fun1() {
  json js;
  js["msg_type"] = 2;
  js["from"] = "host";
  js["to"] = "net";
  js["msg"] = "Hello Json";
  cout << js << endl;
}

int main() {
  fun1();
  return 0;
}
