import os
import resource
import time
times = []
times.append(time.clock())

write = open('result.txt', 'w')
write.write('pattern\ttext\tk\ttime\tmemory\n')
k = 4
R = ''
read = 0
read_pattern = open('./data/Texts/pattern.fa', 'r')
for line in read_pattern.readlines():
    if (line[0] == '>') and (read == 0):
        read = 1
    elif (line[0] != '>') and (read == 1):
        R = R+line.rstrip()
read_pattern.close()
m = len(R)

texts = os.listdir("./data/Texts")
texts.remove("pattern.fa")
for number_of_texts in range(len(texts)):
    B = ''
    path = './data/Texts/'+texts[number_of_texts]
    read = 0
    read_text = open(path, 'r')
    for line in read_text.readlines():
        if (line[0] == '>') and (read == 0):
            read = 1
        elif (line[0] != '>') and (read == 1):
            B = B+line.rstrip()
            
    n = len(B)

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
            L[(d, abs(d)-2)] = -9
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
                    T[(d,e)] = T[(d+1, e-1)]
                    
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
                            if (B[i+row+d] != R[row]):
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
                    print('start =', i, 'end =', i+m+d-1, 'diff =', e)
                    best_match = 1
            
        if (max_j > j):
            j= max_j
            S_ij = T[newS_ij]

    times.append(time.clock())
    read_text.close()
    s = str(m)+'\t'+str(n)+'\t'+str(k)+'\t'+\
        str(round(times[number_of_texts+1]-times[number_of_texts]))+'\n'
        '\t'+str(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss/1000)+'\n'
    write.write(s)
    
write.close()
