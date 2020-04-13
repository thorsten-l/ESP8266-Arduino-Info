# ESP8266-Arduino-Info

Prints out chip, sdk and wifi information using the *Arduino* framework.

## Compile requirements

- rename or copy lib/App/App.hpp.sample to lib/App/App.hpp
  and change &lt;your wifi ssid> and &lt;your wifi password>

## Console Output

```t
ESP8266 Arduino Info - 0.6.0 - Dr. Thorsten Ludewig <t.ludewig@gmail.com>
BUILD: Apr 13 2020 12:17:06

Boot device = 3
Boot mode = 1
Boot version = 31

-- CHIPINFO --
Chip Id = XXXXXXXX
CPU Frequency = 80MHz

Flash real id:   00144051
Flash real size: 1048576
Flash ide  size: 1048576
Flash chip speed: 40000000
Flash ide mode:  DOUT
Flash Chip configuration ok.

Free Heap         : 46000
Sketch Size       : 377184
Free Sketch Space : 581632

WiFi MAC Address  : XX:XX:XX:XX:XX:XX

Scanning WiFi networks...
done.
14 networks found
 1: XXXXXXXX (1,-71)*
 2: XXXXXXXX (1,-70)*
 3: XXXXXXXX (1,-71)*
 4: XXXXXXXX (1,-93)*
 5: XXXXXXXX (1,-94) 
 6: XXXXXXXX (1,-76)*
 7: XXXXXXXX (1,-88)*
 8: XXXXXXXX (6,-72)*
 9: XXXXXXXX (6,-71)*
10: XXXXXXXX (1,-85)*
11: XXXXXXXX (11,-68)*
12: XXXXXXXX (11,-57)*
13: XXXXXXXX (11,-67)*
14: XXXXXXXX (11,-58)*

Best Channel = 11
Best RSSI = -57

(15631) Starting Wifi in Station Mode (SSID=XXXXXXXX)
........

WiFi connected to : XXXXXXXX
WiFi connection # : 1
WiFi Channel      : 11
WiFi phy mode     : 11N
WiFi MAC Address  : XX:XX:XX:XX:XX:XX
WiFi Hostname     : esp8266-info-xxxxxx
WiFi IP-Address   : 192.168.XXX.XXX
WiFi Gateway-IP   : 192.168.XXX.XXX
WiFi Subnetmask   : 255.255.255.0
WiFi DNS Server   : 192.168.XXX.XXX

Time              : 2020-04-13 12:17:44
Timezone          : CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00

(20442) OTA Setup started...
(20449) HTTP server setup...
(20455) HTTP server started
```