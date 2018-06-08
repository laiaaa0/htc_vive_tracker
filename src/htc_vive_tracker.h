#ifndef _HTC_VIVE_TRACKER_H
#define _HTC_VIVE_TRACKER_H

//System libraries
#include <iostream>
#include <vector>
#include <map>
#include <math.h>       /* fmax */

//Specific libraries
#include <openvr.h>

class CHtc_Vive_Tracker
    {
  private:
    //IVR System interface (main)
    vr::IVRSystem * vr_system_;
    //IVR Chaperone (to get boundaries and size)
    vr::IVRChaperone * vr_chaperone_;
    
    //array that contains the positions of all the detected devices
    vr::TrackedDevicePose_t device_poses_[vr::k_unMaxTrackedDeviceCount];

    //map to obtain the device id from the device string
    std::map<std::string,int> devices_id_;
    //vector to obtain the device name from the device id
    std::vector<std::string>devices_names_;
    // max number of devices
    int max_devices_;
    //Number of devices of each type
    int hmd_counts_=1, controller_counts_=1, tracker_counts_=1, track_reference_counts_=1, null_counts_=1;
    
    // function to initialize the data structures (devices_names_ and devices_id)
    void InitializeDeviceMap(bool verbose);
    
    
  public:
  
    //Constructor and destructor
    CHtc_Vive_Tracker();
    ~CHtc_Vive_Tracker();
    
    //Device names
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
    std::vector<std::string> GetAllDeviceNames();
    float GetBatteryLevel (const std::string & device_name);


    //Device position and velocity
    bool GetDevicePoseQuaternion (const std::string & device_name, double (&pose)[3], double (&angle)[4]);
    bool GetDevicePoseEuler (const std::string & device_name, double (&pose)[3], double & roll, double & pitch, double & yaw);
    bool GetDeviceVelocity (const std::string & device_name, double (&linear_velocity)[3], double (&angular_velocity)[3]);



    //Chaperone - related functions
    // Get a vector of the four corners, and the size in X and Z
    bool GetChaperoneDimensions (std::vector<std::vector<float> > & corners, float & pSizeX, float & pSizeZ);


    //Auxiliar functions
    std::string GetDeviceClass (const int device_id);
    std::string SetDeviceName (const int device_id);


    void MatrixToPoseZVertical(const vr::HmdMatrix34_t & device_matrix,double (&pose)[3]);
    void MatrixToQuaternion(const vr::HmdMatrix34_t & device_matrix,double (&angle)[4]);
    bool UpdateDevicePosition (const int device_id);
};

#endif
