
# Optic fiber cable detection

Software for chool project device
## Installation

### Clone project

```bash
  git clone https://github.com/Will-Bee/ESP8266-discordWebhook.git
```
    
### Configuration

* Your device MUST have ESP8266
* This project was made for NodeMcu V3 board

#### Edit variables

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `SENSOR_PIN` | `uint8_t` | **Required**. Pin used by sensor |
| `yourApi` | `String` | **Required**. URL Of your own API |

* In messageBuild() you can edit your custom messages

### Wiring

* Connect sensor to pin of your choice
* Connect LCD Display if you want to (I2C pins)
* Connect power source

### Upload

* upload to board with `PlatformIO`
## Author

- [@Will-Bee](https://github.com/Will-Bee)
