import shlex
from subprocess import Popen, PIPE
from threading import Timer
import time

cmd = 'python top_block.py'


def run(cmd, timeout_sec):
    proc = Popen(shlex.split(cmd), stdout=PIPE, stderr=PIPE)
    timer = Timer(timeout_sec, proc.kill)
    try:
        timer.start()
        stdout, stderr = proc.communicate()
    finally:
        timer.cancel()

    #print("stdout : " + stdout)
    #print("stderr : " + stderr)

    return stdout


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



for subcarrier_index in [0,1,2,3,4,5,6,7,8,9,10,11,12]:

	for nb_subcarrier in range(1 , 12 - subcarrier_index):
		time.sleep(2)

		fdump_nsss = open("dump_npss_sync.log","a+")
		fdump_npss = open("dump_nsss_sync.log","a+")

		fdump_nsss.write("\n\n nb subcarrier : " + str(nb_subcarrier) + ", subcarrier index" + str(subcarrier_index) + "\n\n")
		fdump_npss.write("\n\n nb subcarrier : " + str(nb_subcarrier) + ", subcarrier index" + str(subcarrier_index) + "\n\n")

		fdump_npss.close()
		fdump_nsss.close()

		out = run(cmd,60)

		result = out.split('\n')
		for lin in result:
		    print(lin)

		#print(data)
		if (nb_subcarrier == 12 - subcarrier_index - 1):
			replace_data('self.nb_subcarrier = nb_subcarrier = ' + str(nb_subcarrier), 'self.nb_subcarrier = nb_subcarrier = 1')
		else:
			replace_data('self.nb_subcarrier = nb_subcarrier = ' + str(nb_subcarrier), 'self.nb_subcarrier = nb_subcarrier = ' + str(nb_subcarrier+1))

	if (subcarrier_index == 12):
		replace_data('self.subarrier_index = subarrier_index = ' + str(subcarrier_index), 'self.subarrier_index = subarrier_index = 0')
	else:
		replace_data('self.subarrier_index = subarrier_index = ' + str(subcarrier_index), 'self.subarrier_index = subarrier_index = ' + str(subcarrier_index+1))



	


