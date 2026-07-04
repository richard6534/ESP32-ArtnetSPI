# ESP32-ArtnetSPI
![Image Description](https://github.com/richard6534/ESP32-ArtnetSPI/blob/main/Image/IMG_0637.JPG)

這款 Artnet SPI ESP32 LED 控制器是一款高效能的開源解決方案，可透過乙太網路或 Wi-Fi 控制可尋址 LED。此專案可讓您使用 Artnet 協議，透過配備 W5500 乙太網路模組及 ESP32 微控制器來控制多個 LED 燈條。程式碼包含一個簡單的 Web 伺服器，用於配置和控制 LED 燈條。為了獲得最佳效能。

## Features

- 4 個輸出端口，共 16 個燈路：每個輸出端口最多可連接 4 條 LED 燈帶，每個輸出端口有 4 個燈路，可實現大規模 LED 安裝。
- 1 個DMX輸出端口，可連接DMX相關設備。
- 支援 3 色（RGB）、4 色（RGBW）和 5 色（RGBAW）LED。
- 支援 3 線 LED (WS28xx) 和 4 線 SPI 時脈 LED (APA102)。
- 支援乙太網路和 Wi-Fi：支援有線和無線 Art-Net 控制，實現靈活的連線。
- 靜態 IP 位址選項適用於永久安裝。
- 用於在 artnet、靜態 IP、RGB 測試和靜態顏色之間切換模式的實體按鈕。
- 透過空中下載 (OTA) 更新，可以輕鬆地將您的節點更新到不同版本。
- 透過網頁介面配置 LED 數量、輸出埠、起始和節點名稱。
- 高幀率：超過 40 FPS，取決於所使用的軟體。
- 效能可靠：可靠性高達 99% 以上，丟幀率低於 1%。
- 支援 128x32 OLED 顯示器：提供即時狀態更新和設定回饋。
- 狀態 RGB LED：指示設備狀態，方便調試和監控。
- RGB 測試循環：利用內建循環模式快速測試 LED 輸出。
- 完全開源：包含 PCB 設計和 BOM，使用戶能夠建立自己經濟高效的控制器。

## PCB Design

- 這是正在進行中的PCB設計，採用12V轉5V降壓轉換器，並使用螺絲端子方便連接。
