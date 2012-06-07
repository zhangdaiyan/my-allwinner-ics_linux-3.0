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
#ifndef __RTL8723A_RECV_H__
#define __RTL8723A_RECV_H__

#include <rtl8192c_recv.h>

#define TX_RPT1_PKT_LEN 8

typedef enum _RX_PACKET_TYPE{
	NORMAL_RX,//Normal rx packet
	TX_REPORT1,//CCX
	TX_REPORT2,//TX RPT
	HIS_REPORT,// USB HISR RPT
}RX_PACKET_TYPE, *PRX_PACKET_TYPE;


#ifdef CONFIG_SDIO_HCI
s32 rtl8188es_init_recv_priv(PADAPTER padapter);
void rtl8188es_free_recv_priv(PADAPTER padapter);
void rtl8188es_recv_hdl(PADAPTER padapter, struct recv_buf *precvbuf);
#endif

void rtl8188e_query_rx_phy_status(union recv_frame *prframe, struct phy_stat *pphy_stat);
void rtl8188e_process_phy_info(PADAPTER padapter, void *prframe);

#endif

