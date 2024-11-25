#include "Setting.h"


Setting* setting;

Setting::Setting() {
	std::ifstream configFile("D:/config.ini");
	std::string line;
	std::map<std::string, std::string> config;

	if (!configFile.is_open()) {
		return;
	}
	else
		while (std::getline(configFile, line)) {
			// ÒÔ '=' Îª·Ö¸ô·û
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				config[key] = value;
			}
		}


	configFile.close();
	this->IP = config["IP"];
	this->Port = stoi(config["Port"]);

	this->LeftPressedAfterCancelAim_Time = stoi(config["LeftPressedAfterCancelAim_Time"]);
	this->MovedAfterActiveTriggerTime = stoi(config["MovedAfterActiveTriggerTime"]);
	this->ProcessHappenedEvent_ActiteTime = stoi(config["ProcessHappenedEvent_ActiteTime"]);
	this->ProcessNotEvent_SleepTime_H = stoi(config["ProcessNotEvent_SleepTime_H"]);
	this->ProcessNotEvent_SleepTime_B = stoi(config["ProcessNotEvent_SleepTime_B"]);
	this->TriggerDelayShot_Time = stoi(config["TriggerDelayShot_Time"]);
	this->Per_Trigger_Time = stoi(config["Per_Trigger_Time"]);
	this->Average_One_round_Time = stoi(config["Average_One_round_Time"]);
	this->Initial_st_Y_Rate = stof(config["Initial_st_Y_Rate"]);
	this->PID_Offset = stof(config["PID_Offset"]);

	this->Block_Rate_X = stof(config["Block_Rate_X"]);
	this->Block_Rate_Y = stof(config["Block_Rate_Y"]);
	this->BlockThreading_SleepTime = stof(config["BlockThreading_SleepTime"]);

	this->HR_ThreadMove_First_X_Rate = stof(config["HR_ThreadMove_First_X_Rate"]);
	this->HR_MoveingAndNotMaxAim_Rate = stod(config["HR_MoveingAndNotMaxAim_Rate"]);
	this->HR_MaxMaximum_X_Multiple = stof(config["HR_MaxMaximum_X_Multiple"]);
	this->HR_MaxMaximum_Y_Multiple = stof(config["HR_MaxMaximum_Y_Multiple"]);
	this->HR_NotMaxAim_Y_Rate = stof(config["HR_NotMaxAim_Y_Rate"]);
	this->HR_MaxAimOrOpenedMirror_X_Rate = stof(config["HR_MaxAimOrOpenedMirror_X_Rate"]);


	this->HP_Pid_P = stof(config["HP_Pid_P"]);
	this->HP_Pid_I = stof(config["HP_Pid_I"]);
	this->HP_Pid_D = stof(config["HP_Pid_D"]);
	this->HP_Pid_Imax = stof(config["HP_Pid_Imax"]);
	this->HP_MaxMaximum_X_Multiple = stof(config["HP_MaxMaximum_X_Multiple"]);
	this->HP_MaxMaximum_Y_Multiple = stof(config["HP_MaxMaximum_Y_Multiple"]);
	this->HP_LeftPressed_Y_Rate = stof(config["HP_LeftPressed_Y_Rate"]);
	this->HP_Normal_Y_Rate = stof(config["HP_Normal_Y_Rate"]);
	this->HP_OpenedMirror_X_Rate = stof(config["HP_OpenedMirror_X_Rate"]);

	this->MP_First_X_Rate = stof(config["MP_First_X_Rate"]);
	this->MP_MaxMaximum_X_Multiple = stof(config["MP_MaxMaximum_X_Multiple"]);
	this->MP_MaxMaximum_Y_Multiple = stof(config["MP_MaxMaximum_Y_Multiple"]);
	this->MP_Pid_P = stof(config["MP_Pid_P"]);
	this->MP_Pid_I = stof(config["MP_Pid_I"]);
	this->MP_Pid_D = stof(config["MP_Pid_D"]);
	this->MP_Pid_Imax = stof(config["MP_Pid_Imax"]);
	this->MP_LeftPressed_Y_Rate = stof(config["MP_LeftPressed_Y_Rate"]);
	this->MP_Y_Rate = stof(config["MP_Y_Rate"]);
	this->MP_OpenedMirror_X_Rate = stof(config["MP_OpenedMirror_X_Rate"]);


	this->BR_First_Rate = stof(config["BR_First_Rate"]);
	this->BR_MoveingAndNotMaxAim_Rate = stof(config["BR_MoveingAndNotMaxAim_Rate"]);
	this->BR_MaxMaximum_X_Multiple = stof(config["BR_MaxMaximum_X_Multiple"]);
	this->BR_MaxMaximum_Y_Multiple = stof(config["BR_MaxMaximum_Y_Multiple"]);

	this->BP_Pid_P = stof(config["BP_Pid_P"]);
	this->BP_Pid_I = stof(config["BP_Pid_I"]);
	this->BP_Pid_D = stof(config["BP_Pid_D"]);
	this->BP_Pid_Imax = stof(config["BP_Pid_Imax"]);
	this->BP_MaxMaximum_X_Multiple = stof(config["BP_MaxMaximum_X_Multiple"]);
	this->BP_OpenedMirror_X_Rate = stof(config["BP_OpenedMirror_X_Rate"]);

	this->YQ_basis = stof(config["YQ_basis"]);
	this->YQ_Y_Increase = stof(config["YQ_Y_Increase"]);
	this->YQ_Length_Decline = stof(config["YQ_Length_Decline"]);
	this->YQ_MaxLength = stof(config["YQ_MaxLength"]);
	this->YQ_Per_SleepTime = stof(config["YQ_Per_SleepTime"]);
}

Setting::~Setting() {
	
}

