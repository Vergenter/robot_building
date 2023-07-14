from ortools.linear_solver import pywraplp
# Instantiate a mixed-integer solver.
solver:pywraplp.Solver = pywraplp.Solver.CreateSolver('SCIP')

# Set up the variables and constraints.
T = 100 # Change to the number of time steps
O, C, B, G = [], [], [], []
o, c, b, g = [], [], [], []
for t in range(T):
    if t == 0:
        O.append(solver.IntVar(1, 1, 'O[%i]' % t)) # Number of ore robots is fixed to 1 at t = 0
        C.append(solver.IntVar(0, 0, 'C[%i]' % t)) # Number of clay robots is fixed to 0 at t = 0
        B.append(solver.IntVar(0, 0, 'B[%i]' % t)) # Number of obsidian robots is fixed to 0 at t = 0
        G.append(solver.IntVar(0, 0, 'G[%i]' % t)) # Number of geode robots is fixed to 0 at t = 0
        o.append(solver.IntVar(0, 0, 'o[%i]' % t))
        c.append(solver.IntVar(0, 0, 'c[%i]' % t))
        b.append(solver.IntVar(0, 0, 'b[%i]' % t))
        g.append(solver.IntVar(0, 0, 'g[%i]' % t))
    else:
        O.append(solver.IntVar(0, 3, 'O[%i]' % t)) 
        C.append(solver.IntVar(0, 14, 'C[%i]' % t)) 
        B.append(solver.IntVar(0, 7, 'B[%i]' % t)) 
        G.append(solver.IntVar(0, T, 'G[%i]' % t)) 
        o.append(solver.IntVar(0, T*T, 'o[%i]' % t))
        c.append(solver.IntVar(0, T*T, 'c[%i]' % t))
        b.append(solver.IntVar(0, T*T, 'b[%i]' % t))
        g.append(solver.IntVar(0, T*T, 'g[%i]' % t))

# Add constraints.
for t in range(1, T):
    solver.Add(O[t] >= O[t-1])
    solver.Add(C[t] >= C[t-1])
    solver.Add(B[t] >= B[t-1])
    solver.Add(G[t] >= G[t-1])

    solver.Add(o[t] >= 4*(O[t]-1) + 2*C[t] + 3*B[t] + 2*G[t])
    solver.Add(c[t] >= 14*B[t])
    solver.Add(b[t] >= 7*G[t])

    solver.Add(o[t] == o[t-1] + O[t-1])
    solver.Add(c[t] == c[t-1] + C[t-1])
    solver.Add(b[t] == b[t-1] + B[t-1])
    solver.Add(g[t] == g[t-1] + G[t-1])

    solver.Add(O[t] - O[t-1] + C[t] - C[t-1] + B[t] - B[t-1] + G[t] - G[t-1] <= 1)

# Define the objective.
objective = solver.Objective()
for t in range(T):
    objective.SetCoefficient(G[t], 1)
objective.SetMaximization()

# Solve the problem and print the solution.
status = solver.Solve()


if status == pywraplp.Solver.OPTIMAL:
    print('Solution:')
    for t in range(T-1,T):
        print('o[%i] = %i' % (t, o[t].solution_value()))
        print('c[%i] = %i' % (t, c[t].solution_value()))
        print('b[%i] = %i' % (t, b[t].solution_value()))
        print('g[%i] = %i' % (t, g[t].solution_value()))
        print('O[%i] = %i' % (t, O[t].solution_value()))
        print('C[%i] = %i' % (t, C[t].solution_value()))
        print('B[%i] = %i' % (t, B[t].solution_value()))
        print('G[%i] = %i' % (t, G[t].solution_value()))
    print('Objective value =', solver.Objective().Value())
    print('Problem solved in %f milliseconds' % solver.wall_time())
    print('Problem solved in %d iterations' % solver.iterations())
    print('Problem solved in %d nodes' % solver.nodes())
else:
    print('The problem does not have an optimal solution.')