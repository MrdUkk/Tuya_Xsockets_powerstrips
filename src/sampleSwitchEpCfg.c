/********************************************************************************************************
 * @file    sampleSwitchEpCfg.c
 *
 * @brief   This is the source file for sampleSwitchEpCfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"


extern void cmdOnOff_on(u8 ep);
extern void cmdOnOff_off(u8 ep);

void zcl_nv_attr_reset(void);

/**********************************************************************
 * LOCAL CONSTANTS
 */
#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME    {17,'W','h','o','l','e','s','o','m','e','S','o','f','t','w','a','r','e'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID    {15,'5','S','o','c','k','P','w','r','S','t','r','i','p','-','1'}
#endif


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 App_inClusterList[] = {
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

const u16 App_inAddClusterList[] = {
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 App_outClusterList[] = {
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_IN_CLUSTER_NUM     (sizeof(App_inClusterList) / sizeof(App_inClusterList[0]))
#define APP_OUT_CLUSTER_NUM    (sizeof(App_outClusterList) / sizeof(App_outClusterList[0]))
#define APP_INADD_CLUSTER_NUM  (sizeof(App_inAddClusterList) / sizeof(App_inAddClusterList[0]))
/**
 *  @brief Definition for simple description for HA profile
 */

const af_simple_descriptor_t App_simpleDesc[5] = {
	{
		HA_PROFILE_ID,                      /* Application profile identifier */
    	HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
    	SAMPLE_SWITCH_ENDPOINT,             /* Endpoint */
    	1,                                  /* Application device version */
    	0,                                  /* Reserved */
    	APP_IN_CLUSTER_NUM,        /* Application input cluster count */
    	APP_OUT_CLUSTER_NUM,       /* Application output cluster count */
    	(u16 *)App_inClusterList,  /* Application input cluster list */
		(u16 *)App_outClusterList, /* Application output cluster list */
	},
	{
	    HA_PROFILE_ID,                      /* Application profile identifier */
	    HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
	    SAMPLE_SWITCH_ENDPOINT +1,             /* Endpoint */
	    1,                                  /* Application device version */
	    0,                                  /* Reserved */
		APP_INADD_CLUSTER_NUM,     /* Application input cluster count */
	    0,       /* Application output cluster count */
	    (u16 *)App_inAddClusterList,  /* Application input cluster list */
	    0, /* Application output cluster list */
	},
	{
	    HA_PROFILE_ID,                      /* Application profile identifier */
	    HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
	    SAMPLE_SWITCH_ENDPOINT +2,            /* Endpoint */
	    1,                                  /* Application device version */
	    0,                                  /* Reserved */
		APP_INADD_CLUSTER_NUM,     /* Application input cluster count */
	    0,       /* Application output cluster count */
	    (u16 *)App_inAddClusterList,  /* Application input cluster list */
	    0, 									/* Application output cluster list */
	},
	{
	    HA_PROFILE_ID,                      /* Application profile identifier */
	    HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
	    SAMPLE_SWITCH_ENDPOINT +3,            /* Endpoint */
	    1,                                  /* Application device version */
	    0,                                  /* Reserved */
		APP_INADD_CLUSTER_NUM,     /* Application input cluster count */
	    0,       /* Application output cluster count */
	    (u16 *)App_inAddClusterList,  /* Application input cluster list */
	    0, 									/* Application output cluster list */
	},
	{
	    HA_PROFILE_ID,                      /* Application profile identifier */
	    HA_DEV_ONOFF_SWITCH,                /* Application device identifier */
	    SAMPLE_SWITCH_ENDPOINT +4,            /* Endpoint */
	    1,                                  /* Application device version */
	    0,                                  /* Reserved */
		APP_INADD_CLUSTER_NUM,     /* Application input cluster count */
	    0,       /* Application output cluster count */
	    (u16 *)App_inAddClusterList,  /* Application input cluster list */
	    0, 									/* Application output cluster list */
	}
};

/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs = {
    .zclVersion   = 0x03,
    .appVersion   = 0x00,
    .stackVersion = (STACK_RELEASE|STACK_BUILD),
    .hwVersion    = 0x00,
    .manuName     = ZCL_BASIC_MFG_NAME,
    .modelId      = ZCL_BASIC_MODEL_ID,
    .dateCode     = ZCL_BASIC_DATE_CODE,
    .powerSource  = POWER_SOURCE_MAINS_1_PHASE,
    .swBuildId    = ZCL_BASIC_SW_BUILD_ID,
	.deviceEnable = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] = {
    { ZCL_ATTRID_BASIC_ZCL_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.zclVersion},
    { ZCL_ATTRID_BASIC_APP_VER,           ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.appVersion},
    { ZCL_ATTRID_BASIC_STACK_VER,         ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,            ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.hwVersion},
    { ZCL_ATTRID_BASIC_MFR_NAME,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.manuName},
    { ZCL_ATTRID_BASIC_MODEL_ID,          ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,                        (u8*)g_zcl_basicAttrs.modelId},
    { ZCL_ATTRID_BASIC_DATE_CODE,         ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  					  (u8*)g_zcl_basicAttrs.dateCode},
	{ ZCL_ATTRID_BASIC_POWER_SOURCE,      ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.powerSource},
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,       ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  					  (u8*)&g_zcl_basicAttrs.swBuildId},
	{ ZCL_ATTRID_BASIC_DEV_ENABLED,       ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_BASIC_ATTR_NUM      sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs = {
    .identifyTime = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] = {
    { ZCL_ATTRID_IDENTIFY_TIME,           ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM   sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)


#ifdef ZCL_ON_OFF

/* On/Off */
zcl_onOffAttr_t g_zcl_onOffAttrs[5] = {
		{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
		{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
		{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
		{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
		{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1}
};

const zclAttrInfo_t onOff_attrTbl[5][6] = {
		{
    { ZCL_ATTRID_ONOFF,                     ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs[0].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs[0].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[0].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[0].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[0].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision      },
		},
		{
    { ZCL_ATTRID_ONOFF,                     ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs[1].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs[1].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[1].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[1].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[1].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision      },
		},
		{
    { ZCL_ATTRID_ONOFF,                     ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs[2].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs[2].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[2].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[2].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[2].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision      },
		},
		{
    { ZCL_ATTRID_ONOFF,                     ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs[3].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs[3].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[3].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[3].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[3].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision      },
		},
		{
    { ZCL_ATTRID_ONOFF,                     ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_zcl_onOffAttrs[4].onOff               },
    { ZCL_ATTRID_GLOBAL_SCENE_CONTROL,      ZCL_DATA_TYPE_BOOLEAN,    ACCESS_CONTROL_READ,                             (u8*)&g_zcl_onOffAttrs[4].globalSceneControl  },
    { ZCL_ATTRID_ON_TIME,                   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[4].onTime              },
    { ZCL_ATTRID_OFF_WAIT_TIME,             ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[4].offWaitTime         },
    { ZCL_ATTRID_START_UP_ONOFF,            ZCL_DATA_TYPE_ENUM8,      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,      (u8*)&g_zcl_onOffAttrs[4].startUpOnOff        },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_DATA_TYPE_UINT16,     ACCESS_CONTROL_READ,                             (u8*)&zcl_attr_global_clusterRevision      },
		}
};

#endif

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_sampleSwitchClusterList[5][5] = {
		{
	{ZCL_CLUSTER_GEN_BASIC,      MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,    basic_attrTbl,    zcl_basic_register,    App_basicCb},
    {ZCL_CLUSTER_GEN_IDENTIFY,   MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM, identify_attrTbl, zcl_identify_register, App_identifyCb},
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,     MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    App_groupCb},
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,     MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    App_sceneCb},
#endif
#ifdef ZCL_ON_OFF
    {ZCL_CLUSTER_GEN_ON_OFF,     MANUFACTURER_CODE_NONE, 6,    					onOff_attrTbl[0], zcl_onOff_register,    App_onOffCb},
#endif
		},
		{
	#ifdef ZCL_GROUP
	    {ZCL_CLUSTER_GEN_GROUPS, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    App_groupCb},
	#endif
	#ifdef ZCL_SCENE
	    {ZCL_CLUSTER_GEN_SCENES, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    App_sceneCb},
	#endif
	#ifdef ZCL_ON_OFF
	    {ZCL_CLUSTER_GEN_ON_OFF, MANUFACTURER_CODE_NONE, 6,    					onOff_attrTbl[1], zcl_onOff_register,    App_onOffCb},
	#endif
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL},
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL}
		},
		{
	#ifdef ZCL_GROUP
	    {ZCL_CLUSTER_GEN_GROUPS, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    App_groupCb},
	#endif
	#ifdef ZCL_SCENE
	    {ZCL_CLUSTER_GEN_SCENES, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    App_sceneCb},
	#endif
	#ifdef ZCL_ON_OFF
	    {ZCL_CLUSTER_GEN_ON_OFF, MANUFACTURER_CODE_NONE, 6,    					onOff_attrTbl[2], zcl_onOff_register,    App_onOffCb},
	#endif
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL},
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL}
		},
		{
	#ifdef ZCL_GROUP
	    {ZCL_CLUSTER_GEN_GROUPS, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    App_groupCb},
	#endif
	#ifdef ZCL_SCENE
	    {ZCL_CLUSTER_GEN_SCENES, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    App_sceneCb},
	#endif
	#ifdef ZCL_ON_OFF
	    {ZCL_CLUSTER_GEN_ON_OFF, MANUFACTURER_CODE_NONE, 6,    					onOff_attrTbl[3], zcl_onOff_register,    App_onOffCb},
	#endif
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL},
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL}
		},
		{
	#ifdef ZCL_GROUP
	    {ZCL_CLUSTER_GEN_GROUPS, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_group_register,    App_groupCb},
	#endif
	#ifdef ZCL_SCENE
	    {ZCL_CLUSTER_GEN_SCENES, MANUFACTURER_CODE_NONE, 0,                     NULL,             zcl_scene_register,    App_sceneCb},
	#endif
	#ifdef ZCL_ON_OFF
	    {ZCL_CLUSTER_GEN_ON_OFF, MANUFACTURER_CODE_NONE, 6,    					onOff_attrTbl[4], zcl_onOff_register,    App_onOffCb},
	#endif
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL},
		{0,                      0                     , 0                 ,    NULL,             NULL,                  NULL}
		}
};

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      zcl_onOffAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_save(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
    bool needSave = FALSE;
    zcl_nv_onOff_t zcl_nv_onOff;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    if (st == NV_SUCC) {
        u32 curCRC = 0xffffffff;
        curCRC = xcrc32((u8 *)&zcl_nv_onOff, sizeof(zcl_nv_onOff_t) - sizeof(u32), curCRC);
        if(curCRC == zcl_nv_onOff.crc) {
        	//was something changed?
        	for(u8 idx=0;idx<5;idx++) {
        		//basicly changes on any of this variables will trigger flash save
				if(	g_zcl_onOffAttrs[idx].startUpOnOff != zcl_nv_onOff.attr[idx].startUpOnOff) {
					needSave = TRUE;
					break;
				}
        		//now complicated logic for onOff and startUpOnOff combinations
        		if( g_zcl_onOffAttrs[idx].startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS ||
        			g_zcl_onOffAttrs[idx].startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE) {
        			if(g_zcl_onOffAttrs[idx].onOff != zcl_nv_onOff.attr[idx].onOff) {
        				needSave = TRUE;
        				break;
        			}
        		}
        	}
        }
        else
        	needSave = TRUE;
    } else if (st == NV_ITEM_NOT_FOUND) {
        needSave = TRUE;
    }

    if (needSave) {
    	for(u8 idx=0;idx<5;idx++)
    		memcpy((u8 *)&zcl_nv_onOff.attr[idx], (u8 *)&g_zcl_onOffAttrs[idx], sizeof(zcl_onOffAttr_t));

        u32 curCRC = 0xffffffff;
        curCRC = xcrc32((u8 *)&zcl_nv_onOff, sizeof(zcl_nv_onOff_t) - sizeof(u32), curCRC);
        zcl_nv_onOff.crc = curCRC;

        st = nv_flashWriteNew(1, NV_MODULE_APP, NV_ITEM_APP_USER_CFG, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_onOffAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_restore(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
    zcl_nv_onOff_t zcl_nv_onOff;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);
    if (st == NV_SUCC) {
    	u32 curCRC = 0xffffffff;
    	curCRC = xcrc32((u8 *)&zcl_nv_onOff, sizeof(zcl_nv_onOff_t) - sizeof(u32), curCRC);
    	if(curCRC == zcl_nv_onOff.crc) {
    		//for each relay attribute structure determine do we need to actually perform action or not and finally copy var by var between flash readed value and running
            for(u8 idx=0;idx<5;idx++) {
            	if(zcl_nv_onOff.attr[idx].startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON)
            	{
            		cmdOnOff_on(SAMPLE_SWITCH_ENDPOINT + idx);
            		g_zcl_onOffAttrs[idx].onOff = ZCL_ONOFF_STATUS_ON;
            	}
            	else if(zcl_nv_onOff.attr[idx].startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE) {
            		if(zcl_nv_onOff.attr[idx].onOff == ZCL_ONOFF_STATUS_OFF) {
            			cmdOnOff_on(SAMPLE_SWITCH_ENDPOINT + idx);
            			g_zcl_onOffAttrs[idx].onOff = ZCL_ONOFF_STATUS_ON;
            		}
            	}
            	else if(zcl_nv_onOff.attr[idx].startUpOnOff == ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS) {
            		if(zcl_nv_onOff.attr[idx].onOff == ZCL_ONOFF_STATUS_ON) {
            			cmdOnOff_on(SAMPLE_SWITCH_ENDPOINT + idx);
            			g_zcl_onOffAttrs[idx].onOff = ZCL_ONOFF_STATUS_ON;
            		}
            	}
				g_zcl_onOffAttrs[idx].startUpOnOff = zcl_nv_onOff.attr[idx].startUpOnOff;
            }
    	}
    	else {
			st = NV_ITEM_NOT_FOUND;
		}
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

void zcl_nv_attr_reset(void)
{
	zcl_nv_onOff_t zcl_nv_onOff = {
		.attr = {
			{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
			{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
			{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
			{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1},
			{ 0x0000, 0x0000, ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF, 0x00, 1}
		},
		.crc = 0x00000000
	};

	u32 curCRC = 0xffffffff;
	curCRC = xcrc32((u8 *)&zcl_nv_onOff, sizeof(zcl_nv_onOff_t) - sizeof(u32), curCRC);
	zcl_nv_onOff.crc = curCRC;
	nv_flashWriteNew(1, NV_MODULE_APP, NV_ITEM_APP_USER_CFG, sizeof(zcl_nv_onOff_t), (u8 *)&zcl_nv_onOff);

	SYSTEM_RESET();
}

#endif	/* __PROJECT_TL_SWITCH__ */
