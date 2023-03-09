# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
import matplotlib.pyplot as plt
ser = serial.Serial('/dev/tty.usbserial-10',230400)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('\nPIC32 MOTOR DRIVER INTERFACE\n')
    # display the menu options; this list will grow
    selec = dict()
    selec[0] = "a: Read current sensor (ADC counts)"
    selec[1] = "b: Read current sensor (mA)"
    selec[2] = "c: Read encoder (counts)"
    selec[3] = "d: Read encoder (deg)"
    selec[4] = "e: Reset encoder"
    selec[5] = "f: Read current sensor (ADC counts)"
    selec[6] = "g: Set current gains"
    selec[7] = "h: Get current gains"
    selec[8] = "i: Set position gains"
    selec[9] = "j: Get position gains"
    selec[10] = "k: Test current control"
    selec[11] = "l: Go to angle (deg)"
    selec[12] = "m: Load step trajectory"
    selec[13] = "n: Load cubic trajectory"
    selec[14] = "o: Execute trajectory"
    selec[15] = "p: Unpower the motor"
    selec[16] = "q: Quit client"
    selec[17] = "r: Get mode"
    st = ""
    for i in range(18):
        st += selec[i]
        st += "\t"
        if i > 1:
            st += "\t"
        if i > 3:
            st += "\t"
        if i > 15: 
            st += "\t"
        if (i%2) != 0:
            st += "\n"
    print(st)
    # read the user's choice
    selection = input('ENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'a'):
        adc_counts_str = ser.read_until(b'\n')
        adc_counts = int(adc_counts_str)
        print(f"The motor current is {adc_counts} ADC counts.")
        
    elif (selection == 'b'):
        current_str = ser.read_until(b'\n')
        current = float(current_str)
        print(f"The motor current is {current} mA.")

    elif (selection == 'c'):
        motor_counts_str = ser.read_until(b'\n')
        motor_counts = int(motor_counts_str)
        print(f"The motor angle is {motor_counts} counts.")

    elif (selection == 'd'):
        degs_str = ser.read_until(b'\n')
        degs = float(degs_str)
        print(f"The motor angle is {degs} degrees.")

    elif (selection == 'e'):
        print(f"Encoder reset done!")

    elif (selection == 'f'):
        pwm = int(input("What PWM value would you like [-100 to 100]? "))
        if pwm < 0:
            direction = "counterclockwise"
        else:
            direction = "clockwise"
        pwm_val = abs(pwm)
        ser.write((str(pwm)+'\n').encode())
        print(f"PWM has been set to {pwm_val}% in the {direction} direction.")

    elif (selection == 'g'):
        Kp = float(input("Enter your desired Kp current gain [recommended: 4.76]: "))
        Ki = float(input("Enter your desired Ki current gain [recommended: 0.32]: "))
        print(f"Sending Kp = {Kp} and Ki = {Ki} to the current controller.")

    elif (selection == 'h'):
        counts_str = ser.read_until(b'\n')
        counts = int(counts_str)
        print(f"The current controller is using Kp = {Kp} and Ki = {Ki}.")

    elif (selection == 'i'):
        Kp = float(input("Enter your desired Kp current gain [recommended: 4.76]: "))
        Ki = float(input("Enter your desired Ki current gain [recommended: 0.32]: "))
        Kd = float(input("Enter your desired Ki current gain [recommended: 10.63]: "))
        print(f"Sending Kp = {Kp}, Ki = {Ki}, and Kd = {Kd} to the position controller.")

    elif (selection == 'j'):
        print(f"The position controller is using Kp = {Kp}, Ki = {Ki}, and Kd = {Kd}.")

    elif (selection == 'k'):
        counts_str = ser.read_until(b'\n')
        counts = int(counts_str)
        print(f"Encoder counts: {counts}")

    elif (selection == 'l'):
        ang = int(input("Enter the desired motor angle in degrees: "))
        print(f"Motor moving to {ang} degrees.")

    elif (selection == 'm'):
        counts_str = ser.read_until(b'\n')
        counts = int(counts_str)
        print(f"Encoder counts: {counts}")

    elif (selection == 'n'):
        counts_str = ser.read_until(b'\n')
        counts = int(counts_str)
        print(f"Encoder counts: {counts}")

    elif (selection == 'o'):
        counts_str = ser.read_until(b'\n')
        counts = int(counts_str)
        print(f"Encoder counts: {counts}")

    elif (selection == 'p'):
        print(f"Motor unpowered.")

    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    
    elif (selection == 'r'):
        mode_str = ser.read_until(b'\n')
        print(f"The PIC32 controller mode is currently {mode_str}.")

    else:
        print('Invalid Selection ' + selection_endline)



