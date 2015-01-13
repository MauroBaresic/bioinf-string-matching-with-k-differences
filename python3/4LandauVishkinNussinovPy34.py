import resource     #only works on unix based os
import time

#lines 6-11: input for text file path, pattern file path and value of k
#users input is checked right after every entry
print('type in requested data and press enter after each input')
text_path = input('textFilePath: ')
read_text = open(text_path,'r')
pattern_path = input('patternFilePath: ')
read_pattern = open(pattern_path,'r')
k = int(input('k: '))

#line 14: starting timestamp used later for time measurement
start = time.clock()

#lines 17-19: preparing output file
result = open('result.txt','w')
s = '{0: <10}{1: <10}{2: <10}{3: <10}{4: <10}\n'.format('text', 'pattern', 'k', 'time', 'memory')
result.write(s)

#lines 22-23: defining pattern (R) and text (B) string
R = ''
B = ''

#lines 27-30 and 32-35: parsing of pattern and text file in FASTA format
#and calculating the length of both
lines = read_pattern.readlines()
for i in range(1, len(lines)):
    R = R+lines[i].rstrip()
m = len(R)

lines = read_text.readlines()
for i in range(1, len(lines)):
    B = B+lines[i].rstrip()
n = len(B)

#lines 38-53: pattern analysis -> computing of MAXLENGTH matrix (m x m)
MAXLENGTH = [[0 for i in range(m)] for j in range(m)]
for d in range(m):
    if (R[m-1-d] == R[m-1]):
        MAXLENGTH[m-1-d][m-1] = 1
        MAXLENGTH[m-1][m-1-d] = 1
    else:
        MAXLENGTH[m-1-d][m-1] = 0
        MAXLENGTH[m-1][m-1-d] = 0
for d in range(m):
    for i in range(m-2-d, -1, -1):
        if (R[i] == R[i+d]):
            MAXLENGTH[i][i+d] = 1 + MAXLENGTH[i+1][i+d+1]
            MAXLENGTH[i+d][i] = 1 + MAXLENGTH[i+d+1][i+1]
        else:
            MAXLENGTH[i][i+d] = 0
            MAXLENGTH[i+d][i] = 0

### START of Landau-Vishkin-Nussinov algorithm

#lines 58 and 63-65: defining starting values of variables for later use
S_ij = None     #array of triplets (p,c,f) based on i,j marks:
                #i is curent starting index in text,
                #j is index of rightmost symbol from text to which previous iterations of i has come
                #p is first index of subtext, c is first index of subpattern,
                #f is number of matches between subtext and subpattern
newS_ij = None      #used when expanding S_ij
max_j = 0       #index of rightmost symbol of text to which current iteration of i has come
j = 0       #index of rightmost symbol of text to which last iteration of i has come

for i in range(n-m+k+1):
    #lines 69-70: resetting dictionaries for each iteration of i
    L = dict()
    T = dict()
    #line 72: variable used for printing only one (and the best) solution of current iteration of i
    best_match = 0
    
    #lines 76-84: initialization of dictionaries
    ### 1st step of algorithm 
    for d in range(-(k+1), (k+1)+1):
        L[(d, abs(d)-2)] = -999         #this value represents negative infinity
        T[(d, abs(d)-2)] = list()
        if (d < 0):
            L[(d, abs(d)-1)] = abs(d)-1
            T[(d, abs(d)-1)] = list()
        else:
            L[(d, abs(d)-1)] = -1
            T[(d, abs(d)-1)] = list()
        
    #line 89: variable e goes from 0 to k differences allowed
    #line 90: variable d represents diagonals located within e distance from main diagonal in current iteration of i
    ### 2nd step of algorithm
    for e in range(k+1):
        for d in range(-e, e+1):
            ### 3rd step of algorithm
            row = max(L[(d, e-1)]+1, L[(d-1, e-1)], L[(d+1, e-1)]+1)
            row_starting = row       #used later in 5th step if value of row changes in current iteration

            #lines 97-104: depending on choice made previously by row = max() function,
            #dictionary T is being expanded and/or updated
            if (row == L[(d-1, e-1)]):
                T[(d, e)] = T[(d-1, e-1)]
                T[(d, e)].append((i+row+d-1, 0, 0))
            elif (row == L[(d, e-1)]+1):
                T[(d, e)] = T[(d, e-1)]
                T[(d, e)].append((i+row+d-1, 0, 0))
            elif (row == L[(d+1, e-1)]+1):
                T[(d, e)] = T[(d+1, e-1)]
                
            ### 4th NEW step of algorithm
            #line 109: second and third conditions were added for avoiding "index out of range" errors
            done = 0        #variable used insted of GoTo function
            while ((i+row+d+1) <= j) and (row < m) and ((i+row+d) < n):
                ### 4th NEW.1 step of algorithm
                #Lines 112-124: take from S_ij triple (p,c,f) which contains simbol from text B[i+row+d]
                c = 0
                f = 0
                if (S_ij is not None):
                    j = i+row+d
                    for x in range(len(S_ij)):
                        if (j == S_ij[x][0]) and (S_ij[x][2] == 0):
                            c = f = 0
                            break
                        else:
                            if (j <= (S_ij[x][0]+S_ij[x][2])) and (j > S_ij[x][0]):
                                c = S_ij[x][1]+j-S_ij[x][0]
                                f = S_ij[x][2]-j+S_ij[x][0]
                                break
                            
                ### 4th NEW.2 step of algorithm
                if (f >= 1):        #case a1
                    if (f != MAXLENGTH[c][row]):
                        row = row+min(f, MAXLENGTH[c][row])
                        done = 1
                        break
                    else:       #case a2
                        row = row+f
                else:       #case b
                    if B[i+row+d] != R[row]:        #case b1
                        done = 1
                        break
                    else:       #case b2
                        row = row+1

            if (done == 0):
                #line 144: first and second conditions were added for avoiding "index out of range" errors
                ### 4th OLD step of alogorithm
                while (row < m) and ((i+row+d) < n) and (R[row] == B[i+row+d]):
                    row = row+1
                    
            ### 5th step of algorithm
            L[(d,e)] = row
            
            #lines 151-152: updating of T if row was changed in previous while loop
            if (row_starting < row):
                T[(d,e)].append((i+row_starting+d, row_starting, row-row_starting))
            #lines 155-157: remembering max j to which this iteration of i has come
            #and preparation for changing S_ij if necessary
            if (i+row+d > max_j):
                max_j = i+row+d
                newS_ij = (d,e)
            #lines 160-162: printing output if match was found (row equals m)
            ### 6th step of algorithm
            if (L[(d,e)] == m) and (best_match == 0):
                print('start =', i, 'end =', i+m+d-1, 'diff =', e)
                best_match = 1
            
    ### 7th step of algorithm
    if (max_j > j):
        j = max_j
        S_ij = T[newS_ij]

#line 170: ending timestamp; end minus start equals time this algorithm was running
end = time.clock()

#line 175: preparing string for writing into file with informations about what was algorithm processing,
#and how much time and memory usage was needed, ordered as follows:
#length of text, length of pattern, value of k, how long it was running (sec), memory usage (MB)
s = '{0: <10}{1: <10}{2: <10}{3: <10}{4: <10}\n'.format(str(n), str(m), str(k), \
    str(round(end - start)), str(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss/1000))

result.write(s)
result.close()
