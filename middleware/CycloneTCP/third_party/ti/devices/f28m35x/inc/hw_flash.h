//###########################################################################
// FILE:   hw_flash.h
// TITLE:  Macros used when accessing the flash control hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_FLASHERR_H__
#define __HW_FLASHERR_H__

//*****************************************************************************
// The following are defines for the Flash Control Registers addresses.
//*****************************************************************************
#define FLASHCTRL_O_FRDCNTL                      0x00000000
#define FLASHCTRL_O_FSPRD                        0x00000004
#define FLASHCTRL_O_FBAC                         0x0000003C
#define FLASHCTRL_O_FBFALLBACK                   0x00000040
#define FLASHCTRL_O_FBPRDY                       0x00000044
#define FLASHCTRL_O_FPAC1                        0x00000048
#define FLASHCTRL_O_FPAC2                        0x0000004C
#define FLASHCTRL_O_FMAC                         0x00000050
#define FLASHCTRL_O_FMSTAT                       0x00000054
#define FLASHCTRL_O_SECZONEREQUEST               0x00000160
#define FLASHCTRL_O_FRD_INTF_CTRL                0x00000300

//*****************************************************************************
// The following are defines for the Flash Error Registers addresses.
//*****************************************************************************
#define FLASHERR_O_ECC_ENABLE                    0x00000000
#define FLASHERR_O_SINGLE_ERR_ADDR               0x00000004
#define FLASHERR_O_UNC_ERR_ADDR                  0x00000008
#define FLASHERR_O_ERR_STATUS                    0x0000000C
#define FLASHERR_O_ERR_POS                       0x00000010
#define FLASHERR_O_ERR_STATUS_CLR                0x00000014
#define FLASHERR_O_ERR_CNT                       0x00000018
#define FLASHERR_O_ERR_THRESHOLD                 0x0000001C
#define FLASHERR_O_ERR_INTFLG                    0x00000020
#define FLASHERR_O_ERR_INTCLR                    0x00000024
#define FLASHERR_O_FDATAH_TEST                   0x00000028
#define FLASHERR_O_FDATAL_TEST                   0x0000002C
#define FLASHERR_O_FADDR_TEST                    0x00000030
#define FLASHERR_O_FECC_TEST                     0x00000034
#define FLASHERR_O_FECC_CTRL                     0x00000038
#define FLASHERR_O_FECC_FOUTH_TEST               0x0000003C
#define FLASHERR_O_FECC_FOUTL_TEST               0x00000040
#define FLASHERR_O_FECC_STATUS                   0x00000044

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FRDCNTL
// register.
//**************************************************************************************
#define FLASHCTRL_FRDCNTL_RWAIT_S                8
#define FLASHCTRL_FRDCNTL_RWAIT_M                0x00000F00

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FSPRD
// register.
//**************************************************************************************
#define FLASHCTRL_FSPRD_RM0                      0x00000001
#define FLASHCTRL_FSPRD_RM1                      0x00000002

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FBAC
// register.
//**************************************************************************************
#define FLASHCTRL_FBAC_VREADST_S                 0
#define FLASHCTRL_FBAC_VREADST_M                 0x000000FF
#define FLASHCTRL_FBAC_BAGP_S                    8
#define FLASHCTRL_FBAC_BAGP_M                    0x0000FF00

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FBFALLBACK
// register.
//**************************************************************************************
#define FLASHCTRL_FBFALLBACK_BANKPWR0_S          0
#define FLASHCTRL_FBFALLBACK_BANKPWR0_M          0x00000003

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FBPRDY
// register.
//**************************************************************************************
#define FLASHCTRL_FBPRDY_BANKRDY                 0x00000001
#define FLASHCTRL_FBPRDY_PUMPRDY                 0x00008000

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FPAC1
// register.
//**************************************************************************************
#define FLASHCTRL_FPAC1_PUMPPWR                  0x00000001
#define FLASHCTRL_FPAC1_PSLEEP_S                 16
#define FLASHCTRL_FPAC1_PSLEEP_M                 0x07FF0000

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FPAC2
// register.
//**************************************************************************************
#define FLASHCTRL_FPAC2_PAGP_S                   0
#define FLASHCTRL_FPAC2_PAGP_M                   0x0000FFFF

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FMAC
// register.
//**************************************************************************************
#define FLASHCTRL_FMAC_BANK_S                    0
#define FLASHCTRL_FMAC_BANK_M                    0x00000007

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FMSTAT
// register.
//**************************************************************************************
#define FLASHCTRL_FMSTAT_PSUSP                   0x00000002
#define FLASHCTRL_FMSTAT_ESUSP                   0x00000004
#define FLASHCTRL_FMSTAT_VOLTSTAT                0x00000008
#define FLASHCTRL_FMSTAT_CSTAT                   0x00000010
#define FLASHCTRL_FMSTAT_INVDAT                  0x00000020
#define FLASHCTRL_FMSTAT_PGM                     0x00000040
#define FLASHCTRL_FMSTAT_ERS                     0x00000080
#define FLASHCTRL_FMSTAT_BUSY                    0x00000100
#define FLASHCTRL_FMSTAT_CV                      0x00000200
#define FLASHCTRL_FMSTAT_EV                      0x00000400
#define FLASHCTRL_FMSTAT_PCV                     0x00000800
#define FLASHCTRL_FMSTAT_PGV                     0x00001000
#define FLASHCTRL_FMSTAT_DBT                     0x00002000
#define FLASHCTRL_FMSTAT_ILA                     0x00004000
#define FLASHCTRL_FMSTAT_RVF                     0x00008000
#define FLASHCTRL_FMSTAT_RVER                    0x00010000
#define FLASHCTRL_FMSTAT_RVSUSP                  0x00020000

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_SECZONEREQUEST
// register.
//**************************************************************************************
#define FLASHCTRL_SECZONEREQUEST_SEM_S           0
#define FLASHCTRL_SECZONEREQUEST_SEM_M           0x00000003
#define FLASHCTRL_SECZONEREQUEST_KEY_S           4
#define FLASHCTRL_SECZONEREQUEST_KEY_M           0xFFFFFFF0

//**************************************************************************************
// The following are defines for the bit fields in the FLASHCTRL_FRD_INTF_CTRL
// register.
//**************************************************************************************
#define FLASHCTRL_FRD_INTF_CTRL_PRG_CACHE_EN     0x00000001
#define FLASHCTRL_FRD_INTF_CTRL_DAT_CACHE_EN     0x00000002

//**************************************************************************************
// The following are defines for the bit fields in the FLASHERR ECC Enable
// register.
//**************************************************************************************
#define FLASHERR_ECC_ENABLE_ENABLE_S             0
#define FLASHERR_ECC_ENABLE_ENABLE_M             0x0000000F

//**************************************************************************************
// The following are defines for the bit fields in the FLASHERR Error Position
// register.
//**************************************************************************************
#define FLASHERR_ERR_POS_ERRPOS_S                0
#define FLASHERR_ERR_POS_ERRPOS_M                0x0000003F
#define FLASHERR_ERR_POS_ECCLH                   0x00000040
#define FLASHERR_ERR_POS_ERRTYPE                 0x00000100

//**************************************************************************************
// The following are defines for the bit fields in the FLASHERR Error Status
// register.
//**************************************************************************************
#define FLASHERR_ERR_STATUS_FAIL0                0x00000001
#define FLASHERR_ERR_STATUS_FAIL1                0x00000002
#define FLASHERR_ERR_STATUS_UNC_ERR              0x00000004

//******************************************************************************************
// The following are defines for the bit fields in the FLASHERR Error Status
// Clear register.
//******************************************************************************************
#define FLASHERR_ERR_STATUS_CLR_FAIL0CLR         0x00000001
#define FLASHERR_ERR_STATUS_CLR_FAIL1CLR         0x00000002
#define FLASHERR_ERR_STATUS_CLR_UNCERRCLR        0x00000004

//*******************************************************************************************
// The following are defines for the bit fields in the FLASHERR Count
// register.
//*******************************************************************************************
#define FLASHERR_ERR_CNT_S                       0
#define FLASHERR_ERR_CNT_M                       0x0000FFFF

//*******************************************************************************************
// The following are defines for the bit fields in the FLASHERR Threshold
// register.
//*******************************************************************************************
#define FLASHERR_ERR_THRESHOLD_S                 0
#define FLASHERR_ERR_THRESHOLD_M                 0x0000FFFF

//*******************************************************************************************
// The following are defines for the bit fields in the FLASHERR Error Interrupt
// Flag register.
//*******************************************************************************************
#define FLASHERR_ERR_INTFLG_SINGLE_ERR_INTFLG    0x00000001
#define FLASHERR_ERR_INTFLG_UNC_ERR_INTFLG       0x00000002

//********************************************************************************************
// The following are defines for the bit fields in the FLASHERR Error Interrupt
// Clear register.
//********************************************************************************************
#define FLASHERR_ERR_INTCLR_SINGLE_ERR_INTCLR    0x00000001
#define FLASHERR_ERR_INTCLR_UNC_ERR_INTCLR       0x00000002

//********************************************************************************************
// The following are defines for the bit fields in the FLASHADDR Test Address
// register.
//********************************************************************************************
#define FLASHERR_FADDR_TEST_ADDR_S               3
#define FLASHERR_FADDR_TEST_ADDR_M               0x00FFFFF8

//********************************************************************************************
// The following are defines for the bit fields in the FLASHERR ECC TEST register.
//********************************************************************************************
#define FLASHERR_FECC_TEST_ECC_S                 0
#define FLASHERR_FECC_TEST_ECC_M                 0x000000FF

//********************************************************************************************
// The following are defines for the bit fields in the FLASHERR ECC Control
// register.
//********************************************************************************************
#define FLASHERR_FECC_CTRL_ECC_TEST_EN           0x00000001
#define FLASHERR_FECC_CTRL_ECC_SELECT            0x00000002

//********************************************************************************************
// The following are defines for the bit fields in the FLASHERR ECC Status
// register.
//********************************************************************************************
#define FLASHERR_FECC_STATUS_SINGLE_ERR          0x00000001
#define FLASHERR_FECC_STATUS_UNC_ERR             0x00000002
#define FLASHERR_FECC_STATUS_ERR_POS_S           2
#define FLASHERR_FECC_STATUS_ERR_POS_M           0x000000FC
#define FLASHERR_FECC_STATUS_CHK_ERR             0x00000100


#endif // __HW_FLASHERR_H__



