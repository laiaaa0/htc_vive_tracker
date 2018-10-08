#include "htc_vive_tracker.h"
#include <openvr.h>

const std::string verbose_arg = "-v";
const std::string events_arg = "-e";
const std::string haptic_arg = "-p";
const std::string help_arg = "-h";

void ShowDimensionsPlay (const CHtc_Vive_Tracker & vt){
	Dimension chaperone_dim = vt.GetChaperoneDimensions();
	std::cout<<"Play area corners: "<<std::endl;
	PrintVec3(chaperone_dim.corner1);
	PrintVec3(chaperone_dim.corner2);
	PrintVec3(chaperone_dim.corner3);
	PrintVec3(chaperone_dim.corner4);
	//size in meters
	std::cout<<"Play area size : "<<std::endl<<"  "<<chaperone_dim.size_x<<" x  "<<chaperone_dim.size_z<<" m"<<std::endl;



}

void Usage (){
	std::cout<<"Use argument "<<verbose_arg<<" to " <<"see more information" << std::endl;
	std::cout<<"Use argument "<<events_arg<<" to " <<"loop while checking for events" << std::endl;
	std::cout<<"Use argument "<<haptic_arg<<" to " <<"trigger a haptic pulse on the tracker and controller after setup" << std::endl;
	std::cout<<"Use argument "<<help_arg<<" to " <<"see this help" << std::endl;
}

void PrintAllDevicesInformation (CHtc_Vive_Tracker & vt){
	std::cout<<std::endl<<"**************************"<<std::endl;
	std::vector<std::string> list_of_devices = vt.GetAllDeviceNames();
	float battery_level = 0 ; 
	for (uint i = 0; i < list_of_devices.size(); ++i){
		double pose[3];
		double quat[4];
		if (vt.IsDeviceDetected(list_of_devices[i])){
			bool success = vt.GetDevicePoseQuaternion(list_of_devices[i],pose,quat);
			Velocity velocity = vt.GetDeviceVelocity (list_of_devices[i]);
			if (success){
				std::cout<<list_of_devices[i]<<" Position"<<std::endl;
				for (int i=0; i<3; ++i) std::cout<<pose[i]<<" ";
				std::cout<<std::endl;
				std::cout<<list_of_devices[i]<<" Quaternion"<<std::endl;
				for (int i=0; i<4; ++i) std::cout<<quat[i]<<" ";
				std::cout<<std::endl;
				std::cout<<list_of_devices[i]<<" Linear Velocity"<<std::endl;
				PrintVec3 (velocity.linear_velocity);
				std::cout<<list_of_devices[i]<<" Angular Velocity"<<std::endl;
				PrintVec3 (velocity.linear_velocity);
				battery_level = vt.GetBatteryLevel (list_of_devices[i]);
				std::cout<<std::endl<<"Battery level = "<<battery_level<<std::endl;
				std::cout<<std::endl<<"**************************"<<std::endl;
				}
			} else std::cout<<list_of_devices[i]<<" not detected"<<std::endl;
		}
}
int main(int argc, char *argv[])
{	
	bool verbose = false;
	bool monitor_events = false;
	bool trigger_haptic_pulse = false;
	bool display_help = false;
	for (int i = 1; i < argc; ++i ) {
		if (argv[i] == verbose_arg) verbose = true;	
		if (argv[i] == events_arg) monitor_events = true;	
		if (argv[i] == haptic_arg) trigger_haptic_pulse = true;
		if (argv[i] == help_arg) display_help = true;
	}
	

	if (display_help){
		Usage();
		return 0;
	}

	CHtc_Vive_Tracker vt;

	if (vt.InitializeVR(verbose)){

		
        if(vt.GetAllDeviceNames().size()==1){
            std::cout<<"No devices detected. Check link box are connected and devices are paired"<<std::endl;
            return 0;
        }
		ShowDimensionsPlay(vt);
		std::cout<<"Init done."<<std::endl;
		vt.PrintAllDetectedDevices();

		if (verbose) {
			PrintAllDevicesInformation(vt);
		}
		if (trigger_haptic_pulse){
			vt.HapticPulse("tracker_1",0,3999);
			vt.HapticPulse("controller_1",0,3999);

		}
		if (monitor_events){
		    bool button_menu_pressed = false;
		    while (!button_menu_pressed){
			if (vt.EventPolling()) {
				std::cout<<"Last button pressed : "<<std::endl;
				std::cout<<vt.GetLastButtonPressedString("tracker_1")<<" ";
				std::cout<<vt.GetLastButtonPressedString("tracker_2")<<" ";
				std::cout<<vt.GetLastButtonPressedString("controller_1")<<std::endl;
				if (vt.GetLastButtonPressedEnum("tracker_1")==vr::k_EButton_ApplicationMenu or vt.GetLastButtonPressedEnum("controller_1") == vr::k_EButton_ApplicationMenu) button_menu_pressed = true;
			}
		    }
		}
		
	} else {
		std::cout<<"Init failed"<<std::endl;
		if (!verbose) std::cout<<"Try executing with -v for more information"<<std::endl;
	}
}
