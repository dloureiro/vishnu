%module VISHNU_UMS

%{
#define SWIG_FILE_WITH_INIT
#include "UMS_Data.hpp"
#include "api_ums.hpp"
%}

// this includes the typemaps for STL strings
%include "std_string.i"

// this includes the required type declarations for EMF4CPP
// WARNING: some may be missing!
%include "ecore/EObject.hpp"
%include "ecorecpp/mapping/type_traits.hpp"
%include "ecorecpp/mapping/out_ptr.hpp"
%include "ecorecpp/mapping/EList.hpp"
%include "ecore_forward.hpp"

// All EMF includes (same as in UMS_Data.hpp)
%include "UMS_Data/Command.hpp"
%include "UMS_Data/ConnectOptions.hpp"
%include "UMS_Data/Session.hpp"
%include "UMS_Data/ListSessions.hpp"
%include "UMS_Data/ListSessionOptions.hpp"
%include "UMS_Data/User.hpp"
%include "UMS_Data/ListUsers.hpp"
%include "UMS_Data/Command.hpp"
%include "UMS_Data/ListCommands.hpp"
%include "UMS_Data/ListCmdOptions.hpp"
%include "UMS_Data/LocalAccount.hpp"
%include "UMS_Data/ListLocalAccounts.hpp"
%include "UMS_Data/ListLocalAccOptions.hpp"
%include "UMS_Data/Machine.hpp"
%include "UMS_Data/ListMachines.hpp"
%include "UMS_Data/ListMachineOptions.hpp"
%include "UMS_Data/Configuration.hpp"
%include "UMS_Data/OptionValue.hpp"
%include "UMS_Data/ListOptionsValues.hpp"
%include "UMS_Data/ListOptOptions.hpp"

// TODO make copy of object when using the push_back() method of EList
// because this causes a segfault due to double free

#ifdef SWIGPYTHON

// Instantiate the template for all lists
// the templates used within the list template must be instantiated first
%template(EListPtr) ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList< ::ecore::EObject > >;
%template(ESessionList) ::ecorecpp::mapping::EList<::UMS_Data::Session>;

// Remove output parameters
%typemap(in, numinputs=0) std::string& sessionKey(std::string temp) {
  $1 = &temp;
}

// Handle all returned strings
%typemap(argout) std::string& sessionKey {
  PyObject *o = PyString_FromString($1->c_str());
  $result = SWIG_Python_AppendOutput($result, o);
}

// Handle VISHNU exceptions
%exception {
  try {
    $function
  } catch (UMSVishnuException) {
    PyErr_SetString(PyExc_SystemError,"**** UMS EXCEPTION CATCHED! :) ****");
    return NULL;
  }
}

#endif

#ifdef SWIGJAVA
%include "various.i"
#endif

%include "api_ums.hpp"

