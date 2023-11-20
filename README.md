# Introduction To Robotics
The repo to be used during the Introduction to Robotics course.
# About the repo
Elective course taken during my third year studying at the Faculty of Mathematics and Computer Science at the University of Bucharest.
I will be uploading each homework including requirements, implementation details, code and image files.
# Homework 2
Description: Use a separate potentiometer for controlling each color of the RGB LED: Red,
Green, and Blue. This control must leverage digital electronics. Specifically,
you need to read the potentiometer’s value with Arduino and then write a
mapped value to the LED pins.
Photo : ![H2](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/660a6b0d-6a77-4d1f-ba67-51bf3d2fbdd6)
Video : https://youtu.be/6ztg4XqrFAQ?si=Q9HUOnlddEyuhvI8
# Homework 3
Description: This assignment involves simulating a 3-floor elevator control system using
LEDs, buttons, and a buzzer with Arduino. 
Contains:
- red led - blinks if elevator is moving
- white led - stays on while there is at least a level in the queue
- three yellow leds - on if the elevator is on that level
- three blue leds - show the state of the door on the level (on if open, off if closed)
- buzzer - only for opening or closing door.
Photo : ![20231029_185911](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/9fe33021-7d04-4437-a20b-2fa7e6e942e9)
Video : https://youtu.be/PAQP9rZWVyg?si=_xkBA3vZ4UwzhOFJ
# Homework 4
Description : This assignment involves a 7-segment display and a joystick. <br>
The initial position is: the dp led blinking, and all the others off. As the joystick moves, so should the blinking position.
With a short press of the joystick, the current led (blinking one) should change its state to the negative of its current one.
The current led should blink, its state not being important.
With a long press, everything will reset to the initial position. <br>
Photo : ![20231106_191928](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/47edb403-554a-46aa-896c-635e5eda9d7a)
<br>Video : https://youtu.be/W-_E8yElPA8?si=Ybbr3GiRePs6Vira

# Homework 5
Description : This assignment involves making a stopwatch with a lap functionality and counts in tenths of a second. <br>
Flow: <br>
1. Start State <br>
Start with 000.0 and nothing in memory, in pause mode <br>
  (Only the start/stop button (1) does something) <br>
2. Count State <br>
After press button 1 - start time. Two possible operations: <br>
  ~ button 3 (lap) - put the current time in memory, override if more than four <br>
  ~ button 1 (start) - pause time <br>
3. Pause State <br>
In pause mode: <br>
  ~ button 1 (start) - start from current time, go back to state 2 <br>
  ~ button 2 (reset) - go to 000.0 with laps in memory <br>
4. Prepare State <br>
Have 000.0 on display with laps in memory. One can: <br>
  ~ button 1 (start) - go to 2 (erase laps) <br>
  ~ button 3 (lap) - see saved laps <br>
    /// if there are no saved laps, the flow can't enter state 4 <br>
5. Display State <br>
Display laps: <br>
  ~ button 2 (reset) - go to 3 (erase laps) <br>
  ~ button 3 (lap) - go to next lap <br>

Photo : ![20231113_175822](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/a5ef24b8-6994-4752-a59c-95320a0744ba)
 <br>
Video : https://youtu.be/CzF7UykOvJk?si=xxWZAGPaoMvTiWcf

# Homework 6
Description : This assignment consists of developing a Smart Environment Monitor and Logger, using an Ultrasonic Sensor and a LDR. <br>
The Led is used to warn in case of a threshold being passed.
Menu Description : 
Menu: <br>
  1. Sensor Settings : <br>
    1.1. Sensor Sampling Interval : 1 - 10 sec (for each / for both)<br>
    From here, N thresholds  <br>
    1.2. Ultrasonic Alert Threshold : (min / max value)<br>
    1.3. LDR Alert Threshold<br>
    1.4. Back - N + 2<br><br>

  2. Reset Logger Data : Goto default values (individually / for all) <br>
    2.1. Yes<br>
    2.2. No<br><br>

  3. System Status :<br>
    3.1. Current Sensor Reading : print readings at 1.1. set value and have a key for exit<br>
    3.2. Current Sensor Settings : print sampling rate and threshold values<br>
    3.3. Display Logger Data : last 10 readings for all sensors<br>
    3.4. Back<br><br>

  4. RGB Led Control :<br>
    4.1. Manual Colour Control<br>
    4.2. Toggle Automatic ON/OFF<br>
    4.3. Back<br><br>
  5. Upload Logger data in EEPROM<br>
    - Done on demand to not compromise life expectancy of the board<br><br>
Photo : ![20231120_214919](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/5c6e39df-cee5-41b7-85a2-d4ecaca76d7e)
<br>
Video : https://youtu.be/zD8k7ipKCPA?si=D1q9b2tz1LxEzfFI
