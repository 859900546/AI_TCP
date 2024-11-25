#include <map>
#include <string>
#include <fstream>
#pragma once
class Setting
{
public:
	std::string IP = "192.168.1.100";
	int Port = 5000;

	int LeftPressedAfterCancelAim_Time = 50;
	int MovedAfterActiveTriggerTime = 105;
	int ProcessHappenedEvent_ActiteTime = 2500;
	int ProcessNotEvent_SleepTime_H = 30;
	int ProcessNotEvent_SleepTime_B = 35;
	int TriggerDelayShot_Time = 80;
	int Per_Trigger_Time = 200;
	int Average_One_round_Time = 14;
	float Initial_st_Y_Rate = 0.26;
	float PID_Offset = 0.8;


	float Block_Rate_X = 0.3;
	float Block_Rate_Y = 0;
	float BlockThreading_SleepTime = 8;

	float HR_ThreadMove_First_X_Rate = 0.17;
	float HR_MoveingAndNotMaxAim_Rate = 1.0;
	float HR_MaxMaximum_X_Multiple = 2.0;
	float HR_MaxMaximum_Y_Multiple = 2.0;
	float HR_NotMaxAim_Y_Rate = 0.43;
	float HR_MaxAimOrOpenedMirror_X_Rate = 1.4;

	float HP_Pid_P = 0.38;
	float HP_Pid_I = 2;
	float HP_Pid_D = 0;
	float HP_Pid_Imax = 1.8;
	float HP_MaxMaximum_X_Multiple = 2.0;
	float HP_MaxMaximum_Y_Multiple = 2.0;
	float HP_LeftPressed_Y_Rate = 0;
	float HP_Normal_Y_Rate = 1;
	float HP_OpenedMirror_X_Rate = 1.4;

	float MP_First_X_Rate = 0.65;
	float MP_MaxMaximum_X_Multiple = 3.0;
	float MP_MaxMaximum_Y_Multiple = 3.0;
	float MP_Pid_P = 0.38;
	float MP_Pid_I = 2;
	float MP_Pid_D = 0;
	float MP_Pid_Imax = 1.8;
	float MP_LeftPressed_Y_Rate = 0;
	float MP_Y_Rate = 1.3;
	float MP_OpenedMirror_X_Rate = 1.4;


	float BR_First_Rate = 0.17;
	float BR_MoveingAndNotMaxAim_Rate = 1.0;
	float BR_MaxMaximum_X_Multiple = 2.0;
	float BR_MaxMaximum_Y_Multiple = 2.0;
	
	float BP_Pid_P = 0.38;
	float BP_Pid_I = 2;
	float BP_Pid_D = 0;
	float BP_Pid_Imax = 1.8;
	float BP_MaxMaximum_X_Multiple = 2.0;
	float BP_OpenedMirror_X_Rate = 1.4;

	float YQ_basis = 2;
	float YQ_Y_Increase = 0.2;
	float YQ_Length_Decline = 1;
	float YQ_MaxLength = 8;
	float YQ_Per_SleepTime = 16;
	Setting();
	~Setting();
};