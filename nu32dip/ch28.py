# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
ser = serial.Serial('/dev/tty.usbserial-10',230400)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\td: Dummy Command \tq: Quit \n\tx: Addition') # '\t' is a tab
    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'd'):
        # example operation
        n_str = input('Enter number: ') # get the number to send
        n_int = int(n_str) # turn it into an int
        print('number = ' + str(n_int)) # print it to the screen to double check

        ser.write((str(n_int)+'\n').encode()); # send the number
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str) # turn it into an int
        print('Got back: ' + str(n_int) + '\n') # print it to the screen
    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    elif (selection == 'x'):
        n1_str = input('Enter first number: ') # get the number to send
        n2_str = input('Enter second number: ') # get the number to send
        n1_int = int(n1_str) # turn it into an int
        n2_int = int(n2_str) # turn it into an int
        print(f"n1 is {n1_int}, n2 is {n2_int}") # print it to the screen to double check

        ser.write((str(n1_int)+'\n').encode()); # send the number
        ser.write((str(n2_int)+'\n').encode()); # send the number
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str) # turn it into an int
        print('Got back: ' + str(n_int) + '\n') # print it to the screen
    else:
        print('Invalid Selection ' + selection_endline)


