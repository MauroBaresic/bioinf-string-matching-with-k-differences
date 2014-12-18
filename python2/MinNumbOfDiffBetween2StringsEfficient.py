# author Mauro Baresic
# email: mauro.baresic@outlook.com

import sys
import numpy as np
import psutil
import os
import time

def memory_usage_psutil():
    # return the memory usage in MB    
    process = psutil.Process(os.getpid())
    mem = process.get_memory_info()[0] / float(2 ** 20)
    return str(round(mem,3)) + " MB"

class MinDifferenceEfficient():
    R = ''
    B = ''
    m = 0
    n = 0
    k = 0
    L = None
    MAXLENGTH = None
    T = None
    Sij = None
    newSij = None
    
    def __init__(self,R,B):
        self.R = R
        self.B = B
        self.m = len(R)
        self.n = len(B)
        self.L = dict()
        self.T = dict()
        self.patternAnalysis()

    def patternAnalysis(self): # O(m * m *m)
        if ( len(self.R) < 1 ):
            return -1
        self.MAXLENGTH = np.zeros((self.m,self.m),dtype = np.int)
        for i in xrange(self.m):
            for j in xrange(i,self.m):
                length = 0
                for p in xrange(self.m - j):
                    if (self.R[i + p] != self.R[j + p]):
                        break
                    length += 1
                self.MAXLENGTH[(i,j)] = length
                self.MAXLENGTH[(j,i)] = length
        return 0

    def findCovering(self, S, h, idx): #returns (int, (int, int, int))
        #S = [(20,3,1),(21,0,0),(22,5,2),(24,0,0),(25,7,3),(28,11,1),(29,0,0)]
        id_last = idx
        for c in xrange(id_last, len(S)):
            triple = S[c]
            if (h == triple[0] and h == triple[0] + triple[2]):
                id_last = c            
                break
            elif (h >= triple[0] and h< triple[0] + triple[2]):
                id_last = c
                break
        if (S[id_last][2] == 0):
            #print idx, id_last
            return (id_last, S[id_last])
        else:
            #print idx, id_last
            difference = h - S[id_last][0]
            return (id_last,(difference + S[id_last][0], difference + S[id_last][1], S[id_last][2] - difference))

    def calculate(self, k):
        self.k = k
        if (self.MAXLENGTH is None):
            if (self.patternAnalysis() == -1):
                return -1
        j = 0
        max_j = 0
        idx = 0
        for i in xrange(self.n - self.m + self.k + 1):

            self.L = dict()
            self.T = dict()
            
            #1
            for d in xrange(-(self.k + 1), (self.k + 1) + 1):
                self.L[(d,abs(d)-2)] = - float("inf")
                self.T[(d,abs(d)-2)] = list()#
                if (d < 0):
                    self.L[(d,abs(d)-1)] = abs(d) - 1
                    self.T[(d,abs(d)-1)] = list()#
                else:
                    self.L[(d,abs(d)-1)] = -1
                    self.T[(d,abs(d)-1)] = list()#

            #2
            for e in xrange(self.k + 1):
                for d in xrange(- e, e +1):
                    #3
                    row = max((self.L[(d,e-1)] + 1, self.L[(d-1,e-1)], self.L[(d+1,e-1)] + 1))
                    
                    row_old = row
                    
                    if (row == self.L[(d-1,e-1)]):
                        idx = 0
                        self.T[(d,e)] = self.T[(d-1,e-1)][:]#
                        self.T[(d,e)].append((i+row+d-1,0,0))#
                    elif (row == self.L[(d,e-1)] + 1):
                        idx = 1
                        self.T[(d,e)] = self.T[(d,e-1)][:]#
                        self.T[(d,e)].append((i+row+d-1,0,0))#
                    elif (row == self.L[(d+1,e-1)] + 1):
                        idx = 2
                        self.T[(d,e)] = self.T[(d+1,e-1)][:]#
                        
                    #4 new
                    GoTo = False
                    #print i + row + d + 1, j, d, e
                    while ((row < self.m) and (i + row + d < self.n) and ( (i + row + d + 1) <= j)):
                        #4 new.1
                        c=0
                        f=0
                        if (self.Sij is not None):
                            #print "i,j:",i,j
                            last_id, triple = self.findCovering(self.Sij,i+row+d,0)
                            c = triple[1]
                            f = triple[2]
                        else:
                            print i,d,e,j
                        #4 new.2
                        if (f >= 1): #case a
                            #4 new.3
                            if (f != self.MAXLENGTH[(c,row)]): #case a1
                                row += min((f,self.MAXLENGTH[(c,row)]))
                                #GOTO 5
                                GoTo = True
                                break
                            else: #case a2
                                row += f
                        else: #case b
                            #4 new.4
                            try:
                                if (self.B[i + row + d] != self.R[row]): #case b1
                                    #GOTO 5
                                    GoTo = True
                                    break
                            except IndexError:
                                print row, i+row+d+1, j
                            else: #case b2
                                row += 1

                    if (False == GoTo):
                        #4 old
                        while ((row < self.m) and (i + row + d < self.n) and (self.R[row] == self.B[i + row + d])):
                            row += 1
                    #5
                    self.L[(d,e)] = row

                    if (row > row_old):
                        self.T[(d,e)].append((i + row_old + d, row_old, row - row_old))
                    
                    if (row + d + i > max_j):
                        max_j = row + d + i
                        self.newSij = (d,e)
                    #6
                    if (row == self.m):
                        print "An occurence with <=",self.k,"differences of the pattern starts at",i
                        #print memory_usage_psutil()
                        #GOTO 7 ili ne
                        
            #7
            if (max_j > j):
                j = max_j
                self.Sij = self.T[self.newSij]
            #self.outputScreen()
                
                
    def outputScreen(self):
        for i in xrange(- self.k, self.k +1):
            row = ''
            for j in xrange(- self.k, self.k +1):
                if self.L.has_key((i,j)):
                    row += "L(" + str(i) + ', '+ str(j) + ')=' + str(self.L[(i,j)]) + '\t'
            sys.stdout.write(row + '\n')

    def outputFile(self, path):
        f = open(path,'w')
        for i in xrange(- self.k, self.k +1):
            row = ''
            for j in xrange(- self.k, self.k +1):
                if self.L.has_key((i,j)):
                    row += "L(" + str(i) + ', '+ str(j) + ')=' + str(self.L[(i,j)]) + '\t'
            f.write(row + '\n')
        f.close()

    def computeMAXLENGTH(self):
        self.MAXLENGTH = np.zeros((self.m,self.m),dtype = np.int)
        for d in xrange(self.m):
            if (self.R[self.m-d-1] == self.R[self.m-1]):
                self.MAXLENGTH[(self.m-d-1,self.m-1)] = 1
                self.MAXLENGTH[(self.m-1,self.m-d-1)] = 1
            #else:
                #self.MAXLENGTH[(self.m-d-1,self.m-1)] = 0
        for d in xrange(self.m):
            for i in xrange(self.m - 2 -d, -1, -1):
                if (self.R[i] == self.R[i +d]):
                    self.MAXLENGTH[(i,i+d)] = 1 + self.MAXLENGTH[(i + 1,i + d +1)]
                    self.MAXLENGTH[(i+d,i)] = self.MAXLENGTH[(i,i+d)]
                #else:
                    #self.MAXLENGTH[(i,i+d)] = 0
                    #self.MAXLENGTH[(i+d,i)] = 0

if __name__ == "__main__":
    timeStart = time.time()
    #path = sys.argv[1]
    path = "podaci.txt"
    f = open(path,'r')
    rows = [row.strip() for row in f.readlines()]
    f.close()

    if (len(rows) != 2):
        print "Use!"
        sys.exit()

    R = rows[0]
    B = rows[1]

    mdi = MinDifferenceEfficient(R,B)
    x1 = mdi.MAXLENGTH
    mdi.calculate(4)
    print "Elapsed time", time.time() - timeStart, "s"
    print memory_usage_psutil()
    """mdi.computeMAXLENGTH()
    x2 = mdi.MAXLENGTH
    for i in xrange(mdi.m):
        for j in xrange(mdi.m):
            if x1[i,j] != x2[i,j]:
                print (i,j)
    print x1
    print x2"""
