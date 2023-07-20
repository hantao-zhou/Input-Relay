#include <Arduino.h>
#include <BleGamepad.h>

#define numOfButtons 64
#define numOfHatSwitches 4

BleGamepad bleGamepad;
BleGamepadConfiguration bleGamepadConfig;

const int LOWER_LIMIT = bleGamepadConfig.getAxesMin();
const int UPPER_LIMIT = bleGamepadConfig.getAxesMax();

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);
    bleGamepadConfig.setVid(0xe502);
    bleGamepadConfig.setPid(0xabcd);
    // Some non-Windows operating systems and web based gamepad testers don't like min axis set below 0, so 0 is set by default
    //bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
	bleGamepadConfig.setAxesMin(0x0000); // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
    bleGamepadConfig.setAxesMax(0x7FFF); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal 
    bleGamepad.begin(&bleGamepadConfig); // Simulation controls, special buttons and hats 2/3/4 are disabled by default

    // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again
}
 /*
        we shall receive the telegram sent using serial, the format will be like the follow
        NUM_OF_OPs  OP_TYPE  AXIS/BUTTON_ID  VALUE  OP_TYPE  AXIS/BUTTON_ID  VALUE ... 
        there are three types of OP in total, one being press, second being release, third being set axis.
        the num of them will be 0,1,2 accordingly
        axis/button id will be integer num
        value for setting axis will be the double float between -1 and 1, map the value to a int between LOWER_LIMIT and UPPER_LIMIT
        value for button operations will all be 0, which is useless

        for each button press, use the function: bleGamepad.press(i); i will be the button id
        for each button released, use the function: bleGamepad.release(i); i is the button id

        for setting the axis, the function corresponding to axis id 0~7 are
        bleGamepad.setX(i);
        bleGamepad.setY(i);
        bleGamepad.setZ(i);
        bleGamepad.setRX(i);
        bleGamepad.setRY(i);
        bleGamepad.setRZ(i);
        bleGamepad.setSlider1(i);
        bleGamepad.setSlider2(i); the i here is the value
        */
void loop()
{
    if (bleGamepad.isConnected())
    {
        if (Serial.available() > 0) {
            int numOps = Serial.parseInt();
            for (int i = 0; i < numOps; i++) {
                int opType = Serial.parseInt();
                int id = Serial.parseInt();
                double value = 0;
                if (opType == 2) {
                    value = Serial.parseFloat();
                    value = constrain(value, -1.0, 1.0);
                    value = map(value, -1.0, 1.0, LOWER_LIMIT, UPPER_LIMIT);
                }
                switch (opType) {
                    case 0:
                        bleGamepad.release(id);
                        break;
                    case 1:
                        bleGamepad.press(id);
                        break;
                    case 2:
                        switch (id) {
                            case 0:
                                bleGamepad.setX(value);
                                break;
                            case 1:
                                bleGamepad.setY(value);
                                break;
                            case 2:
                                bleGamepad.setZ(value);
                                break;
                            case 3:
                                bleGamepad.setRX(value);
                                break;
                            case 4:
                                bleGamepad.setRY(value);
                                break;
                            case 5:
                                bleGamepad.setRZ(value);
                                break;
                            case 6:
                                bleGamepad.setSlider1(value);
                                break;
                            case 7:
                                bleGamepad.setSlider2(value);
                                break;
                        }
                        break;
                }
            }
            bleGamepad.sendReport();
        }
    }
}