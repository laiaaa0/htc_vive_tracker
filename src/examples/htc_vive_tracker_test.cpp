#include "htc_vive_tracker.h"
#include <openvr.h>
int main(int argc, char *argv[])
{
	bool verbose = false;
	if (argc>=2){
		verbose = true;
	}
	CHtc_Vive_Tracker vt;

	if (vt.InitializeVR(verbose)){

		float sizeX, sizeZ;
		std::vector<std::vector<float> > play_corners;
		if (vt.GetChaperoneDimensions(play_corners,sizeX,sizeZ)){
			std::cout<<"Play area corners: "<<std::endl;

			for (int i=0; i<play_corners.size(); ++i){
				std::cout<<"  ("<<play_corners[i][0]<<", "<<play_corners[i][1]<<", "<<play_corners[i][2]<<")"<<std::endl;
			}
			//size in meters
			std::cout<<"Play area size : "<<std::endl<<sizeX<<" "<<sizeZ<<std::endl;


		}
		else {
			std::cout<<"Could not calculate play area size"<<std::endl;
		}
		
		std::cout<<"Init done. Detected devices are :"<<std::endl;
		vt.PrintAllDetectedDevices();

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
		
	} else {
		std::cout<<"Init failed"<<std::endl;
	}
}
