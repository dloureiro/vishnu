/**
 * \file slave.cpp
 * \brief This file contains the VISHNU TMS slave main function.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"

#include "DIET_client.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "VishnuException.hpp"
#include "BatchFactory.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"
#include "Env.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac
using namespace std;
using namespace vishnu;


/**
 * \brief To show how to use the slave
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
void usage(char* cmd) {
  cerr << "Usage: " << cmd << "COMMANDE_TYPE[SUBMIT] <BatchType> <BatchVersion>"
       << " <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>"
       << " <SubmitOptionsSerializedPath> <job_script_path>\n"
       << "\t\t\t\t\t" << " or\n"
       << "Usage: " << cmd << "COMMANDE_TYPE[CANCEL] <BatchType> <BatchVersion>"
       << " <JobSerializedPath> <SlaveErrorPath>\n";
  exit(EXIT_FAILURE);
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int
main(int argc, char* argv[], char* envp[]) {

  std::string action;
  char* jobSerializedPath = NULL;
  char* optionsPath = NULL;
  char* slaveJobFile = NULL;
  char* slaveErrorPath = NULL;
  char* jobScriptPath = NULL;
  BatchType batchType;
  std::string batchVersion;

  if(argc < 6) { // Too few arguments
    usage(argv[0]);
  }
  action = std::string(argv[1]);
  batchType = vishnu::convertToBatchType(argv[2]);
  batchVersion = argv[3];
  jobSerializedPath = argv[4];
  slaveErrorPath = argv[5];

  if(action.compare("SUBMIT")==0) {
    if(argc < 9) {
      // Too few arguments
      usage(argv[0]);
    }
    // Get batchtype
    slaveJobFile = argv[6];
    optionsPath = argv[7];
    jobScriptPath = argv[8];
  } else if(action.compare("CANCEL")!=0) {
    usage(argv[0]);
  }

  if(batchType == UNDEFINED) {
    std::string msg = "Slave: Invalid batch. Batch type must be TORQUE, LOADLEVLER, SLURM, LSF, SGE, PBSPRO, DELTACLOUD or POSIX\n";
    std::cerr << msg;
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, msg);
  }

  TMS_Data::Job_ptr job = NULL;
  TMS_Data::SubmitOptions_ptr submitOptions = NULL;
  BatchServer* batchServer;

  try {

    //To create batchServer Factory
    BatchFactory factory;
    batchServer = factory.getBatchServerInstance(batchType, batchVersion);
    if(batchServer==NULL) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: getBatchServerInstance return NULL instance");
    }
    std::string jobSerialized = vishnu::get_file_content(jobSerializedPath);
    if(!parseEmfObject(std::string(jobSerialized), job)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: job object is not well built");
    }

    if(action.compare("SUBMIT")==0) {
      std::string options  = vishnu::get_file_content(optionsPath);
      if(!parseEmfObject(std::string(options), submitOptions)) {
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, "slave: SubmitOptions object is not well built");
      }

      //Submits the job
      if(batchServer->submit(jobScriptPath, *submitOptions, *job)==0){;
        //To serialize the job object
        ::ecorecpp::serializer::serializer _ser;
        std::string slaveJob = _ser.serialize_str(job);

        std::ofstream os_slaveJobFile(slaveJobFile);
        os_slaveJobFile << slaveJob;
        os_slaveJobFile.close();
      }
    } else if(action.compare("CANCEL")==0) {
      //To cancel the job
      std::string jobdDescr = job->getJobId();
      if(batchType == DELTACLOUD) {
        jobdDescr += "@"+job->getVmId();
      }
      batchServer->cancel(jobdDescr.c_str());
    }
  } catch (VishnuException& ve) {
    std::string errorInfo =  ve.buildExceptionString();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
  } catch (std::exception& e) {
    std::string errorInfo = e.what();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
  }

  delete job;
  delete submitOptions;
  delete batchServer;

  return 0;
}
