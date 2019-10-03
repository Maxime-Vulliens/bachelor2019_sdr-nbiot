'''
Basic NB-IoT example with Vodafone

########## FIRST EXAMPLE ##########

from network import LTE
lte = LTE()
lte.attach(band=20, apn="nb.inetd.gdsp")
while not lte.isattached():
    time.sleep(0.25)
lte.connect()       # start a data session and obtain an IP address
while not lte.isconnected():
    time.sleep(0.25)

# now use socket as usual...

########## SECOND EXAMPLE(old) ##########
from network import LTE

lte = LTE()
lteM.send_at_cmd('AT+CFUN=0')
lteM.send_at_cmd('AT!="clearscanconfig"')
lteM.send_at_cmd('AT!="addscanfreq band=5 dl-earfcn=6430"')
lteM.send_at_cmd('AT!="zsp0:npc 1"')
lteM.send_at_cmd('AT+CGDCONT=1,"IP","test123"')
lteM.send_at_cmd('AT+CFUN=1')

while not lte.isattached():
    pass

lte.connect()
while not lte.isconnected():
    pass

# now use socket as usual...

!!!!! https://docs.pycom.io/tutorials/lte/firmware.html !!! Need to update firmware to have a LTE modem one
'''

'''
An other way to try :
import uos
from network import LTE
from utime import sleep

print('Instantiate the LTE object to private ')
lteM = LTE(carrier="standard")
sleep(5)
print(lteM.send_at_cmd('ATI1'))
sleep(5)
print(uos.uname())
sleep(5)
lteset = lteM.send_at_cmd('AT+SQNCTM?')
print("LTE set to {}".format(lteset))
lteM.attach()
for l in range (0,10):
    lteM.isattached()
    sleep(1)
noltcon = 0
if not lteM.isattached():
    print("Attempting to attach to LTE ", end='')
    while(True):
        noltcon += 1
        if noltcon >= 50:
            # If not attached in 50 sec, flash error then try again
            print(" Filed after 50 attempts")
            break
        elif lteM.isattached():
            print(" Success")
            lteset = lteM.send_at_cmd('AT+SQNCTM?')
            print("LTE attached to {}".format(lteset))
            # Get the current LTE signal quality
            squallteM = lteM.send_at_cmd('AT+CSQ').strip().split()
            rquallteM = int(squallteM[1].split(',')[0])
            # Evaluate the returned signal quality Data
            rssilteM = 113 - ((rquallteM) * 2)
            ltesqual = ("LTE RSSI: -{} dbM".format(rssilteM))
            print(ltesqual)
            break
        print('.', end='')
        sleep(1)
sleep(10)
print(lteM.send_at_cmd('AT+CEREG?'))
sleep(10)
print(lteM.send_at_cmd('AT+CEREG?'))
sleep(10)
print(lteM.send_at_cmd('AT+CEREG?'))
sleep(10)
print(lteM.send_at_cmd('AT+SQNMONI=9'))
'''


'''
Command used :
print(lteM.send_at_cmd('ATI1'))  -> get status and modem firmware

lteset = lteM.send_at_cmd('AT+SQNCTM?')
print("LTE set to {}".format(lteset))  -> get current carrier

def send_at_cmd_pretty(cmd):
    response =  lte.send_at_cmd(cmd).split('\r\n')
    for line in response:
        print(line)

send_at_cmd_pretty('AT+CEREG?') -> same as isattached
send_at_cmd_pretty('AT!="showphy"')     # get the PHY status
send_at_cmd_pretty('AT!="fsm"')         # get the System FSM
'''
