/**
 * \file ConfigurationServer.cpp
 * \brief This file implements the Class which manipulates VISHNU configuration data on server side.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2011
 */

#include "ConfigurationServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "DbFactory.hpp"

/**
 * \brief Constructor
 * \fn ConfigurationServer(SessionServer session)
 * \param session The object which encapsulates session data
 */
ConfigurationServer::ConfigurationServer(SessionServer session):msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  mconfiguration = NULL;

}
/**
 * \brief Constructor
 * \fn ConfigurationServer(Configuration*& configuration, SessionServer session);
 * \param configuration The Configuration data structure
 * \param session The object which encapsulates session data
 */
ConfigurationServer::ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session)
:mconfiguration(configuration), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to save a VISHNU configuration
 * \fn int save()
 * \return raises an exception on error
 */
int
ConfigurationServer::save() {

  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users "
  "where not userid='"+ROOTUSERNAME+"'";

  std::string sqlListofMachines = "SELECT machineid, name, site, status, sshpublickey, lang, description from machine, description "
  "where machine.nummachineid = description.machine_nummachineid";

  std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home "
  "from account, machine, users where account.machine_nummachineid=machine.nummachineid and "
  "account.users_numuserid=users.numuserid";

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mconfiguration= ecoreFactory->createConfiguration();

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      //To get the list of users from the database
      boost::scoped_ptr<DatabaseResult> ListofUsers (mdatabaseVishnu->getResult(sqlListofUsers.c_str()));

      if (ListofUsers->getNbTuples() != 0){
        for (size_t i = 0; i < ListofUsers->getNbTuples(); ++i) {
          results.clear();
          results = ListofUsers->get(i);
          ii = results.begin();

          UMS_Data::User_ptr user = ecoreFactory->createUser();
          user->setUserId(*ii);
          user->setPassword(*(++ii));
          user->setFirstname(*(++ii));
          user->setLastname(*(++ii));
          user->setPrivilege(convertToInt(*(++ii)));
          user->setEmail(*(++ii));
          user->setStatus(convertToInt(*(++ii)));
          mconfiguration->getListConfUsers().push_back(user);
        }
      }

      //To get the list of machines from the database
      boost::scoped_ptr<DatabaseResult> ListofMachines (mdatabaseVishnu->getResult(sqlListofMachines.c_str()));

      if (ListofMachines->getNbTuples() != 0){
        for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
          results.clear();
          results = ListofMachines->get(i);
          ii = results.begin();

          UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
          machine->setMachineId(*ii);
          machine->setName(*(++ii));
          machine->setSite(*(++ii));
          machine->setStatus(convertToInt(*(++ii)));
          machine->setSshPublicKey(*(++ii));
          machine->setLanguage(*(++ii));
          machine->setMachineDescription(*(++ii));
          mconfiguration->getListConfMachines().push_back(machine);
        }
      }

      //To get the list of local accounts from the database
      boost::scoped_ptr<DatabaseResult> ListofLocalAccount (mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str()));

      if (ListofLocalAccount->getNbTuples() != 0){
        for (size_t i = 0; i < ListofLocalAccount->getNbTuples(); ++i) {
          results.clear();
          results = ListofLocalAccount->get(i);
          ii = results.begin();

          UMS_Data::LocalAccount_ptr localAccount = ecoreFactory->createLocalAccount();
          localAccount->setMachineId(*ii);
          localAccount->setUserId(*(++ii));
          localAccount->setAcLogin(*(++ii));
          localAccount->setSshKeyPath(*(++ii));
          localAccount->setHomeDirectory(*(++ii));
          mconfiguration->getListConfLocalAccounts().push_back(localAccount);
        }
      }
    } //End if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End //if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}

/**
 * \brief Function to restore a VISHNU configuration
 * \fn int restore(int vishnuId)
 * \param vishnuId The identifier of the vishnu instance
 * \return raises an exception on error
 */
int ConfigurationServer::restore(int vishnuId) {
  std::string sqlcode = "";
  std::string sqlCodeDescMachine = "";

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the userid is the super vishnu admin userid
  if (userServer.getData().getUserId().compare(ROOTUSERNAME) == 0) {
    //if the user exists
    if (userServer.exist()) {

      mdatabaseVishnu->process("DELETE FROM users where not userid='"+ROOTUSERNAME+"';"
      "DELETE FROM machine; DELETE FROM account;");

      //To get all users
      for(unsigned int i = 0; i < mconfiguration->getListConfUsers().size(); i++) {
        UMS_Data::User_ptr user = mconfiguration->getListConfUsers().get(i);
        //userServer.add(user);
        sqlcode.append(userToSql(user, vishnuId));
      }

      //To get all machines
      for(unsigned int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
        sqlcode.append(machineToSql(machine, vishnuId));
      }

      //To insert machines and users
      mdatabaseVishnu->process(sqlcode.c_str());

      //To get machines description
      for(unsigned int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
        sqlCodeDescMachine.append(machineDescToSql(machine));
      }

      //To insert machines description
      mdatabaseVishnu->process(sqlCodeDescMachine.c_str());

      //To insert localAccount
      for(unsigned int i = 0; i < mconfiguration->getListConfLocalAccounts().size(); i++) {
        UMS_Data::LocalAccount_ptr localAccount = mconfiguration->getListConfLocalAccounts().get(i);
        LocalAccountServer localAccountServer = LocalAccountServer (localAccount, msessionServer);
        localAccountServer.add();
      }
    } //End if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
  }//End if the userid is the super vishnu admin userid
  else {
    UMSVishnuException e (ERRCODE_ROOT_USER_ONLY);
    throw e;
  }
  return 0;
}

/**
 * \fn ~ConfigurationServer()
 * \brief Destructor
*/
ConfigurationServer::~ConfigurationServer() {
  delete mconfiguration;
}
/**
 * \brief Function to get Configuration data structure
 * \fn UMS_Data::Configuration*  getData()
 * \return  The LocalAccount data structure
 */
UMS_Data::Configuration*
ConfigurationServer::getData() {
  return mconfiguration;
}

/**
 * \brief Function to get the sql code of users from a VISHNU configuration
 * \fn std::string userToSql(UMS_Data::User_ptr user, int vishnuId)
 * \param vishnuId The identifier of the vishnu instance
 * \return the sql code containing the sql code of users
 */
std::string
ConfigurationServer::userToSql(UMS_Data::User_ptr user, int vishnuId) {

  std::string sqlInsert = "insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,"
 " privilege, email, passwordstate, status) values ";

  return (sqlInsert + "(" + convertToString(vishnuId) +", "
  "'"+user->getUserId()+"','"+user->getPassword()+"','"
  + user->getFirstname()+"','"+user->getLastname()+"',"+
  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', "
  "1, "+convertToString(user->getStatus())+");");
}

/**
 * \brief Function to get the sql code of machines from a VISHNU configuration
 * \fn std::string machineToSql(UMS_Data::Machine_ptr machine, int vishnuId)
 * \param machine The machine object
 * \param vishnuId The identifier of the vishnu instance
 * \return the sql code containing the sql code of machines
 */
std::string
ConfigurationServer::machineToSql(UMS_Data::Machine_ptr machine, int vishnuId) {
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid, status, sshpublickey) values ";

  sqlInsert.append("("+convertToString(vishnuId)+",'"+machine->getName()+"'"
  ",'"+ machine->getSite()+"','"+machine->getMachineId()+"',"
  +convertToString(machine->getStatus())+",'"+machine->getSshPublicKey() +"');");

  return sqlInsert;
}

/**
 * \brief Function to get the sql code of machines description from a VISHNU configuration
 * \fn std::string machineDescToSql(UMS_Data::Machine_ptr machine)
 * \return the sql code containing the sql code of machines description
 */
std::string
ConfigurationServer::machineDescToSql(UMS_Data::Machine_ptr machine) {

  UMS_Data::Machine* machinetmp = new UMS_Data::Machine();
  MachineServer machineServer = MachineServer(machinetmp);
  std::string res;
  res = "insert into description (machine_nummachineid, lang, "
  "description) values "
  "("+machineServer.getAttribut("where machineid='"+machine->getMachineId()+"';")+","
  "'"+ machine->getLanguage()+"','"+machine->getMachineDescription()+"');";

  delete machinetmp;
  return res;
}