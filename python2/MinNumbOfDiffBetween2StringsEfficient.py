# author Mauro Baresic
# email: mauro.baresic@outlook.com

import sys

# procedure for parsing file in FASTA format
def parseFastaFile(path):
    f = open(path,'r')
    sequence = ""
    readingSequence = False
    for row in f.readlines():
        if (row[0] == ','):
            continue
        elif (row[0] == '>' and readingSequence == False):
            readingSequence = True
        elif (row[0] != '>' and readingSequence == True):
            sequence += row.rstrip()
        else:
            break        
    f.close()
    return sequence

# class that implements Landau-Vishkin-Nussinov algorithm
class MinDifferenceEfficient():
    
    # pattern string
    R = ''
    
    # text string
    B = ''
    
    # length of pattern
    m = 0
    
    # length of text
    n = 0
    
    # maximal number of allowed differences
    k = 0
    
    # hashtable L[d,e] that denotes the largest
    # row i on diagonal d with e differences 
    L = None
    
    # [m x m] matrix for pattern analysis
    MAXLENGTH = None
    
    # hash for all sequences of triples (p,c,f)
    T = None
    
    # current sequence of triples (p,c,f)
    Sij = None
    
    # pair (d,e) that denotes new current sequence
    newSij = None
    
    # constructor
    def __init__(self,R,B):
        self.R = R
        self.B = B
        self.m = len(R)
        self.n = len(B)
        self.L = dict()
        self.T = dict()
        self.patternAnalysis()
    
    # procedure that computes pattern analysis
    def patternAnalysis(self): # O(m * m *m)
        if ( len(self.R) < 1 ):
            return -1
        self.MAXLENGTH = dict()
        for i in xrange(self.m):
            for j in xrange(self.m):
                self.MAXLENGTH[(i,j)] = 0
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
        
    # procedure that finds covering of index h in sequence of triples (p,c,f)
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
            return (id_last, S[id_last])
        else:
            difference = h - S[id_last][0]
            return (id_last,(difference + S[id_last][0], difference + S[id_last][1], S[id_last][2] - difference))

    # procedure for calculating Landau-Vishkin-Nussinov algorithm
    def calculate(self, k):
        self.k = k
        if (self.MAXLENGTH is None):
            if (self.patternAnalysis() == -1):
                return -1
        j = 0
        max_j = 0
        idx = 0
        
        for i in xrange(self.n - self.m + self.k + 1):
            next_i = False
            self.L = dict()
            self.T = dict()
            
            #1 Initialization
            for d in xrange(-(self.k + 1), (self.k + 1) + 1):
                self.L[(d,abs(d)-2)] = - float("inf")
                self.T[(d,abs(d)-2)] = list()
                if (d < 0):
                    self.L[(d,abs(d)-1)] = abs(d) - 1
                    self.T[(d,abs(d)-1)] = list()
                else:
                    self.L[(d,abs(d)-1)] = -1
                    self.T[(d,abs(d)-1)] = list()

            #2
            for e in xrange(self.k + 1):
                for d in xrange(- e, e +1):
                    #3
                    row = max((self.L[(d,e-1)] + 1, self.L[(d-1,e-1)], self.L[(d+1,e-1)] + 1))
                    
                    row_old = row
                    
                    if (row == self.L[(d-1,e-1)]):
                        idx = 0
                        self.T[(d,e)] = self.T[(d-1,e-1)][:]
                        self.T[(d,e)].append((i+row+d-1,0,0))
                    elif (row == self.L[(d,e-1)] + 1):
                        idx = 1
                        self.T[(d,e)] = self.T[(d,e-1)][:]
                        self.T[(d,e)].append((i+row+d-1,0,0))
                    elif (row == self.L[(d+1,e-1)] + 1):
                        idx = 2
                        self.T[(d,e)] = self.T[(d+1,e-1)][:]
                        
                    #4 new
                    GoTo = False
                    while ((row < self.m) and (i + row + d < self.n) and ( (i + row + d + 1) <= j)):
                        #4 new.1
                        c=0
                        f=0
                        if (self.Sij is not None):
                            last_id, triple = self.findCovering(self.Sij,i+row+d,0)
                            c = triple[1]
                            f = triple[2]
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
                            if (self.B[i + row + d] != self.R[row]): #case b1
                                #GOTO 5
                                GoTo = True
                                break
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
                        #GOTO 7
                        next_i = True
                        break
                
                #GOTO 7
                if (next_i == True):
                    break
                        
            #7
            if (max_j > j):
                j = max_j
                self.Sij = self.T[self.newSij]
            #self.outputScreen()
            

                
    # procedure that prints L to screen
    def outputScreen(self):
        for i in xrange(- self.k, self.k +1):
            row = ''
            for j in xrange(- self.k, self.k +1):
                if self.L.has_key((i,j)):
                    row += "L(" + str(i) + ', '+ str(j) + ')=' + str(self.L[(i,j)]) + '\t'
            sys.stdout.write(row + '\n')

    # procedure that prints L to file
    def outputFile(self, path):
        f = open(path,'w')
        for i in xrange(- self.k, self.k +1):
            row = ''
            for j in xrange(- self.k, self.k +1):
                if self.L.has_key((i,j)):
                    row += "L(" + str(i) + ', '+ str(j) + ')=' + str(self.L[(i,j)]) + '\t'
            f.write(row + '\n')
        f.close()
        
    # procedure that computes pattern analysis
    def computeMAXLENGTH(self):
        self.MAXLENGTH = dict()
        for i in xrange(self.m):
            for j in xrange(self.m):
                self.MAXLENGTH[(i,j)] = 0
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
    
    patternPath = sys.argv[1]
    textPath = sys.argv[2]
    k = int(sys.argv[3])
    
    R = parseFastaFile(patternPath)
    B = parseFastaFile(textPath)

    mde = MinDifferenceEfficient(R,B)
    mde.calculate(k)
