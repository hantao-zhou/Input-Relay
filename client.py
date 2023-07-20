import pygame
import serial

# Initialize the Pygame library
pygame.init()

# Initialize the Xbox controller
joystick = None
for i in range(pygame.joystick.get_count()):
    if "X" in pygame.joystick.Joystick(i).get_name():
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
if joystick is None:
    raise Exception("No Xbox controller found")

# Open the serial connection
ser = serial.Serial('/dev/cu.usbserial-310', 115200)

# Define the constants
numOfButtons = joystick.get_numbuttons()
numOfAxes = joystick.get_numaxes()

LOWER_LIMIT = -32767
UPPER_LIMIT = 32767

def map_range(x, in_min, in_max, out_min, out_max):
    """
    Map the given value x from the input range to the output range.
    """
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

def process(events, out_min=0x0000, out_max=0x7FFF):
    """
    The purpose of the process function is to create the command and send it in serial to communicate with ESP32

    The telegram sent using serial, the format will be like the following:
    NUM_OF_OPs  OP_TYPE  AXIS/BUTTON_ID  VALUE  OP_TYPE  AXIS/BUTTON_ID  VALUE ... 
    There are three types of OP in total, one being press, second being release, third being set axis.
    The num of them will be 0,1,2 accordingly.
    Axis/button id will be an integer num.
    Value for axis is double float between -1 and 1, map the value to an int between 0x0000 and 0x7FFF.
    Value for button operations will all be 0, which is useless.
    """

    # Initialize the command string
    cmd = str(len(events)) + " "

    # Process each event
    for event in events:
        # Process button events
        if event.type == pygame.JOYBUTTONDOWN:
            cmd += "1 " + str(event.button) + " 0 "
        elif event.type == pygame.JOYBUTTONUP:
            cmd += "0 " + str(event.button) + " 0 "
        # Process axis events
        elif event.type == pygame.JOYAXISMOTION:
            axis_value = event.value
            if axis_value < -0.99:
                axis_value = -1.0
            elif axis_value > 0.99:
                axis_value = 1.0
            else:
                # axis_value = round(axis_value, 2)
                pass
            axis_id = event.axis
            axis_value_mapped = map_range(axis_value, -1.0, 1.0, out_min, out_max)
            axis_cmd = "2 " + str(axis_id) + " " + str(axis_value_mapped) + " "
            cmd += axis_cmd

    # Send the command via serial
    print(cmd)
    ser.write(cmd.encode())

# Main loop
while True:
    # Check for events and process them
    events = pygame.event.get()
    if len(events) > 0:
        process(events)