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

    //boolean to execute with additional information or not
    bool verbose_;
    
    //Duration of the haptic pulse
    const uint32_t MAX_PULSE_DURATION = 3999;

    //Device names
    static constexpr const char* NAME_HMD = "hmd";
    static constexpr const char* NAME_CONTROLLER = "controller";
    static constexpr const char* NAME_TRACKER = "tracker";
    static constexpr const char* NAME_TREFERENCE = "tracking_reference";
    static constexpr const char* NAME_NULL = "invalid";

    //Auxiliary functions to perform transformations
    //Get the position with Z as the vertical component given a matrix
    Vec3 MatrixToPoseZVertical(const vr::HmdMatrix34_t & device_matrix);
    //Get the Quaternion given a matrix
    void MatrixToQuaternion(const vr::HmdMatrix34_t & device_matrix,double (&angle)[4]);
    //Update the last pose obtained by a device given its ID
    bool UpdateDevicePosition(const int device_id);


    //Auxiliar functions
    //Get the class to which the device belongs (tracker, controller, tracking_reference, hmd)
    std::string GetDeviceClass(const int device_id);
    //Set the device name by concatenating the class and the index within that class
    std::string SetDeviceName(const int device_id);
    //Add new device to the internal list of current active devices
    bool AddNewDevice(const int device_id);
    //Delete device from internal list of current active devices
    bool DeleteDevice(const int device_id);


  public:
  
    //Constructor and destructor
    CHtc_Vive_Tracker();
    ~CHtc_Vive_Tracker();
    
    //Initialize and shutdown functionalities
    bool InitializeVR(bool verbose);
    bool ShutDownVR();

    void Update();
    
    //Device detection
    //Check if device with a given name is detected
    bool IsDeviceDetected(const std::string & device_name);
    //Print all currently detected devices
    void PrintAllDetectedDevices();
    //Get names of all devicse
    std::vector<std::string> GetAllDeviceNames();
    //Get battery level percentage of the device
    float GetBatteryLevel(const std::string & device_name);
    //Check for new events
    bool EventPolling();


    //Obtain pose (position + quaternion) of a device given its name
    bool GetDevicePoseQuaternion(const std::string & device_name, double (&pose)[3], double (&angle)[4]);
    //Obtain pose (position + Euler angles) of a device given its name
    bool GetDevicePoseEuler(const std::string & device_name, double (&pose)[3], double & roll, double & pitch, double & yaw);
    //Obtain velocity of a device given its name
    Velocity GetDeviceVelocity(const std::string & device_name);



    //Chaperone - related functions
    // Get a vector of the four corners, and the size in X and Z
    Dimension GetChaperoneDimensions() const;

    //POGO pin interface - get pressed buttons and send vibrations
    std::string GetLastButtonPressedString(const std::string & device_name);
    //Get the last button pressed as obtained from the API
    vr::EVRButtonId GetLastButtonPressedEnum(const std::string & device_name);
    // Trigger a haptic pulse on a device given its name
    bool HapticPulse(const std::string & device_name, uint32_t axis_id, unsigned short duration_microsec);


};

#endif
