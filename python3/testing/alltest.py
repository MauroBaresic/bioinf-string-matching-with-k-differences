import os
import sys
import subprocess

file = open('resultTEXTS.txt', 'w')
file.write('text\tpattern\tk\ttime\tmemory\n')
file.close()

texts = os.listdir("./data/Texts")
texts.remove("pattern.fa")
texts.sort()
for number_of_texts in range(len(texts)):
    path = './data/Texts/'+texts[number_of_texts]
    print(path)
    subprocess.call([sys.executable, 'testTEXTS.py', path])


file = open('resultMTest.txt', 'w')
file.write('text\tpattern\tk\ttime\tmemory\n')
file.close()

patterns = os.listdir("./data/Test data/MTest")
patterns.remove("text.fa")
patterns.remove("kVal.txt")
patterns.sort()
for number_of_patterns in range(len(patterns)):
    path = './data/Test data/MTest/'+patterns[number_of_patterns]
    print(path)
    subprocess.call([sys.executable, 'Mtest.py', path])


file = open('resultKTest.txt', 'w')
file.write('text\tpattern\tk\ttime\tmemory\n')
file.close()

read_k = open('./data/Test data/KTest/kVals.txt')
line = read_k.readline()
diff = line.split()
for i in range(len(diff)):
    k = diff[i]
    print(k)
    subprocess.call([sys.executable, 'Ktest.py', k])
