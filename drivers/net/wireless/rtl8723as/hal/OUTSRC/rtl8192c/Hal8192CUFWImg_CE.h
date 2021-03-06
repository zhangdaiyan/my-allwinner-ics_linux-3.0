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
#ifndef __INC_HAL8192CU_FW_IMG_H
#define __INC_HAL8192CU_FW_IMG_H

/*Created on  2011/ 6/15,  5:45*/

#ifdef CONFIG_BT_COEXIST
#define TSMCImgArrayLength 16248 //v79 TSMC COMMON 2011-10-06
#else //#ifdef CONFIG_P2P
#define TSMCImgArrayLength 16404 //v79 TSMC P2PPS 2011-10-06
#endif
extern u8 Rtl8192CUFwTSMCImgArray[TSMCImgArrayLength];

#ifdef CONFIG_BT_COEXIST
#define UMCACutImgArrayLength 16248 //v79 UMC A Cut COMMON 2011-10-06
#else //#ifdef CONFIG_P2P
#define UMCACutImgArrayLength 16404 //v79 UMC A Cut P2PPS 2011-10-06
#endif
extern u8 Rtl8192CUFwUMCACutImgArray[UMCACutImgArrayLength];

#ifdef CONFIG_BT_COEXIST
#define UMCBCutImgArrayLength 16254 //v79 UMC B Cut COMMON 2011-10-06
#else //#ifdef CONFIG_P2P
#define UMCBCutImgArrayLength 16386 //v79 UMC B Cut P2PPS 2011-10-06
#endif
extern u8 Rtl8192CUFwUMCBCutImgArray[UMCBCutImgArrayLength];

#endif //__INC_HAL8192CU_FW_IMG_H
