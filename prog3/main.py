import numpy as np
from tabulate import tabulate
from north_west import north_west_corner_method
from vogels import vogels_approximation_method
from russell import russells_approximation_method

DEBUG = False

def read_data():
    if DEBUG:
        supply = [300, 400, 500]
        demand = [250, 350, 400, 200]
        costs = np.array([
            [3, 1, 7, 4], 
            [2, 6, 5, 9],
            [8, 3, 3, 2]
        ])
        return supply, demand, costs
    
    supply = [int(i) for i in input("Supplies: ").split()]
    demand = [int(i) for i in input("Demands: ").split()]
    costs = [[int(i) for i in input("Row of the costs: ").split()] for _ in range(len(supply))]
    return supply, demand, costs

# что такое вектор начального базисного решения?
# в этой программе вместо вектора выводится конечная матрица, по которой вычислялось решение
# не проверено: когда методы не применимы?
def main():
    supply, demand, costs = read_data()

    if sum(supply) == sum(demand) and len(supply) == 3 and len(demand) == 4:
        table_data = []
        for i, row in enumerate(costs):
            table_data.append([f"S{i + 1}"] + [str(val) for val in row] + [str(supply[i])])
        table_data.append(["Demand"] + demand + [""])
        headers = ["", "D1", "D2", "D3", "D4", "Supply"]
        print(tabulate(table_data, headers=headers, tablefmt="grid"))
        nw_x0, nw_ans = north_west_corner_method(supply, demand, costs)
        print("North-west corner matrix: ")
        print(nw_x0)
        print("Initial basic feasible solution by North-west: ")
        print(nw_ans)

        vg_x0, vg_ans = vogels_approximation_method(supply, demand, costs)
        print("Vogel's approximation matrix")
        print(vg_x0)
        print("Initial basic feasible solution by Vogel's:")
        print(vg_ans)
  
        rus_x0, rus_ans = russells_approximation_method(supply, demand, costs)
        print("Russell's approximation matrix")
        print(rus_x0)
        print("Initial basic feasible solution by Russell's:")
        print(rus_ans)
    elif sum(supply) != sum(demand):
        print("The problem is not balanced!")
    elif len(supply) != 3 or len(demand) != 4:
        print("The method is not applicable!")

    
if __name__ == "__main__":
    main()
