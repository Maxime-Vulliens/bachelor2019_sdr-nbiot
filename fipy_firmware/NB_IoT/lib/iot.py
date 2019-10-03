from network import LTE
import time

# Class to support specific command
class NagraLTE(LTE):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs)
        # Use to monitor if current statut change while polling isAttached()
        last_synchro_line=""

    # Method use to test normal procedure
    # 1) soft reset  2) set verbose mode
    # 3) attach      4) poll and SQNMONI monitor
    def test( self ):

        # Soft-reset, recommended after LTE instantiation
        if True:
            print("Soft reset")
            self.at( "AT+CFUN=1,1" )   # Recommanded https://forum.pycom.io/post/23987
            self.at() # Catching "+SHUTDOWN"
            self.at() # Catching "+SYSSTART"
        self.at( "AT+Cmee=2")  # use verbose error value
        #self.static_info()
        print("Attaching")
        self.at("AT+SQNINS=0") # full informal network scanning
        self.attach(band=20)
        print("Waiting to be attached")
        while not self.isattached():
            time.sleep(1)
            self.at("AT+SQNMONI=7")
            #self.at("AT+SQNMONI=9")    # Check info about current network scan
            #self.at("AT+SQNINS=0")     # informal network scanning
            #self.at("AT+CEREG?")       # is attached to the network ?
        print("Attached")

    # Method to get static info about modem
    # Manufacturer, firmware, IMSI, IMEI, IMEISV, SVN, etc..
    def static_info(self):
        print("Modem manufacturer and model")
        self.at('ATI')

        print("Modem firmware version")
        self.at('ATI1')     # modem firmware version

        print("IMSI number")
        self.at('AT+CIMI')  # IMSI (SIM card number?)

        print("SIM card identification number (ICCID)")
        self.at('AT+SQNCCID?')

        print("IMEI (International Mobile station Equipment Identity number)")
        self.at('AT+CGSN=1')

        print("IMEISV (International Mobile station Equipment Identity and Software Version number")
        self.at('AT+CGSN=2')

        print("SVN (Software Version Number)")
        self.at('AT+CGSN=3')

        print("MSISDNs (Subscriber number)")
        self.at('AT+CNUM')

        print("List of operators")
        self.at("AT+COPN")
        self.at()   # List is too long to fit in a single response
        self.at()   # List is too long to fit in a single response

        print("Current operator?")
        self.at("AT+COPS?")

        print("PIN code (in case of ERROR, means SIM is not present)")
        self.at("AT+CPIN?")

        print("Host Device Identifivation values such awvUID, Device model, Firmware version")
        self.at("AT+SQNOMAHDEV?")

    # Method to diplay device capability
    def show_caps(self):
        self.at('AT!="RRC:setDbgPerm full"')
        self.at('AT!="RRC:showcaps"')

    # Send specifig AT command with pretty formatting
    def at(self, cmd=""):
        print( "  " + cmd, end="")
        response = self.send_at_cmd(cmd).strip()
        if response.endswith("\r\nOK"):
            response = response[:-4].strip()
        response = response.replace("\r\n", "\r\n\t")
        print( " --> " + response)

    # Create LTE object and run standard attach procedure
    def standard_test():
        print("Is attaching...")
        self.attach()
        print("wait for attachement...")
        while not self.isattached():
            #get_info()
            self.get_info_syncrho()
            time.sleep(0.25)
        print("Is attached... YEAAAAHHH")
        self.connect()       # start a data session and obtain an IP address
        while not self.isconnected():
            time.sleep(0.25)

    def get_info_syncrho():
        rsp_raw = self.at('AT!="showphy"')
        rsp = rsp_raw.split('\n')     # get the PHY status
        try:
            synchro_line = next( line for line in rsp if "Synchro state" in line )
        except:
            synchro_line = "not found in " + rsp_raw
        if synchro_line != last_synchro_line:
            print( synchro_line )
            last_synchro_line = synchro_line


# Create NagraLTE object and run test
def test():
    lte = NagraLTE()
    lte.test()

# Create NagraLTE object and run test
def std_test():
    lte = NagraLTE()
    lte.standard_test()



if 0:
    def get_info():
        #send_at_cmd_pretty('AT!="showphy"')     # get the PHY status
        #send_at_cmd_pretty('AT!="fsm"')         # get the System FSM
        #send_at_cmd_pretty('AT+CEREG?')         # Same as isattached
        send_at_cmd_pretty('AT+')

    def show_caps():
        send_at_cmd_pretty('AT!="RRC:setDbgPerm full"')
        send_at_cmd_pretty('AT!="RRC:showcaps"')

    def at_test():

        #
        lte = LTE()
        lte.send_at_cmd("AT+CFUN=1,1")
