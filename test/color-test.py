#!/usr/bin/python3
import sys
import os


def rain():
    r=[rg for rg in range(0, 5)]
    a=[[5-i%6+i%6, i%6,0] for i in r[:-1]]+[[i%6, 5-i%6+i%6,0] for i in r[::-1]]
    a=a+[i[::-1] for i in a][:-1][::-1]+[[i%6, 0, 5-i%6+i%6] for i in r[1:]]+[[5,0,j-1] for j in range(5,1,-1)]
    for i in range(len(a)):
        r,g,b=a[i]
        a[i]=36*r+6*g+b+16
    return a


def avg(arr): return sum(arr)//len(arr)


def box(i): return str("\033[48;5;"+str(i)+"m \033[0m")


def ax(b,f): return box(b).replace("m ",";38;5;"+str(f)+"m▄\033[0m")


def boxs(ip:list()):
	if len(ip) == 4: return ax(ip[0],ip[1])+ax(ip[2],ip[3])
	if len(ip) == 2: return ax(ip[0],ip[1])
	else: return ""


def rgb(ip):
	ip=[int(i) for i in ip]
	ip=[avg(ip[2:3]),0,ip[1],0,255-avg(ip[2:3]),ip[0]]
	ip=[str(i) for i in ip]
	s="\033[48;2;"
	s+=";".join(ip[:3])
	s+=";38;2;"
	s+=";".join(ip[3:])
	s+="m▄\033[0m"
	return s


def ip2rgb(ip):
	if type(ip)==type(""): ip=ip.split(".")
	if not (type(ip) is type([]) and len(ip) == 4): raise TypeError
	col=0
	if os.getenv("IPCOLOR")in["less","216","0","alt"]:col=16
	ip = [(int(i)*(216+col)//256)+(16-col) for i in ip]
	return ip


HXC=[rain()[i] for i in range(len(rain())) if int(i)%6<4]
HX="0123456789abcdef"
def hx(i: int):
	s=""
	while i>0:
		s=str(HX[i%16])+s
		i=i//16
	return s


def hexstr(instr:str): return hx(sum([[int(j) for j in instr.split(".")][::-1][i]<<8*i for i in range(4)]))
def hexc(instr:str):
	h=hexstr(instr)
	a=[]
	for i in h:
		a+=[HXC[HX.index(i)]]
	return a

if os.getenv("IPADDR") is not None:
	try:
		arg=os.getenv("IPADDR")
		ip=ip2rgb(arg)
		o=boxs(ip)
		print(o)
		exit(0)
	except:
		exit(1)
	finally:
		exit(2)


for arg in sys.argv:
	try:
		ip = ip2rgb(arg)
		i=[int(j) for j in arg.split(".")]
		print(rgb(i)+str(arg).rjust(15)+" "+boxs(ip))
	except TypeError:
		continue

