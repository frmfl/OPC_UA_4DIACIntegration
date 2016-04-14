/*
 * opcuahandler.cpp
 *
 *  Created on: 02.03.2016
 *      Author: florian
 */

#include "opcuahandler.h"
#include <string.h>
#include <cstdbool>



DEFINE_SINGLETON(COPC_UA_Handler);

UA_ServerConfig COPC_UA_Handler::configureUAServer() {
	m_server_config = UA_ServerConfig_standard;
	m_server_config.enableUsernamePasswordLogin = false;
	m_server_config.networkLayersSize = 1;
	m_server_config.logger = Logger_Stdout;
	m_server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
	m_server_config.networkLayers = &m_server_networklayer;
	return m_server_config;
}

UA_ClientConfig COPC_UA_Handler::configureUAClient(){
	m_client_config = UA_ClientConfig_standard;
	return m_client_config;
}

void COPC_UA_Handler::createUAClient(UA_ClientConfig m_client_config){
	mOPCUAClient = UA_Client_new(m_client_config, Logger_Stdout);
}

void COPC_UA_Handler::createUAServer(UA_ServerConfig m_server_config){
	mOPCUAServer = UA_Server_new(m_server_config);	// create the server specified in config
}

COPC_UA_Handler::COPC_UA_Handler() : m_server_config(), m_server_networklayer(), mbServerRunning(0) {
	configureUAServer(); 	// configure a standard server
	createUAServer(m_server_config);	// create a OPC_UA Server with specified configuration

	// OPTION: add a namespace in xml format to the server containing the application configuration.
	//UA_Server_addExternalNamespace()
}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopUAServer();
	UA_Server_delete(mOPCUAServer);
	m_server_networklayer.deleteMembers(&m_server_networklayer);
	destroyUAClient(getClient());
}

void COPC_UA_Handler::run(){
	UA_StatusCode retVal = UA_Server_run(mOPCUAServer, mbServerRunning);	// server keeps iterating as long as running is true;
	COPC_UA_Handler::runUAServer();
	DEVLOG_INFO("UA_Server run status code %s", retVal);
}

void COPC_UA_Handler::enableHandler(void){
	start();
}

void COPC_UA_Handler::disableHandler(void){
	COPC_UA_Handler::stopUAServer();
	end();
}

void COPC_UA_Handler::setPriority(int){
	//currently we are doing nothing here.
	//TODO We should adjust the thread priority.
}

int COPC_UA_Handler::getPriority(void) const{
	//the same as for setPriority
	return 0;
}

UA_Server * COPC_UA_Handler::getServer(){
	return mOPCUAServer;
}

UA_Client * COPC_UA_Handler::getClient(){
	return mOPCUAClient;
}


void COPC_UA_Handler::runUAServer(){
	*mbServerRunning = UA_Boolean (true);
	//	char mylogmsg = "Value of boolean 'running' ";
	//	logMessage(E_INFO, &mylogmsg, (char*) mbServerRunning);

	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, nThreads, mbServerRunning);
	//FIXME add forward declaration of enum to obtain server status information
}

void COPC_UA_Handler::stopUAServer(){
	*mbServerRunning = UA_Boolean (false);
	//	char mylogmsg = "Value of boolean 'stop' ";
	//	logMessage(E_INFO, &mylogmsg, (char*) mbServerRunning);
	//nThreads = 1;
	//UA_StatusCode statCode;
	//UA_StatusCode retval = UA_Server_run(mOPCUAServer, mbServerRunning);
	//return statCode;
	//FIXME add forward declaration of enum to obtain server status information
}

UA_StatusCode COPC_UA_Handler::connectUAClient(UA_Client* client){
	UA_StatusCode retVal;
	retVal = UA_Client_connect(client, UA_ClientConnectionTCP, "opc.tcp://localhost:16664");
	return retVal;
}

void COPC_UA_Handler::destroyUAClient(UA_Client *client){
	UA_Client_disconnect(client);
	UA_Client_delete(client);
}

void COPC_UA_Handler::registerNode(){

	/*
	char *fb_name;
	strcpy(fb_name, NodeAttr->fb_name);

	char *var_name;
	strcpy(var_name, NodeAttr->port_name);

	char *var_id;
	strcpy(var_id, NodeAttr->port_id);

	UA_Client *myClient = getClient();


	char node_id_string[64];
	int node_found = 0;


	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); //browse objects folder
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; //return everything

	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

	//createObjectNode
	//nodetype depends on the return value of the client. If parent node already exists, then construct
	//variable node. If variable node exists, do nothing.
	/*switch (nodetype)
	{
	case "object":
	   Aktion1
	   break;
	case "variable":
	   Aktion2
	   break;

	default:
	   Aktion4
	}
	 */
	/*

	UA_Server_addVariableNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
			const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
			const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
			const UA_VariableAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)

					UA_Server_addObjectNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
							const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
							const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
							const UA_ObjectAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)

	UA_VariableAttributes attr;
	UA_VariableAttributes_init(&attr);
	UA_Int32 myInteger = 42;
	UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
	attr.description = UA_LOCALIZEDTEXT("en_US","the answer");
	attr.displayName = UA_LOCALIZEDTEXT("en_US","the answer");


	UA_NodeId objNodeId = UA_NODEID_STRING(1, "system.node");
	UA_NodeId_init(&objNodeId);
	UA_QualifiedName



	//createVariableNode
	 */
}



/*
 * Get Function Block Node Id from the pointer to a CFunctionBlock.
 * Method is used to check if a not to the pointed function block already
 * exists in the address space of the OPC-UA Server.
 */

UA_StatusCode COPC_UA_Handler::getFBNodeId(const CFunctionBlock* pCFB, UA_NodeId* returnFBNodeId){
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;
	const char* FBInstanceName = pCFB->getInstanceName();	// Name of the SourcePoint function block
	UA_NodeId FBNodeId = UA_NODEID_STRING_ALLOC(1, FBInstanceName);		// Create new FBNodeId from c string

	UA_NodeId* returnNodeId = UA_NodeId_new();
	retVal = UA_Server_readNodeId(mOPCUAServer, FBNodeId, returnNodeId);		// read node of given ID
	if(retVal != UA_STATUSCODE_GOOD){
		return retVal;		// reading not successful
	}else{
		retVal = UA_NodeId_copy(returnNodeId, returnFBNodeId);	// reading successful, return NodeId
	};
	return retVal;


	// Careful container!
	/*CStringDictionary::TStringId sourceFBTypeNameId = sourceFB->getFBTypeId();
						const char * sourceFBTypeName = CStringDictionary::getInstance().get(sourceFBTypeNameId);
						//FIXME Retrieve System name

						CStringDictionary::TStringId sourceFBNameId = sourceFB->getInstanceNameId();
						const char* sourceFBName = sourceFB->getInstanceName();

						SFBInterfaceSpec* sourceFBInterface = sourceFB->getFBInterfaceSpec();

						CStringDictionary::TStringId sourceRDNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
						const char * sourceRDName = CStringDictionary::getInstance().get(sourceRDNameId);

						CStringDictionary::TStringId sourceRDTypeNameId = sourceFBInterface->m_aunDODataTypeNames[sourceRD.mPortId];
						const char * sourceRDTypeName = CStringDictionary::getInstance().get(sourceRDTypeNameId);

						char message[128];
						sprintf(message,"%s %s %s\n", sourceFBName, sourceFBTypeName, sourceRDName, sourceRDTypeName);
						DEVLOG_INFO(message);

						char *fb_name;
						strcpy(fb_name, sourceFBName);

						char *fb_typename;
						strcpy(fb_typename, sourceFBTypeName);

						char *var_name;
						strcpy(var_name, sourceRDName);

						char *var_typename;
						strcpy(var_typename, sourceRDTypeName);

						char varBrowse_id[64];
						sprintf(varBrowse_id, "%s.%s", fb_name, var_name);

						COPC_UA_Handler::getInstance().NodeAttr.fb_name = fb_name;
						COPC_UA_Handler::getInstance().NodeAttr.fb_typename = fb_typename;
						COPC_UA_Handler::getInstance().NodeAttr.port_name = var_name;
						COPC_UA_Handler::getInstance().NodeAttr.port_typename = var_typename;
	 */

}


UA_StatusCode COPC_UA_Handler::getSPNodeId(const CFunctionBlock *pCFB, SConnectionPoint& sourceRD, UA_NodeId* returnSPNodeId){
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;

	// Important Comment: Reading the node without reference to parent node id, unknown if this works.
	const SFBInterfaceSpec* sourceFBInterface = pCFB->getFBInterfaceSpec();

	CStringDictionary::TStringId SPNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
	const char * SPName = CStringDictionary::getInstance().get(SPNameId);
	UA_NodeId SPNodeId = UA_NODEID_STRING_ALLOC(1, SPName);

	UA_NodeId* returnNodeId = UA_NodeId_new();
	retVal = UA_Server_readNodeId(mOPCUAServer,SPNodeId, returnNodeId);		// read node of given ID
	if(retVal != UA_STATUSCODE_GOOD){
		return retVal;		// reading not successful
	}else{
		retVal = UA_NodeId_copy(returnNodeId, returnSPNodeId);	// reading successful, return NodeId
	};
	return retVal;
}


/* Function creates an address space obeject node defined by given pointer to
 * a control function block. If creation successful the NodeId is returned otherwise
 * UA_StatusCode from node creation with error message.
 */
UA_StatusCode COPC_UA_Handler::createUAObjNode(const CFunctionBlock* pCFB, UA_NodeId * returnObjNodeId){

	UA_StatusCode retVal = UA_STATUSCODE_GOOD;
	CStringDictionary::TStringId sourceFBNameId = pCFB->getInstanceNameId();
	const char* FBInstanceName = CStringDictionary::getInstance().get(sourceFBNameId); 		// Name of the SourcePoint function block

	UA_NodeId FBNodeId = UA_NODEID_STRING_ALLOC(1, FBInstanceName);		// Create new FBNodeId from c string
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_QualifiedName objBrowseName = UA_QUALIFIEDNAME_ALLOC(0, FBInstanceName);
	UA_NodeId objTypeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);

	UA_ObjectAttributes obj_attr;
	UA_ObjectAttributes_init(&obj_attr);
	char dispName[32];
	sprintf(dispName, "FB-%s\n", FBInstanceName);
	obj_attr.displayName = UA_LOCALIZEDTEXT("en_US", dispName);
	char descpName[64];
	sprintf(descpName, "Object node of FB-%s, origin: Publisher\n", FBInstanceName);
	//sprintf(descpName, "Publisher FB");
	obj_attr.description =  UA_LOCALIZEDTEXT("en_US", descpName);
	UA_NodeId * returnNodeId = UA_NodeId_new();
	retVal = UA_Server_addObjectNode(mOPCUAServer, FBNodeId, parentNodeId, parentReferenceTypeId, objBrowseName, objTypeDefinition, obj_attr, NULL, returnNodeId);

	if(retVal == UA_STATUSCODE_GOOD ){
		DEVLOG_INFO("Created new object %s \n", dispName);
		retVal = UA_NodeId_copy(returnNodeId, returnObjNodeId);
	}else{
		DEVLOG_INFO("Error creating object %s: %x\n", dispName, retVal);
		return retVal;

	}
	return retVal;
}


UA_StatusCode COPC_UA_Handler::createUAVarNode(const CFunctionBlock* pCFB, SConnectionPoint& sourceRD, UA_NodeId * returnVarNodeId){

	/*
			UA_Server_addVariableNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
					const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
					const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
					const UA_VariableAttributes attr, UA_InstantiationCallback *instantiationCallback, UA_NodeId *outNewNodeId)
	 */


	CStringDictionary::TStringId sourceFBNameId = pCFB->getInstanceNameId();
	const char* FBInstanceName = CStringDictionary::getInstance().get(sourceFBNameId);


	const SFBInterfaceSpec* sourceFBInterface = pCFB->getFBInterfaceSpec();

	CStringDictionary::TStringId SPNameId = sourceFBInterface->m_aunDONames[sourceRD.mPortId];
	const char * SPName = CStringDictionary::getInstance().get(SPNameId);

	char browse[32];
	sprintf(browse, "%s.%s\n", FBInstanceName, SPName);

	UA_NodeId newVarNodeId = UA_NODEID_STRING_ALLOC(1,browse);
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	char browsename[32];
	sprintf(browsename, "%s\n", SPName);
	UA_QualifiedName varBrowseName = UA_QUALIFIEDNAME(1, browse);
	UA_NodeId varType = UA_NODEID_NULL;

	UA_UInt32 myInteger = 42;

	UA_VariableAttributes var_attr;
	UA_VariableAttributes_init(&var_attr);
	char display[32];
	sprintf(display, "Variable-%s\n", SPName);
	var_attr.displayName = UA_LOCALIZEDTEXT("en_US", display);
	var_attr.description = UA_LOCALIZEDTEXT("en_US", "Variable Node");
	UA_Variant_setScalar(&var_attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);

	UA_StatusCode retVal = UA_Server_addVariableNode(
			mOPCUAServer,
			newVarNodeId,
			parentNodeId,
			parentReferenceTypeId,
			varBrowseName,
			UA_NODEID_NULL, // no variable type
			var_attr, NULL, returnVarNodeId);

	if(retVal == UA_STATUSCODE_GOOD )
		printf("Created new object %s \n", browse);
	else
		printf("Error creating object : %x\n", retVal);

	return retVal;
}


/*
UA_Boolean write_value = 0;

int write_type = UA_NS0ID_BOOLEAN;
//int write_type = UA_NS0ID_SBYTE;
//int write_type = UA_NS0ID_BYTE;
//int write_type = UA_NS0ID_INT16;
//int write_type = UA_NS0ID_UINT16;
//int write_type = UA_NS0ID_INT32;
//int write_type = UA_NS0ID_UINT32;
//int write_type = UA_NS0ID_INT64;
//int write_type = UA_NS0ID_UINT64;
//int write_type = UA_NS0ID_FLOAT;
//int write_type = UA_NS0ID_DOUBLE;
 */


/*enum EComResponse{
  e_Nothing = 0,
  e_InitOk = e_InitPositive | e_Ok,
  e_ProcessDataOk = e_ProcessDataPositive | e_Ok,
  e_InitInvalidId = e_InitNegative | e_InvalidId,
  e_InitTerminated = e_InitNegative | e_Terminated,
  e_ProcessDataInvalidObject = e_ProcessDataNegative | e_InvalidObject,
  e_ProcessDataDataTypeError = e_ProcessDataNegative | e_DataTypeError,
  e_ProcessDataInhibited = e_ProcessDataNegative | e_Inhibited,
  e_ProcessDataNoSocket = e_ProcessDataNegative | e_NoSocket,
  e_ProcessDataSendFailed = e_ProcessDataNegative | e_SendFailed,
  e_ProcessDataRecvFaild = e_ProcessDataNegative | e_RecvFailed

 */








