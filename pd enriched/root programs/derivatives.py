import sympy as sym
import matplotlib 
import numpy as np 



def main():
    p0, p1, p2, p3, p4, p5, p6, x=sym.symbols('p0 p1 p2 p3 p4 p5 p6 x')
    f=p0*(sym.exp(-p1*x**p2)+sym.exp(-p3*x**p4))*(1-sym.exp(p5*x**p6))
    print("derx: ",sym.diff(f,x))
    print("derp0: ",sym.diff(f,p0))
    print("derp1: ",sym.diff(f,p1))
    print("derp2: ",sym.diff(f,p2))
    print("derp3: ",sym.diff(f,p3))
    print("derp4: ",sym.diff(f,p4))
    print("derp5: ",sym.diff(f,p5))
    print("derp6: ", sym.diff(f,p6))
    
    #compute derivative for given parameters
    x=342
    ex=0.01
    p0=-0.02462
    p1=0.1758
    p2=0.548
    p3=442.4
    p4=45.05
    p5=0.001839
    p6=1.073
    ep0=0.00082
    ep1=0.0027
    ep2=0.00225
    ep3=0
    ep4=0.0019
    ep5=0.000069
    ep6=0.005

    errorEff=((( -p0*p5*p6*x**p6*(sym.exp(-p3*x**p4) + sym.exp(-p1*x**p2))*sym.exp(p5*x**p6)/x + p0*(1 - sym.exp(p5*x**p6))*(-p1*p2*x**p2*sym.exp(-p1*x**p2)/x - p3*p4*x**p4*sym.exp(-p3*x**p4)/x))*ex)**2+(((1 - sym.exp(p5*x**p6))*(sym.exp(-p3*x**p4) + sym.exp(-p1*x**p2)))*ep0)**2+((-p0*x**p2*(1 - sym.exp(p5*x**p6))*sym.exp(-p1*x**p2))*ep1)**2+((-p0*p1*x**p2*(1 - sym.exp(p5*x**p6))*sym.exp(-p1*x**p2)*sym.log(x))*ep2)**2+((-p0*x**p4*(1 - sym.exp(p5*x**p6))*sym.exp(-p3*x**p4))*ep3)**2+((-p0*p3*x**p4*(1 - sym.exp(p5*x**p6))*sym.exp(-p3*x**p4)*sym.log(x))*ep4)**2+(( -p0*x**p6*(sym.exp(-p3*x**p4) + sym.exp(-p1*x**p2))*sym.exp(p5*x**p6))*ep5)**2+((-p0*p5*x**p6*(sym.exp(-p3*x**p4) + sym.exp(-p1*x**p2))*sym.exp(p5*x**p6)*sym.log(x))*ep6)**2)**0.5
    print("error efficiency: ", errorEff)
    # if __name__ == "__derivatives__":
main()


