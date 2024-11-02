from math import log

def check_solved(c, A, b):
    for i in range(len(c)):
        if c[i] < 0:
            return False  
    return True

def simplex_step(c, A, b, solution, eps, X):
    # find pivot
    pivot = None
    min_pivot = None
    for i in range(len(c)):
        if (min_pivot is None or c[i] < min_pivot) and c[i] < 0:
            min_pivot = c[i]
            pivot = i
    if pivot is None:   # solved already
        return c, A, b, solution, X, True
    min_ratio = None
    row = None
    for i in range(len(A)):
        if A[i][pivot] == 0:
            ratio = 10**100
        else:
            ratio = b[i] / A[i][pivot]
            if (min_ratio is None or ratio < min_ratio) and ratio > 0:  # find the minimum ratio
                min_ratio = ratio
                row = i
    # update the X
    if min_ratio is None: # check if solvable
        return c, A, b,solution, X, False
    divider = A[row][pivot] # save our value 
    b[row] = b[row] / divider # normalize the pivot row
    for i in range(len(A[row])):
        A[row][i] = A[row][i] / divider
    for i in range(len(A)):
        if i != row:
            if A[i][pivot] != 0:    # update the other rows
                fact = A[row][pivot]  / A [i][pivot]
                b[i] -=  b[row]/fact
                for j in range(len(A[i])):
                    A[i][j] -=  A[row][j]/fact
    # update the vector
    c_factor = c[pivot] / A[row][pivot]
    for i in range(len(c)):
        c[i] -= c_factor * A[row][i]
        
    solution -= c_factor * b[row]
    X[pivot] = row
    return c, A, b,solution, X, True
    

def simplex_method(c, A, b, epsilon, maximize=True):
    solution = 0
    X = [None for _ in range(len(c))]
    ifSolvable = True
    check_for_accuracy = 0
    if maximize:
        c = [-i for i in c]
    while not check_solved(c, A, b):
        if ifSolvable is False: # check if unbounded
            return c, A, b, solution, X, ifSolvable
        c, A, b, solution, X, ifSolvable = simplex_step(c, A, b, solution, epsilon, X)
        if abs(check_for_accuracy - solution) <= epsilon: # check for accuracy
            if not maximize:
                solution *= -1 
            for i in range(len(X)):
                X[i] = 0
                if X[i] is not None:
                    X[i] = b[X[i]]
            return c, A, b, solution, X, ifSolvable
        check_for_accuracy = solution
    for i in range(len(X)):
        if X[i] is None:
            X[i] = 0
        else:
            X[i] = b[X[i]]
    if(not maximize):
                solution *= -1            
    return c, A, b, solution, X, ifSolvable


def main():
    # Vector of coefficients of the objective function
    c = [float(i) for i in input("Vector of coefficients of the objective function: ").split()]
    # X of coefficients of the constraints
    A = []
    while True:
        row_input = input("Row of X of the coefficients (empty string to end reading): ")
        if row_input == "":
            break
        row = list(map(float, row_input.split()))
        A.append(row)
    # Vector of coefficients of the right-hand side of the constraints  
    b = [float(i) for i in input("Vector of coefficients of the right-hand side of the constraints: ").split()]
    # Maximization problem
    maximize = input("max/min: ") == "max"
    # The approximation accuracy
    epsilon  = float(input("The approximation accuracy: "))
    c, A, b, solution, X, ifSolvable = simplex_method(c, A, b, epsilon, maximize)
    if(ifSolvable):
        num_of_zeros = round(1/log(0.1, epsilon))
        print("Optimal solution:")
        print(("{:." + str(num_of_zeros) + "f}" ).format(round(solution, num_of_zeros)))
        output_X = ""
        for x in X:
            output_X += ("{:." + str(num_of_zeros) + "f}" ).format(round(x, num_of_zeros)) + " "
        print(output_X)
    else:
        print("Unbounded solution")    
        
if __name__ == "__main__":
    main()
    
