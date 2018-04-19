#include "MasterObjDictConfig.h"
#include "MasterObjDict.h"

CO_Data* CANOpenMasterObject = (CO_Data*)&MasterDict_Data;

/**
  * @brief  Master_heartbeatError.
  * @param  d:CANOpen object dictionary. heartbeatID:Node id
  * @retval None
  */
void Master_HeartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	//SEGGER_RTT_printf(0, "Master_heartbeatError %d\n", heartbeatID);
}

/**
  * @brief  Master_initialisation.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Initialisation(CO_Data* d)
{
	UNS32 TPDO1_COBID = 0x0181; 
	UNS32 RPDO1_COBID = 0x201;
	UNS32 TPDO2_COBID = 0x0182; 
	UNS32 RPDO2_COBID = 0x202;
	UNS32 size = sizeof(UNS32); 
	UNS8 unsize = sizeof(unsize); 
	UNS8  TpdoTransmissionType = 0;
	
	TpdoTransmissionType = TpdoTransmissionType;
	//SEGGER_RTT_printf(0, "Master_initialisation\n");

	/*****************************************
	 * Define RPDOs to match slave ID=2 TPDOs*
	 *****************************************/
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&RPDO1_COBID, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	TpdoTransmissionType = TRANS_SYNC_ACYCLIC;	
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&TpdoTransmissionType, /*void * pSourceData,*/ 
			(UNS32*)&unsize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
	writeLocalDict( d, /*CO_Data* d*/
			0x1401, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&RPDO2_COBID, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	TpdoTransmissionType = TRANS_SYNC_ACYCLIC;	
	writeLocalDict( d, /*CO_Data* d*/
			0x1401, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&TpdoTransmissionType, /*void * pSourceData,*/ 
			(UNS32*)&unsize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&TPDO1_COBID, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	TpdoTransmissionType = TRANS_SYNC_MIN;	
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&TpdoTransmissionType, /*void * pSourceData,*/ 
			(UNS32*)&unsize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
	writeLocalDict( d, /*CO_Data* d*/
			0x1801, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&TPDO2_COBID, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	TpdoTransmissionType = TRANS_SYNC_MIN;	
	writeLocalDict( d, /*CO_Data* d*/
			0x1801, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&TpdoTransmissionType, /*void * pSourceData,*/ 
			(UNS32*)&unsize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
					
}
/**
  * @brief  Master_preOperational.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_PreOperational(CO_Data* d)
{
	//SEGGER_RTT_printf(0, "Master_preOperational\n");
}
/**
  * @brief  Master_preOperational.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Operational(CO_Data* d)
{
	UNS32 SyncTimePeriod = 1000;
	UNS32 size = sizeof(SyncTimePeriod);
	
	writeLocalDict( d, /*CO_Data* d*/
			0x1006, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&SyncTimePeriod, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	stopSYNC(d);
	startSYNC(d
	);
	setTimer(1000);
}
/**
  * @brief  Master_stopped.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Stopped(CO_Data* d)
{
}

/**
  * @brief  Master_post_sync.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Post_Sync(CO_Data* d)
{

	ControlWordAxis1 = 0x0f;
	InterpolationDataAxis1 = 100;
	
	ControlWordAxis2 = 0x0f;
	InterpolationDataAxis2 = 100;
	//SEGGER_RTT_printf(0, "Master_post_sync\n");
}
/**
  * @brief  Master_post_emcy.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Post_Emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	//SEGGER_RTT_printf(0, "Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}
/**
  * @brief  Master_post_TPDO.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Master_Post_TPDO(CO_Data* d)
{

}
/**
  * @brief  Master_post_SlaveBootup.
  * @param  d:CANOpen object dictionary.nodeId:Node id
  * @retval None
  */
void Master_Post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
		masterSendNMTstateChange (d, nodeid, NMT_Start_Node);
		//SEGGER_RTT_printf(0, "Master_post_SlaveBootup %x\n", nodeid);
}
/**
  * @brief  Master_post_SlaveBootup.
  * @param  d:CANOpen object dictionary.nodeId:Node id newNodeState:Slave change state
  * @retval None
  */
void Master_Post_SlaveStateChange(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState)
{
		static UNS8 NodeIdNum = 0;
		if (newNodeState == Operational)
		{
				NodeIdNum++;
				if (NodeIdNum == MAX_CAN_BUS_ID)
				{
						NodeIdNum = 0;
						setState(d, Operational);
				}
		}
}
/**
  * @brief  CANOpenMasterInit.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void CANOpenMasterInit(CO_Data* d)
{
	d->heartbeatError = Master_HeartbeatError;
	d->initialisation = Master_Initialisation;
	d->preOperational = Master_PreOperational;
	d->operational = Master_Operational;
	d->stopped = Master_Stopped;
	d->post_sync = Master_Post_Sync;
	d->post_TPDO = Master_Post_TPDO;
	d->post_emcy = Master_Post_Emcy;
	d->post_SlaveBootup = Master_Post_SlaveBootup;
	d->post_SlaveStateChange = Master_Post_SlaveStateChange;
}
/**
  * @brief  CanopenInit
  * @param  NONE
  * @retval NONE
  */
void CanopenMasterInit(void)
{
	initTimer();
	CANOpenMasterInit(CANOpenMasterObject);
	canInit(CANOpenMasterObject,CAN1,110,CAN_BAUD_100K,1);
	setNodeId (CANOpenMasterObject, 110);
	setState(CANOpenMasterObject,Initialisation);
}
