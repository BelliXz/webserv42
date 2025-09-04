
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    bool    round_of_existences = true;
    bool    enlightenment = false;
    int     learn;
	pid_t	soul;
    // ...     meeting;

    soul = fork();
    // if (soul == 0){
    if (soul){
        while (round_of_existences){
            learn++;
            printf("1_All living beings are subject to karma\n");
            // if (enlightenment)
                break ;
        }
    }
    else{
        while (round_of_existences){
            learn--;
            printf("2_All living beings are subject to karma\n");
            // if (enlightenment)
                break ;
        }
    }
    // if (meeting)
    //     ...
}



// int main
// 	soul=fork
// 	if soul
// 		while round_existence
// 			learn++
// 			print All living being are subject karma
// 		if enlightenment
// 			break
// 	else
// 		while round_existence
// 			.
// 	if meeting
// 		.


bool	ConnectionManager::read(int clientSocket)
{
	std::cout  << 	"events[i].data.fd : clientSocket :" << clientSocket  << "\n";


	Connection *conn;

	std::map<int,Connection>::iterator it = connections.find(clientSocket);
	if(it != connections.end())
	{
		std::cout << "112===============\n";
		conn =  &(it->second);	
	}
	else
	{
		std::cout << "117===============\n";
		conn = NULL;
	}



}




------WebKitFormBoundaryg8KFNmEp5J5bDora