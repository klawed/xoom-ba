## **Project Overview**

*ESP32/XOOM with a 4096 OLED Display—The Gonzo Access Point!*

Lo and behold! Somewhere at the blasted intersection of microcontrollers and madness, we find ourselves staring directly into the pixelated abyss: an ESP32 (or is it a XOOM? It doesn’t matter—the universe is collapsing) with a 4096-pixel OLED display, beaming diagnostics out like a stoned lighthouse-keeper on ether. Kermit reporting for duty, folks: this thing doesn’t just operate as its own, freakish WiFi access point; it’s a feral beast, forever scanning for WiFi and Bluetooth specters in the electromagnetic swamp, greedily sucking up every sensor reading in its path and splattering the diagnostic guts across a tiny glowing rectangle. Redemption, or insanity, is just a connection away—even for you, friend. Just log in. The truth is out there, and it’s web-accessible.

---

## **Key Features:**

### 1. **Access Point Functionality**
- *The Frog will see you now!* ESP32 howls into the void, broadcasting its own WiFi signal like a distress flare after three whiskeys.
    - Custom SSID. Something trashy like “DiagAP” or “EatMyPixels”.
    - Any browser that dares connect is swept onto a psychedelic web UI. No escape.

### 2. **WiFi and Bluetooth Scanning**
- *It’s a jungle out there, and all the animals are trying to connect.*
    - Scours the airwaves, harvesting WiFi networks: SSID, RSSI, security, channel, MACs—the full rap sheet.
    - Sniffs out Bluetooth creatures—names, addresses, signal strength, and whatever else makes them squirm.
    - Only the strongest (or closest to the edge) networks make it to the big screen.

### 3. **Sensor Diagnostics**
- *All the twitching guts of science, in real time!*
    - Every connected sensor—temp, humidity, pressure, the whole I2C freakshow—gets polled and prodded, numbers wriggling across the OLED like ticker tape in a hurricane.

### 4. **OLED Display Output**
- *No page left unturned. No pixel unfilled.*
    - It scrolls, it rotates, it blinks—sensor data, WiFi/Bluetooth spottings, the loathsome AP’s own credentials, plus any catastrophic errors (Kermit’s not above panic).

### 5. **Web UI / API**
- *Open the portal, friend, and the truth comes streaming out.*
    - Connect to the AP, slam open a browser, and behold! All the grubby details splayed out in living color.
    - Hungry for automation? JSON by the quart.
    - Firmware updates, if you dare tempt the gods.

---

## **Hardware Components**

- *ESP32 Dev Board*: the brain-boiling nucleus—WiFi, Bluetooth, and existential dread included.
- *OLED Display*: 4096 angry, luminous pixels (128x32 or 128x64), wired up for maximum effect.
- *Sensors*: Your choice—be creative, be afraid.
- *Power*: Pray the batteries last through the night.

---

## **Software Flow**

1. **System Boot**
    - Initializes OLED so everything can start twitching on cue.
    - Auto-discovers whatever mutant sensors are strapped on.
    - Starts the WiFi AP/captive portal—no one escapes the landing page.
    - The scans. The perennial, hungry scans.

2. **Main Loop**  
   - Reads the sensors, peeps the airwaves, and catalogs the whole parade in real time.
   - Updates the list of WiFi and Bluetooth suspects, sorting through frequencies like a junkie sifting through couch cushions for change.
   - Prepares each page of glorious OLED nonsense, rotating every few ticks—a mad carousel of numbers and nonsense.
   - Answers every shrieking web request that smashes against its tiny, silicon door; no rest for the wicked or the wireless.

3. **Display Management**  
   - The OLED: showtime for the senses and senseless alike.  
     - **Page 1:** What’s the temperature, doc? And how’s the humidity—suitable for amphibians, or the death zone?  
     - **Page 2:** WiFi: The strongest signals, lined up for scrutiny like suspects in a procedural.  
     - **Page 3:** Bluetooth: Each device, name and number, dancing cheek-to-cheek in the EM spectrum.  
     - **Page 4:** System vitals—Access Point SSID, IP address, perhaps your own digital epitaph.  
   - The scene rotates; if your fingers get twitchy, hit that GPIO button—change your fate, change the page.

4. **Web Interface**  
   - When you finally connect, what a sight to see: every stat mirrored, every truth exposed.
   - Options galore, settings to tweak for the obsessive and the damned: AP name, scan rate, maybe a secret button for an all-sensors-overload.
   - Click here, try an update—be brave, this frog’s got backups…you hope.

---

## **Example OLED Displays (Straight from the Edge)**
```
[ SENSORS ]
Temp: 27.1°C  Humidity: 69%
Light: 42lx   Pressure: 999hPa
The forecast? Only Kermit knows.

[ WIFI ]
1. Muppet_Lab   (-40dBm)
2. DeltaHouse   (-68dBm)
3. OminousNet   (-91dBm)
Swamp's crowded tonight.

[ BLUETOOTH ]
1. Statler M0:12:AB  -77dBm
2. Waldorf  C1:13:CD  -80dBm
Hecklers at every frequency.

[ AP INFO ]
SSID: GonzoAP  
IP: 192.168.4.1  
Kermit sees you.
```

---

## **Final Gonzo Note**

And that’s the beast in all its glory—a psychedelic, sensor-sniffing, network-gobbling amphibian, blinking in a fever dream of OLED light, always awaiting your connection. Jump in, the WiFi’s fine! And remember: When the going gets weird, the weird turn wireless.2. **Main Loop**
    - Reads the sensors, peeps the airundefined
