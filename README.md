# Readme

Copyright (C) 2009-2010 Institut de Robòtica i Informàtica Industrial, CSIC-UPC.
Author pmlab (pmlab@iri.upc.edu)
All rights reserved.

This file is part of HTC Vive tracker library
HTC Vive tracker library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>


# Building instructions

Install openvr
```
sudo apt-get install libopenvr-dev
```
If this command fails, jump to the OpenVR section in the readme


Clone and build the repo

```
git clone https://github.com/laiaaa0/htc_vive_tracker.git
cd htc_vive_tracker
mkdir _build
cd _build
cmake ..
make
sudo make install
```

# Installation of dependencies

* Is your PC Ready for HTC - VIVE? 

    Check it [here](https://www.vive.com/us/ready/)


### SteamVR


1. Create a steam account [here](https://store.steampowered.com/join/?)
2. Install python-apt and steam-launcher:
```
sudo apt install python-apt steam-launcher
```
3. Download and install Steam [here](https://store.steampowered.com/about/)
```
sudo dpkg -i steam_latest.deb
```
4. Download and install SteamVR

    1. Open Steam.
    2. Go to LIBRARY>Tools
    3. Right click SteamVR > Install Game ...
    
5. Edit configuration files so that it works without the headset

    1. Edit the file ``` ~/.steam/steam/steamapps/common/SteamVR/resources/settings/default.vrsettings ``` 
        1. Search for the “requireHmd” key under “steamvr”, set the value of this key to “false”.
        2. Search for the “forcedDriver” key under “steamvr”, set the value of this key to “null”.
        3. Search for the “activateMultipleDrivers” key under “steamvr”, set the value of this key to “true”.
    2. Edit the file ``` ~/.steam/steam/steamapps/common/SteamVR/drivers/null/resources/settings/default.vrsettings```
        1. Search for the "enable" key under "driver_null", set the value of this key to "true".
        
6. Activate the Beta SteamVR

    1. Go to LIBRARY -> TOOLs
    2. Right click on SteamVR > Properties
    3. BETAS tab > SteamVR beta update

### OpenVR

Only required if the following command fails

```
sudo apt-get install libopenvr-dev
```

You can install from source, however since February 2022 the master branch has been failing builds . Check [this](https://github.com/ValveSoftware/openvr/issues/1642) issue.
```
sudo apt install libvulkan-dev
git clone https://github.com/ValveSoftware/openvr.git
cd openvr
mkdir build
cd build
cmake .. -DBUILD_SHARED=ON
make
sudo make install
```


Also install the headers automatically

1. Edit openvr/src/CMakeLists.txt and add the following line at the end of it

    install(FILES ${CMAKE_SOURCE_DIR}/headers/openvr.h ${CMAKE_SOURCE_DIR}/headers/openvr_capi.h DESTINATION include)



## FAQ

Check the [HTC_Vive_Tracker developer guidelines](https://dl.vive.com/Tracker/Guideline/HTC_Vive_Tracker(2018)_Developer+Guidelines_v1.0.pdf) for more information 

The coordinate system **by default** is the following 


![htc_original_axis](https://raw.githubusercontent.com/laiaaa0/htc_vive_tracker/master/htc_original_axis.png)

 Poses are provided relative to the safe bounds configured by the user
