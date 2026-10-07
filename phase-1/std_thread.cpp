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
	std::vector<int> o;
	o.push_back(0); // Add the first offset
        std::vector<int> edges;
	unsigned int x = 0;
	unsigned int refx = x;
	unsigned int y = 0;
	int max_vertex = 0;	
	while(std::getline(input, line)) {
		if(line[0] == '#') {
			continue;
		}
		if(std::sscanf(line.c_str(), "%d %d", &x, &y) == 2) {
			while (refx < x) {
                o.push_back(edges.size());
                refx++;
            }
			refx = x;
			edges.push_back(y);
			// The following is a legacy way to determine list length.
			 max_vertex = (x > max_vertex || y > max_vertex) ? ((x > y) ? x : y) : max_vertex; // assumes that the highest vertex number is equivalent to the number of vertices present.
		}	
				
	}
    while ((int)o.size() <= max_vertex + 1) {
        o.push_back(edges.size());
    }

	return CSRGraph {/*o.size() - 1*/ max_vertex + 1, o, edges}; // more elegant. uses length of offset - 1

}
using QueryCallback = std::function<std::string(const CSRGraph&, int src, int K)>;

struct QueryTask {
	int src;
	int K;
	QueryCallback cb;
	std::string result;
}

void RunTasksParallel(const CSRGraph& g, std::vector<QueryTask>& tasks, int num_threads) {
	std::vector<std::thread> p;
	for(QueryTask a : tasks) {
		std::thread t(a.cb, g, a.src, a.K);
		p.push_back(t);
	}
	
	for(std::thread i : p) {
		i.join();
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
  CSRGraph g = LoadGraph("data/soc-Slashdot0902.txt");
  int v = 6;
  std::cout << "Outdegree of 6: " << g.offsets[v+1] - g.offsets[v] << std::endl;
  for(int i = g.offsets[v]; i < g.offsets[v+1]; ++i){
  	std::cout << i - g.offsets[v] + 1 << " " << g.edges[i] << ' ';
	
  }
  std::cout << std::endl << "Vertices: " << g.num_vertices << std::endl << "Edges: " << g.edges.size() << std::endl;
  return 0;
}
