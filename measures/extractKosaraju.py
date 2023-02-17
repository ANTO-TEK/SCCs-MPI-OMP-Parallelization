# 
# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Amato Emilio        0622701903      e.amato16@studenti.unisa.it             
# Bove Antonio        0622701898      a.bove57@studenti.unisa.it 
# De Gregorio Marco   0622701865      m.degregorio19@studenti.unisa.it
# 
# Source Code for sequential version:
# https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
#
# Copyright (C) 2023 - All Rights Reserved
#
# This file is part of FinalProjectHPC.
#
# This program is free software: you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with ContestOMP. 
# If not, see <http://www.gnu.org/licenses/>.
#

import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY
import re

config = {
			'seqKey': "MPI_00",
			'filenameRegex': "DIM_[0-9]+_LB_[0-9]+_HB_[0-9]+_MPI_[0-9]+_OMP_[0-9]*",
			'folderRegex':"DIM_[0-9]+_LB_[0-9]+_HB_[0-9]",
			"cols":{
				'kosaraju_time':{

					'jpg':False,
					'computeSpeedup':False,

				},
				'comm_time':{

					'jpg':False,
					'computeSpeedup':False,

				},
				'elapsed':{

					'jpg':False,
					'computeSpeedup':True,
				}
			},

			"table":{
				"header": ['Version','Processes', 'OMP','Kosaraju','Communication','Elapsed','Speedup','Efficiency'],
			},
			"plot":{
				"x_from_table":"Processes",
				"y_from_table":"Speedup",
			},
			"calcComplExpr":""
		}

def _extract(path_to_folder, plot_columns):
	prev = os.getcwd()
	os.chdir(path_to_folder)

	#List diresctory
	filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	#Remove not csv files
	#"SIZE-[0-9]+-NTH-[0-9]{2}-O[0-9]-?[0-9]*"
	filenames = [f for f in os.listdir('.') if f.endswith(".csv") and re.match(config["filenameRegex"],f) ]
	print(filenames)

	filenames = sorted(filenames)
	means = {}
	
	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		ds = pd.read_csv(filename)
		for col in plot_columns.keys():
			print('Processing : ' + filename + ", Col : " + col)

			#extract the selected column
			x_data = ds[col]
			#compute gaussian mean
			mean,std=stats.norm.fit(x_data)
			#compute mean as usual, to use when only few measures are taken
			np_mean = np.mean(x_data)

			#68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
			x_data = ds[(ds[col] <= (mean + std)) & (ds[col] >= (mean - std))][col]
			mean,std=stats.norm.fit(x_data)
			file_mean[col] = mean if np_mean == mean else np_mean
			
			if plot_columns[col]['jpg']:
				sns.histplot(x_data, kde=True)
				plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
				plt.close()
			
		means[filename] = file_mean
	os.chdir(prev)
	return means

def _compute_speedup(t,tp,nt,psize):
	speedup = t/tp
	efficiency = t/(tp*float(nt))
	return speedup,efficiency

def _make_table(header,rows,print_table=False,save=True,name=""):
	if save and not name:
		raise Exception("No filename to save file")
	x = PrettyTable()
	x.field_names = header
	x.add_rows(rows)
	if save:
		_save_table(x,name)
	if print_table:
		print(x)
	return x

def _save_table(table,filename):
	with open(filename,"w") as table_file:
		#table.set_style(MARKDOWN)
		table.set_style(MSWORD_FRIENDLY)
		data = table.get_string()
		table_file.write(data)

def _plot_from_table(header,rows,save=True,name="",show_plot=False):
	if save and not name:
		raise Exception("No filename to save file")
	
	data = [[0], [0], [0], [0], [0]]
	mpi = [0, 1, 2, 4, 8]

	i = 0

	for row in rows[1:]:
		data[i].append(row[6])
		if i == 4:
			i = 0
		else:
			i+=1

	x = [0]
	y = [0]
	try:
		x_from_table = config["plot"]["x_from_table"]
		y_from_table = config["plot"]["y_from_table"]
		speedup_pos = config["table"]["header"].index(y_from_table) #header.index("Speedup")
		thread_pos = config["table"]["header"].index(x_from_table) #header.index("Threads")
	except Exception as e:
		print("config table or plot error")

	x_th = np.array(mpi)
	
	fig, ax = plt.subplots(figsize=(12, 8))
	ax.plot(x_th, data[0], 'r-', label='1 OpenMP thread')
	ax.plot(x_th, data[1], 'g-', label='2 OpenMP threads')
	ax.plot(x_th, data[2], 'b-', label='4 OpenMP threads')
	ax.plot(x_th, data[3], 'y-', label='8 OpenMP threads')
	ax.plot(x_th, data[4], 'c-', label='16 OpenMP threads')

	ax.plot(x_th, x_th, color='blue', label='Ideal')
	#same as y_th, bisection
	plt.style.use('seaborn-whitegrid')
	

	plt.autoscale(enable=True, axis='x', tight=True)
	plt.autoscale(enable=True, axis='y', tight=True)	
	plt.legend()
	plt.xlabel(x_from_table)
	plt.ylabel(y_from_table)
	if show_plot:
		plt.show()
	if save:
		plt.savefig(name)
	plt.close()


def extraction(root, cols=config["cols"], threads=[0,1,2,4,8]):
	print("Listing folder for problem size")
	folders =  [f for f in os.listdir(root) if (os.path.isdir(os.path.join(root,f)) and re.match(config['folderRegex'],f))]
	print(f"Found folders : {folders}")

	for folder in folders:
		print(f"Folder : {folder}")
		joined_path = os.path.join(root,folder)
		means = _extract(joined_path,cols)
		header = {'values':config["table"]["header"]}
		cells = {'values':[]}
		n_mpi = -1
		n_omp = -1
		for filename_key in means:
			cell = []
			splitted_filename = filename_key.split("_")
			if config["seqKey"] in filename_key:
				seq = means[filename_key]['elapsed']
				n_mpi = 1
				n_omp = 0
				cell.append('Serial')
				cell.append(n_mpi)
				cell.append(n_omp)
			else:
				n_mpi = int(splitted_filename[7])
				n_omp = int(splitted_filename[9].split(".")[0])
				cell.append('Parallel')
				cell.append(n_mpi)
				cell.append(n_omp)
			for col in cols:
				cell.append(means[filename_key][col])
				if cols[col]['computeSpeedup']:
					psize = splitted_filename[1]
					speedup,efficiency = _compute_speedup(seq,means[filename_key][col],n_mpi,psize)
					cell.append(speedup)
					cell.append(efficiency)
			cells['values'].append(cell)
		
		splitted_folder = folder.split("_")
		size = splitted_folder[1]
		opt = splitted_folder[2]
		table_filename = joined_path + "/psize-" + size + "-" + str(opt) + "-table.csv"
		plot_filename = joined_path + "/speedup-" + str(size) + "-" + str(opt) +  ".jpg"
		table = _make_table(header['values'],cells['values'],name=table_filename)
		_plot_from_table(header["values"],cells["values"],name=plot_filename)

if __name__ == "__main__":
	extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"kosaraju/measure_O0/"))
	extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"kosaraju/measure_O1/"))
	extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"kosaraju/measure_O2/"))
	extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"kosaraju/measure_O3/"))