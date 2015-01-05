import resource
import time

start = time.clock()

read_pattern = open('patternfasta.fa','r')
read_text = open('textfasta.fa','r')
result = open('result.txt','w')
result.write('pattern\ttext\tk\ttime\tmemory\n')

R = read_pattern.readline().rstrip()
B = read_text.readline().rstrip()
n = len(B)
m = len(R)
k = 3

MAXLENGTH = [[0 for i in range(m)] for j in range(m)]
for i in range(m):
    for j in range(i, m):
        length = 0
        for p in range(m-j):
            if (R[i+p] != R[j+p]):
                break
            length = length+1
        MAXLENGTH[i][j] = length
        MAXLENGTH[j][i] = length

S_ij = None
newS_ij = None
j = 0
max_j = 0
choice = 0
for i in range(n-m+k+1):
    L = dict()
    T = dict()
    best_match = 0
    
    for d in range(-(k+1), (k+1)+1):
        L[(d, abs(d)-2)] = -999
        T[(d, abs(d)-2)] = list()
        if (d < 0):
            L[(d, abs(d)-1)] = abs(d)-1
            T[(d, abs(d)-1)] = list()
        else:
            L[(d, abs(d)-1)] = -1
            T[(d, abs(d)-1)] = list()
        
    for e in range(k+1):
        for d in range(-e, e+1):
            row = max(L[(d, e-1)]+1, L[(d-1, e-1)], L[(d+1, e-1)]+1)
            old_row = row

            if (row == L[(d-1, e-1)]):
                choice = 0
                T[(d, e)] = T[(d-1, e-1)]
                T[(d, e)].append((i+row+d-1,0,0))
            elif (row == L[(d, e-1)]+1):
                choice = 1
                T[(d, e)] = T[(d, e-1)]
                T[(d, e)].append((i+row+d-1,0,0))
            elif (row == L[(d+1, e-1)]+1):
                choice = 2
                T[(d, e)] = T[(d+1, e-1)]
                
            done = 0
            while ((i+row+d+1) <= j) and (row < m) and ((i+row+d) < n):
                c=0
                f=0
                if (S_ij is not None):
                    j=i+row+d
                    for x in range(len(S_ij)):
                        triple = S_ij[x]
                        if (j == triple[0]) and (triple[2] == 0):
                            c = f = 0
                            break
                        else:
                            if (j <= triple[0]+triple[2]) and (j > triple[0]):
                                difference = j-triple[0]
                                c = triple[1]+difference
                                f = triple[2]-difference
                                break
                            
                if (f >= 1):
                    if (f != MAXLENGTH[c][row]):
                        row = row+min(f, MAXLENGTH[c][row])
                        done = 1
                        break
                    else:
                        row = row+f
                else:
                    try:
                        if B[i+row+d] != R[row]:
                            done = 1
                            break
                    except IndexError:
                        print(row, i+row+d, j)
                    else:
                        row = row+1

            if (done == 0):
                while (row < m) and (i+row+d < n) and (R[row] == B[i+row+d]):
                    row = row+1
                    
            L[(d,e)] = row
            
            if (old_row < row):
                T[(d,e)].append((i+old_row+d, old_row, row-old_row))
            if (i+row+d > max_j):
                max_j = i+row+d
                newS_ij = (d,e)
            if (row == m) and (best_match == 0):
                ##nest upisat u result
                print('start =', i, 'end =', i+m+d-1, 'diff =', e)
                best_match = 1
        
    if (max_j > j):
        j= max_j
        S_ij = T[newS_ij]

end = time.clock()
s = str(m)+'\t'+'\t'+str(n)+'\t'+'\t'+str(k)+'\t'+str(round(end - start))+'\t'+\
    '\t'+str(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss/1000)+'\n'
result.write(s)

read_pattern.close()
read_text.close()
result.close()
