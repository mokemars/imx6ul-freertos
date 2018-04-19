#include "SlaveObjDictConfig.h"
#include "SlaveObjDict.h"

CO_Data* CANOpenSlaveObject = (CO_Data*)&SlaveDict_Data;

/**
  * @brief  Slave_heartbeatError.
  * @param  d:CANOpen object dictionary. heartbeatID:Node id
  * @retval None
  */
void Slave_HeartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	MSG("Slave_heartbeatError %d\r\n", heartbeatID);
}

/**
  * @brief  Slave_initialisation.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Initialisation(CO_Data* d)
{
	UNS32 TPDO1_COBID = 0x0181; 
	UNS32 RPDO1_COBID = 0x201;
	UNS32 TPDO2_COBID = 0x0182; 
	UNS32 RPDO2_COBID = 0x202;
	UNS32 size = sizeof(UNS32); 
	UNS8 unsize = sizeof(unsize); 
	UNS8  TpdoTransmissionType = 0;
	
	TpdoTransmissionType = TpdoTransmissionType;
	//SEGGER_RTT_printf(0, "Slave_initialisation\n");

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
  * @brief  Slave_preOperational.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_PreOperational(CO_Data* d)
{
	MSG("Slave_preOperational\r\n");
}
/**
  * @brief  Slave_preOperational.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Operational(CO_Data* d)
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
	MSG("Slave_operational\r\n");
}
/**
  * @brief  Slave_stopped.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Stopped(CO_Data* d)
{
	MSG("Slave_stopped\r\n");
}

/**
  * @brief  Slave_post_sync.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Post_Sync(CO_Data* d)
{

	ControlWordAxis1 = 0x0f;
	InterpolationDataAxis1 = 100;
	
	ControlWordAxis2 = 0x0f;
	InterpolationDataAxis2 = 100;
	MSG("Slave_post_sync\r\n");
}
/**
  * @brief  Slave_post_emcy.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Post_Emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	MSG("Slave received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\r\n", nodeID, errCode, errReg);
}
/**
  * @brief  Slave_post_TPDO.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void Slave_Post_TPDO(CO_Data* d)
{

}
/**
  * @brief  Slave_post_SlaveBootup.
  * @param  d:CANOpen object dictionary.nodeId:Node id
  * @retval None
  */
void Slave_Post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
	masterSendNMTstateChange (d, nodeid, NMT_Start_Node);
	MSG("Slave_post_SlaveBootup %x\r\n", nodeid);
}
/**
  * @brief  Slave_post_SlaveBootup.
  * @param  d:CANOpen object dictionary.nodeId:Node id newNodeState:Slave change state
  * @retval None
  */
void Slave_Post_SlaveStateChange(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState)
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
  * @brief  CANOpenSlaveInit.
  * @param  d:CANOpen object dictionary.
  * @retval None
  */
void CANOpenSlaveInit(CO_Data* d)
{
	d->heartbeatError = Slave_HeartbeatError;
	d->initialisation = Slave_Initialisation;
	d->preOperational = Slave_PreOperational;
	d->operational = Slave_Operational;
	d->stopped = Slave_Stopped;
	d->post_sync = Slave_Post_Sync;
	d->post_TPDO = Slave_Post_TPDO;
	d->post_emcy = Slave_Post_Emcy;
	d->post_SlaveBootup = Slave_Post_SlaveBootup;
	d->post_SlaveStateChange = Slave_Post_SlaveStateChange;
}
/**
  * @brief  CanopenInit
  * @param  NONE
  * @retval NONE
  */
void CanopenSlaveInit(void)
{
	initTimer();
	canInit(CANOpenSlaveObject,CAN1,110,CAN_BAUD_100K,0);
	CANOpenSlaveInit(CANOpenSlaveObject);
	setNodeId(CANOpenSlaveObject, 110);
	setState(CANOpenSlaveObject,Initialisation);
}
