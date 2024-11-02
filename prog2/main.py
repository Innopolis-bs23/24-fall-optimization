import numpy as np
from interior import interior_point_method
from simplex import simplex_method

DEBUG = False

def read_data():
    if DEBUG:
        c = [-2, 3, 0, 0]
        A = [[2, -2, 8, 0], [-6, -1, 0, -1]]
        b = [-2, 3, 0, 0]
        x = [2, 2, 4, 3]
        maximize = True
        eps = 0.01
        return c, A, b, x, maximize, eps
    
    # Input all data
    c = [float(i) for i in input("Vector of coefficients of the objective function: ").split()]
    A = [[float(i) for i in input("Row of X of the coefficients: ").split()] for _ in range(int(input("Number of constraints: ")))]
    b = [float(i) for i in input("Vector of coefficients of the right-hand side of the constraints: ").split()]
    x = [float(i) for i in input("Vector of coefficients of the initial point: ").split()]
    maximize = input("max/min: ") == "max"
    eps = float(input("The approximation accuracy: "))
    return c, A, b, x, maximize, eps


def main():
    c, A, b, x, maximize, eps = read_data()
    # Running simplex method (Assigment 1)
    _, _, _, soltuion, simplex_result, _ = simplex_method(c, A, b, eps, maximize)
    print("Simplex method result: ")
    print("X=", end="")
    print("(", end="")
    print(*simplex_result, end="", sep=", ")
    print(")")
    print("Solution=", soltuion, sep="")
    print()

    c_np = np.array(c)
    A_np = np.array(A)
    x_np = np.array(x)
    # Running interior point method (Assigment 2)
    interior_result = interior_point_method(c_np, A_np, x_np, eps, 0.5, maximize)
    print("Interior method result (alpha = 0.5): ")
    print("(", end="")
    print(*interior_result, end="", sep=", ")
    print(")")
    print("Solution=", np.dot(c_np, interior_result), sep="")

    interior_result = interior_point_method(c_np, A_np, x_np, eps, 0.9, maximize)
    print("Interior method result (alpha = 0.9): ")
    print("(", end="")
    print(*interior_result, end="", sep=", ")
    print(")")
    print("Solution=", np.dot(c_np, interior_result), sep="")

if __name__ == "__main__":
    main()