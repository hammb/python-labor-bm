import numpy as np

class Quaternion:
    
    def __init__(self):
        self.i = 0
    
    def normalize(self,v, tolerance=0.00001):
        mag2 = np.sum([n * n for n in v])
        if np.abs(mag2 - 1.0) > tolerance:
            mag = np.sqrt(mag2)
            v = (n / mag for n in v)
        return v

    def q_mult(self, q1, q2):
        w1, x1, y1, z1 = q1
        w2, x2, y2, z2 = q2
        w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2
        x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2
        y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2
        z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2
        return w, x, y, z

    def q_conjugate(self,q):
        w, x, y, z = q
        return (w, -x, -y, -z)
    
    def qv_mult(self,q1, v1):
        q2 = (0.0,) + v1
        return self.q_mult(self.q_mult(q1, q2), self.q_conjugate(q1))[1:]
    
    def axisangle_to_q(self,v, theta):
        v = self.normalize(v)
        x, y, z = v
        theta /= 2
        w = np.cos(theta)
        x = x * np.sin(theta)
        y = y * np.sin(theta)
        z = z * np.sin(theta)
        return w, x, y, z
    
    def q_to_axisangle(self,q):
        w, v = q[0], q[1:]
        theta = np.acos(w) * 2.0
        return self.normalize(v), theta
    
#x_axis_unit = (1, 0, 0)
y_axis_unit = (2, 1, 0)
#z_axis_unit = (0, 0, 1)

#q1 = Quaternion()
q2 = Quaternion()
#q3 = Quaternion()

#r1 = q1.axisangle_to_q(x_axis_unit, np.pi / 2)
r2 = q2.axisangle_to_q(y_axis_unit, np.pi)
#r3 = q3.axisangle_to_q(z_axis_unit, np.pi / 2)

v = q2.qv_mult(r2, y_axis_unit)
#v = q2.qv_mult(r2, v)
#v = q3.qv_mult(r3, v)

print(v)