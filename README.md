# videoP2Proxy
Proxy to enable xiaomi P2P only cameras to work with standard protocols.  
**Works without changing the firmware of the camera, MiHome cloud will continue to work.**


 Known working models
-----------
* Xiaomi Aqara Smart Gateway IP Camera: **lumi.camera.aq1**
* Other Xiaomi cameras should also work, please test and [report with a issue](https://github.com/miguelangel-nubla/videoP2Proxy/issues/new)

Quick start on Raspbian
-----------

#### Install dependencies
1. Install dependency packages
```
sudo apt-get install build-essential git python3 python3-dev autoconf automake libtool liblivemedia-dev libjson-c-dev
```

2. Install [https://github.com/rytilahti/python-miio](https://python-miio.readthedocs.io/en/latest/discovery.html#installation)
```
pip3 install python-miio
```

##### Clone repository
```
git clone https://github.com/miguelangel-nubla/videoP2Proxy.git
cd videoP2Proxy
```

##### Build
```
./autogen.sh
make
sudo make install
```

##### Run the RTSP proxy
```
videop2proxy --ip CAMERA_IP --token CAMERA_HEX_TOKEN --rtsp 8554
```
Example output running correctly:
```
Starting proxy...
Starting RTSP server on "rtsp://192.168.1.2:8554/"
(Using port 8000 for optional RTSP-over-HTTP tunneling.)

[P2P client] starting...
IOTC_Initialize2...
IOTC_Connect_ByUID_Parallel... SID:0
avClientStart2... userName: c8da138df16 passWord: a1c8da1df11a
avClientStart2 mAvIndex=0
startIOTC video ret=0
startIOTC audio ret=0
[P2P client] running
[ReceiveVideo] Running
[ReceiveAudio] Running
```


Use it with Home Assistant
-----------

Simply add to your configuration.yaml
```
camera:
  - platform: ffmpeg
    input: -rtsp_transport tcp -i rtsp://192.168.1.2:8554/
```


Usage
-----------
```
$ videop2proxy --help
Usage: videop2proxy --ip CAMERA_IP --token CAMERA_HEX_TOKEN [...] 

Options:
  --ip IP             [Required] Camera ip address.
  --token HEX_TOKEN   [Required] Camera miio token.

Modes:
  --rtsp PORT         Enable RTSP server.
  --stdout            Enable output to stdout.
```
##### --token [hex_token]
If you don't already have the miio token for your device you can try:
* [https://python-miio.readthedocs.io/en/latest/discovery.html#tokens-from-backups](https://python-miio.readthedocs.io/en/latest/discovery.html#tokens-from-backups)
* [https://github.com/jghaanstra/com.xiaomi-miio/blob/master/docs/obtain_token.md](https://github.com/jghaanstra/com.xiaomi-miio/blob/master/docs/obtain_token.md)

*Keep in mind that reseting and re-adding the device to MiHome app will change the token.*

#### Modes:

##### --rtsp [port]
Starts a rtsp server on the specified port with live video data.

##### --stdout
Dumps the raw h264 stream to stdout on realtime to allow usage to other programs via linux pipe.

Example use with a [webRTC server running janus on a Raspberry Pi](https://www.rs-online.com/designspark/building-a-raspberry-pi-2-webrtc-camera)
```
videop2proxy --ip CAMERA_IP --token CAMERA_HEX_TOKEN --stdout \
| gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=8004
```


Known limitations
-----------
- The program does not do any additional processing to the raw h264 video feed provided by the P2P camera.Some h264 decoders will like it less than others.
- Given the nature of the UDP connection for the P2P client, some frames/data will be lost and video may freeze intermittently. It is the expected behaviour. You can only try to improve the network path.
- This is a fast hack, pull requests welcome.


-----------

Was this useful to you? Consider donating  
@PayPal: miguelangel.nubla@gmail.com  
@bitcoin: bc1q2s5jdm7vem8ygv2fgxtfukmxuruyfsqjrexk3c
