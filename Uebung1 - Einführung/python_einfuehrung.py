# -*- coding: utf-8 -*-
"""
Created on Tue Apr  3 15:58:49 2018

@author: bkraus
"""
## Kurze Einfuerung in Python3


## Typen & Variablen

a = 12  # int
s = '12'
S = "Hello World"
print(s)
print(S , a) 
print("Test %4d %s %2.3f" % (a,S, 5.7777))

S[6]
S[1:5]
S[-1]

b = a/5 #float

a = a/5


## Operatoren
a = 12

a = a//5

a = -a
a = a**4
a <= b
#a++     #nope
(a == b) or (a < b)  # or, and, not

6<<3 
a += a

## Tupel,  Listen, Dicts

list_a = []
list_a = [1, 2, 3, 5]

list_a[-1]= 4
list_a.append(5)    
list_b = ['foo','bar']
list_c = ['test', list_a, list_b]
list_c[1][2]

"test" in list_c
"foo" in list_c

list_a = ['a', 'b', 'c']

list_a + list_a

4 * list_a

## Sliceing

list_d = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

list_d[1:7:2]
list_d[:]
list_d[3:]
list_d[:3]
list_d[::3]
list_d[:-3]
list_d[::-1]
list_d[::-3]

tupel = (2, 3) 
tupel[1]
#tupel[1] = 1 #performanter, nicht manipulierbar
tupel = (3, 4)

dict_a ={}
dict_a ={'a' : 1, 'b': 2, 'c' : 3}
dict_a['c']
dict_a['d'] = 4
      
#dict_b = {[1,2] : 'y'}
dict_b = {(1,2) : 'y'}

len(dict_a)
del dict_a['b']

print(dict_a)
print(dict_a.keys())
#dict_a.keys()[1]
type(dict_a.keys())
list(dict_a.keys())[1]
[*dict_a][1] #unpacking. Neu in Python 3.5

## Bedingungen
if 2 == (1+1) and 2-1 == 1:
    print('I know math')

if 4 in list_d:
    print ("element 4 in list")
elif 5 in list_d:
    print ("element 5 in list")
else:
    print ("element 4 and 5 not in list")

    
print(any([False, True, False]))
print(all([True, 2!= 3-2, True]))
# there is no switch/case, move along.

## While

count = 0
while (count < 3):    
    count = count+1
    print(count)
    
## For

# C/C++:  for (int i = 0; i < n; i++)
for i in range(5):
    print(i, '.')
    
list_e = ["Hello", "World", [1, 2, 3]]
for ele in list_e:
    print(ele)
    
S = "Hello World"
for letter in S:
    if letter == 'e':
        pass
    elif letter == 'r':
        break
    else:
        print(letter)

## List Comprehension

list_f = [1, '4', 9, 'a', 0, 4]
squared_ints = [ element**2 for element in list_f if type(element) == int ]
#       Output expression   variable   input sequence      optional Predicate
print(squared_ints)

colours = [ "red", "green", "blue" ]
things = [ "house", "car", "tree" ]
coloured_things = [ (x,y) for x in colours for y in things ]
print( coloured_things )


print(dict_a)
[element for element in dict_a.keys()]

## Funktionen

def numbers_to_strings(argument):
    case = {
        0: "zero",
        1: "one",
        2: "two",
    }
    return case.get(argument, "nothing")

def zero():
    return "zero"

def one():
    return "one"

def numbers_to_functions_to_strings2(argument):
    case = {
        0: zero,
        1: one,
        2: lambda: "two",
    }
    # Get the function from switcher dictionary
    func = case.get(argument, lambda: "nothing")
    # Execute the function
    return func()

def more_params_more_return( a=1, b=2):
    return a, b+a, b

more_params_more_return(2, 4)

## Weitere Compound statements
with open("x.txt") as f:
    data = f.read()

def foo():
    try:
        #1/0
        print( 'try' )
        return 1
    except: 
        print( 'except')
        return -1
    finally:
        print('finally')
        return 0
foo()


## Classes

class Test:
    __hiddenVar = 42
    def __init__(self, mString = 'World'):
        self.mString = mString
    def mFun(self):
        print('Hello ' + self.mString)
    def getHidden(self):
        return self.__hiddenVar
        
t = Test('Ben')
t.mFun()
t = Test()
t.mFun()
class Base1:
    def __init__(self):
        self.str1 = "Hello"
        print( "Base1" )
 
class Base2:
    def __init__(self):
        self.str2 = "World"       
        print( "Base2" )
 
class Derived(Base1, Base2):
    def __init__(self):
         
        # Calling constructors of Base1
        # and Base2 classes
        Base1.__init__(self)
        Base2.__init__(self)
        print( "Derived" )
         
    def printStrs(self):
        print(self.str1, self.str2)

ob = Derived()
ob.printStrs()

## Globale Variablen
a = 'global a'
b = 'globalb '

def global_vars():
    global a
    print(a)
    a ='immernoch global'
    b = 'local b'
    print(b)
    

global_vars()
print(a)
print(b)

## Tiefes und Flaches Kopieren

x = 3
y = x
print(id(x), id(y))
y = 4
print(id(x), id(y))
print(x,y)

colours1 = [ "red", "green", "blue" ]
colours2 = colours1

colours2[2] = 'yellow'

colours3 = colours2[:]

colours3[2] = 'purple'

## Imports
import numpy as np
from matplotlib.pyplot import plot
sine= np.sin(np.arange(0,2*np.pi, 0.01))
plot(sine)

