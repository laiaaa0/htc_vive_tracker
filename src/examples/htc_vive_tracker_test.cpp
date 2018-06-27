#include "htc_vive_tracker.h"
#include <openvr.h>
int main(int argc, char *argv[])
{
        const std::string verbose_arg = "-v";
        const std::string events_arg = "-e";

	bool verbose = false;
	bool monitor_events = false;
	if (argc>=2){
		for (int i = 1; i < argc; ++i ) {
			if (argv[i] == verbose_arg) verbose = true;	
			if (argv[i] == events_arg) monitor_events = true;	
		}
	}
	CHtc_Vive_Tracker vt;

	if (vt.InitializeVR(verbose)){

		float sizeX, sizeZ;
		std::vector<std::vector<float> > play_corners;
		if (vt.GetChaperoneDimensions(play_corners,sizeX,sizeZ)){
			std::cout<<"Play area corners: "<<std::endl;

			for (uint i=0; i<play_corners.size(); ++i){
				std::cout<<"  ("<<play_corners[i][0]<<", "<<play_corners[i][1]<<", "<<play_corners[i][2]<<")"<<std::endl;
			}
			//size in meters
			std::cout<<"Play area size : "<<std::endl<<"  "<<sizeX<<" x  "<<sizeZ<<" m"<<std::endl;


		}
		else {
			std::cout<<"Could not calculate play area size"<<std::endl;
		}
		
		std::cout<<"Init done. Detected devices are :"<<std::endl;
		vt.PrintAllDetectedDevices();

		std::cout<<std::endl<<"**************************"<<std::endl;
		std::vector<std::string> list_of_devices = vt.GetAllDeviceNames();
		float battery_level = 0 ; 
		for (uint i = 0; i < list_of_devices.size(); ++i){
			double pose[3];
			double quat[4];
			double angular_v[3];
			double linear_v[3];
			if (vt.IsDeviceDetected(list_of_devices[i])){
				
				bool success = vt.GetDevicePoseQuaternion(list_of_devices[i],pose,quat);
				success = vt.GetDeviceVelocity (list_of_devices[i], linear_v, angular_v);
				if (success){
					std::cout<<list_of_devices[i]<<" Position"<<std::endl;
					for (int i=0; i<3;++i) std::cout<<pose[i]<<" ";
					std::cout<<std::endl;
					std::cout<<list_of_devices[i]<<" Quaternion"<<std::endl;
					for (int i=0; i<4;++i) std::cout<<quat[i]<<" ";
					std::cout<<std::endl;
					std::cout<<list_of_devices[i]<<" Linear Velocity"<<std::endl;
					for (int i=0; i<3;++i) std::cout<<linear_v[i]<<" ";
					std::cout<<std::endl;
					std::cout<<list_of_devices[i]<<" Angular Velocity"<<std::endl;
					for (int i=0; i<3;++i) std::cout<<angular_v[i]<<" ";
					battery_level = vt.GetBatteryLevel (list_of_devices[i]);
					std::cout<<std::endl<<"Battery level = "<<battery_level<<std::endl;
					std::cout<<std::endl<<"**************************"<<std::endl;
				}
			} else std::cout<<list_of_devices[i]<<" not detected"<<std::endl;


		}
		if (monitor_events){
		    while (vt.GetLastButtonPressed()!=BUTTON_MENU){
			if (vt.EventPolling()) {
				std::cout<<"Last button pressed : "<<std::endl;
				std::cout<<vt.GetLastButtonPressed()<<std::endl;
			}
				//vt.HapticPulse("tracker_1",0,3999);
		    }
		}
		
	} else {
		std::cout<<"Init failed"<<std::endl;
		if (!verbose) std::cout<<"Try executing with -v for more information"<<std::endl;
	}
}
