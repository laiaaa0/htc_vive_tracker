#ifndef _HTC_VIVE_TRACKER_H
#define _HTC_VIVE_TRACKER_H

//System libraries
#include <iostream>
#include <vector>
#include <map>
#include <math.h>       /* fmax */
//Specific libraries
//#include "openvr.h"
#include <openvr.h>

class CHtc_Vive_Tracker
{
  private:
	vr::IVRSystem * vr_system_;
	vr::TrackedDevicePose_t device_poses_[vr::k_unMaxTrackedDeviceCount];
	std::map<std::string,int> devices_id_;
	std::vector<std::string>devices_names_;
	uint max_devices_;

	int hmd_counts_=1, controller_counts_=1, tracker_counts_=1, track_reference_counts_=1, null_counts_=1;
	void InitializeDeviceMap(bool verbose);
  public:
    CHtc_Vive_Tracker();
    ~CHtc_Vive_Tracker();
    static constexpr const char* NAME_HMD = "hmd";
    static constexpr const char* NAME_CONTROLLER = "controller";
    static constexpr const char* NAME_TRACKER = "tracker";
    static constexpr const char* NAME_TREFERENCE = "tracking_reference";
    static constexpr const char* NAME_NULL = "invalid";

//Initialize and shutdown functionalities
    bool InitializeVR (bool verbose);
    bool ShutDownVR (bool verbose);

    void Update (bool verbose);
//Device detection
    bool IsDeviceDetected (const std::string & device_name);
    void PrintAllDetectedDevices ();


//Device position 
    bool GetDevicePoseQuaternion (const std::string & device_name, double (&pose)[3], double (&angle)[4]);
    bool GetDevicePoseEuler (const std::string & device_name, double (&pose)[3], double & roll, double & pitch, double & yaw);
    bool GetDeviceVelocity (const std::string & device_name, double (&linear_velocity)[3], double (&angular_velocity)[3]);


//Auxiliar functions

    std::string GetDeviceName(const int device_id);
    std::string GetDeviceClass (const int device_id);
    std::string SetDeviceName (const int device_id);
    
    void MatrixToPose(const vr::HmdMatrix34_t & device_matrix,double (&pose)[3]);
    void MatrixToQuaternion(const vr::HmdMatrix34_t & device_matrix,double (&angle)[4]);
};

#endif
