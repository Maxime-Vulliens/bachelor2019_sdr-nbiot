###############################################################
# Author : Maxime Vulliens
# Date : 21.09.2019
#
# Description : used to execute a GNU Radio diagram multiple
# times with variatiion of parameters between each round
#
###############################################################

import numpy
import shlex
from subprocess import Popen, PIPE
from threading import Timer


cmd = 'python top_block.py'

# this method is used to execute a shell cmd with a specific timeout
def run(cmd, timeout_sec):
    proc = Popen(shlex.split(cmd), stdout=PIPE, stderr=PIPE)
    timer = Timer(timeout_sec, proc.kill)
    try:
        timer.start()
        stdout, stderr = proc.communicate()
    finally:
        timer.cancel()

    return stdout


# this method is used to replace data in "top_python.py" file
def replace_data(str_to_find,str_to_replace):
	fin = open("top_block.py", "rt")
	data = fin.read()

	#print(data)
	data = data.replace(str_to_find,str_to_replace)
	#print(data)

	fin.close()
	fin = open("top_block.py", "wt")
	fin.write(data)
	fin.close()


# main start here
# loop to execute the flow graph with differen noise gain

for noise_gain in numpy.arange(0.1,0.205,0.005):


	fdump_nsss = open("dump_npss_sync.log","a+")
	fdump_npss = open("dump_nsss_sync.log","a+")

	fdump_nsss.write("\n\n noise gain : " + str(noise_gain) + "\n\n")
	fdump_npss.write("\n\n noise gain  : " + str(noise_gain) + "\n\n")

	fdump_npss.close()
	fdump_nsss.close()

	out = run(cmd,60)

	result = out.split('\n')
	for lin in result:
	    print(lin)

	print(str(noise_gain))
	if (noise_gain == 0.2):
		replace_data('self.noise_gain = noise_gain = ' + str(noise_gain), 'self.noise_gain = noise_gain = 0.1')
	else:
		replace_data('self.noise_gain = noise_gain = ' + str(noise_gain), 'self.noise_gain = noise_gain = ' + str(noise_gain+0.005))

