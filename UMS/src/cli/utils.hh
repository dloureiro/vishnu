#ifndef CLIUTILS_HH
#define CLIUTILS_HH

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"
#include "MachineProxy.h"
#include "LocalAccountProxy.h"
#include "ConfigurationProxy.h"

void printSession(UMS_Data::Session_ptr session);
void printLocalAccount(UMS_Data::LocalAccount_ptr account);
void printMachine(UMS_Data::Machine_ptr machine);
void printCommand(UMS_Data::Command_ptr command);
void printOptionValue(UMS_Data::OptionValue_ptr optionValue);
void printUser(UMS_Data::User_ptr user);

void print(UMS_Data::User_ptr user, int id);
void print(UMS_Data::Machine_ptr machine, int id);
void  print(UMS_Data::LocalAccount_ptr localAccount, int id);

template<typename T, unsigned short int N>
class requiredParameter{

	public:

		requiredParameter():allProvided(false),provided(0){}

		void notify(const T& val){// this parameter is not used by the member function but it is here for convenience

			provided< N ? provided++:allProvided=true;
		}

		bool given()const{

			return allProvided;
		}

		~requiredParameter(){}

	private:
		unsigned short int provided;
		
		bool allProvided;
};














#endif
