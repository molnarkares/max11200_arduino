# max11200_arduino
MAX11200 ADC Arduino library

## Usage

### Includes

_#include "max11200.h"_   

_#include "SPI.h"_

### Instantiation

_Max11210 sensor(SPI, PIN_SPI_SS, PIN_SPI_MISO);_

Where 
* SPI is the instance of the SPI driver class, 
* PIN_SPI_SS is the peripheral select pin connected to MAX11200 and
* PIN_SPI_MISO is the MISO pin of SPI

### Initialization

You shall call _sensor.begin();_ to initialize the sensor in the Arduino setup() function.

### Reading data

First check if the data acquisition has completed yet by calling _sensor.waitForReady(t); where t is a wait timeout in milliseconds.
If waitForReady returns true, you can call _sensor.analogReadUnsigned()_ or _sensor.analogReadSigned()_ depending on the application configuration.

### Configuration

After initialization with _begin()_ you can set up the max11200 parameters with the following APIs:
* _sensor.config(val);_ where val is the desired value of the CTRL1 register of the device. Check max11200.h for the pre-defined values. 
Example: _sensor.config(Max11210::linef|Max11210::ub);_

* _sensor.setRate(rate);_ will set the conversion rate. Check max11200.h for the pre-defined values. 
Example: _sensor.setRate(Max11210::rate::rate120sps);_

### Controlling the digital IOs of MAX11200

You can configure the digital IO pins of the device with the _pinMode(pin,direction)_ method.
Example: _sensor.pinMode(Max11210::dPin::gpio4,Max11210::dMode::output);_

Then get or set the digital values via the digitalWrite and digitalRead APIs.
Example: _sensor.digitalWrite(Max11210::dPin::gpio4,Max11210::dVal::low);_

### Calibration

Only the self calibration is implemented and it is called by the _begin()_ method at startup. If needed, the self calibration can be triggered via calling the _calibrateSelf()_ method.




