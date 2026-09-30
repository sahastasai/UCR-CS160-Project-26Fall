// g++ -std=c++20 -pthread std_thread.cpp -o std_thread && ./std_thread

#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct CSRGraph {
    int num_vertices;
    std::vector<int> offsets;  // size: num_vertices + 1
    std::vector<int> edges;    // destination IDs, grouped by source vertex
};

CSRGraph LoadGraph(const char *filename){
	std::ifstream input(filename);
	std::string line;
	while(std::getline(input, line)) {
		if(line)	
	}
}

void worker(int id) {
  for (int i = 0; i < 5; ++i) {
    std::cout << "std::thread worker " << id << ", iteration " << i
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

int main() {
  std::thread t1(worker, 1);
  std::thread t2(worker, 2);

  t1.join();
  t2.join();

  std::cout << "All std::thread workers finished." << std::endl;
  return 0;
}
