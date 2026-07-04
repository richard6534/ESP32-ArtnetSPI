# ESP32-ArtnetSPI
![Image Description](https://github.com/richard6534/ESP32-ArtnetSPI/blob/main/Image/IMG_0637.JPG)

這款 Artnet SPI ESP32 LED 控制器是一款高效能的開源解決方案，可透過乙太網路或 Wi-Fi 控制可尋址 LED。此專案可讓您使用 Artnet 協議，透過配備 W5500 乙太網路模組及 ESP32 微控制器來控制多個 LED 燈條。程式碼包含一個簡單的 Web 伺服器，用於配置和控制 LED 燈條。為了獲得最佳效能。

## Features

- 4 Outputs, 16 Universes in Total: Supports up to 4 LED strips with 4 universes per output, enabling large-scale LED installations.
- Supports 3 (RGB), 4 (RGBW), and 5 (RGBAW) color order LEDs.
- Supports 3-wire LEDs (WS28xx) and 4-wire  SPI clock-based LEDs (APA102).
- Ethernet and Wi-Fi Support: Supports both wired and wireless Art-Net control for flexible connectivity.
- AP Mode for Debugging: Easily configure the device in standalone mode without needing an external network.
- Static IP option for permanent installations.
- Physical button for Mode switching between artnet, static IP, RGB test and static color.
- Over-The-Air (OTA) updates to update your node with different versions easily.
- Configure the number of LEDs, outputs, start universe, and node name via a web interface.
- High Frame Rates: Over 40 FPS, depending on the software used.
- Reliable Performance: Boasts 99%+ reliability with less than 1% dropped frames.
- 128x32 OLED Display Support: Provides real-time status updates and configuration feedback.
- Status RGB LED: Indicates device status for easy debugging and monitoring.
- **RGB Test Cycle**: Quickly test LED outputs with built-in cycling modes.
- Fully Open-Source: Includes PCB design and BOM, allowing users to build their own cost-effective controllers.
