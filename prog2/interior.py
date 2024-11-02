import numpy as np
import typing as tp

MAX_ITERATIONS = 1000

def interior_point_method(c, A, x, eps: float, alpha: float, maximize: bool):
    if maximize:
        c = np.multiply(-1, c)

    i = 0
    x_prev = x
    while True:
        i += 1
        if i > MAX_ITERATIONS:
            print("Interior method did not converge")
            break
        
        D = np.diag(x)
        
        A_prime = np.dot(A, D)
        c_prime = np.dot(c, D)
        if np.linalg.det(np.dot(A_prime, np.transpose(A_prime))) == 0:
            print("Interior method did not converge")
            break
        aTaaT1a = np.dot(np.dot(np.transpose(A_prime), np.linalg.inv(np.dot(A_prime, np.transpose(A_prime)))), A_prime)
        P = np.eye(A.shape[1]) - aTaaT1a
        c_p = np.dot(P, c_prime)
        
        nu = np.absolute(np.min(c_p))
        if nu == 0:
            print("Interior method did not converge")
            break

        x_prime = np.add(np.ones(len(x)), np.multiply(alpha / nu, c_p))
        x_prev = x
        x = np.dot(D, x_prime)

        
        if np.linalg.norm(x - x_prev) < eps:
            print(f"Interior method converged in {i} iterations")
            break

    return x


def main():
    c = np.array([1, 1, 1])
    A = np.array([[1, 2, 3], [2, 3, 4], [3, 4, 5]])
    x = np.array([1, 1, 1])
    eps = 1e-6
    alpha = 0.5
    print(interior_point_method(c, A, x, eps, alpha))

if __name__ == "__main__":
    main()