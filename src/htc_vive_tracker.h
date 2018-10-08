#ifndef _HTC_VIVE_TRACKER_H
#define _HTC_VIVE_TRACKER_H

//System libraries
#include <iostream>
#include <vector>
#include <map>
#include <math.h>       /* fmax */

//Specific libraries
#include <openvr.h>

struct Vec3 {
	double x;
	double y;
	double z;
};

struct Velocity {
	Vec3 linear_velocity;
	Vec3 angular_velocity;
};

struct Dimension {
	Vec3 corner1;
	Vec3 corner2;
	Vec3 corner3;
	Vec3 corner4;

	float size_x;
	float size_z;
};

enum EventFlags {
	BUTTONPRESS,
	BUTTONUNPRESS
};

enum ButtonFlags {
	BUTTON_OUT,
	BUTTON_GND,
	BUTTON_GRIP,
	BUTTON_TRIGGER,
	BUTTON_TOUCHPAD,
	BUTTON_MENU,
	BUTTON_SYSTEM,
	BUTTON_OTHER
};
/*
vr::k_EButton_Grip
vr::k_EButton_SteamVR_Touchpad
vr::k_EButton_ApplicationMenu
vr::k_EButton_System
*/




void PrintVec3(const Vec3 & v);

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
    std::map<std::string,uint32_t> devices_id_;
    //vector to obtain the device name from the device id
    std::vector<std::string>devices_names_;
    //vector to obtain the last pressed button from the device id
    std::vector<vr::EVRButtonId>last_button_pressed_;
    // max number of devices
    uint32_t max_devices_;
    //Number of devices of each type
    int hmd_counts_=1, controller_counts_=1, tracker_counts_=1, track_reference_counts_=1, null_counts_=1;
    
    // function to initialize the data structures (devices_names_ and devices_id)
    void InitializeDeviceMap();
    
    EventFlags events_;

    bool verbose_;
    
    const uint32_t MAX_PULSE_DURATION = 3999;

    //Device names
    static constexpr const char* NAME_HMD = "hmd";
    static constexpr const char* NAME_CONTROLLER = "controller";
    static constexpr const char* NAME_TRACKER = "tracker";
    static constexpr const char* NAME_TREFERENCE = "tracking_reference";
    static constexpr const char* NAME_NULL = "invalid";


  public:
  
    //Constructor and destructor
    CHtc_Vive_Tracker();
    ~CHtc_Vive_Tracker();
    
    //Initialize and shutdown functionalities
    bool InitializeVR(bool verbose);
    bool ShutDownVR();

    void Update();
    
    //Device detection
    bool IsDeviceDetected(const std::string & device_name);
    void PrintAllDetectedDevices();
    std::vector<std::string> GetAllDeviceNames();
    float GetBatteryLevel(const std::string & device_name);
    bool EventPolling();


    //Device position and velocity
    bool GetDevicePoseQuaternion(const std::string & device_name, double (&pose)[3], double (&angle)[4]);
    bool GetDevicePoseEuler(const std::string & device_name, double (&pose)[3], double & roll, double & pitch, double & yaw);
    Velocity GetDeviceVelocity(const std::string & device_name);



    //Chaperone - related functions
    // Get a vector of the four corners, and the size in X and Z
    Dimension GetChaperoneDimensions() const;


    //Auxiliar functions
    std::string GetDeviceClass(const int device_id);
    std::string SetDeviceName(const int device_id);
    bool AddNewDevice(const int device_id);
    bool DeleteDevice(const int device_id);

    //POGO pin interface - get pressed buttons and send vibrations
    std::string GetLastButtonPressedString(const std::string & device_name);
    vr::EVRButtonId GetLastButtonPressedEnum(const std::string & device_name);
    bool HapticPulse(const std::string & device_name, uint32_t axis_id, unsigned short duration_microsec);

    Vec3 MatrixToPoseZVertical(const vr::HmdMatrix34_t & device_matrix);
    void MatrixToQuaternion(const vr::HmdMatrix34_t & device_matrix,double (&angle)[4]);
    bool UpdateDevicePosition(const int device_id);

};

#endif
