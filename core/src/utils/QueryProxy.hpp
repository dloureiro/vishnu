/**
 * \file QueryProxy.hpp
 * \brief This file contains the VISHNU QueryProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _QUERY_PROXY_H_
#define _QUERY_PROXY_H_

#include <string>
#include <iostream>

#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"

#include "SessionProxy.hpp"
#include "Session.hpp"
#include "ListCmdOptions.hpp"
#include "ListCommands.hpp"
#include "ListLocalAccOptions.hpp"
#include "ListLocalAccounts.hpp"
#include "ListMachineOptions.hpp"
#include "ListMachines.hpp"
#include "ListOptionsValues.hpp"
#include "ListOptOptions.hpp"
#include "ListSessionOptions.hpp"
#include "ListSessions.hpp"
#include "ListUsers.hpp"

/**
 * \class QueryProxy
 * \brief QueryProxy class implementation
 */
template <class QueryParameters, class ListObject>
class QueryProxy
{

  public:

    /**
     * \param params The object which encapsulates the information of queryProxy options
     * \param session The object which encapsulates the session information (ex: identifier of the session)
     * \param serviceName The name of the service to call
     * \brief Constructor, raises an exception on error
     */
    QueryProxy(const QueryParameters& params,
               const SessionProxy& session,
               const std::string& serviceName);
    /**
     * \param params The object which encapsulates the information of queryProxy options
     * \param session The object which encapsulates the session information (ex: identifier of the session)
     * \param serviceName The name of the service to call
     * \param machineId The identifier of the machine in which the query will be done
     * \brief Constructor, raises an exception on error
     */
    QueryProxy(const QueryParameters& params,
               const SessionProxy& session,
               const std::string& serviceName,
               const std::string& machineId);

    /**
     * \brief Function to change or reset the machineId
     * \param the id of the machine used by the query
     */
    void
    setMachineId(const std::string& machineId);

    /**
     * \brief Function to get the machineId used by the query
     * \return the id of the machine used by the query
     */
    std::string
    getMachineId() const;

    /**
     * \brief Function to list QueryProxy information
     * \fn  ListObject* list()
     * \return The pointer to the ListOject containing list information
     * \return raises an exception on error
     */
    ListObject*
    list();
    /**
     * \brief Function to list QueryProxy information where QueryParameters type is string
     * \fn  ListObject* listWithParamsString()
     * \return The pointer to the ListOject containing list information
     * \return raises an exception on error
     */
    ListObject*
    listWithParamsString();
    /**
     * \fn ~QueryProxy()
     * \brief Destructor, raises an exception on error
     */
    ~QueryProxy();

  private:
    /**
     * \brief Define a pointer type for more facilities
     */
    typedef ListObject* ListObject_ptr;
    /**
     * \brief Define a pointer type for more facilities
     */
    typedef QueryParameters* QueryParameters_ptr;

    /////////////////////////////////
    // Attributes
    /////////////////////////////////

    /**
     * \brief The QueryParameters information
     */
    QueryParameters mparameters;
    /**
     * \brief The name of the service
     */
    std::string mserviceName;
    /**
     * \brief The SessionProxy object containing the encrypted identifier of the session
     *  generated by VISHNU
     */
    SessionProxy msessionProxy;
    /**
     * \brief The Object containing query information
     */
    ListObject* mlistObject;
    /**
     * \brief The id of the machine used by the query
     */
    std::string mmachineId;
};

/**
<<<<<<< HEAD
 * \brief A Constructor of the QueryProxy template class
=======
>>>>>>> abe4bf42273b9f77629ba311e1c0cdf300bbf5c9
 * \param params The object which encapsulates the information of queryProxy options
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \param serviceName The name of the service to call
 * \brief Constructor, raises an exception on error
 */
template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::QueryProxy(const QueryParameters& params, const SessionProxy& session,
    const std::string& serviceName):
  mparameters(params), mserviceName(serviceName), msessionProxy(session)
{
  mlistObject = NULL;
  mmachineId = "";
}

/**
 * \brief Another constructor of the QueryProxy template class
 * \param params The object which encapsulates the information of queryProxy options
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \param serviceName The name of the service to call
 * \param machineId The identifier of the machine in which the query will be done
 * \brief Constructor, raises an exception on error
 */

template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::QueryProxy(const QueryParameters& params, const SessionProxy& session,
    const std::string& serviceName, const std::string& machineId):
  mparameters(params), mserviceName(serviceName), msessionProxy(session), mmachineId (machineId)
{
  mlistObject = NULL;
}


/**
* \brief Function to reset machineId
* \param machineId The identifier of the machine in which the query will be done
*/
template <class QueryParameters, class ListObject>
void
QueryProxy<QueryParameters, ListObject>::setMachineId(const std::string& machineId) {
  mmachineId = machineId;
}


 /**
* \brief Function to get the machineId used by the query
* \return the id of the machine used by the query
*/
template <class QueryParameters, class ListObject>
std::string
QueryProxy<QueryParameters, ListObject>::getMachineId() const {
  return mmachineId;
}

/**
 * \brief Function to list QueryProxy information
 * \fn  ListObject* QueryProxy<QueryParameters, ListObject>::list()
 * \return The pointer to the ListOject containing list information
 * \return raises an exception on error
 */
  template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::list()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string queryParmetersToString;
  char* listObjectInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  //If the query uses the machineId (machineId not null)
  if (mmachineId.size() != 0) {
    profile = diet_profile_alloc(mserviceName.c_str(), 2, 2, 4);
  }
  else {
    profile = diet_profile_alloc(mserviceName.c_str(), 1, 1, 3);
  }

  sessionKey = msessionProxy.getSessionKey();

  const char* name = mserviceName.c_str();
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the mparameters object in to queryParmetersToString
  queryParmetersToString =  _ser.serialize(const_cast<QueryParameters_ptr>(&mparameters));
  //IN Parameters

  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+sessionKey;
      raiseDietMsgException(msg);
    }

  //If the query uses the machineId (machineId not null)
  if (mmachineId.size() != 0) {

    if(diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+mmachineId;
      raiseDietMsgException(msg);
    }

    if(diet_string_set(diet_parameter(profile,2), strdup(queryParmetersToString.c_str()), DIET_VOLATILE)) {
      msg += "with queryParmetersToString parameter "+queryParmetersToString;
      raiseDietMsgException(msg);
    }

    //OUT Parameters
    diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,3), &listObjectInString, NULL)){
        msg += "by receiving listObjectInString message";
        raiseDietMsgException(msg);
      }
      if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
        msg += "by receiving errorInfo message";
        raiseDietMsgException(msg);
      }
    }
    else {
      raiseDietMsgException("DIET call failure");
    }
  }
  else {

    if(diet_string_set(diet_parameter(profile,1), strdup(queryParmetersToString.c_str()), DIET_VOLATILE)) {
      msg += "with queryParmetersToString parameter "+queryParmetersToString;
      raiseDietMsgException(msg);
    }

    //OUT Parameters
    diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,2), &listObjectInString, NULL)){
        msg += "by receiving listObjectInString message";
        raiseDietMsgException(msg);
      }
      if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
        msg += "by receiving errorInfo message";
        raiseDietMsgException(msg);
      }
    }
    else {
      raiseDietMsgException("DIET call failure");
    }
  }


  /*To check the receiving message error*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  //To parse List object serialized
  parseEmfObject(std::string(listObjectInString), mlistObject, "Error by receiving List object serialized");

  return mlistObject;
}

/**
 * \brief Function to list QueryProxy information where QueryParameters type is string
 * \fn  ListObject* QueryProxy<QueryParameters, ListObject>::listWithParamsString()
 * \return The pointer to the ListOject containing list information
 * \return raises an exception on error
 */
  template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::listWithParamsString()
{
  diet_profile_t* profile = NULL;
  char* listObjectInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  if (mmachineId.size() != 0) {
    profile = diet_profile_alloc(mserviceName.c_str(), 2, 2, 4);
  } else {
    profile = diet_profile_alloc(mserviceName.c_str(), 1, 1, 3);
  }

   //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy.getSessionKey()).c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+msessionProxy.getSessionKey();
    raiseDietMsgException(msg);
  }

   //If the query uses the machineId (machineId not null)
  if (mmachineId.size() != 0) {
    //IN Parameters
    if(diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+mmachineId;
      raiseDietMsgException(msg);
    }

    if(diet_string_set(diet_parameter(profile,2), strdup(mparameters.c_str()), DIET_VOLATILE)) {
      msg += "with with mparameters parameter "+mparameters;
      raiseDietMsgException(msg);
    }

    //OUT Parameters
    diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,3), &listObjectInString, NULL)){
        msg += "by receiving listObjectInString message";
        raiseDietMsgException(msg);
      }
      if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
        msg += "by receiving errorInfo message";
        raiseDietMsgException(msg);
      }
    }
    else {
      raiseDietMsgException("DIET call failure");
    }
  } else {
 
  //IN Parameters
  if(diet_string_set(diet_parameter(profile,1), strdup(mparameters.c_str()), DIET_VOLATILE)) {
    msg += "with mparameters parameter "+mparameters;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &listObjectInString, NULL)){
      msg += "by receiving listObjectInString message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }
  }

  /*To check the receiving message error*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  //To parse ListUsers object serialized
  parseEmfObject(std::string(listObjectInString), mlistObject, "Error by receiving ListUsers object serialized");

  return mlistObject;

}

/**
 * \brief A default destructor for QueryProxy
 * \brief Destructor, raises an exception on error
 */
  template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::~QueryProxy()
{
}

#endif
