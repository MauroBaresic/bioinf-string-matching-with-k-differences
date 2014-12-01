read=open('data.txt','r')
write=open('result.txt','w')

r='X'+read.readline()
b='x'+read.readline()
n=len(b)-1
m=len(r)-2
k=3

MAXLENGHT[0][0]=0
for d in range 
   

j=0
for i in range(n-m+k+1):
    L[d,d-2]=L[d,-d-2]=NEG_INF
    if d<0:
        L[d,abs(d)-1]=abs(d)-1
    else:
        L[d,abs(d)-1]=-1
    for e in range(k+1):
        for d in range(-e, e+1):
            row=max((L[d,e-1]+1),L[d-1,e-e],(L[d+1,e-1]+1))
            done=0
            while (i+row+d+1)<=j and done==0 and row<m:
                ##neki tripleti
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
