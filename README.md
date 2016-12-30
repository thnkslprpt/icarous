![](logo/ICAROUS.jpeg "")

Integrated Configurable Algorithms for Reliable Operations of Unmanned Systems
========

### Current Release

V-0.a (June, 2016)

### License

This code is released under NASA's Open Source Agreement. For more
information see files in directory Licenses.

REQUIRED PACKAGES
=================

The following repositories are required to run icarous. 

- Ardupilot (https://github.com/ArduPilot/ardupilot.git)
- mavlink (https://github.com/ArduPilot/mavlink.git)
- MAVProxy (https://github.com/ArduPilot/MAVProxy.git)

BEFORE LAUNCHING ICAROUS
========================

ICAROUS uses several messages that are not part of the common/ardupilotmega MAVlink message set. To ensure that the MAVProxy ground station can receive these custom messages, perform the following steps:

- copy icarous.xml in the `msg/` folder in the icarous repository to `mavlink/message_definitions/v1.0`
- (re)install pymavlink library to incorporate the new dialect (icarous.xml). To do this, goto `mavlink/pymavlink` and run:

```
    $python setup.py install
```

- Install the custom modules provided as part of the ICAROUS repository:

```
    $cd Python/CustomModules
    $bash SetuMavProxy.sh <Location of MAVProxy/>
```

LAUNCHING ICAROUS
=================

The various parameters that control the behavior of ICAROUS can be found in `params/icarous.txt` under Java or C++. The default parameters found in icarous.txt were selected after several flight tests to yield acceptable performances. For convenience, a script (run.sh) is provided to launch ICAROUS and other supporting applications. The run script also helps configure several port options.

To run ICAROUS on a companion computer for Ardupilot, first make sure that the pixhawk port and baud rate settings in the run script are the same as the telemetry port settings on the pixhawk. First lauch the ICAROUS application as follows:

	$nohup ./run.sh PX4 > pxout.txt &

Note that the nohup (no hang up) command enables the application to run as a background process without interruption. To enable interaction with ICAROUS and the pixhawk through a ground station uplink, launch the radio interface application:

    $nohup ./run.sh RADIO > radioout.txt &

Once the above two applications are launched, you should be able to receive data via the telemetry link connected to the ground station (e.g. mission planner/mavproxy). When using radios with the SiK firmware, it is recommended that the firmware is configured with the following settings:

* MAVLink - None (Raw data mode).
* Op Resend - disabled.
* ECC - disabled.
The Sik radio firmware can be configured using Mission planner or APM Planner.

We strongly recommend using MAVProxy as a ground station to communicate with ICAROUS. To simplify uploading geofence data to ICAROUS, additional modules are provided in `Python/CustomModules`. The MAVProxy ground station can be lauched using the run script:

    $./run.sh GS

Waypoints can be uploaded from MAVProxy using the `wp` command. An example waypoint is provided in `Java/params`

	wp load params/flightplan.txt

Geofence can be uploaded from MAVProxy using the `geofence` command. An example geofence file is provided in `Java/params`

	geofence load params/geofence.xml

Once waypoints and geofence are uploaded, the mission can be started from MAVProxy as follows:

    long MISSION_START


Running ICAROUS with the ardupilot SITL
---------------------------------------

Setup and launch the ardupilot SITL as described in <http://ardupilot.org/dev/docs/setting-up-sitl-on-linux.html>. Specify the SITL host address in the run script. By default, the ardupilot SITL is configured to output packets to udp port 14551. Launch ICAROUS in the SITL mode as follows:

    $./run.sh SITL

While running ICAROUS in the SITL framework, it is possible to communicate with ICAROUS via a ground station using the UDP sockets instead of using a radio link. See `GS_MASTER` variable in the run.sh script to enable the ground station over a UDP socket.
