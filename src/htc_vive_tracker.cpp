#include "htc_vive_tracker.h"

CHtc_Vive_Tracker::CHtc_Vive_Tracker() {

}
 
CHtc_Vive_Tracker::~CHtc_Vive_Tracker() {
	this->ShutDownVR();
}

void CHtc_Vive_Tracker::InitializeDeviceMap() {
	int num_detected_devices = max_devices_;
	if (this->verbose_) std::cout<<"Detected devices:"<<std::endl;	
	std::string device_name;
	for (int i = 0; i<num_detected_devices; ++i){
		if (device_poses_[i].bDeviceIsConnected && device_poses_[i].bPoseIsValid){
			device_name = this->SetDeviceName(i);
			if (this->verbose_) std::cout<<i<<" :  "<<device_name<<std::endl;
			devices_id_[device_name] = i;
			devices_names_[i] = device_name;
		}
	}
}


void PrintVec3(const Vec3 & vector_) {
	std::cout<<vector_.x<<" "<<vector_.y<<" "<<vector_.z<<std::endl;
}
// HtcViveTrackerAlgorithm Public API
//Initialize and shutdown functionalities
bool CHtc_Vive_Tracker::InitializeVR(bool verbose) {
	this->verbose_ = verbose;
        bool runtime_ok  = vr::VR_IsRuntimeInstalled();
        bool hmd_present = vr::VR_IsHmdPresent();
        vr::EVRInitError er;
        this->vr_system_ = vr::VR_Init (&er, vr::VRApplication_Background);
        std::string init_error = vr::VR_GetVRInitErrorAsSymbol(er);

	if (this->verbose_){
                std::cout<<"VR is runtime installed : " <<runtime_ok<<std::endl;
                std::cout<<"VR is HMD present : " <<hmd_present<<std::endl;
                std::cout<<"VR init error : " <<er<<init_error<<std::endl;
        }

        if (runtime_ok && hmd_present && er==vr::VRInitError_None) {
		this->max_devices_ = vr::k_unMaxTrackedDeviceCount;
		for (uint i = 0; i < this->max_devices_; ++i){
			devices_names_.push_back("");
			last_button_pressed_.push_back(vr::k_EButton_Max);
		}
		this->vr_chaperone_ = (vr::IVRChaperone *)vr::VR_GetGenericInterface(vr::IVRChaperone_Version, &er);
		if (er == 0) {
			if (this->verbose_) std::cout<<"Chaperone initialized correctly"<<std::endl;
		} else {
			if (this->verbose_) std::cout<<"Problem initializing chaperone"<<std::endl;
			return false;
		}
		this->Update();
	 	this->InitializeDeviceMap();
		return true;
        }
	else {
		if (this->verbose_){
			std::cout<<"Problem initializing VR"<<std::endl;
		}
		return false;
	}
}

bool CHtc_Vive_Tracker::ShutDownVR() {
 	if (this->vr_system_){
      		vr::VR_Shutdown();
	if (this->verbose_){
		std::cout<<"Device is shut down"<<std::endl;
	}
		return true;
	} 
	else {
		return false;
	}
}
 

void CHtc_Vive_Tracker::Update() {
	this->vr_system_->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, device_poses_, max_devices_);
	for (vr::TrackedDeviceIndex_t device_index  = vr::k_unTrackedDeviceIndex_Hmd; device_index < max_devices_; ++device_index){
		if (device_poses_[device_index].bDeviceIsConnected && device_poses_[device_index].bPoseIsValid){
			if (this->verbose_) {
				std::string info = ("device["+std::to_string(device_index)+"]: " + this->GetDeviceClass(device_index) + " " + std::to_string (device_poses_[device_index].eTrackingResult));
				std::cout<<info<<std::endl;
			}
		}
	}
}

//Device detection
bool CHtc_Vive_Tracker::IsDeviceDetected(const std::string & device_name) {
	if (devices_id_.find(device_name) == devices_id_.end()){
		return false;
	}
	uint32_t device_index = devices_id_[device_name];
	if (device_index < max_devices_){
		return this->vr_system_->IsTrackedDeviceConnected(device_index);
	}
	else return false;
}

void CHtc_Vive_Tracker::PrintAllDetectedDevices() {
	for (vr::TrackedDeviceIndex_t device_index  = vr::k_unTrackedDeviceIndex_Hmd; device_index < max_devices_; ++device_index){
		if (device_poses_[device_index].bDeviceIsConnected && device_poses_[device_index].bPoseIsValid){
			std::string device_name = devices_names_[device_index];
			std::string info = ("device["+std::to_string(device_index)+"]: " + device_name + " is connected");
			std::cout<<info<<std::endl;
		}
	}
}


float CHtc_Vive_Tracker::GetBatteryLevel(const std::string & device_name) {
	if (devices_id_.find(device_name) == devices_id_.end()){
		return 0;
	}
	float level = 1.0;
	uint32_t device_id = devices_id_[device_name];
	vr::ETrackedDeviceProperty device_property = vr::Prop_DeviceBatteryPercentage_Float;
	vr::ETrackedPropertyError error;
	std::string device_class = this->GetDeviceClass(device_id);
	//Only return battery if device is not connected (controller or tracker)
	if (device_class == NAME_CONTROLLER || device_class == NAME_TRACKER){
		if (device_id < max_devices_){
			level = this->vr_system_->GetFloatTrackedDeviceProperty(device_id, device_property, &error);
		}
	}
	return level;
}

std::vector<std::string> CHtc_Vive_Tracker::GetAllDeviceNames() {
	std::vector<std::string> non_empty_device_names;	
	for (uint i = 0; i<devices_names_.size(); ++i){
		if (devices_names_[i]!=""){
			non_empty_device_names.push_back(devices_names_[i]);
		}
	}
	return non_empty_device_names;
}

bool CHtc_Vive_Tracker::EventPolling() {
	vr::VREvent_t event;
	if (this->vr_system_->PollNextEvent(&event, sizeof(event))){
		std::cout<<this->vr_system_->GetEventTypeNameFromEnum((vr::EVREventType)event.eventType)<<std::endl;
		switch (event.eventType){
			case vr::VREvent_TrackedDeviceActivated:
				this->AddNewDevice(event.trackedDeviceIndex);
				break;
			case vr::VREvent_TrackedDeviceDeactivated:
				this->DeleteDevice(event.trackedDeviceIndex);
				break;
			case vr::VREvent_ButtonPress:
				events_ = BUTTONPRESS;
				last_button_pressed_[event.trackedDeviceIndex] = (vr::EVRButtonId)event.data.controller.button;
				break;
			case vr::VREvent_ButtonUnpress:
				events_ = BUTTONUNPRESS;
				break;
			case vr::VREvent_ButtonTouch:
				break;
			case vr::VREvent_TrackedDeviceRoleChanged:
				break;
				
			default:
				break;
		}
		return true;
	}
	return false;
}
//Device position 
bool CHtc_Vive_Tracker::GetDevicePoseQuaternion(const std::string & device_name,double (&pose)[3], double (&angle)[4]) {
	if (devices_id_.find(device_name) == devices_id_.end()){
		return false;
	}
	int device_index = devices_id_[device_name];
        if (this-> UpdateDevicePosition (device_index)){
		vr::TrackedDevicePose_t current_device_pose = this->device_poses_[device_index];
		if (current_device_pose.bDeviceIsConnected && current_device_pose.bPoseIsValid){
			vr::HmdMatrix34_t device_matrix = current_device_pose.mDeviceToAbsoluteTracking;
			Vec3 pose_vec3 = this->MatrixToPoseZVertical(device_matrix);
			pose[0] = pose_vec3.x;
			pose[1] = pose_vec3.y;
			pose[2] = pose_vec3.z;
			this->MatrixToQuaternion(device_matrix,angle);
			std::string info = std::to_string (current_device_pose.eTrackingResult);
			return true;
		}
	}
	return false;
}
bool CHtc_Vive_Tracker::GetDevicePoseEuler(const std::string & device_name, double (&pose)[3], double & roll, double & pitch, double &yaw) {
	double quaternion[4];
	if (this->GetDevicePoseQuaternion(device_name,pose,quaternion)){
		double sinr = 2 * (quaternion[3]*quaternion[0] + quaternion[1]*quaternion[2]);
		double cosr = 1 - (2*(quaternion[0]*quaternion[0]+quaternion[1]*quaternion[1]));

		roll = atan2 (sinr,cosr);

		double sinp = 2 * (quaternion[3]*quaternion[1] - quaternion[2]*quaternion[0]);
		if (fabs(sinp)>=1){
			pitch = copysign (M_PI/2, sinp);
		} else {
      			pitch = asin (sinp);
		}
		double siny = 2 * (quaternion[4]*quaternion[3] + quaternion[0]*quaternion[1]);
		double cosy = 1 - (2*(quaternion[1]*quaternion[1] + quaternion[2]*quaternion[2]));
		yaw = atan2(siny,cosy);
		return true;
	}	
	return false;
}

Velocity CHtc_Vive_Tracker::GetDeviceVelocity(const std::string & device_name) {
	Velocity velocity;
	this->vr_system_->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, device_poses_, max_devices_);
	uint32_t device_index = devices_id_[device_name];
	if (device_index < this->max_devices_){
		if (device_poses_[device_index].bDeviceIsConnected && device_poses_[device_index].eTrackingResult == vr::TrackingResult_Running_OK){
			for (int i = 0; i < 3; ++i){
				velocity.linear_velocity.x = device_poses_[device_index].vVelocity.v[0];
				velocity.linear_velocity.y = device_poses_[device_index].vVelocity.v[1];
				velocity.linear_velocity.z = device_poses_[device_index].vVelocity.v[2];
				velocity.angular_velocity.x = device_poses_[device_index].vAngularVelocity.v[0];
				velocity.angular_velocity.y = device_poses_[device_index].vAngularVelocity.v[1];
				velocity.angular_velocity.z = device_poses_[device_index].vAngularVelocity.v[2];
			}
		}
	}
	return velocity;
}


Dimension CHtc_Vive_Tracker::GetChaperoneDimensions() const {
	vr::HmdQuad_t rect;
	Dimension chaperone_dimension;
	std::vector<float>pose(3);
	if(this->vr_chaperone_->GetPlayAreaSize(&chaperone_dimension.size_x, &chaperone_dimension.size_z)){
		if (this->vr_chaperone_->GetPlayAreaRect(& rect)){
			chaperone_dimension.corner1.x = rect.vCorners[0].v[0];
			chaperone_dimension.corner1.y = rect.vCorners[0].v[1];
			chaperone_dimension.corner1.z = rect.vCorners[0].v[2];

			chaperone_dimension.corner2.x = rect.vCorners[1].v[0];
			chaperone_dimension.corner2.y = rect.vCorners[1].v[1];
			chaperone_dimension.corner2.z = rect.vCorners[1].v[2];

			chaperone_dimension.corner3.x = rect.vCorners[2].v[0];
			chaperone_dimension.corner3.y = rect.vCorners[2].v[1];
			chaperone_dimension.corner3.z = rect.vCorners[2].v[2];

			chaperone_dimension.corner4.x = rect.vCorners[3].v[0];
			chaperone_dimension.corner4.y = rect.vCorners[3].v[1];
			chaperone_dimension.corner4.z = rect.vCorners[3].v[2];
			
		} 	
	}
	return chaperone_dimension;
}
//https://github.com/osudrl/CassieVrControls/wiki/OpenVR-Quick-Start
std::string CHtc_Vive_Tracker::GetDeviceClass(const int device_id) {
    vr::ETrackedDeviceClass tracked_device_class = this->vr_system_ ->GetTrackedDeviceClass (device_id);
    if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller){
	return NAME_CONTROLLER;
    } else if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_HMD){
	return NAME_HMD;
    } else if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference){
	return NAME_TREFERENCE;
    } else if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker){
    	return NAME_TRACKER;
    } else if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_Invalid){
	return NAME_NULL;
    } else return NAME_NULL;
}


bool CHtc_Vive_Tracker::AddNewDevice(const int device_id) {
	std::string name = this->SetDeviceName (device_id);
	devices_id_[name] = device_id;
	devices_names_[device_id] = name;
	return true;
}
bool CHtc_Vive_Tracker::DeleteDevice(const int device_id) {
	std::string name = devices_names_[device_id];
	devices_names_[device_id] = "";
	devices_id_.erase(name);
	return true;
}


std::string CHtc_Vive_Tracker::SetDeviceName(const int device_id) {
	std::string class_name = this->GetDeviceClass (device_id);
	std::string device_name;
	if (class_name == NAME_CONTROLLER){
		device_name = class_name+"_"+std::to_string(controller_counts_);
		controller_counts_++;
	} else if (class_name == NAME_HMD) {
		device_name = class_name+"_"+std::to_string(hmd_counts_);
		hmd_counts_++;
	} else if (class_name == NAME_TREFERENCE) {
		device_name = class_name+"_"+std::to_string(track_reference_counts_);
		track_reference_counts_++;
	} else if (class_name == NAME_TRACKER){
		device_name = class_name+"_"+ std::to_string (tracker_counts_);
		tracker_counts_++;
	} else if (class_name == NAME_NULL){
		device_name = class_name +"_"+ std::to_string (null_counts_);
		null_counts_++;
	} else device_name = NAME_NULL;
	
	return device_name;
}


//https://github.com/osudrl/CassieVrControls/wiki/OpenVR-Quick-Start              
Vec3 CHtc_Vive_Tracker::MatrixToPoseZVertical(const vr::HmdMatrix34_t & matrix) {
	// matrix.m[1] is the vertical component.
	// we want that on the third component of our pose. (Z is up)
	// we also want the coordinate system to be direct-cartesian
	Vec3 pose;
	pose.x = matrix.m[2][3]; // X
	pose.y = matrix.m[0][3]; // Y
	pose.z = matrix.m[1][3]; // Z
	return pose;
}

void CHtc_Vive_Tracker::MatrixToQuaternion(const vr::HmdMatrix34_t & matrix,double (&quaternion)[4]) {
	//w
	quaternion[3] = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	//x
	quaternion[0] = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	//y
	quaternion[1] = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	//z
	quaternion[2] = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	quaternion[0] = copysign(quaternion[0], matrix.m[2][1] - matrix.m[1][2]);
	quaternion[1] = copysign(quaternion[1], matrix.m[0][2] - matrix.m[2][0]);
	quaternion[2] = copysign(quaternion[2], matrix.m[1][0] - matrix.m[0][1]);
}


bool CHtc_Vive_Tracker::UpdateDevicePosition(const int device_id) {
	vr::ETrackedDeviceClass device_class = this->vr_system_->GetTrackedDeviceClass(device_id);
	vr::TrackedDevicePose_t tracked_device_pose;
	vr::VRControllerState_t controller_state;
        this->vr_system_->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, this->device_poses_, this->max_devices_);
	if (device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller or device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker){
		this->vr_system_->GetControllerStateWithPose(vr::TrackingUniverseStanding, device_id, &controller_state, sizeof(controller_state), &tracked_device_pose);
		this->device_poses_[device_id] = tracked_device_pose;
	}
	else if (device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_Invalid) return false;
	return true;
}


std::string CHtc_Vive_Tracker::GetLastButtonPressedString(const std::string & device_name) {
	return this->vr_system_->GetButtonIdNameFromEnum(this->GetLastButtonPressedEnum(device_name));
}

vr::EVRButtonId CHtc_Vive_Tracker::GetLastButtonPressedEnum(const std::string & device_name) {
	if (devices_id_.find(device_name) == devices_id_.end()) {
		return vr::k_EButton_Max;
	}
	uint32_t device_index = devices_id_[device_name];
	return this->last_button_pressed_[device_index];
}

bool CHtc_Vive_Tracker::HapticPulse(const std::string & device_name, uint32_t axis_id, unsigned short duration_microsec) {
	if (devices_id_.find(device_name) == devices_id_.end()){
		return false;
	}
	if (duration_microsec > this->MAX_PULSE_DURATION) duration_microsec = this->MAX_PULSE_DURATION;
	uint32_t device_index = devices_id_[device_name];
	this->vr_system_->TriggerHapticPulse(device_index, axis_id, duration_microsec);
	return true;
}
