# define BOLD 	"\033[1m"
# define RED 	"\033[0;31m"
# define GREEN 	"\033[0;32m"
# define BLUE	"\033[0;34m"
# define CYAN 	"\033[0;36m"
# define GRAY 	"\033[0;37m"
# define LRED 	"\033[0;91m"
# define YELLOW	"\033[0;33m"
# define MAGENTA "\033[0;35m" 
# define RESET  "\033[0m"
# define RGB 	"\033[1m\033[38;2;255;182;193m"





#include <iostream>
#include <vector>
#include <map>
#include <string>



int main() {    
    std::cout <<YELLOW "==== test 56 63 ==== vector\n" RESET;
    std::vector<int> vector_1;
    for (int i = 0;  i< 15; ++i){
        vector_1.push_back(i * 10);
        if  (i == 5){
            std::cout << "vector_1.pop_back();  = delete = " << vector_1[i] ;///
            vector_1.pop_back();
            std::cout << "\n";
        }
    }
    std::cout << "vector_1[3]= " << vector_1[3] << std::endl;
    // print แบบวน for
    std::cout <<BLUE "for (int i = 0;  i < vector_1.size();  ++i)\n" RESET;
    for (int i = 0; i < (int)vector_1.size(); ++i) {
        std::cout << "vector_1   = " << vector_1[i] << std::endl;
        if (vector_1[i+1] == 30){
            vector_1.erase(vector_1.begin() + (i+1));  // ลบ 30
        }
        if (vector_1[i+1] == 80){
            vector_1.erase((vector_1.begin() + (i+1)) ,  (vector_1.begin() + (i+4))); // ลบ 80 -100
        }
    }
    // print แบบวน iterator
    std::cout <<BLUE "for (std::vector<int>::iterator it = vector_1.begin();\n      it != vector_1.end();\n      ++it) \n" RESET;
    for (std::vector<int>::iterator it = vector_1.begin(); 
                                    it != vector_1.end(); 
                                    ++it) {
        std::cout << *it << " "; // แสดงผล: 10 20 30 40
    }
    std::cout << "\n";
    std::cout <<YELLOW "==== test 58 ==== map\n" RESET;
    std::map<std::string, int> map_1;
    map_1["bob"] = 90;
    map_1["john"] = 80;
    map_1["alice"] = 85;
    map_1["Alice"] = 75;
    // ลองหา Alice (case sensitive)
    std::cout <<BLUE "std::map<std::string,int>::iterator it = map_1.find(Alice);\nif (it != map_1.end())\n" RESET;
    std::map<std::string,int>::iterator it = map_1.find("Alice");
    if (it != map_1.end()) {
        std::cout << "map_1.find(Alice)\n" ; // << it->second << std::endl;
        std::cout << "it->first  = " << it->first << std::endl; 
        std::cout << "it->second = " << it->second << std::endl;
    } else {
        std::cout << "map_1.find(Alice)\n";
        std::cout << "not find(Alice ,alice)\n";
    }
    std::cout << BLUE "for (std::map<std::string,int>::iterator it = map_1.begin();\n   it != map_1.end();\n   ++it) \n" RESET;
    for (std::map<std::string,int>::iterator it = map_1.begin(); 
                                             it != map_1.end(); 
                                             ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }
    return 0;
}
