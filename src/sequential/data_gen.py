from random import random
import sys

def get_rand(r):
    return random() * 2 * r - r

def dist(x1, y1, z1, x2, y2, z2):
    return ((x1-x2)**2 + (y1-y2)**2 + (z1-z2)**2)**0.5

def main():
    fout = open("config.in",'w')
    a = float(sys.argv[1])
    n = int(1000 * a)
    duration = 160000
    fout.write("Number of objects: %d\nDuration: %d (s)\n\n" %(n, duration))
    bodys = []
    V = 0.1
    P = 10 * (a ** 0.333)
    k = 0
    for i in range(n):
        x, y, z = get_rand(P), get_rand(P), get_rand(P)
        vx, vy, vz = get_rand(V), get_rand(V), get_rand(V)
        m = 0.2 * random()
        r = 0.2 * (m ** (1/3.))
        flag = False
        for X, Y, Z, R in bodys:
            if r + R >= dist(x, y, z, X, Y, Z):
                print "boooom"
                flag = True
                break
        if flag:
            i -= 1
            continue
        bodys.append((x, y, z, r))
        fout.write("%f %f %f\n%f %f %f\n%f\n\n" %(x, y, z, vx, vy, vz, m))
        k += 1
    while k < n:
        x, y, z = get_rand(P), get_rand(P), get_rand(P)
        vx, vy, vz = get_rand(V), get_rand(V), get_rand(V)
        m = 0.2 * random()
        r = 0.2 * (m ** (1/3.))
        bodys.append((x, y, z, r))
        fout.write("%f %f %f\n%f %f %f\n%f\n\n" %(x, y, z, vx, vy, vz, m))
        k += 1
    fout.close()

if __name__ == "__main__":
    main()
