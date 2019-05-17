# ESP8266-Arduino-Info

Prints out chip, sdk and wifi information using the *Arduino* framework.

## Compile requirements

- rename or copy lib/App/App.hpp.sample to lib/App/App.hpp
  and change &lt;your wifi ssid> and &lt;your wifi password>

## Console Output

```t
ESP8266 Arduino Info - 0.3.1 - Dr. Thorsten Ludewig <t.ludewig@gmail.com>
BUILD: May 17 2019 14:17:27
Boot device = 3
Boot mode = 1
Boot version = 31

-- CHIPINFO --
Chip Id = 00376668
CPU Frequency = 80MHz

Flash real id:   00144068
Flash real size: 1048576
Flash ide  size: 1048576
Flash chip speed: 40000000
Flash ide mode:  DOUT
Flash Chip configuration ok.

Free Heap         : 44752
Sketch Size       : 361088
Free Sketch Space : 598016

(10052) Starting Wifi in Station Mode
.......

WiFi connected to : WWXXYYZZ
WiFi connection # : 1
WiFi Channel      : 6
WiFi phy mode     : 11N
WiFi MAC Address  : CC:50:E3:37:66:68
WiFi Hostname     : esp8266-info-376668
WiFi IP-Address   : 192.168.1.213
WiFi Gateway-IP   : 192.168.1.1
WiFi Subnetmask   : 255.255.255.0
WiFi DNS Server   : 192.168.1.5

(14544) OTA Setup started...
(14551) HTTP server setup...
(14556) HTTP server started
```