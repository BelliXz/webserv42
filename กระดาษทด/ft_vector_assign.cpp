

// assign ใน
// 		req.rawBody.assign(buffer, buffer + strlen(buffer));
// 		เป็นการใช้ฟังก์ชัน .assign() ของ std::vector<char> 
// 		เพื่อ คัดลอกข้อมูลจาก pointer (buffer) มาเก็บใน vector.
// โครงสร้าง:
// 		std::vector<char>::assign(InputIt_first  ,  InputIt_last)
// 		first → จุดเริ่มต้นของข้อมูล
// 		last  → จุดสุดท้าย (exclusive)
// 		จะ ลบข้อมูลเดิม แล้วแทนที่ด้วยข้อมูลใหม่ที่อยู่ระหว่าง [first, last)
// อธิบายตามโค้ด:
// 		req.rawBody.assign(buffer, buffer + strlen(buffer));
// 		buffer: pointer ไปยังข้อมูล (เช่น char array หรือ raw bytes ที่อ่านมาจาก socket)
// 		strlen(buffer): หาความยาวของ string แบบ C (จนเจอ '\0')
// 		buffer + strlen(buffer): pointer ที่ชี้ถัดจากตัวสุดท้าย
// ดังนั้น…
// 		assign(buffer, buffer + strlen(buffer));
// 		→ คือ "ให้ req.rawBody เก็บทุกตัวอักษรใน buffer ตั้งแต่ต้นจนถึงก่อน '\0'"




#include <iostream>
#include <vector>
#include <list>
#include <cstring> // assign    strlen



void	ft0 (){
	char 				buffer[] = "1234567";
	std::vector<char> 	data;

	// data.assign(buffer, buffer + strlen(buffer));
	// for (size_t i = 0; i < data.size();  ++i)
	//     std::cout << data[i];  				// จะพิมพ์ Hello World


	data.assign(buffer, buffer+strlen(buffer)); 

	char 				test[] = "abc";
	data.assign(test, test+strlen(buffer)); //??


	for (size_t i = 0; i < data.size();  ++i)
	    std::cout << data[i];  				// จะพิมพ์ Hello World
}



// 1. std::vector<char>::assign() → ใช้กับ buffer (เช่น HTTP body)
// ใช้เมื่อต้องการเก็บข้อมูล binary-safe หรือ raw body ของ HTTP request
void ft1() {
    const char* 			buffer = "Hello, HTTP body!";
    std::vector<char> 		rawBody;
    rawBody.assign(buffer, buffer + strlen(buffer));  	// คัดลอกจาก C-style string
    // for (size_t i = 0; i < rawBody.size(); ++i)
    for (size_t i = 0; i < 9; ++i)
		std::cout << rawBody[i];  						// พิมพ์ Hello, HTTP body!

}




// 2. std::vector<int>::assign(n, value) → ใส่ค่าซ้ำ
// ใช้เมื่ออยากเริ่มต้น vector ด้วยค่าเดียวกันหลายตัว (เช่น reset)
void ft2() 
{
    std::vector<int> 		numbers;
    numbers.assign(5, 42);  // ใส่ 42 จำนวน 5 ตัว
    for (size_t i = 0; i < numbers.size(); ++i)
        std::cout << numbers[i] << " ";  // 42 42 42 42 42
}




// 3. std::vector<std::string>::assign(begin, end) → คัดลอกจากอีก container
// ใช้เมื่อย้ายข้อมูลระหว่าง container ต่างชนิด
void ft3() 
{
    std::list<std::string> 		names = { "Alice", "Bob", "Charlie" };
    std::vector<std::string> 	vec;
    vec.assign(names.begin(), names.end());  	// คัดลอกจาก list มาลง vector
    for (size_t i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";  			// Alice Bob Charlie

	std::cout << "\n";  			
    std::cout << vec[0] << "\n";  			// Alice 
    std::cout << vec[1] << "\n";  			// 		 Bob 
    std::cout << vec[2] << "\n";  			// 			 Charlie


}



// 4. std::string::assign(...) → ใช้แทนที่เนื้อหา string
// เหมาะกับการตัด string หรือ clone
void ft4() {
    std::string 	s;
    s.assign("webserv", 3);  // เอาแค่ 3 ตัวแรก
    std::cout << s << "\n\n";  // "web"

	// หรือจากอีก string:
	std::string 	full = "webserv project";
	std::string 	shortDesc;
	shortDesc.assign(full.begin(), full.begin() + 7); // "webserv"
	std::cout << shortDesc ;  // "web"
}




// สรุป: .assign() ใช้เมื่อ...
// ประเภท			รูปแบบ					 ความหมาย
// vector<T>		assign(n, value)		เติม value ซ้ำ n ตัว
// vector<T>		assign(first, last)		คัดลอกจาก iterator/pointer
// string			assign(cstr, n)			เอา n ตัวอักษรแรกจาก C-string
// string/vector	assign(begin, end)		คัดลอกช่วงข้อมูล



int main()
{
	ft0();	
	std::cout<<"\n\n";

	ft1();
	std::cout<<"\n\n";

	ft2();
	std::cout<<"\n\n";

	ft3();
	std::cout<<"\n\n	ft 4 = 2 line\n";

	ft4();
}