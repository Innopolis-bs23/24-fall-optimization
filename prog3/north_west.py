import numpy as np

def north_west_corner_method(supply, demand, costs):
    s = supply.copy()
    d = demand.copy()
    
    # initialize zeros matrix for x0
    x0 = np.zeros((len(s), len(d)), dtype=int) 

    # the north-west corner method
    i, j =0, 0
    ans = 0
    while i != len(costs) and j != len(costs[0]):
        min_x0 = min(s[i], d[j])
        ans += min_x0 * costs[i][j]
        x0[i, j] = min_x0
        s[i] -= min_x0
        d[j] -= min_x0

        # move to the next
        if s[i] == 0:
            i += 1
        if d[j] == 0:
            j += 1

    return x0, ans

def main():
    supply = [300, 400, 500]
    demand = [250, 350, 400, 200]
    costs = np.array([
        [3, 1, 7, 4], 
        [2, 6, 5, 9],
        [8, 3, 3, 2]
    ])
    
    x0, total_cost = north_west_corner_method(supply, demand, costs)
    print("North-west corner matrix:")
    print(x0)
    print("Initial basic feasible solution by North-west:")
    print(total_cost)

if __name__ == "__main__":
    main()
