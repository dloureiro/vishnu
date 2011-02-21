/**
 * \file LocalAccountProxy.hpp
 * \brief This file contains the VISHNU LocalAccountProxy class.
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#ifndef _LOCAL_ACCOUNT_PROXY_H_
#define _LOCAL_ACCOUNT_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.hpp"
#include "LocalAccount.hpp"

/**
 * \class LocalAccountProxy 
 * \brief LocalAccountProxy class implementation 
 */
class LocalAccountProxy
{
 
public:
  
  /**
   * \fn LocalAccountProxy(const UMS_Data::LocalAccount& localAccount,
   *                       const SessionProxy& session)
   * \param localAccount The object which encapsulates the user Acccount information 
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  LocalAccountProxy(const UMS_Data::LocalAccount& localAccount, const SessionProxy& session);
  /**
   * \brief Function to add a new local user configuration 
   * \fn  int add()
   * \return raises an exception on error
   */
  int add();
  /**
   * \brief Function to update a new local user configuration 
   * \fn  int update()
   * \return raises an exception on error
   */  
  int update();
  /**
   * \brief Function to removes a local user configuration (for a given user on a given machine) from VISHNU 
   * \fn  int deleteLocalAccount()
   * \return raises an exception on error
   */
  int deleteLocalAccount();
  /**
   * \brief Function get SessionProxy object which contains the VISHNU session identifier 
   * \fn SessionProxy getSessionProxy() 
   * \return a SessionProy object which contains the VISHNU session information 
   * \return raises an exception on error
   */
  SessionProxy getSessionProxy() const;
  /**
   * \brief Function get the VISHNU ssh public key  
   * \fn std::string getSshPublicKey() const 
   * \return the VISHNU ssh public Key 
   * \return raises an exception on error
   */
  std::string getSshPublicKey() const;
  /**
   * \fn ~LocalAccountProxy()
   * \brief Destructor, raises an exception on error
   */
  ~LocalAccountProxy();

private:

  /**
   * \brief Function to combine add() and update() into one function 
   * \fn  int _addLocalAccountInformation(bool isNewMachine=true); 
   * \param isNewLocalAccount to select the call of add or update function 
   * \return raises an exception on error
   */
  int _addLocalAccountInformation(bool isNewLocalAccount=true);

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

 /**
  * \brief The object which encapsulates the user Acccount information 
  */ 
  UMS_Data::LocalAccount mlocalAccount;
 /**
  * \brief The SessionProxy object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionProxy msessionProxy;
  /**
   * The VISHNU ssh public Key
   */
  std::string  msshPublicKey;
};
#endif
