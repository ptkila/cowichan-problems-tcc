from matplotlib import pyplot as plt
import time
import os
import subprocess

threads = [2, 4, 8, 16, 32, 64]
sizes = [10, 100, 1000]
tests = 30

# tempos de execucao
times = {};

# nomes para o grafico
parsedItemsList = [];

# exes
items = [];

def getExes(printf):
	
	for file in os.listdir(os.curdir):
		if file.endswith(".out"):
			items.append(file)
	if printf:
		print items

def generateData(printf):
	# Tempos -> t = {<nomedoproblema>:{<numero_threads>:{<entrada>:[val,...,val_n]}}}
	# Hash nome do problema
	for indexParsedItems in range(len(parsedItemsList)):
		times[parsedItemsList[indexParsedItems]] = {}

	# Hash numero de threads
	for key in times.keys():
		for indexThreads in range(len(threads)):
			times[key][str(threads[indexThreads])] = {}

	# Hash tamanhos
	for key0 in times.keys():
		for key1 in times[key0].keys():
			for indexSize in range(len(sizes)):
				times[key0][key1][str(sizes[indexSize])] = []

	# Executa testes e salva na lista
	for indexItems in range(0, len(items)):
		for indexThreads in range(0, len(threads)):
			for indexSize in range(0, len(sizes)):
				vals = []
				path = ("./" + items[indexItems] + " " + str(sizes[indexSize]) + " " + 
					str(threads[indexThreads]) + " 0")
				for i in range(0, tests):
					start = time.time()
					runProgram(path)
					end = time.time()
					vals.append(end - start);
				times[parsedItemsList[indexItems]][str(threads[indexThreads])][str(sizes[indexSize])] = vals
	if printf:
		print times

def parseNames(printf):
	# Parsing
	for indexItems in range(len(items)):
		
		# Retira extensao
		fullName = items[indexItems].split('.', 1);
		
		# Separa por _
		fullNameParts = fullName[0].split('_', 10);
		
		# Junta no nome, saltando o nome do problema
		parsedName = fullNameParts[0];
		for namePart in range(len(fullNameParts)):
			if namePart > 1:
				parsedName = parsedName + " " + fullNameParts[namePart]
		
		parsedItemsList.append(parsedName)

	if printf:
		print parsedItemsList
	

def generateGraphics():
	
	print "dkasnd"

def runProgram(path):

    p = subprocess.Popen(path, shell=True, stdout=subprocess.PIPE)
    p.communicate()[0]

    if (p.returncode > 1): 
        return False
    return True

def main():

	getExes(True)
	parseNames(True)
	generateData(True)
	#generateGraphics()

if __name__ == '__main__':
	main()