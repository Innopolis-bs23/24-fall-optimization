import numpy as np

INF = 10**3

# function for finding the difference of the supply and demand
def find_diff(supply, demand, costs):
    row_diff = []
    for i in range(len(supply)):
        rows = [val for val in costs[i] if val != INF]
        if len(rows) > 1:
            rows.sort()
            row_diff.append(rows[1] - rows[0])
        else:
            row_diff.append(0)

    col_diff = []
    for j in range(len(demand)):
        cols = [costs[i][j] for i in range(len(supply)) if costs[i][j] != INF]
        if len(cols) > 1:
            cols.sort()
            col_diff.append(cols[1] - cols[0])
        else:
            col_diff.append(0) 
    return row_diff, col_diff

# function for Vogel's approximation
def vogels_approximation_method(supply, demand, costs):
    grid = costs.copy()
    s = supply.copy()
    d = demand.copy()
    n = len(supply)
    m = len(demand)
    x0 = np.zeros((len(s), len(d)), dtype=int) 
    ans = 0
    
    # main loop
    while max(s) > 0 and max(d) > 0:
        row, col = find_diff(s, d, grid)
        max_row = max(row)
        max_col = max(col)

        # checking where is the greatest max element
        if max_row >= max_col:
            for i, val_s in enumerate(row):
                if val_s == max_row:
                    valid_row = [val for val in grid[i] if val != INF]
                    if not valid_row:
                        continue 
                    min_row = min(valid_row)
                    for j, val_s2 in enumerate(grid[i]):
                        if val_s2 == min_row:
                            # finding min of supply and demand
                            min_sd = min(s[i], d[j])
                            ans += min_row * min_sd
                            x0[i][j] = min_sd
                            s[i] -= min_sd
                            d[j] -= min_sd
                            if d[j] == 0:
                                for r in range(n):
                                    grid[r][j] = INF
                            if s[i] == 0:
                                grid[i] = [INF for _ in range(m)]
                            break
                    break
        else:
            for j, val_d in enumerate(col):
                if val_d == max_col:
                    valid_col = [grid[i][j] for i in range(n) if grid[i][j] != INF]
                    if not valid_col:
                        continue
                    min_col = min(valid_col)
                    for i in range(n):
                        if grid[i][j] == min_col:
                            # finding min of supply and demand
                            min_sd = min(s[i], d[j])
                            ans += min_col * min_sd
                            x0[i][j] = min_sd
                            s[i] -= min_sd
                            d[j] -= min_sd
                            if d[j] == 0:
                                for r in range(n):
                                    grid[r][j] = INF
                            if s[i] == 0:
                                grid[i] = [INF for _ in range(m)]
                            break
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
    
    x0, total_cost = vogels_approximation_method(supply, demand, costs)
    print("Vogel's approximation matrix")
    print(x0)
    print("Initial basic feasible solution by Vogel's:")
    print(total_cost)

if __name__ == "__main__":
    main()