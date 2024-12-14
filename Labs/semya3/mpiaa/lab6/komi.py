import itertools
import time
import random

def tsp_brute_force(graph, start):
    nodes = list(graph.keys())
    nodes.remove(start)
    min_path = None
    min_cost = float('inf')

    for perm in itertools.permutations(nodes):
        current_cost = graph[start][perm[0]]
        for i in range(len(perm) - 1):
            current_cost += graph[perm[i]][perm[i + 1]]
        current_cost += graph[perm[-1]][start]

        if current_cost < min_cost:
            min_cost = current_cost
            min_path = [start] + list(perm)

    return min_path, min_cost


def lower_bound(graph, visited):
    bound = 0
    for node in graph:
        if node not in visited:
            min1, min2 = float('inf'), float('inf')
            for neighbor in graph[node]:
                if graph[node][neighbor] < min1:
                    min2 = min1
                    min1 = graph[node][neighbor]
                elif graph[node][neighbor] < min2:
                    min2 = graph[node][neighbor]
            bound += (min1 + min2) / 2
    return bound


def bnb(graph, visited, best_path, best_cost):
    if len(visited) == len(graph):
        current_cost = 0
        for i in range(len(visited) - 1):
            current_cost += graph[visited[i]][visited[i + 1]]
        current_cost += graph[visited[-1]][visited[0]]
        if current_cost < best_cost:
            return visited[:], current_cost
        return best_path, best_cost

    for node in graph:
        if node not in visited:
            new_visited = visited + [node]
            if lower_bound(graph, new_visited) < best_cost:
                new_path, new_cost = bnb(graph, new_visited, best_path, best_cost)
                if new_cost < best_cost:
                    best_path, best_cost = new_path, new_cost
    return best_path, best_cost


def tsp_bnb(graph, start):
    initial_visited = [start]
    best_path, best_cost = bnb(graph, initial_visited, [start], float('inf'))
    return best_path, best_cost


def tsp_greedy(graph, start):
    current = start
    path = [current]
    unvisited = set(graph.keys())
    unvisited.remove(current)

    while unvisited:
        next_node = min(unvisited, key=lambda x: graph[current][x])
        path.append(next_node)
        unvisited.remove(next_node)
        current = next_node

    path.append(start)
    cost = sum(graph[path[i]][path[i + 1]] for i in range(len(path) - 1))
    return path, cost


def tsp_ls(graph, start):
    def length(path):
        """Вычисляет длину маршрута."""
        return sum(graph[path[i]][path[i + 1]] for i in range(len(path) - 1))

    def two_opt_improve(path):
        """Улучшает маршрут с помощью 2-opt."""
        n = len(path)
        improved = False
        while True:
            for i in range(1, n - 2):
                for j in range(i + 1, n - 1):
                    # Ребра E1 = (A, B) и E2 = (C, D)
                    A, B = path[i - 1], path[i]
                    C, D = path[j], path[j + 1]
                    # Старый вес
                    old_weight = graph[A][B] + graph[C][D]
                    # Новый вес
                    new_weight = graph[A][C] + graph[B][D]
                    # Если новый вес меньше, перестраиваем маршрут
                    if new_weight < old_weight:
                        # Переворачиваем часть маршрута от B до C
                        path = path[:i] + path[i:j + 1][::-1] + path[j + 1:]
                        improved = True
            if not improved:
                break
            improved = False
        return path

    # Генерация случайного маршрута
    nodes = list(graph.keys())
    nodes.remove(start)
    random.shuffle(nodes)
    current_path = [start] + nodes + [start]

    # Улучшение маршрута с помощью 2-opt
    improved_path = two_opt_improve(current_path)

    # Повторяем улучшение, пока маршрут не перестанет улучшаться
    while length(improved_path) < length(current_path):
        current_path = improved_path
        improved_path = two_opt_improve(current_path)

    return current_path, length(current_path)

def tsp_ls_multiple_restarts(graph, start, num_restarts=10):
    best_path = None
    best_cost = float('inf')

    for _ in range(num_restarts):
        current_path, current_cost = tsp_ls(graph, start)
        if current_cost < best_cost:
            best_path = current_path
            best_cost = current_cost

    return best_path, best_cost


def tsp_ga(graph, P=50, N=20, MaxIt=100, Pm=0.1):
    def length(path):
        """Вычисляет длину маршрута."""
        return sum(graph[path[i]][path[i + 1]] for i in range(len(path) - 1))

    def generate_initial_population(graph, P):
        """Генерирует начальную популяцию из P хромосом."""
        population = []
        nodes = list(graph.keys())
        for _ in range(P):
            random.shuffle(nodes)
            population.append([nodes[0]] + nodes[1:] + [nodes[0]])  # Добавляем стартовую вершину в начало и конец
        return population

    def sus(population, N):
        """Селекция родителей с использованием SUS."""
        weights = [1 / length(chromosome) for chromosome in population]  # Используем обратный вес для выбора
        total_weight = sum(weights)
        distance = total_weight / N
        start = random.uniform(0, distance)
        chosen = []
        k = 0
        sum_weight = weights[0]
        for i in range(N):
            point = start + i * distance
            while sum_weight < point:
                k += 1
                sum_weight += weights[k]
            chosen.append(population[k])
        return chosen

    def crossoverER(parent1, parent2):
        """Скрещивание с использованием карты ребер."""
        edge_map = {node: set() for node in graph}
        for parent in [parent1, parent2]:
            for i in range(len(parent) - 1):
                edge_map[parent[i]].add(parent[i + 1])
                edge_map[parent[i + 1]].add(parent[i])

        offspring = []
        start_node = random.choice(list(graph.keys()))
        offspring.append(start_node)
        current_node = start_node

        while len(offspring) < len(graph):
            candidates = list(edge_map[current_node])
            for neighbor in candidates:
                edge_map[neighbor].remove(current_node)
            edge_map[current_node] = set()

            if not candidates:
                next_node = random.choice([node for node in graph if node not in offspring])
            else:
                next_node = min(candidates, key=lambda x: len(edge_map[x]))
                if candidates.count(next_node) > 1:
                    next_node = random.choice([node for node in candidates if len(edge_map[node]) == len(edge_map[next_node])])

            offspring.append(next_node)
            current_node = next_node

        offspring.append(start_node)
        return offspring

    def two_opt_improve(path):
        """Улучшение маршрута с помощью 2-opt."""
        n = len(path)
        improved = False
        while True:
            for i in range(1, n - 2):
                for j in range(i + 1, n - 1):
                    A, B = path[i - 1], path[i]
                    C, D = path[j], path[j + 1]
                    old_weight = graph[A][B] + graph[C][D]
                    new_weight = graph[A][C] + graph[B][D]
                    if new_weight < old_weight:
                        path = path[:i] + path[i:j + 1][::-1] + path[j + 1:]
                        improved = True
            if not improved:
                break
            improved = False
        return path

    # Генерация начальной популяции
    population = generate_initial_population(graph, P)

    for iteration in range(MaxIt):
        # Вычисление веса для каждой хромосомы
        weights = [length(chromosome) for chromosome in population]

        # Селекция родителей
        parents = sus(population, N)

        # Скрещивание
        offspring = []
        while len(offspring) < P:
            parent1, parent2 = random.sample(parents, 2)
            child = crossoverER(parent1, parent2)
            offspring.append(child)

        # Мутация
        for i in range(len(offspring)):
            if random.random() < Pm:
                offspring[i] = two_opt_improve(offspring[i])

        # Обновление популяции
        population = offspring

    # Выбор лучшей хромосомы
    best_chromosome = min(population, key=lambda x: length(x))
    return best_chromosome, length(best_chromosome)


def generate_random_graph(n, max_weight=100):
    graph = {i: {j: random.randint(1, max_weight) for j in range(n) if j != i} for i in range(n)}
    return graph

def main():
    for i in range(100):
        for n in [11]:
            print(f"Graph size: {n}")
            graph = generate_random_graph(n)
            start = 0

            # Brute Force
            start_time = time.time()
            path_bf, cost_bf = tsp_brute_force(graph, start)
            end_time = time.time()
            print(f"Brute Force: Cost = {cost_bf}, Time = {end_time - start_time:.12f} seconds")

            # Branch and Bound
            start_time = time.time()
            path_bnb, cost_bnb = tsp_bnb(graph, start)
            end_time = time.time()
            print(f"Branch and Bound: Cost = {cost_bnb}, Time = {end_time - start_time:.12f} seconds")

            # Greedy
            start_time = time.time()
            path_greedy, cost_greedy = tsp_greedy(graph, start)
            end_time = time.time()
            print(f"Greedy: Cost = {cost_greedy}, Time = {end_time - start_time:.12f} seconds")

            # Local Search (2-opt)
            start_time = time.time()
            path_ls, cost_ls = tsp_ls(graph, start)
            end_time = time.time()
            print(f"Local Search (2-opt): Cost = {cost_ls}, Time = {end_time - start_time:.12f} seconds")

            # Local Search with Multiple Restarts
            start_time = time.time()
            path_ls_multi, cost_ls_multi = tsp_ls_multiple_restarts(graph, start, num_restarts=10)
            end_time = time.time()
            print(f"Local Search (2-opt, multiple restarts): Cost = {cost_ls_multi}, Time = {end_time - start_time:.12f} seconds")

            # Genetic Algorithm
            start_time = time.time()
            path_ga, cost_ga = tsp_ga(graph, P=50, N=20, MaxIt=100, Pm=0.1)
            end_time = time.time()
            print(f"Genetic Algorithm: Cost = {cost_ga}, Time = {end_time - start_time:.12f} seconds")

            print()

if __name__ == "__main__":
    main()