import numpy as np

INF = 10**3

def calculate_penalties(costs):
    s_penalty = []
    d_penalty = []
    for i in range(len(costs)):
        arr = costs[i][:]
        arr.sort()
        s_penalty.append(arr[1]-arr[0])
    d = 0
    while d < len(costs[0]):
        arr = []
        for i in range(len(costs)):
            arr.append(costs[i][d])
        arr.sort()
        d += 1
        d_penalty.append(arr[1]-arr[0])
    return s_penalty, d_penalty

def vogel_approximation_method(supply, demand, costs):
    s = supply.copy()
    d = demand.copy()
    ans = 0
    while max(s) != 0 or max(d) != 0:
        row, col = calculate_penalties(costs)
        max_s_penalty = max(row)
        max_d_penalty = max(col)

    if max_s_penalty >= max_d_penalty:
        for ind, val in enumerate(row):
            if(val == max_s_penalty):
                min_s_penalty = min(costs[ind])
                for ind2, val2 in enumerate(costs[ind]):
                    if(val2 == min_s_penalty):
                        min_d_penalty = min(supply[ind], demand[ind2])
                        ans += min_d_penalty * min_s_penalty
                        supply[ind] -= min_d_penalty
                        demand[ind2] -= min_d_penalty
                        if(demand[ind2] == 0):
                            for r in range(len(costs)):
                                costs[r][ind2] = INF
                        else:
                            costs[ind] = [INF for i in range(len(costs[0]))]
                        break
                break
    else:
        for ind, val in enumerate(col):
            if(val == max_d_penalty):
                min_s_penalty = INF
                for j in range(len(costs)):
                    min_s_penalty = min(min_s_penalty, costs[j][ind])

                for ind2 in range(len(costs)):
                    val2 = costs[ind2][ind]
                    if val2 == min_s_penalty:
                        min_d_penalty = min(supply[ind2], demand[ind])
                        ans += min_d_penalty * min_s_penalty
                        supply[ind2] -= min_d_penalty
                        demand[ind] -= min_d_penalty
                        if(demand[ind] == 0):
                            for r in range(len(costs)):
                                costs[r][ind] = INF
                        else:
                            costs[ind2] = [INF for i in range(len(costs[0]))]
                        break
                break

    return ans

def total_cost(x0, costs):
    total = np.sum(x0 * costs)
    return total

def main():
    supply = [300, 400, 500]
    demand = [250, 350, 400, 200]
    costs = np.array([
        [3, 1, 7, 4], 
        [2, 6, 5, 9],
        [8, 3, 3, 2]
    ])
    
    print(vogel_approximation_method(supply, demand, costs))
    # x0 = vogel_approximation_method(supply, demand, costs)
    # print("Vogel's Approximation Method Allocation:")
    # print(x0)
    
    # total_cost_value = total_cost(x0, costs)
    # print("Total transportation cost:")
    # print(total_cost_value)

if __name__ == "__main__":
    main()