/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : app_config.h
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��13��
  ����޸�   :
  ��������   : ϵͳ����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��4��13��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
 typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
 #define GUI_DisplayScan()			DisplayScan()
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
 #include <stdio.h>
 #include <stdint.h>
 #include <string.h>
 #include "Platformconfig.h"
 #include "timeout_api.h"
 #include "app_debug.h"

 #if defined(STM32F030)
 #include "stm32f0xx.h"
 #include "stm32072b_eval.h"
 #include "stm32f0xx_conf.h"
 #include "single_pwm_led.h"
 #include "single_pwm_light.h"
 #include "func_keymsg.h"

 #elif defined(GD32E230)
 #include "gd32e230.h"
 #include "gd32e230c_eval.h"
 #include "gd32e230_libopt.h"

 #endif

#endif
