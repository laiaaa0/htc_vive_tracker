#include "htc_vive_tracker.h"
#include <openvr.h>
int main(int argc, char *argv[])
{
	bool verbose = false;
	if (argc>=2){
		verbose = true;
		//if (argv[1]=="true" or argv[1]=="1") verbose = true; 
	}
	CHtc_Vive_Tracker vt;
	if (vt.InitializeVR(verbose)){
		std::cout<<"Init done"<<std::endl;
		vt.PrintAllDetectedDevices();
		if (vt.IsDeviceDetected("tracker_1")){
			double pose [3];
			double quat [4];
			bool success = vt.GetDevicePoseQuaternion("tracker_1",pose,quat);
			std::cout<<"tracker 1 pose and quaternion : "<<std::endl;
			for (int i=0; i<3;++i) std::cout<<pose[i]<<" ";
			std::cout<<std::endl;
			
			for (int i=0; i<4;++i) std::cout<<quat[i]<<" ";
			std::cout<<std::endl;

			double angular_v[3];
			double linear_v[3];
			success = vt.GetDeviceVelocity("tracker_1",linear_v,angular_v);
			for (int i =0;i<3; ++i){
				std::cout<<angular_v[i]<<";" <<linear_v[i]<<std::endl;
			}
		}
		else {
			std::cout<<"Tracker 1 not detected"<<std::endl;
		}
	}
	else {
		std::cout<<"Init failed"<<std::endl;
	}
}
