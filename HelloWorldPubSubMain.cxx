// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file HelloWorldPubSubMain.cpp
 * This file acts as a main entry point to the application.
 *
 * This file was generated by the tool fastddsgen.
 */


#include "HelloWorldDriver.h"
#include "HelloWorldServer.h"


int main(int argc, char** argv) {
    int type = 0;
	unsigned message_size = 0;
	unsigned message_rate = 1; // Default 1 Hz
	for (int i = 0; i < argc; ++i) {
		if(!strncmp(argv[i], "-rate",5)){
			if(i < argc-1){
				message_rate = atoi(argv[i+1]);
			}else{
				printf("ERROR: RATE NOT PROVIDED\n");
				return -1;
			}
		}else if(!strncmp(argv[i], "-size",5)){
			if(i < argc-1){
				message_size = atoi(argv[i+1]);
			}else{
				printf("ERROR: SIZE NOT PROVIDED\n");
				return -1;
			}
        }else if (strcmp(argv[i], "-driver") == 0){
            type = 1;
        }else if (strcmp(argv[i], "-server") == 0){
            type = 2;
        }
    }

    if ((type == 0) || (message_size == 0)){
        std::cout << "Error: Incorrect arguments." << std::endl;
        std::cout << "Usage: " << std::endl << std::endl;
        std::cout << argv[0] << "-rate R -size S -driver|-server" << std::endl << std::endl;
        return 0;
    }

    std::cout << "Starting " << std::endl;

	// Actually use size and rate params
    switch (type)
    {
        case 1:
        {
            HelloWorldDriver mypub;
            if (mypub.init(message_size, message_rate)) {
                mypub.run(message_size, message_rate);
            }
            break;
        }
        case 2:
        {
            HelloWorldServer mysub;
            if (mysub.init()) {
                mysub.run();
            }
            break;
        }
    }

    return 0;
}
