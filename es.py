import pygame

# Initialize the Pygame library
pygame.init()

# Define the Xbox controller's input event codes
XBOX_BTN_A = 0
XBOX_BTN_B = 1
XBOX_BTN_X = 2
XBOX_BTN_Y = 3

# Initialize the Xbox controller
joystick = None
for i in range(pygame.joystick.get_count()):
    if "32" in pygame.joystick.Joystick(i).get_name():
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
if joystick is None:
    raise Exception("No ESP controller found")

# Read input events from the Xbox controller
while True:
    events = pygame.event.get()
    if len(events) > 0:
        print(events)
        # break

    # for event in events:
    #     if event.type == pygame.JOYBUTTONDOWN:
    #         if event.button == XBOX_BTN_A:
    #             print("Button A pressed")
    #         elif event.button == XBOX_BTN_B:
    #             print("Button B pressed")
    #         elif event.button == XBOX_BTN_X:
    #             print("Button X pressed")
    #         elif event.button == XBOX_BTN_Y:
    #             print("Button Y pressed")