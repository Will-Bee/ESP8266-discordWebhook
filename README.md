
# Optic fiber cable detection

Software for chool project device
## Installation

### Clone project

```bash
  git clone https://github.com/Will-Bee/ESP8266-discordWebhook.git
```
    
### Configuration

* Your device MUST have ESP8266
* This project uses NodeMcu V3

#### Edit variables

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `host` | `string` | **Required**. Your Discord webhook URL |
| `SENSOR_PIN` | `uint8_t` | **Required**. Pin used by sensor |

* In messageBuild() you can edit your custom messages

### Wiring

* Connect sensor to pin of your choice
* Connect LCD Display if you want to (I2C pins)
* Connect power source

### Upload

* upload to board with `PlatformIO`
## Author

- [@Will-Bee](https://github.com/Will-Bee)