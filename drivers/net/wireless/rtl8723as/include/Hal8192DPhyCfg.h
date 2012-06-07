/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
/*****************************************************************************
 *
 * Module:	__INC_HAL8192DPHYCFG_H
 *
 *
 * Note:	
 *			
 *
 * Export:	Constants, macro, functions(API), global variables(None).
 *
 * Abbrev:	
 *
 * History:
 *		Data		Who		Remark 
 *      08/07/2007  MHC    	1. Porting from 9x series PHYCFG.h.
 *							2. Reorganize code architecture.
 * 
 *****************************************************************************/
 /* Check to see if the file has been included already.  */
#ifndef __INC_HAL8192DPHYCFG_H
#define __INC_HAL8192DPHYCFG_H


/*--------------------------Define Parameters-------------------------------*/
#define LOOP_LIMIT				5
#define MAX_STALL_TIME			50		//us
#define AntennaDiversityValue	0x80	//(Adapter->bSoftwareAntennaDiversity ? 0x00:0x80)
#define MAX_TXPWR_IDX_NMODE_92S	63
#define Reset_Cnt_Limit			3


#ifdef CONFIG_PCI_HCI
#define	SET_RTL8192SE_RF_SLEEP(_pAdapter)							\
{																	\
	u1Byte		u1bTmp;												\
	u1bTmp = PlatformEFIORead1Byte(_pAdapter, REG_LDOV12D_CTRL);		\
	u1bTmp |= BIT0;													\
	PlatformEFIOWrite1Byte(_pAdapter, REG_LDOV12D_CTRL, u1bTmp);		\
	PlatformEFIOWrite1Byte(_pAdapter, REG_SPS_OCP_CFG, 0x0);				\
	PlatformEFIOWrite1Byte(_pAdapter, TXPAUSE, 0xFF);				\
	PlatformEFIOWrite2Byte(_pAdapter, CMDR, 0x57FC);				\
	delay_us(100);													\
	PlatformEFIOWrite2Byte(_pAdapter, CMDR, 0x77FC);				\
	PlatformEFIOWrite1Byte(_pAdapter, PHY_CCA, 0x0);				\
	delay_us(10);													\
	PlatformEFIOWrite2Byte(_pAdapter, CMDR, 0x37FC);				\
	delay_us(10);													\
	PlatformEFIOWrite2Byte(_pAdapter, CMDR, 0x77FC);				\
	delay_us(10);													\
	PlatformEFIOWrite2Byte(_pAdapter, CMDR, 0x57FC);				\
}
#endif


/*--------------------------Define Parameters-------------------------------*/


/*------------------------------Define structure----------------------------*/ 
typedef enum _SwChnlCmdID{
	CmdID_End,
	CmdID_SetTxPowerLevel,
	CmdID_BBRegWrite10,
	CmdID_WritePortUlong,
	CmdID_WritePortUshort,
	CmdID_WritePortUchar,
	CmdID_RF_WriteReg,
}SwChnlCmdID;


/* 1. Switch channel related */
typedef struct _SwChnlCmd{
	SwChnlCmdID	CmdID;
	u32			Para1;
	u32			Para2;
	u32			msDelay;
}SwChnlCmd;

typedef enum _HW90_BLOCK{
	HW90_BLOCK_MAC = 0,
	HW90_BLOCK_PHY0 = 1,
	HW90_BLOCK_PHY1 = 2,
	HW90_BLOCK_RF = 3,
	HW90_BLOCK_MAXIMUM = 4, // Never use this
}HW90_BLOCK_E, *PHW90_BLOCK_E;

//vivi added this for read parameter from header, 20100908
typedef enum _RF_CONTENT{
	radioa_txt = 0x1000,
	radiob_txt = 0x1001,
	radioc_txt = 0x1002,
	radiod_txt = 0x1003
} RF_CONTENT;

typedef enum _RF_RADIO_PATH{
	RF_PATH_A = 0,			//Radio Path A
	RF_PATH_B = 1,			//Radio Path B
	RF_PATH_C = 2,			//Radio Path C
	RF_PATH_D = 3,			//Radio Path D
	//RF_PATH_MAX				//Max RF number 90 support 
}RF_RADIO_PATH_E, *PRF_RADIO_PATH_E;

#define	RF_PATH_MAX			2


typedef enum _WIRELESS_MODE {
	WIRELESS_MODE_UNKNOWN = 0x00,
	WIRELESS_MODE_A = 0x01,
	WIRELESS_MODE_B = 0x02,
	WIRELESS_MODE_G = 0x04,
	WIRELESS_MODE_AUTO = 0x08,
	WIRELESS_MODE_N_24G = 0x10,
	WIRELESS_MODE_N_5G = 0x20
} WIRELESS_MODE;


#if(TX_POWER_FOR_5G_BAND == 1)
#define CHANNEL_MAX_NUMBER		14+24+21	// 14 is the max channel number
#define CHANNEL_GROUP_MAX		3+9	// ch1~3, ch4~9, ch10~14 total three groups
#define MAX_PG_GROUP 13
#else
#define CHANNEL_MAX_NUMBER		14	// 14 is the max channel number
#define CHANNEL_GROUP_MAX		3	// ch1~3, ch4~9, ch10~14 total three groups
#define MAX_PG_GROUP 7
#endif
#define	CHANNEL_GROUP_MAX_2G		3
#define	CHANNEL_GROUP_IDX_5GL		3
#define	CHANNEL_GROUP_IDX_5GM		6
#define	CHANNEL_GROUP_IDX_5GH		9
#define	CHANNEL_GROUP_MAX_5G		9
#define	CHANNEL_MAX_NUMBER_2G		14

#if (RTL8192D_DUAL_MAC_MODE_SWITCH == 1)
typedef enum _BaseBand_Config_Type{
	BaseBand_Config_PHY_REG = 0,			
	BaseBand_Config_AGC_TAB = 1,			
	BaseBand_Config_AGC_TAB_2G = 2,
	BaseBand_Config_AGC_TAB_5G = 3,
}BaseBand_Config_Type, *PBaseBand_Config_Type;
#else
typedef enum _BaseBand_Config_Type{
	BaseBand_Config_PHY_REG = 0,			//Radio Path A
	BaseBand_Config_AGC_TAB = 1,			//Radio Path B
}BaseBand_Config_Type, *PBaseBand_Config_Type;
#endif


typedef enum _MACPHY_MODE_8192D{
	SINGLEMAC_SINGLEPHY,	//SMSP
	DUALMAC_DUALPHY,		//DMDP
	DUALMAC_SINGLEPHY,	//DMSP	
}MACPHY_MODE_8192D,*PMACPHY_MODE_8192D;

typedef enum _MACPHY_MODE_CHANGE_ACTION{
	DMDP2DMSP = 0,
	DMSP2DMDP = 1,
	DMDP2SMSP = 2,
	SMSP2DMDP = 3,
	DMSP2SMSP = 4,
	SMSP2DMSP = 5,
	MAXACTION
}MACPHY_MODE_CHANGE_ACTION,*PMACPHY_MODE_CHANGE_ACTION;

typedef enum _BAND_TYPE{
	BAND_ON_2_4G 	= 1,
	BAND_ON_5G	= 2,
	BAND_ON_BOTH,
	BANDMAX
}BAND_TYPE,*PBAND_TYPE;

typedef enum _PHY_Rate_Tx_Power_Offset_Area{
	RA_OFFSET_LEGACY_OFDM1,
	RA_OFFSET_LEGACY_OFDM2,
	RA_OFFSET_HT_OFDM1,
	RA_OFFSET_HT_OFDM2,
	RA_OFFSET_HT_OFDM3,
	RA_OFFSET_HT_OFDM4,
	RA_OFFSET_HT_CCK,
}RA_OFFSET_AREA,*PRA_OFFSET_AREA;


/* BB/RF related */
typedef	enum _RF_TYPE_8190P{
	RF_TYPE_MIN,	// 0
	RF_8225=1,			// 1 11b/g RF for verification only
	RF_8256=2,			// 2 11b/g/n 
	RF_8258=3,			// 3 11a/b/g/n RF
	RF_6052=4,		// 4 11b/g/n RF
	//RF_6052=5,		// 4 11b/g/n RF
	// TODO: We sholud remove this psudo PHY RF after we get new RF.
	RF_PSEUDO_11N=5,	// 5, It is a temporality RF. 
}RF_TYPE_8190P_E,*PRF_TYPE_8190P_E;


typedef enum _RATR_TABLE_MODE_8192C{
	RATR_INX_WIRELESS_NGB = 0,
	RATR_INX_WIRELESS_NG = 1,
	RATR_INX_WIRELESS_NB = 2,
	RATR_INX_WIRELESS_N = 3,
	RATR_INX_WIRELESS_GB = 4,
	RATR_INX_WIRELESS_G = 5,
	RATR_INX_WIRELESS_B = 6,
	RATR_INX_WIRELESS_MC = 7,
	RATR_INX_WIRELESS_A = 8,
}RATR_TABLE_MODE_8192C, *PRATR_TABLE_MODE_8192C;

typedef struct _BB_REGISTER_DEFINITION{
	u32 rfintfs;			// set software control: 
							//		0x870~0x877[8 bytes]
							
	u32 rfintfi;			// readback data: 
							//		0x8e0~0x8e7[8 bytes]
							
	u32 rfintfo; 		// output data: 
							//		0x860~0x86f [16 bytes]
							
	u32 rfintfe; 		// output enable: 
							//		0x860~0x86f [16 bytes]
							
	u32 rf3wireOffset;	// LSSI data:
							//		0x840~0x84f [16 bytes]
							
	u32 rfLSSI_Select;	// BB Band Select: 
							//		0x878~0x87f [8 bytes]
							
	u32 rfTxGainStage;	// Tx gain stage: 
							//		0x80c~0x80f [4 bytes]
							
	u32 rfHSSIPara1; 	// wire parameter control1 : 
							//		0x820~0x823,0x828~0x82b, 0x830~0x833, 0x838~0x83b [16 bytes]
							
	u32 rfHSSIPara2; 	// wire parameter control2 : 
							//		0x824~0x827,0x82c~0x82f, 0x834~0x837, 0x83c~0x83f [16 bytes]
								
	u32 rfSwitchControl; //Tx Rx antenna control : 
							//		0x858~0x85f [16 bytes]
								
	u32 rfAGCControl1; 	//AGC parameter control1 : 
							//		0xc50~0xc53,0xc58~0xc5b, 0xc60~0xc63, 0xc68~0xc6b [16 bytes] 
								
	u32 rfAGCControl2; 	//AGC parameter control2 : 
							//		0xc54~0xc57,0xc5c~0xc5f, 0xc64~0xc67, 0xc6c~0xc6f [16 bytes] 
							
	u32 rfRxIQImbalance; //OFDM Rx IQ imbalance matrix : 
							//		0xc14~0xc17,0xc1c~0xc1f, 0xc24~0xc27, 0xc2c~0xc2f [16 bytes]
							
	u32 rfRxAFE;  		//Rx IQ DC ofset and Rx digital filter, Rx DC notch filter : 
							//		0xc10~0xc13,0xc18~0xc1b, 0xc20~0xc23, 0xc28~0xc2b [16 bytes]
							
	u32 rfTxIQImbalance; //OFDM Tx IQ imbalance matrix
							//		0xc80~0xc83,0xc88~0xc8b, 0xc90~0xc93, 0xc98~0xc9b [16 bytes]
							
	u32 rfTxAFE; 		//Tx IQ DC Offset and Tx DFIR type
							//		0xc84~0xc87,0xc8c~0xc8f, 0xc94~0xc97, 0xc9c~0xc9f [16 bytes]
								
	u32 rfLSSIReadBack; 	//LSSI RF readback data SI mode
								//		0x8a0~0x8af [16 bytes]

	u32 rfLSSIReadBackPi; 	//LSSI RF readback data PI mode 0x8b8-8bc for Path A and B

}BB_REGISTER_DEFINITION_T, *PBB_REGISTER_DEFINITION_T;


typedef struct _R_ANTENNA_SELECT_OFDM{	
	u32			r_tx_antenna:4;	
	u32			r_ant_l:4;
	u32			r_ant_non_ht:4;	
	u32			r_ant_ht1:4;
	u32			r_ant_ht2:4;
	u32			r_ant_ht_s1:4;
	u32			r_ant_non_ht_s1:4;
	u32			OFDM_TXSC:2;
	u32			Reserved:2;
}R_ANTENNA_SELECT_OFDM;

typedef struct _R_ANTENNA_SELECT_CCK{
	u8			r_cckrx_enable_2:2;	
	u8			r_cckrx_enable:2;
	u8			r_ccktx_enable:4;
}R_ANTENNA_SELECT_CCK;

/*------------------------------Define structure----------------------------*/ 


/*------------------------Export global variable----------------------------*/
/*------------------------Export global variable----------------------------*/


/*------------------------Export Marco Definition---------------------------*/

/*--------------------------Exported Function prototype---------------------*/
//
// BB and RF register read/write
//
void	rtl8192d_PHY_SetBBReg1Byte(	IN	PADAPTER	Adapter,
								IN	u32		RegAddr,
								IN	u32		BitMask,
								IN	u32		Data	);
u32	rtl8192d_PHY_QueryBBReg(	IN	PADAPTER	Adapter,
								IN	u32		RegAddr,
								IN	u32		BitMask	);
void	rtl8192d_PHY_SetBBReg(	IN	PADAPTER	Adapter,
								IN	u32		RegAddr,
								IN	u32		BitMask,
								IN	u32		Data	);
u32	rtl8192d_PHY_QueryRFReg(	IN	PADAPTER			Adapter,
								IN	RF_RADIO_PATH_E	eRFPath,
								IN	u32				RegAddr,
								IN	u32				BitMask	);
void	rtl8192d_PHY_SetRFReg(	IN	PADAPTER			Adapter,
								IN	RF_RADIO_PATH_E	eRFPath,
								IN	u32				RegAddr,
								IN	u32				BitMask,
								IN	u32				Data	);

//
// Initialization related function
//
/* MAC/BB/RF HAL config */
extern	int	PHY_MACConfig8192D(	IN	PADAPTER	Adapter	);
extern	int	PHY_BBConfig8192D(	IN	PADAPTER	Adapter	);
extern	int	PHY_RFConfig8192D(	IN	PADAPTER	Adapter	);
/* RF config */
int	rtl8192d_PHY_ConfigRFWithParaFile(	IN	PADAPTER	Adapter,
												IN	u8* 	pFileName,
												IN	RF_RADIO_PATH_E	eRFPath);
int	rtl8192d_PHY_ConfigRFWithHeaderFile(	IN	PADAPTER			Adapter,
												IN	RF_CONTENT			Content,
												IN	RF_RADIO_PATH_E	eRFPath);
/* BB/RF readback check for making sure init OK */
int	rtl8192d_PHY_CheckBBAndRFOK(	IN	PADAPTER			Adapter,
										IN	HW90_BLOCK_E		CheckBlock,
										IN	RF_RADIO_PATH_E	eRFPath	  );
/* Read initi reg value for tx power setting. */
void	rtl8192d_PHY_GetHWRegOriginalValue(	IN	PADAPTER		Adapter	);

//
// RF Power setting
//
//extern	BOOLEAN	PHY_SetRFPowerState(IN	PADAPTER			Adapter, 
//									IN	RT_RF_POWER_STATE	eRFPowerState);

//
// BB TX Power R/W
//
void	PHY_GetTxPowerLevel8192D(	IN	PADAPTER		Adapter,
											OUT u32*    		powerlevel	);
void	PHY_SetTxPowerLevel8192D(	IN	PADAPTER		Adapter,
											IN	u8			channel	);
BOOLEAN	PHY_UpdateTxPowerDbm8192D(	IN	PADAPTER	Adapter,
											IN	int		powerInDbm	);

//
VOID 
PHY_ScanOperationBackup8192D(IN	PADAPTER	Adapter,
										IN	u8		Operation	);

//
// Switch bandwidth for 8192S
//
//void	PHY_SetBWModeCallback8192C(	IN	PRT_TIMER		pTimer	);
void	PHY_SetBWMode8192D(	IN	PADAPTER			pAdapter,
									IN	HT_CHANNEL_WIDTH	ChnlWidth,
									IN	unsigned char	Offset	);

//
// Set FW CMD IO for 8192S.
//
//extern	BOOLEAN HalSetIO8192C(	IN	PADAPTER			Adapter,
//									IN	IO_TYPE				IOType);

//
// Set A2 entry to fw for 8192S
//
extern	void FillA2Entry8192C(		IN	PADAPTER			Adapter,
										IN	u8				index,
										IN	u8*				val);


//
// channel switch related funciton
//
//extern	void	PHY_SwChnlCallback8192C(	IN	PRT_TIMER		pTimer	);
void	PHY_SwChnl8192D(	IN	PADAPTER		pAdapter,
									IN	u8			channel	);
				// Call after initialization
void	PHY_SwChnlPhy8192D(	IN	PADAPTER		pAdapter,
									IN	u8			channel	);

extern void ChkFwCmdIoDone(	IN	PADAPTER	Adapter);

#ifdef USE_WORKITEM	
//extern 	void SetIOWorkItemCallback( IN PVOID            pContext );
#else
//extern	void SetIOTimerCallback( IN PRT_TIMER		pTimer);
#endif	
				
//
// BB/MAC/RF other monitor API
//
void	PHY_SetMonitorMode8192D(IN	PADAPTER	pAdapter,
										IN	BOOLEAN		bEnableMonitorMode	);

BOOLEAN	PHY_CheckIsLegalRfPath8192D(IN	PADAPTER	pAdapter,
											IN	u32		eRFPath	);


//
// Modify the value of the hw register when beacon interval be changed.
//
void	
rtl8192d_PHY_SetBeaconHwReg(	IN	PADAPTER		Adapter,
					IN	u16			BeaconInterval	);


extern	VOID
PHY_SwitchEphyParameter(
	IN	PADAPTER			Adapter
	);

extern	VOID
PHY_EnableHostClkReq(
	IN	PADAPTER			Adapter
	);

BOOLEAN
SetAntennaConfig92C(
	IN	PADAPTER	Adapter,
	IN	u8		DefaultAnt	
	);

VOID
PHY_UpdateBBRFConfiguration8192D(
	IN PADAPTER Adapter,
	IN BOOLEAN bisBandSwitch
);

VOID PHY_ReadMacPhyMode92D(
	IN PADAPTER	Adapter,
	IN BOOLEAN 	AutoloadFail	
);

VOID PHY_ConfigMacPhyMode92D(
	IN PADAPTER	Adapter
);

VOID PHY_ConfigMacPhyModeInfo92D(
	IN PADAPTER	Adapter
);

VOID PHY_ConfigMacCoexist_RFPage92D(
	IN PADAPTER	Adapter
);

VOID
rtl8192d_PHY_InitRxSetting(
	IN	PADAPTER Adapter
);


VOID 
rtl8192d_PHY_SetRFPathSwitch(IN	PADAPTER	pAdapter, IN	BOOLEAN		bMain);

VOID
HalChangeCCKStatus8192D(
	IN	PADAPTER	Adapter,
	IN	BOOLEAN		bCCKDisable
);

VOID 
PHY_InitPABias92D(IN	PADAPTER Adapter);

/*--------------------------Exported Function prototype---------------------*/

#define PHY_SetBBReg1Byte(Adapter, RegAddr, BitMask, Data) rtl8192d_PHY_SetBBReg1Byte((Adapter), (RegAddr), (BitMask), (Data))
#define PHY_QueryBBReg(Adapter, RegAddr, BitMask) rtl8192d_PHY_QueryBBReg((Adapter), (RegAddr), (BitMask))
#define PHY_SetBBReg(Adapter, RegAddr, BitMask, Data) rtl8192d_PHY_SetBBReg((Adapter), (RegAddr), (BitMask), (Data))
#define PHY_QueryRFReg(Adapter, eRFPath, RegAddr, BitMask) rtl8192d_PHY_QueryRFReg((Adapter), (eRFPath), (RegAddr), (BitMask))
#define PHY_SetRFReg(Adapter, eRFPath, RegAddr, BitMask, Data) rtl8192d_PHY_SetRFReg((Adapter), (eRFPath), (RegAddr), (BitMask), (Data))

#define PHY_SetMacReg	PHY_SetBBReg

#endif	// __INC_HAL8192SPHYCFG_H

