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
Video : https://youtu.be/zD8k7ipKCPA?si=D1q9b2tz1LxEzfFI <br>

# Homework 7
Description : Small game on 8x8 matrix - Shoot walls<br>
Game rules and logic :<br>
  - Setup : Random player starting position and wall placement<br>
  - Player : Blinks slowly, moves depending on joystick input, can't go through walls or bullet<br>
  - Bullet : one at a time on the map, blinks fast, destroys walls on contact, destroyed if it lands on player's position<br>
  - Four Leds : to indicate the direction of the player - the direction that the bullet follows when shot<br><br>
Photo : ![20231127_220453](https://github.com/Teo0o0/IntroductionToRobotics/assets/99832898/848423e9-cc64-4583-b75a-be91ed4c9448)
<br>
Video : https://youtu.be/ontv38z4HZ4?si=7ap1l3sIgo80eOBp

# Homework 8
Description : Implement Menu on LCD<br><br>
Checkpoint #1 : LCD Base Functionalities<br>
  - Intro Message + animation<br>
  - Menu :<br>
    - Start game<br>
    - Settings - mandatory :<br>
      - LCD brightness control in EEPROM<br>
      - Matrix brightness control in EEPROM<br>
    - About<br>
  - End Message<br>
  - During gameplay logic<br><br>

<br>
Video : https://youtu.be/X9R6KxoAT14?si=7UkmwZ8ulPAXuoUE
<h2>Extra Project - Line Follower</h2>
Assembled a robot capable of autonomously following a predefined path.<br>
This project was developed within a 3-person team, "Robo Putrezitul", along with <a href="http://github.com/criss505">@criss505</a> and <a href="https://github.com/blwUsama">@blwUsama</a>.<br><br><details>
<summary>Project details</summary>
This project integrates sensors, motors, and a control algorithm to enable navigating along the track marked by a black line.<br>
Our robot completed the final track in 20.5 seconds, slightly over the maximum-grade threshold (20 seconds).<br>
  
<h3>Components</h3>
  1.Arduino Uno<br>
  2.Breadboard (a small one)<br>
  3.Power source (LiPo battery)<br>
  4.QTR reflectance sensor<br>
  5.2 Wheels<br>
  6.2 DC motors<br>
  7.Ball caster<br>
  8.Wires<br>
  9.Zip-ties<br>
  10.Chassis (created with foamcore)<br>

<h3>Main Functionalities</h3>
  Line Detection - The sensors continuously monitor the surface and process the data to determine the robot's position relative to the line.<br>
  Control Algorithm - The code is implemented to make decisions regarding direction based on the line sensor data (through adjusting motor speeds).<br>
  Autonomous Navigation - The robot makes real-time adjustments to navigate through curves and stay on track.<br>

<h3>Chassis Design</h3>
The chassis was created out of foamboard, with a few points in mind when designed.<br>
Complex forms were avoided to improve stability and durableness, while securing the other pieces attached.<br>
The sensors were placed in front, along with the ball caster, to facilitate the detection of incoming path, while the motor-controlled wheels were placed at the back for more control when making turns.<br>
We placed the LiPo battery (heaviest piece) on the lowest level in the center of the chassis and the breadboard above it, to lower the center of gravity.<br>
When fixing the battery, we used a pin to hold it in place to ensure a smooth change when changing it to a charged one is needed.<br>


<h3>Calibration</h3>
The robot is able to self-calibrate, not needing manual handling when being turned on.<br>
This is done through the robot turning left-right before starting to follow the line.<br>

<h3>Parameter Tuning</h3>
The PID values were low or 0 at first, until we experimented with several sets to find the optimal numbers for our setup.<br>
The main traits we looked at when evaluating the parameter values were the sharpness of the turns and the shaking on straight paths.<br>
The final values were kp=15, ki=0, kd=7.<br>


</details>

-> <a href="https://youtu.be/uPR8K8UPLq4">video of the best lap</a>

