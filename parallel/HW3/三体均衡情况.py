# -*- coding: utf-8 -*-
"""
Created on Fri Jul  5 02:48:26 2019

@author: NeoMerlin
"""

import numpy as np
import matplotlib.pyplot as pl
from matplotlib import animation
n=50000#将其均分为1000段
t=0.0001#每段时间间隔为1
G=10#万有引力系数
a=[-2,0,3,0,0,np.sqrt(3)]
b=[0,0.5,0,-0.5,0.5,0]
c=[3,1,2]#质量数组

X1=a[0]
Y1=a[1]
V1x=b[0]
V1y=b[1]
m1=c[0]#第一个质点的初始值
X2=a[2]
Y2=a[3]
V2x=b[2]
V2y=b[3]
m2=c[1]#第二个质点的初始值
X3=a[4]
Y3=a[5]
V3x=b[4]
V3y=b[5]
m3=c[2]#第三个质点的初始值
Loc1x=[]
Loc1y=[]
Loc2x=[]
Loc2y=[]
Loc3x=[]
Loc3y=[]
for i in range(n):
    k1_x1=V1x
    k1_y1=V1y
    k1_x2=V2x
    k1_y2=V2y
    k1_x3=V3x
    k1_y3=V3y#速度
    l1_v1x=G*m2*(X2-X1)/(np.sqrt((X2-X1)**2+(Y2-Y1)**2))**3\
              +G*m3*(X3-X1)/(np.sqrt((X3-X1)**2+(Y3-Y1)**2))**3
    l1_v1y=G*m2*(Y2-Y1)/(np.sqrt((X2-X1)**2+(Y2-Y1)**2))**3\
              +G*m3*(Y3-Y1)/(np.sqrt((X3-X1)**2+(Y3-Y1)**2))**3
    l1_v2x=G*m1*(X1-X2)/(np.sqrt((X1-X2)**2+(Y1-Y2)**2))**3\
              +G*m3*(X3-X2)/(np.sqrt((X3-X2)**2+(Y3-Y2)**2))**3
    l1_v2y=G*m1*(Y1-Y2)/(np.sqrt((X1-X2)**2+(Y1-Y2)**2))**3\
              +G*m3*(Y3-Y2)/(np.sqrt((X3-X2)**2+(Y3-Y2)**2))**3
    l1_v3x=G*m1*(X1-X3)/(np.sqrt((X1-X3)**2+(Y1-Y3)**2))**3\
              +G*m2*(X2-X3)/(np.sqrt((X2-X3)**2+(Y2-Y3)**2))**3
    l1_v3y=G*m1*(Y1-Y3)/(np.sqrt((X1-X3)**2+(Y1-Y3)**2))**3\
              +G*m2*(Y2-Y3)/(np.sqrt((X2-X3)**2+(Y2-Y3)**2))**3#加速度
    k2_x1=V1x+t/2*l1_v1x   
    k2_y1=V1y+t/2*l1_v1y    
    k2_x2=V2x+t/2*l1_v2x   
    k2_y2=V2y+t/2*l1_v2y
    k2_x3=V3x+t/2*l1_v3x
    k2_y3=V3y+t/2*l1_v3y#速度
    l2_v1x=G*m2*((X2+t/2*k1_x2)-(X1+t/2*k1_x1))/(np.sqrt(((X2+t/2*k1_x2)-(X1+t/2*k1_x1))**2+((Y2+t/2*k1_y2)-(Y1+t/2*k1_y1))**2))**3\
              +G*m3*((X3+t/2*k1_x3)-(X1+t/2*k1_x1))/(np.sqrt(((X3+t/2*k1_x3)-(X1+t/2*k1_x1))**2+((Y3+t/2*k1_y3)-(Y1+t/2*k1_y1))**2))**3
    l2_v1y=G*m2*((Y2+t/2*k1_y2)-(Y1+t/2*k1_y1))/(np.sqrt(((X2+t/2*k1_x2)-(X1+t/2*k1_x1))**2+((Y2+t/2*k1_y2)-(Y1+t/2*k1_y1))**2))**3\
              +G*m3*((Y3+t/2*k1_y3)-(Y1+t/2*k1_y1))/(np.sqrt(((X3+t/2*k1_x3)-(X1+t/2*k1_x1))**2+((Y3+t/2*k1_y3)-(Y1+t/2*k1_y1))**2))**3
    l2_v2x=G*m1*((X1+t/2*k1_x1)-(X2+t/2*k1_x2))/(np.sqrt(((X1+t/2*k1_x1)-(X2+t/2*k1_x2))**2+((Y1+t/2*k1_y1)-(Y2+t/2*k1_y2))**2))**3\
              +G*m3*((X3+t/2*k1_x3)-(X2+t/2*k1_x2))/(np.sqrt(((X3+t/2*k1_x3)-(X2+t/2*k1_x2))**2+((Y3+t/2*k1_y3)-(Y2+t/2*k1_y2))**2))**3
    l2_v2y=G*m1*((Y1+t/2*k1_y1)-(Y2+t/2*k1_y2))/(np.sqrt(((X1+t/2*k1_x1)-(X2+t/2*k1_x2))**2+((Y1+t/2*k1_y1)-(Y2+t/2*k1_y2))**2))**3\
              +G*m3*((Y3+t/2*k1_y3)-(Y2+t/2*k1_y2))/(np.sqrt(((X3+t/2*k1_x3)-(X2+t/2*k1_x2))**2+((Y3+t/2*k1_y3)-(Y2+t/2*k1_y2))**2))**3
    l2_v3x=G*m1*((X1+t/2*k1_x1)-(X3+t/2*k1_x3))/(np.sqrt(((X1+t/2*k1_x1)-(X3+t/2*k1_x3))**2+((Y1+t/2*k1_y1)-(Y3+t/2*k1_y3))**2))**3\
              +G*m2*((X2+t/2*k1_x2)-(X3+t/2*k1_x3))/(np.sqrt(((X2+t/2*k1_x2)-(X3+t/2*k1_x3))**2+((Y2+t/2*k1_y2)-(Y3+t/2*k1_y3))**2))**3
    l2_v3y=G*m1*((Y1+t/2*k1_y1)-(Y3+t/2*k1_y3))/(np.sqrt(((X1+t/2*k1_x1)-(X3+t/2*k1_x3))**2+((Y1+t/2*k1_y1)-(Y3+t/2*k1_y3))**2))**3\
              +G*m2*((Y2+t/2*k1_y2)-(Y3+t/2*k1_y3))/(np.sqrt(((X2+t/2*k1_x2)-(X3+t/2*k1_x3))**2+((Y2+t/2*k1_y2)-(Y3+t/2*k1_y3))**2))**3#加速度
    k3_x1=V1x+t/2*l2_v1x   
    k3_y1=V1y+t/2*l2_v1y    
    k3_x2=V2x+t/2*l2_v2x   
    k3_y2=V2y+t/2*l2_v2y
    k3_x3=V3x+t/2*l2_v3x
    k3_y3=V3y+t/2*l2_v3y#速度
    l3_v1x=G*m2*((X2+t/2*k2_x2)-(X1+t/2*k2_x1))/(np.sqrt(((X2+t/2*k2_x2)-(X1+t/2*k2_x1))**2+((Y2+t/2*k2_y2)-(Y1+t/2*k2_y1))**2))**3\
              +G*m3*((X3+t/2*k2_x3)-(X1+t/2*k2_x1))/(np.sqrt(((X3+t/2*k2_x3)-(X1+t/2*k2_x1))**2+((Y3+t/2*k2_y3)-(Y1+t/2*k2_y1))**2))**3
    l3_v1y=G*m2*((Y2+t/2*k2_y2)-(Y1+t/2*k2_y1))/(np.sqrt(((X2+t/2*k2_x2)-(X1+t/2*k2_x1))**2+((Y2+t/2*k2_y2)-(Y1+t/2*k2_y1))**2))**3\
              +G*m3*((Y3+t/2*k2_y3)-(Y1+t/2*k2_y1))/(np.sqrt(((X3+t/2*k2_x3)-(X1+t/2*k2_x1))**2+((Y3+t/2*k2_y3)-(Y1+t/2*k2_y1))**2))**3
    l3_v2x=G*m1*((X1+t/2*k2_x1)-(X2+t/2*k2_x2))/(np.sqrt(((X1+t/2*k2_x1)-(X2+t/2*k2_x2))**2+((Y1+t/2*k2_y1)-(Y2+t/2*k2_y2))**2))**3\
              +G*m3*((X3+t/2*k2_x3)-(X2+t/2*k2_x2))/(np.sqrt(((X3+t/2*k2_x3)-(X2+t/2*k2_x2))**2+((Y3+t/2*k2_y3)-(Y2+t/2*k2_y2))**2))**3
    l3_v2y=G*m1*((Y1+t/2*k2_y1)-(Y2+t/2*k2_y2))/(np.sqrt(((X1+t/2*k2_x1)-(X2+t/2*k2_x2))**2+((Y1+t/2*k2_y1)-(Y2+t/2*k2_y2))**2))**3\
              +G*m3*((Y3+t/2*k2_y3)-(Y2+t/2*k2_y2))/(np.sqrt(((X3+t/2*k2_x3)-(X2+t/2*k2_x2))**2+((Y3+t/2*k2_y3)-(Y2+t/2*k2_y2))**2))**3
    l3_v3x=G*m1*((X1+t/2*k2_x1)-(X3+t/2*k2_x3))/(np.sqrt(((X1+t/2*k2_x1)-(X3+t/2*k2_x3))**2+((Y1+t/2*k2_y1)-(Y3+t/2*k2_y3))**2))**3\
              +G*m2*((X2+t/2*k2_x2)-(X3+t/2*k2_x3))/(np.sqrt(((X2+t/2*k2_x2)-(X3+t/2*k2_x3))**2+((Y2+t/2*k2_y2)-(Y3+t/2*k2_y3))**2))**3
    l3_v3y=G*m1*((Y1+t/2*k2_y1)-(Y3+t/2*k2_y3))/(np.sqrt(((X1+t/2*k2_x1)-(X3+t/2*k2_x3))**2+((Y1+t/2*k2_y1)-(Y3+t/2*k2_y3))**2))**3\
              +G*m2*((Y2+t/2*k2_y2)-(Y3+t/2*k2_y3))/(np.sqrt(((X2+t/2*k2_x2)-(X3+t/2*k2_x3))**2+((Y2+t/2*k2_y2)-(Y3+t/2*k2_y3))**2))**3#加速度
    k4_x1=V1x+t*l3_v1x   
    k4_y1=V1y+t*l3_v1y    
    k4_x2=V2x+t*l3_v2x   
    k4_y2=V2y+t*l3_v2y
    k4_x3=V3x+t*l3_v3x
    k4_y3=V3y+t*l3_v3y#速度
    l4_v1x=G*m2*((X2+t*k3_x2)-(X1+t*k3_x1))/(np.sqrt(((X2+t*k3_x2)-(X1+t*k3_x1))**2+((Y2+t*k3_y2)-(Y1+t*k3_y1))**2))**3\
              +G*m3*((X3+t*k3_x3)-(X1+t*k3_x1))/(np.sqrt(((X3+t*k3_x3)-(X1+t*k3_x1))**2+((Y3+t*k3_y3)-(Y1+t*k3_y1))**2))**3
    l4_v1y=G*m2*((Y2+t*k3_y2)-(Y1+t*k3_y1))/(np.sqrt(((X2+t*k3_x2)-(X1+t*k3_x1))**2+((Y2+t*k3_y2)-(Y1+t*k3_y1))**2))**3\
              +G*m3*((Y3+t*k3_y3)-(Y1+t*k3_y1))/(np.sqrt(((X3+t*k3_x3)-(X1+t*k3_x1))**2+((Y3+t*k3_y3)-(Y1+t*k3_y1))**2))**3
    l4_v2x=G*m1*((X1+t*k3_x1)-(X2+t*k3_x2))/(np.sqrt(((X1+t*k3_x1)-(X2+t*k3_x2))**2+((Y1+t*k3_y1)-(Y2+t*k3_y2))**2))**3\
              +G*m3*((X3+t*k3_x3)-(X2+t*k3_x2))/(np.sqrt(((X3+t*k3_x3)-(X2+t*k3_x2))**2+((Y3+t*k3_y3)-(Y2+t*k3_y2))**2))**3
    l4_v2y=G*m1*((Y1+t*k3_y1)-(Y2+t*k3_y2))/(np.sqrt(((X1+t*k3_x1)-(X2+t*k3_x2))**2+((Y1+t*k3_y1)-(Y2+t*k3_y2))**2))**3\
              +G*m3*((Y3+t*k3_y3)-(Y2+t*k3_y2))/(np.sqrt(((X3+t*k3_x3)-(X2+t*k3_x2))**2+((Y3+t*k3_y3)-(Y2+t*k3_y2))**2))**3
    l4_v3x=G*m1*((X1+t*k3_x1)-(X3+t*k3_x3))/(np.sqrt(((X1+t*k3_x1)-(X3+t*k3_x3))**2+((Y1+t*k3_y1)-(Y3+t*k3_y3))**2))**3\
              +G*m2*((X2+t*k3_x2)-(X3+t*k3_x3))/(np.sqrt(((X2+t*k3_x2)-(X3+t*k3_x3))**2+((Y2+t*k3_y2)-(Y3+t*k3_y3))**2))**3
    l4_v3y=G*m1*((Y1+t*k3_y1)-(Y3+t*k3_y3))/(np.sqrt(((X1+t*k3_x1)-(X3+t*k3_x3))**2+((Y1+t*k3_y1)-(Y3+t*k3_y3))**2))**3\
              +G*m2*((Y2+t*k3_y2)-(Y3+t*k3_y3))/(np.sqrt(((X2+t*k3_x2)-(X3+t*k3_x3))**2+((Y2+t*k3_y2)-(Y3+t*k3_y3))**2))**3#加速度 
    X1=X1+t/6*(k1_x1+2*k2_x1+2*k2_x1+k4_x1)
    Y1=Y1+t/6*(k1_y1+2*k2_y1+2*k2_y1+k4_y1)
    X2=X2+t/6*(k1_x2+2*k2_x2+2*k2_x2+k4_x2)
    Y2=Y2+t/6*(k1_y2+2*k2_y2+2*k2_y2+k4_y2)
    X3=X3+t/6*(k1_x3+2*k2_x3+2*k2_x3+k4_x3)
    Y3=Y3+t/6*(k1_y3+2*k2_y3+2*k2_y3+k4_y3)
    V1x=V1x+t/6*(l1_v1x+2*l2_v1x+2*l3_v1x+l4_v1x)
    V1y=V1y+t/6*(l1_v1y+2*l2_v1y+2*l3_v1y+l4_v1y)
    V2x=V2x+t/6*(l1_v2x+2*l2_v2x+2*l3_v2x+l4_v2x)
    V2y=V2y+t/6*(l1_v2y+2*l2_v2y+2*l3_v2y+l4_v2y)
    V3x=V3x+t/6*(l1_v3x+2*l2_v3x+2*l3_v3x+l4_v3x)
    V3y=V3y+t/6*(l1_v3y+2*l2_v3y+2*l3_v3y+l4_v3y)
    Loc1x.append(X1)
    Loc1y.append(Y1)
    Loc2x.append(X2)
    Loc2y.append(Y2)
    Loc3x.append(X3)
    Loc3y.append(Y3)       
def update_points1(num):
    point_ani1.set_data(Loc1x[50*num], Loc1y[50*num])
    return point_ani1,#更新point1
def update_points2(num):
    point_ani2.set_data(Loc2x[50*num], Loc2y[50*num])
    return point_ani2,#更新point2
def update_points3(num):
    point_ani3.set_data(Loc3x[50*num], Loc3y[50*num])
    return point_ani3,#更新point3
fig = pl.figure(figsize=(8,8))
pl.plot(Loc1x,Loc1y,linewidth=0.5,color='Green',label='point1')
pl.plot(Loc2x,Loc2y,linewidth=0.5,color='red',label='point2')
pl.plot(Loc3x,Loc3y,linewidth=0.5,color='orange',label='point3')#三个点的轨迹
pl.legend()
pl.xlim(-3,3)
pl.ylim(-2,4)
pl.ylabel(r'y', fontsize=20) 
pl.xlabel(r'x', fontsize=20) 
point_ani1, = pl.plot(Loc1x[0], Loc1y[0], "ro")
point_ani2, = pl.plot(Loc2x[0], Loc2y[0], "ro")
point_ani3, = pl.plot(Loc3x[0], Loc3y[0], "ro")
ani1 = animation.FuncAnimation(fig, update_points1, np.arange(0, 1000), interval=50, blit=False)
ani2 = animation.FuncAnimation(fig, update_points2, np.arange(0, 1000), interval=50, blit=False)
ani3 = animation.FuncAnimation(fig, update_points3, np.arange(0, 1000), interval=50, blit=False)
pl.show()
