import numpy as np

INF = 10**3

# function to calculate the largest costs in the remaining rows and columns
def find_russell_costs(supply, demand, costs):
    u = [0] * len(supply)  # largest cost in each row
    v = [0] * len(demand)  # largest cost in each column

    # calculate the largest costs for each row and column
    for i in range(len(supply)):
        valid_costs = [cost for cost in costs[i] if cost != INF]
        if valid_costs:
            u[i] = max(valid_costs)

    for j in range(len(demand)):
        valid_costs = [costs[i][j] for i in range(len(supply)) if costs[i][j] != INF]
        if valid_costs:
            v[j] = max(valid_costs)

    return u, v

# function for Russell's approximation
def russells_approximation_method(supply, demand, costs):
    grid = costs.copy()
    s = supply.copy()
    d = demand.copy()
    n = len(s)
    m = len(d)
    x0 = np.zeros((len(s), len(d)), dtype=int) 
    ans = 0
    
    # main loop
    while max(s) > 0 and max(d) > 0:
        u, v = find_russell_costs(s, d, grid)
        
        delta = np.full((n, m), INF)  # initialize delta matrix
        for i in range(n):
            for j in range(m):
                if grid[i][j] != INF: 
                    delta[i][j] = grid[i][j] - u[i] - v[j]

        # find the maximum absolute negative delta
        min_delta = np.min(delta)
        if min_delta >= 0:
            break

        # find the indices of the minimum delta
        for i in range(n):
            for j in range(m):
                if delta[i][j] == min_delta:
                    min_sd = min(s[i], d[j])
                    x0[i][j] = min_sd
                    ans += grid[i][j] * min_sd
                    s[i] -= min_sd
                    d[j] -= min_sd

                    if s[i] == 0:
                        grid[i] = [INF] * m
                    if d[j] == 0:
                        for r in range(n):
                            grid[r][j] = INF
                    break
            else:
                continue
            break

    return x0, ans

def main():
    supply = [300, 400, 500]
    demand = [250, 350, 400, 200]
    costs = np.array([
        [3, 1, 7, 4], 
        [2, 6, 5, 9],
        [8, 3, 3, 2]
    ])
    
    x0, total_cost = russells_approximation_method(supply, demand, costs)
    print("Russell's approximation matrix")
    print(x0)
    print("Initial basic feasible solution by Russell's:")
    print(total_cost)

if __name__ == "__main__":
    main()