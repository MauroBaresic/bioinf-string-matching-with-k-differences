read = open('data.txt','r')
write = open('result.txt','w')

R = read.readline().rstrip()
B = read.readline().rstrip()
n = len(B)
m = len(R)
k = 3

print(R)
print(B)
print(m, n)

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
        

j = 0
choice = 0
for i in range(n-m+k+1):
    L = dict()
    T = dict()
    
    L[(d, abs(d)-2)] = -999
    if (d < 0):
        L[d, abs(d)-1] = abs(d)-1
    else:
        L[d, abs(d)-1] = -1
        
    for e in range(k+1):
        for d in range(-e, e+1):
            row = max(L[(d, e-1)]+1, L[(d-1, e-1)], L[(d+1, e-1)]+1)
            
            print(row)
            
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
                
            print(T(d, e)]
                
            done=0
            while ((i+row+d+1) <= j) and (row < m) and ((i+row+d) < n):
                ##c i f
                if f>=1:
                    if f!=MAXLENGHT[c][row]:
                        row=row+min(f,MAXLENGHT(c,row))
                        done=1
                    else:
                        row=row+f
                else:
                    if b[i+row+d+1]!=r[row+1]:
                        done=1
                    else:
                        row=row+1
            while r[row+1]==b[i+row+1+d] and done==0 and row<m:
                row=row+1
            L[d,e]=row
            if L[d,e]==m:
                print('YES')
                break ##exits from last for loop
    ##novi simboli


read.close()
write.close()
