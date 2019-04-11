///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: bt_hf_control.c
//  maintainer: Halley
///////////////////////////////////////////////////////////////////////////////

#include "app_config.h"
#include "sys_app.h"
//#include "player_control.h"
#include "bt_stack_api.h"
#include "bt_control_api.h"
#include "bt_app_func.h"
#include "pcm_fifo.h"
#include "dev_detect_driver.h"
#include "sd_card.h"
#include "dac.h"
#include "pcm_sync.h"
#include "sys_vol.h"
#include "recorder_control.h"
#include "mixer.h"
#include "sound_remind.h"
#include "clk.h"
#ifdef FUNC_DISP_EN
#include "led_display.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#endif
#ifdef FUNC_AIP1629A_LED_EN
#include "aip1629a.h"
#endif

#ifdef FUNC_BT_HF_EN

static bool		bt_hf_audio_enable = FALSE;

bool GetHfTransferState(void)
{
	if(!bt_hf_audio_enable)
	{
		return FALSE;
	}
	return IsScoConneted();
}

bool BtHfControl(void)
{
	uint16_t 	Msg = 0;
	//bool		bt_hf_audio_enable = FALSE;
	
	APP_DBG("[BT] BtHfControl Entry\n");
#ifndef FUNC_MIXER_SRC_EN
	AudioSampleRateSet(44100);
#endif
	LockHfMode();
	if(GetHfModeState() == BT_HF_MODE_STATE_ENTERING)
	{
		SetHfModeState(BT_HF_MODE_STATE_ACTIVE);
	}
	UnLockHfMode();
	InitBtHfTimer();

	/*�������HF֮ǰ���ֻ��ʹ��ڵ绰״̬������ͨ����ʱ��*/
	if(GetHfpState() == BT_HFP_STATE_ATCTIVE_CALL && IsBtHfTimerInited())
	{
		StartBtHfTimer();
	}

//	while(Msg != MSG_COMMON_CLOSE)
	while(GetHfModeState() != BT_HF_MODE_STATE_EXITING)
	{
		CheckBtHfTimer();

#ifdef BT_HF_NUMBER_REMIND
		/*���籨��*/
		if(IsRemindingPhoneNumbers() && (GetHfpState() == BT_HFP_STATE_INCOMING_CALL) && !IsSoundRemindPlaying())
		{
			uint8_t			number;
			number = GetNextNumber();
			if(number != END_OF_PHONE_NUM)
			{
				RemindNumber(number);
			}
			else
			{
				StopRemindingPhoneNumbers();
			}
		}

		/*
		* Ϊ��ֹֹͣ�������뵼��SoundRemindֹͣ��־δ������
		*/
		if(!IsRemindingPhoneNumbers())
		{
			ResetSoundRemindStopFlag();
		}
#endif
		/*SCO�����󣬴�ͨ·��*/
#ifdef BT_HF_NUMBER_REMIND
		if(!bt_hf_audio_enable && (GetHfpState() > 3/*BT_HFP_STATE_INCOMING_CALL*/))
		//�����ж�HFP״̬�Ƿ���BT_HFP_STATE_OUTGOING_CALL��4����BT_HFP_STATE_ATCTIVE_CALL��5����
#else
		if(!bt_hf_audio_enable && IsScoConneted())
#endif
		{
			PcmSyncInit(MV_MCU_AS_PCMSYNC_CLK_SLAVE);
			WaitMs(50); // ����POP��
			CodecDacMuteSet(FALSE, FALSE);
			/*����HF����*/
			SetBtHfVol(GetBtHfVol());
			bt_hf_audio_enable = TRUE;
		}

#ifdef BT_HF_RECORD_FUNC
		/* ¼��д�̣�ÿ2K����дһ���� */
		if(GetBtRecordState() == BT_RECORD_STATE_RECORDING)
		{
			EncodedDataBufferToFile(2048);
			Msg = MsgRecv(2);
		}
		else if(GetBtRecordState() == BT_RECORD_STATE_EXITING)
		{
			/* �����buff�е�����д����*/
//			extern uint32_t EncodedBufDataLen;
			
			APP_DBG("Exiting BT Recording\n");
			EncodedDataBufferToFile(EncodedBufDataLen);
			/*д���˳�¼��*/
			RecoderExit(WAV_FORMAT);
			SetBtRecordState(BT_RECORD_STATE_NONE);
			Msg = MsgRecv(5);
		}
		else
#endif
		{
			Msg = MsgRecv(20);
		}

		switch(Msg)
		{
			case MSG_STOP:
				switch(GetHfpState())
				{
					case BT_HFP_STATE_INCOMING_CALL:
					case BT_HFP_STATE_OUTGOING_CALL:
						BTInCallReject();
						break;

					case BT_HFP_STATE_ATCTIVE_CALL:
						BTCallHangUp();
						break;

					case BT_HFP_STATE_3WAY_INCOMING_CALL:
					case BT_HFP_STATE_3WAY_OUTGOING_CALL:
						BTInCallReject();
						break;
						
					case BT_HFP_STATE_3WAY_ATCTIVE_CALL:
						BTCallHangUp();
						SetHfpState(BT_HFP_STATE_ATCTIVE_CALL);
						break;
						
					default:
						break;
				}
				break;
				
			case MSG_PLAY_PAUSE: 
				switch(GetHfpState())
				{
					case BT_HFP_STATE_INCOMING_CALL:
						BTInCallAccept();
						break;

					case BT_HFP_STATE_OUTGOING_CALL:
						BTInCallReject();
						break;

					case BT_HFP_STATE_ATCTIVE_CALL:
						BTCallHangUp();
						break;

					case BT_HFP_STATE_3WAY_INCOMING_CALL:
						//�Ҷ�����ͨ���ĵ绰,��������绰
						//BTCallHold(HF_HOLD_RELEASE_ACTIVE_CALLS);
						break;
						
					case BT_HFP_STATE_3WAY_OUTGOING_CALL:
						BTInCallReject();
						break;
						
					case BT_HFP_STATE_3WAY_ATCTIVE_CALL:
						BTCallHangUp();
						SetHfpState(BT_HFP_STATE_ATCTIVE_CALL);
						break;
						
					default:
						break;

				}
				break;

			case MSG_VOL_UP:
				APP_DBG("[BT] MSG_VOL_UP\n");
				{
					uint8_t vol = GetBtHfVol();
					if(vol < MAX_BTHF_VOLUME)
						vol++;
					SetBtHfVol(vol);
					BTPhoneCallVolSyncSet(vol);
				}
#ifdef FUNC_DISP_EN
				DispSendMessage(INTERIM_INTERVAL, DISP_MSG_BTHF_VOL);
#endif

				break;
				
			case MSG_VOL_DW:
				APP_DBG("[BT] MSG_VOL_DW\n");
				{
					uint8_t vol = GetBtHfVol();
					if(vol > 0)
						vol--;
					SetBtHfVol(vol);
					BTPhoneCallVolSyncSet(vol);
				}
#ifdef FUNC_DISP_EN
				DispSendMessage(INTERIM_INTERVAL, DISP_MSG_BTHF_VOL);
#endif
				break;

			case MSG_PRE:
			case MSG_NEXT:
				APP_DBG("[BT] MSG_MODE or MSG_PRE or MSG_NEXT\n");
				{
					//BTStartHfTransfer();
				}
				break;

#ifdef BT_HF_RECORD_FUNC
			case MSG_REC:
				APP_DBG("[BT] MSG_REC\n");
				if(GetBtRecordState() == BT_RECORD_STATE_NONE)
				{
					if(GetHfpState() > BT_HFP_STATE_CONNECTED)
						BtHfRecordStart();
				}
				else
				{
					BtHfRecordExit();
				}
				break;
#endif
			case MSG_POWER:
				APP_DBG("[BT] MSG_POWER\n");
				BTDisConnect();
			#ifdef FUNC_WIFI_POWER_KEEP_ON
				WiFiRequestMcuPowerOff();
			#else
				gSys.NextModuleID = MODULE_ID_STANDBY;
				Msg = MSG_COMMON_CLOSE;
			#endif
				break;

			//��APϵ���Դ�������������ͨ�������в�����ʾ����
			case MSG_BT_PHONE_CALL_INCOMING:
				SoundRemind(SOUND_CALL_INCOM);
				break;

			default:
				CommonMsgProccess(Msg);
				break;
		}
	}
	APP_DBG("[BT] end BtHfControl \n");
	//��ֹ�Ҷϵ绰�󲥷�������ʾ��
	gSys.ModuleIDBackUp = MODULE_ID_BT_HF;
	bt_hf_audio_enable = FALSE;

#ifdef BT_HF_RECORD_FUNC
	if(GetBtRecordState() == BT_RECORD_STATE_EXITING)
	{
		/* �����buff�е�����д����*/
//		extern uint32_t EncodedBufDataLen;

		APP_DBG("Exiting BT Recording\n");
		EncodedDataBufferToFile(EncodedBufDataLen);
		/*д���˳�¼��*/
		RecoderExit(WAV_FORMAT);
		SetBtRecordState(BT_RECORD_STATE_NONE);
	}
#endif

	DacSoftMuteSet(TRUE, TRUE);
	DeinitBtHfTimer();
	SetHfModeState(BT_HF_MODE_STATE_NONE);
	PcmSyncUnInit();
	AudioAnaSetChannel(AUDIO_CH_NONE);
    AudioSampleRateSet(32000);
	AudioAnaInit();	
	/*�ָ�Mixer״̬*/
	MixerInit((void*)PCM_FIFO_ADDR, PCM_FIFO_LEN);
//	if(GetHistroyModeId() != MODULE_ID_UNKNOWN && gSys.NextModuleID != MODULE_ID_STANDBY)
//	{
//		gSys.NextModuleID = GetHistroyModeId();
//	}
#ifdef FUNC_OUTPUT_CHANNEL_AUTO_CHANGE
	DacConfig(MDAC_DATA_FROM_DEC, NORMAL_MODE);
	DacAdcSampleRateSet(44100, NORMAL_MODE);	 //���ֲ���
#else
    AudioOutputInit();//���ͨ����ʼ��
#endif
//	CodecDacMuteSet(FALSE, FALSE);

	return TRUE;
}

#endif/*FUNC_BT_HF_EN*/



