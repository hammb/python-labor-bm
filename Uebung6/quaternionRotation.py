import numpy as np

class Quaternion:
    def __init__(self, vector, real = 0, ij = 0, jk = 0, ki = 0, ji = 0, kj = 0, ik = 0):
        
        self.i = vector[0]
        self.j = vector[1]
        self.k = vector[2]
        
        self.ij = ij
        self.jk = jk
        self.ki = ki
        
        self.ji = ji
        self.kj = kj
        self.ik = ik
        
        self.real = real
        
        if self.real < 0.00000000001:
            self.real = 0.
    
    def rotateY(self, angle):
        
        q = Quaternion(np.array([0,np.sin(angle/2.),0]),np.cos(angle/2.))
        qs = Quaternion(np.array([0,-np.sin(angle/2.),0]),np.cos(angle/2.))
        
        self.ij = self.i * q.j
        
        if self.ij != 0:
            self.i = 0
        
        if self.ij < 0:
            self.ji = self.ij
            self.ij = 0
        
        self.jk = self.k * q.j
        
        if self.jk != 0:
            self.k = 0
            
        if self.jk < 0:
            self.kj = self.jk
            self.jk = 0
        
        
            
        self.printQuaternion()
        
    def printQuaternion(self):
        
        print(self.real)
        
        print("i: " + str(self.i))
        print("j: " + str(self.j))
        print("k: " + str(self.k))
        
        print("ij: " + str(self.ij))
        print("jk: " + str(self.jk))
        print("ki: " + str(self.ki))
        
        print("ji: " + str(self.ji))
        print("kj: " + str(self.kj))
        print("ik: " + str(self.ik))
        
q = Quaternion(np.array([2,1,0]))
q.rotateY(np.pi)